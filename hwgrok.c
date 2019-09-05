/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019 Joyent, Inc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fm/libtopo.h>
#include <fm/topo_hc.h>
#include <sys/fm/protocol.h>
#include <sys/varargs.h>

#include "hwgrok.h"

extern int dump_hw_config_json(hwg_info_t *, char *);

#define	topo_list_next(elem)	((void *)(((topo_list_t *)(elem))->l_next))

static const char *pname;
static const char optstr[] = "hR:v";

static void
usage()
{
	(void) fprintf(stderr, "\n%s [-h][-R root][-v]\n\n", pname);
}

void
hwg_error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	(void) vfprintf(stderr, format, ap);
	va_end(ap);
}

static void
hwg_free_sensor(llist_t *node, void *arg)
{
	hwg_sensor_t *hwsen = (hwg_sensor_t *)node;
	topo_hdl_t *thp = (topo_hdl_t *)arg;

	topo_hdl_strfree(thp, hwsen->hwsen_name);
	topo_hdl_strfree(thp, hwsen->hwsen_type);
	topo_hdl_strfree(thp, hwsen->hwsen_units);
	topo_hdl_strfree(thp, hwsen->hwsen_state_descr);
	topo_hdl_free(thp, hwsen, sizeof (hwg_sensor_t));
}

static void
hwg_free_led(llist_t *node, void *arg)
{
	hwg_led_t *hwled = (hwg_led_t *)node;
	topo_hdl_t *thp = (topo_hdl_t *)arg;

	topo_hdl_strfree(thp, hwled->hwled_type);
	topo_hdl_strfree(thp, hwled->hwled_mode);
	topo_hdl_free(thp, hwled, sizeof (hwg_led_t));
}

static void
hwg_free_common(topo_hdl_t *thp, hwg_common_info_t *cinfo)
{
	if (cinfo == NULL)
		return;

	free(cinfo->hwci_fmri);
	topo_hdl_strfree(thp, cinfo->hwci_model);
	topo_hdl_strfree(thp, cinfo->hwci_part);
	topo_hdl_strfree(thp, cinfo->hwci_serial);
	topo_hdl_strfree(thp, cinfo->hwci_version);
	topo_hdl_strfree(thp, cinfo->hwci_manufacturer);
	topo_hdl_strfree(thp, cinfo->hwci_label);
	llist_destroy(&cinfo->hwci_sensors, hwg_free_sensor, thp);
	llist_destroy(&cinfo->hwci_sensors, hwg_free_led, thp);
}

static void
hwg_free_processor(llist_t *node, void *arg)
{
	hwg_processor_t *hwproc = (hwg_processor_t *)node;
	topo_hdl_t *thp = (topo_hdl_t *)arg;

	hwg_free_common(thp, &hwproc->hwpr_common_info);
	topo_hdl_strfree(thp, hwproc->hwpr_brand);
	topo_hdl_free(thp, hwproc, sizeof (hwg_processor_t));
}

static void
hwg_free_dimm(topo_hdl_t *thp, hwg_dimm_t *dimm)
{
	hwg_free_common(thp, &dimm->hwdi_common_info);
	topo_hdl_strfree(thp, dimm->hwdi_size);
	topo_hdl_free(thp, dimm, sizeof (hwg_dimm_t));
}

static void
hwg_free_disk(topo_hdl_t *thp, hwg_disk_t *disk)
{
	hwg_free_common(thp, &disk->hwdk_common_info);
	topo_hdl_strfree(thp, disk->hwdk_devpath);
	topo_hdl_strfree(thp, disk->hwdk_ctd);
	topo_hdl_free(thp, disk, sizeof (hwg_disk_t));
}

static void
hwg_free_disk_bay(llist_t *node, void *arg)
{
	hwg_disk_bay_t *bay = (hwg_disk_bay_t *)node;
	topo_hdl_t *thp = (topo_hdl_t *)arg;

	hwg_free_common(thp, &bay->hwdkb_common_info);
	if (bay->hwdkb_disk != NULL)
		hwg_free_disk(thp, bay->hwdkb_disk);
	topo_hdl_free(thp, bay, sizeof (hwg_disk_bay_t));
}

static void
hwg_free_pcidev(llist_t *node, void *arg)
{
	hwg_pcidev_t *dev = (hwg_pcidev_t *)node;
	topo_hdl_t *thp = (topo_hdl_t *)arg;

	hwg_free_common(thp, &dev->hwpci_common_info);
	topo_hdl_strfree(thp, dev->hwpci_vendor);
	topo_hdl_strfree(thp, dev->hwpci_devnm);
	topo_hdl_strfree(thp, dev->hwpci_subsysnm);
	topo_hdl_strfree(thp, dev->hwpci_devpath);
	topo_hdl_strfree(thp, dev->hwpci_drivernm);
	topo_hdl_free(thp, dev, sizeof (hwg_pcidev_t));
}

static void
hwg_free_psu(llist_t *node, void *arg)
{
	hwg_psu_t *psu = (hwg_psu_t *)node;
	topo_hdl_t *thp = (topo_hdl_t *)arg;

	hwg_free_common(thp, &psu->hwps_common_info);
	topo_hdl_free(thp, psu, sizeof (hwg_psu_t));
}

static void
hwg_free_fan(llist_t *node, void *arg)
{
	hwg_fan_t *fan = (hwg_fan_t *)node;
	topo_hdl_t *thp = (topo_hdl_t *)arg;

	hwg_free_common(thp, &fan->hwfa_common_info);
	topo_hdl_free(thp, fan, sizeof (hwg_fan_t));
}

static void
hwg_free_sp(topo_hdl_t *thp, hwg_sp_t *sp)
{
	hwg_free_common(thp, &sp->hwsp_common_info);
	topo_hdl_strfree(thp, sp->hwsp_macaddr);
	topo_hdl_strfree(thp, sp->hwsp_ipv4_addr);
	topo_hdl_strfree(thp, sp->hwsp_ipv4_subnet);
	topo_hdl_strfree(thp, sp->hwsp_ipv4_gateway);
	topo_hdl_strfree(thp, sp->hwsp_ipv4_config_type);
	topo_hdl_strfree(thp, sp->hwsp_vlan_id);
	topo_hdl_strfree(thp, sp->hwsp_ipv6_addr);
	topo_hdl_strfree(thp, sp->hwsp_ipv6_config_type);
	topo_hdl_free(thp, sp, sizeof (hwg_sp_t));
}

static void
hwg_free_usbdev(topo_hdl_t *thp, hwg_usbdev_t *usbdev)
{
	hwg_free_common(thp, &usbdev->hwusb_common_info);
	topo_hdl_strfree(thp, usbdev->hwusb_version);
	topo_hdl_strfree(thp, usbdev->hwusb_speed);
	topo_hdl_strfree(thp, usbdev->hwusb_vendor);
	topo_hdl_strfree(thp, usbdev->hwusb_devpath);
	topo_hdl_strfree(thp, usbdev->hwusb_drivernm);
	topo_hdl_free(thp, usbdev, sizeof (hwg_usbdev_t));
}

static void
hwg_free_motherboard(topo_hdl_t *thp, hwg_motherboard_t *mb)
{
	hwg_free_common(thp, &mb->hwmbo_common_info);
	topo_hdl_free(thp, mb, sizeof (hwg_motherboard_t));
}

static void
hwg_free_chassis(topo_hdl_t *thp, hwg_chassis_t *chassis)
{
	hwg_free_common(thp, &chassis->hwch_common_info);
	topo_hdl_free(thp, chassis, sizeof (hwg_chassis_t));
}

static int
hwg_get_prop(tnode_t *node, topo_type_t ptype, const char *pgrp,
    const char *pnm, hwg_numeric_prop_t *pval)
{
	int err, ret;

	switch (ptype) {
	case TOPO_TYPE_UINT32:
		if (topo_prop_get_uint32(node, pgrp, pnm, &pval->hnp_u32,
		    &err) != 0) {
			pval->hnp_is_set = B_FALSE;
			ret = (err == ETOPO_PROP_NOENT ? 0 : -1);
			return (ret);
		}
		pval->hnp_type = ptype;
		pval->hnp_is_set = B_TRUE;
		break;
	case TOPO_TYPE_INT32:
		if (topo_prop_get_int32(node, pgrp, pnm, &pval->hnp_i32,
		    &err) != 0) {
			pval->hnp_is_set = B_FALSE;
			ret = (err == ETOPO_PROP_NOENT ? 0 : -1);
			return (ret);
		}
		pval->hnp_type = ptype;
		pval->hnp_is_set = B_TRUE;
		break;
	case TOPO_TYPE_UINT64:
		if (topo_prop_get_uint64(node, pgrp, pnm, &pval->hnp_u64,
		    &err) != 0) {
			pval->hnp_is_set = B_FALSE;
			ret = (err == ETOPO_PROP_NOENT ? 0 : -1);
			return (ret);
		}
		pval->hnp_type = ptype;
		pval->hnp_is_set = B_TRUE;
		break;
	case TOPO_TYPE_INT64:
		if (topo_prop_get_int64(node, pgrp, pnm, &pval->hnp_i64,
		    &err) != 0) {
			pval->hnp_is_set = B_FALSE;
			ret = (err == ETOPO_PROP_NOENT ? 0 : -1);
			return (ret);
		}
		pval->hnp_type = ptype;
		pval->hnp_is_set = B_TRUE;
		break;
	case TOPO_TYPE_DOUBLE:
		if (topo_prop_get_double(node, pgrp, pnm, &pval->hnp_dbl,
		    &err) != 0) {
			pval->hnp_is_set = B_FALSE;
			ret = (err == ETOPO_PROP_NOENT ? 0 : -1);
			return (ret);
		}
		pval->hnp_type = ptype;
		pval->hnp_is_set = B_TRUE;
		break;
	default:
		hwg_error("%s: invalid type: %u", __func__, ptype);
		return (-1);
	}
	return (0);
}

/*
 * This routine takes a string containing an HC-scheme FMRI and returns
 * a new string with the authority portion of the FMRI stripped out.  We
 * do this to make the FMRI more readable and because the HW identity
 * information in the authority is already represented by other object
 * properties and thus is redundant.
 *
 * Caller is responsible for freeing returned string.
 */
static char *
hwg_fmri_strip(const char *fmri)
{
	char *stripped = NULL, *fmriend;

	/*
	 * HC scheme FMRI's have the format:
	 *  hc://<authority>/<hc-name>=<hc-inst>...
	 *
	 * So we want the part of the FMRI after the 3rd "/"
	 */
	fmriend = strstr(fmri + 5, "/");
	if (fmriend == NULL) {
		hwg_error("%s: malformed FMRI", __func__);
		return (NULL);
	}
	if (asprintf(&stripped, "hc://%s", fmriend) < 0) {
		hwg_error("%s: failed to alloc string (%s)", __func__,
		    strerror(errno));
		return (NULL);
	}
	return (stripped);
}

static void
get_sensor_type(topo_hdl_t *thp, uint32_t reading_type, uint32_t units,
    char **typestr)
{
	char buf[64];

	if (reading_type != TOPO_SENSOR_TYPE_THRESHOLD_STATE) {
		topo_sensor_type_name(reading_type, buf, sizeof (buf));
		*typestr = topo_hdl_strdup(thp, buf);
		return;
	}

	/*
	 * For threshold sensors, we infer the type based on the type of units
	 * used for the reading.
	 */
	switch (units) {
	case TOPO_SENSOR_UNITS_DEGREES_C:
	case TOPO_SENSOR_UNITS_DEGREES_F:
	case TOPO_SENSOR_UNITS_DEGREES_K:
		*typestr = topo_hdl_strdup(thp, "temperature");
		break;
	case TOPO_SENSOR_UNITS_HZ:
	case TOPO_SENSOR_UNITS_RPM:
		*typestr = topo_hdl_strdup(thp, "speed");
		break;
	case TOPO_SENSOR_UNITS_VOLTS:
		*typestr = topo_hdl_strdup(thp, "voltage");
		break;
	case TOPO_SENSOR_UNITS_AMPS:
		*typestr = topo_hdl_strdup(thp, "current");
		break;
	case TOPO_SENSOR_UNITS_WATTS:
		*typestr = topo_hdl_strdup(thp, "power consumption");
		break;
	default:
		*typestr = topo_hdl_strdup(thp, "unknown");
	}
}

/*
 * Gather up information that is common to all node types:
 *  - FMRI
 *  - serial number
 *  - part number
 *  - revision id
 *  - FRU label
 *  - associated sensors and/or indicators
 *
 * XXX - add code to gather any open FM cases associated with this node
 */
static int
get_common_props(topo_hdl_t *thp, tnode_t *node, hwg_common_info_t *cinfo)
{
	int err, ret = 0;
	nvlist_t *fmri = NULL;
	char *val, *fmristr = NULL, buf[64];
	topo_faclist_t sensorlist = { 0 }, ledlist = { 0 }, *fp;

	if (topo_node_resource(node, &fmri, &err) != 0 ||
	    topo_fmri_nvl2str(thp, fmri, &fmristr, &err) != 0 ||
	    (cinfo->hwci_fmri = hwg_fmri_strip(fmristr)) == NULL) {
		goto out;
	}

	if (nvlist_lookup_string(fmri, FM_FMRI_HC_SERIAL_ID, &val) == 0 &&
	    (cinfo->hwci_serial = topo_hdl_strdup(thp, val)) == NULL)
		goto out;
	if (nvlist_lookup_string(fmri, FM_FMRI_HC_PART, &val) == 0 &&
	    (cinfo->hwci_part = topo_hdl_strdup(thp, val)) == NULL)
		goto out;
	if (nvlist_lookup_string(fmri, FM_FMRI_HC_REVISION, &val) == 0 &&
	    (cinfo->hwci_version = topo_hdl_strdup(thp, val)) == NULL)
		goto out;

	if (topo_prop_get_string(node, "protocol", "label",
	    &cinfo->hwci_label, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto out;
	}

	(void) topo_node_facility(thp, node, TOPO_FAC_TYPE_SENSOR,
	    TOPO_FAC_TYPE_ANY, &sensorlist, &err);

	/*
	 * Iterate through all of the child sensor facilty nodes, which
	 * represent sensors that are associated with this hardware resource.
	 */
	for (fp = topo_list_next(&sensorlist.tf_list);
	    fp != NULL;
	    fp = topo_list_next(fp)) {
		hwg_sensor_t *sensor;
		uint32_t reading_type, state, units;
		char *sensor_class = NULL;

		if ((sensor = topo_hdl_zalloc(thp, sizeof (hwg_sensor_t))) ==
		    NULL) {
			hwg_error("alloc failed!\n");
			goto out;
		}

		/*
		 * Lookup the sensor reading type and sensor class. This tell
		 * us what type of sensor we're dealing with, which in turn
		 * informs what data we need to gather on it.
		 */
		if (topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    "type", &reading_type, &err) != 0 ||
		    topo_prop_get_string(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_CLASS, &sensor_class, &err) != 0) {
			hwg_error("failed to get %s or %s prop on sensor "
			    "node: %s\n", "type", TOPO_SENSOR_CLASS,
			    topo_node_name(fp->tf_node));
			hwg_free_sensor((struct llist *)sensor, thp);
			continue;
		}
		if (topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_STATE, &sensor->hwsen_state, &err) != 0) {
			if (err != ETOPO_PROP_NOENT) {
				hwg_error("failed to get %s prop on sensor "
				    "node: %s\n", TOPO_SENSOR_STATE,
				    topo_node_name(fp->tf_node));
				hwg_free_sensor((struct llist *)sensor, thp);
				continue;
			}
		} else {
			sensor->hwsen_hasstate = B_TRUE;
		}

		if (sensor->hwsen_hasstate == B_TRUE) {
			topo_sensor_state_name(reading_type,
			    sensor->hwsen_state, buf, 64);
			sensor->hwsen_state_descr = strdup(buf);
		}
		sensor->hwsen_name = topo_hdl_strdup(thp,
		    topo_node_name(fp->tf_node));

		llist_append(&(cinfo->hwci_sensors), sensor);

		/*
		 * If it's a discrete sensor, get the sensor type and then
		 * move on to the next sensor.
		 */
		if (strcmp(sensor_class, TOPO_SENSOR_CLASS_THRESHOLD) != 0) {
			get_sensor_type(thp, reading_type, 0,
			    &(sensor->hwsen_type));
			continue;
		}

		sensor->hwsen_hasreading = B_TRUE;
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_READING, &(sensor->hwsen_reading), &err) != 0 ||
		    topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_UNITS, &units, &err) != 0) {
			hwg_error("failed to get %s or %s prop on sensor node: "
			    "%s\n", TOPO_SENSOR_READING, TOPO_SENSOR_UNITS,
			    topo_node_name(fp->tf_node));
			continue;
		}
		get_sensor_type(thp, reading_type, units, &sensor->hwsen_type);
		topo_sensor_units_name(units, buf, 64);
		sensor->hwsen_units = topo_hdl_strdup(thp, buf);

		/*
		 * Gather the upper and lower sensor reading thresholds, if
		 * available.
		 */
		if (hwg_get_prop(fp->tf_node, TOPO_TYPE_DOUBLE,
		    TOPO_PGROUP_FACILITY, TOPO_PROP_THRESHOLD_LNC,
		    &(sensor->hwsen_thresh_lnc)) != 0) {
			goto out;
		}
		if (hwg_get_prop(fp->tf_node, TOPO_TYPE_DOUBLE,
		    TOPO_PGROUP_FACILITY, TOPO_PROP_THRESHOLD_LCR,
		    &(sensor->hwsen_thresh_lcr)) != 0) {
			goto out;
		}
		if (hwg_get_prop(fp->tf_node, TOPO_TYPE_DOUBLE,
		    TOPO_PGROUP_FACILITY, TOPO_PROP_THRESHOLD_LNR,
		    &(sensor->hwsen_thresh_lnr)) != 0) {
			goto out;
		}
		if (hwg_get_prop(fp->tf_node, TOPO_TYPE_DOUBLE,
		    TOPO_PGROUP_FACILITY, TOPO_PROP_THRESHOLD_UNC,
		    &(sensor->hwsen_thresh_unc)) != 0) {
			goto out;
		}
		if (hwg_get_prop(fp->tf_node, TOPO_TYPE_DOUBLE,
		    TOPO_PGROUP_FACILITY, TOPO_PROP_THRESHOLD_UCR,
		    &(sensor->hwsen_thresh_ucr)) != 0) {
			goto out;
		}
		if (hwg_get_prop(fp->tf_node, TOPO_TYPE_DOUBLE,
		    TOPO_PGROUP_FACILITY, TOPO_PROP_THRESHOLD_UNR,
		    &(sensor->hwsen_thresh_unr)) != 0) {
			goto out;
		}
	}

	(void) topo_node_facility(thp, node, TOPO_FAC_TYPE_INDICATOR,
	    TOPO_FAC_TYPE_ANY, &ledlist, &err);

	for (fp = topo_list_next(&ledlist.tf_list);
	    fp != NULL;
	    fp = topo_list_next(fp)) {
		hwg_led_t *led;
		uint32_t ledmode, ledtype;

		if ((led = topo_hdl_zalloc(thp, sizeof (hwg_led_t))) == NULL) {
			hwg_error("alloc failed!\n");
			goto out;
		}
		if (topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_LED_MODE, &ledmode, &err) != 0 ||
		    topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    "type", &ledtype, &err) != 0) {
			hwg_free_led((struct llist *)led, thp);
			goto out;
		}

		topo_led_type_name(ledtype, buf, 64);
		led->hwled_type = topo_hdl_strdup(thp, buf);
		if (ledmode == TOPO_LED_STATE_ON)
			led->hwled_mode = topo_hdl_strdup(thp, "on");
		else
			led->hwled_mode = topo_hdl_strdup(thp, "off");

		llist_append(&(cinfo->hwci_leds), led);
	}
	ret = 0;
out:
	topo_hdl_strfree(thp, fmristr);
	nvlist_free(fmri);
	while ((fp = topo_list_next(&sensorlist.tf_list)) != NULL) {
		topo_list_delete(&sensorlist.tf_list, fp);
		topo_hdl_free(thp, fp, sizeof (topo_faclist_t));
	}
	while ((fp = topo_list_next(&ledlist.tf_list)) != NULL) {
		topo_list_delete(&ledlist.tf_list, fp);
		topo_hdl_free(thp, fp, sizeof (topo_faclist_t));
	}

	return (ret);
}

static int
grok_bay(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_disk_bay_t *bay;

	if ((bay = topo_hdl_zalloc(thp, sizeof (hwg_disk_bay_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(bay->hwdkb_common_info)) != 0) {
		hwg_error("failure gathering common props\n");
		hwg_free_disk_bay((struct llist *)bay, thp);
		return (-1);
	}
	llist_append(&(hwinfo->hwi_disk_bays), bay);
	cbarg->cb_currbay = bay;
	return (0);
}

static int
grok_disk(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_disk_t *disk;
	hwg_common_info_t *cinfo;
	int err;
	char *capstr = NULL;
	uint32_t rpm;

	if ((disk = topo_hdl_zalloc(thp, sizeof (hwg_disk_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	cinfo = &(disk->hwdk_common_info);
	if (get_common_props(thp, node, cinfo) != 0) {
		hwg_error("failure gathering common props\n");
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_STORAGE, "model",
	    &(cinfo->hwci_model), &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_STORAGE, "manufacturer",
	    &(cinfo->hwci_manufacturer), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_IO, TOPO_IO_DEV_PATH,
	    &disk->hwdk_devpath, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_STORAGE, "logical-disk",
	    &disk->hwdk_ctd, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (hwg_get_prop(node, TOPO_TYPE_UINT32, TOPO_PGROUP_STORAGE,
	    "speed-in-rpm", &disk->hwdk_speed) != 0) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_STORAGE,
	    "capacity-in-bytes", &capstr, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (capstr != NULL)  {
		disk->hwdk_size = (uint64_t)strtoll(capstr, NULL, 10);
		topo_hdl_strfree(thp, capstr);
	} else {
		hwg_error("no capacity reported for drive: %s\n",
		    cinfo->hwci_fmri);
	}

	llist_append(&(hwinfo->hwi_disks), disk);
	/*
	 * USB disks won't have an associated bay, in which case cb_currbay
	 * will be NULL.
	 */
	if (cbarg->cb_currbay != NULL) {
		cbarg->cb_currbay->hwdkb_disk = disk;
		cbarg->cb_currbay->hwdkb_present = B_TRUE;
	}
	return (0);
err:
	hwg_free_disk(thp, disk);
	return (-1);
}

static int
grok_strand(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;

	cbarg->cb_currchip->hwpr_num_threads++;
	return (0);
}

static int
grok_core(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;

	cbarg->cb_currchip->hwpr_num_cores++;
	return (0);
}

static int
grok_chip(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_processor_t *processor;
	char *vendor = NULL, *brand = NULL;
	int err;

	if ((processor = topo_hdl_zalloc(thp, sizeof (hwg_processor_t))) ==
	    NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &processor->hwpr_common_info) != 0) {
		hwg_error("failure gathering common props on node: %s=%d\n",
		    cbarg->cb_nodename, cbarg->cb_nodeinst);
		goto err;
	}
	llist_append(&(hwinfo->hwi_processors), processor);
	cbarg->cb_currchip = processor;

	if (topo_prop_get_int32(node, "chip-properties", "family",
	    &processor->hwpr_family, &err) != 0 ||
	    topo_prop_get_int32(node, "chip-properties", "model",
	    &processor->hwpr_model, &err) != 0 ||
	    topo_prop_get_int32(node, "chip-properties", "stepping",
	    &processor->hwpr_stepping, &err) != 0) {
		hwg_error("required chip node properties missing on node: "
		    "%s=%d", cbarg->cb_nodename, cbarg->cb_nodeinst);
		goto err;
	}
	if (topo_prop_get_string(node, "chip-properties", "vendor_id", &vendor,
	    &err) != 0 && err != ETOPO_PROP_NOENT) {
		hwg_error("failed to lookup prop chip-properties/vendor_id\n");
		goto err;
	}
	if (vendor != NULL && strcmp(vendor, "GenuineIntel") == 0)
		processor->hwpr_common_info.hwci_manufacturer =
		    topo_hdl_strdup(thp, "Intel");
	else if (vendor != NULL && strcmp(vendor, "AuthenticAMD") == 0)
		processor->hwpr_common_info.hwci_manufacturer =
		    topo_hdl_strdup(thp, "AMD");
	else if (vendor != NULL)
		processor->hwpr_common_info.hwci_manufacturer =
		    topo_hdl_strdup(thp, vendor);

	topo_hdl_strfree(thp, vendor);

	if (topo_prop_get_string(node, "chip-properties", "brand",
	    &processor->hwpr_brand, &err) != 0 && err != ETOPO_PROP_NOENT) {
		hwg_error("failed to lookup prop chip-properties/brand\n");
		goto err;
	}

	return (0);
err:
	hwg_free_processor((struct llist *)processor, thp);
	return (-1);
}

static int
grok_psu(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_psu_t *psu;

	if ((psu = topo_hdl_zalloc(thp, sizeof (hwg_psu_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(psu->hwps_common_info)) != 0) {
		hwg_error("failure gathering common props on node: %s=%d\n",
		    cbarg->cb_nodename, cbarg->cb_nodeinst);
		hwg_free_psu((struct llist *)psu, thp);
		return (-1);
	}
	llist_append(&(hwinfo->hwi_psus), psu);
	return (0);
}

static int
grok_mem_ctrl(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	char *ecc = NULL;
	int err;

	if (topo_prop_get_string(node, "memory-controller-properties",
	    "memory-ecc", &ecc, &err) != 0 && err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	cbarg->cb_ecc_supp = B_FALSE;
	if (ecc != NULL && strcmp(ecc, "enabled") == 0)
		cbarg->cb_ecc_supp = B_TRUE;

	topo_hdl_strfree(thp, ecc);

	return (0);
}

static int
grok_dimm(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_dimm_t *dimm;
	hwg_common_info_t *cinfo;
	int err;

	if ((dimm = topo_hdl_zalloc(thp, sizeof (hwg_dimm_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	cinfo = &(dimm->hwdi_common_info);
	if (get_common_props(thp, node, cinfo) != 0) {
		hwg_error("failure gathering common props\n");
		goto err;
	}
	if (topo_prop_get_string(node, "dimm-properties", "dimm-size",
	    &(dimm->hwdi_size), &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	dimm->hwdi_ecc_supp = cbarg->cb_ecc_supp;

	llist_append(&(hwinfo->hwi_dimms), dimm);
	return (0);
err:
	hwg_free_dimm(thp, dimm);
	return (-1);
}

static int
grok_fan(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_fan_t *fan;

	if ((fan = topo_hdl_zalloc(thp, sizeof (hwg_fan_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &fan->hwfa_common_info) != 0) {
		hwg_error("failure gathering common props on node: %s=%d\n",
		    cbarg->cb_nodename, cbarg->cb_nodeinst);
		hwg_free_fan((struct llist *)fan, thp);
		return (-1);
	}
	llist_append(&(hwinfo->hwi_fans), fan);
	return (0);
}

static int
grok_pcidev(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_pcidev_t *pcidev;

	if ((pcidev = topo_hdl_zalloc(thp, sizeof (hwg_pcidev_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(pcidev->hwpci_common_info)) != 0) {
		hwg_error("failure gathering common props\n");
		hwg_free_pcidev((struct llist *)pcidev, thp);
		return (-1);
	}
	if (hwg_get_prop(node, TOPO_TYPE_UINT32, TOPO_PGROUP_PCI,
	    TOPO_PCI_MAX_WIDTH, &pcidev->hwpci_maxlanes) != 0) {
		return (-1);
	}
	if (hwg_get_prop(node, TOPO_TYPE_UINT32, TOPO_PGROUP_PCI,
	    TOPO_PCI_CUR_WIDTH, &pcidev->hwpci_currlanes) != 0) {
		return (-1);
	}
	if (hwg_get_prop(node, TOPO_TYPE_UINT64, TOPO_PGROUP_PCI,
	    TOPO_PCI_MAX_SPEED, &pcidev->hwpci_maxspeed) != 0) {
		return (-1);
	}
	if (hwg_get_prop(node, TOPO_TYPE_UINT64, TOPO_PGROUP_PCI,
	    TOPO_PCI_CUR_SPEED, &pcidev->hwpci_currspeed) != 0) {
		return (-1);
	}
	llist_append(&(hwinfo->hwi_pcidevs), pcidev);
	cbarg->cb_currdev = pcidev;

	return (0);
}

static int
grok_pcifn(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_pcidev_t *pcidev;
	int err;

	pcidev = cbarg->cb_currdev;
	if (topo_prop_get_string(node, TOPO_PGROUP_PCI, TOPO_PCI_VENDNM,
	    &pcidev->hwpci_vendor, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_PCI, TOPO_PCI_DEVNM,
	    &pcidev->hwpci_devnm, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_PCI, TOPO_PCI_SUBSYSNM,
	    &pcidev->hwpci_subsysnm, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_IO, TOPO_IO_DEV,
	    &pcidev->hwpci_devpath, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_IO, TOPO_IO_DRIVER,
	    &pcidev->hwpci_drivernm, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (hwg_get_prop(node, TOPO_TYPE_UINT32, TOPO_PGROUP_IO,
	    TOPO_IO_INSTANCE, &pcidev->hwpci_driverinst) != 0) {
		return (-1);
	}

	return (0);
}

static int
grok_usbdev(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_usbdev_t *usbdev;
	hwg_common_info_t *cinfo;
	int err;

	if ((usbdev = topo_hdl_zalloc(thp, sizeof (hwg_usbdev_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	cinfo = &(usbdev->hwusb_common_info);
	if (get_common_props(thp, node, cinfo) != 0) {
		hwg_error("failure gathering common props\n");
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_USB_PROPS,
	    TOPO_PGROUP_USB_PROPS_VNAME, &usbdev->hwusb_vendor, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_USB_PROPS,
	    TOPO_PGROUP_USB_PROPS_SPEED, &usbdev->hwusb_speed, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_USB_PROPS,
	    TOPO_PGROUP_USB_PROPS_VERSION, &usbdev->hwusb_version, &err) !=
	    0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_IO, TOPO_IO_DEV_PATH,
	    &usbdev->hwusb_devpath, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_IO, TOPO_IO_DRIVER,
	    &usbdev->hwusb_drivernm, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_uint32(node, TOPO_PGROUP_IO, TOPO_IO_INSTANCE,
	    &usbdev->hwusb_driverinst, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	usbdev->hwusb_is_internal = !(cbarg->cb_is_chassis_dev);

	llist_append(&(hwinfo->hwi_usbdevs), usbdev);
	return (0);
err:
	hwg_free_usbdev(thp, usbdev);
	return (-1);
}


static int
grok_sp(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_sp_t *sp;
	int err;

	if ((sp = topo_hdl_zalloc(thp, sizeof (hwg_sp_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(sp->hwsp_common_info)) != 0) {
		hwg_error("failure gathering common props\n");
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "mac-address",
	    &sp->hwsp_macaddr, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-address",
	    &sp->hwsp_ipv4_addr, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-subnet",
	    &sp->hwsp_ipv4_subnet, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-gateway",
	    &sp->hwsp_ipv4_gateway, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-config-type",
	    &sp->hwsp_ipv4_config_type, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "vlan-id",
	    &sp->hwsp_vlan_id, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "ipv6-address",
	    &sp->hwsp_ipv6_addr, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, "network-config", "ipv6-config-type",
	    &sp->hwsp_ipv6_config_type, &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	hwinfo->hwi_sp = sp;
	return (0);
err:
	hwg_free_sp(thp, sp);
	return (-1);
}

static int
grok_motherboard(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_common_info_t *cinfo;
	hwg_motherboard_t *mb;
	int err;

	cbarg->cb_is_chassis_dev = B_FALSE;
	if ((mb = topo_hdl_zalloc(thp, sizeof (hwg_motherboard_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	cinfo = &(mb->hwmbo_common_info);
	if (get_common_props(thp, node, cinfo) != 0) {
		hwg_error("failure gathering common props on node: %s=%d\n",
		    cbarg->cb_nodename, cbarg->cb_nodeinst);
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_MOTHERBOARD,
	    TOPO_PROP_MB_MANUFACTURER, &(cinfo->hwci_manufacturer),
	    &err) != 0 && err != ETOPO_PROP_NOENT) {
		goto err;
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_MOTHERBOARD,
	    TOPO_PROP_MB_PRODUCT, &(cinfo->hwci_model), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		goto err;
	}
	hwinfo->hwi_motherboard = mb;
	return (0);
err:
	hwg_free_motherboard(thp, mb);
	return (-1);
}

static int
grok_chassis(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_chassis_t *chassis;

	cbarg->cb_is_chassis_dev = B_TRUE;
	if ((chassis = topo_hdl_zalloc(thp, sizeof (hwg_chassis_t))) == NULL) {
		hwg_error("alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(chassis->hwch_common_info)) != 0) {
		hwg_error("failure gathering common props on node: %s=%d\n",
		    cbarg->cb_nodename, cbarg->cb_nodeinst);
		hwg_free_chassis(thp, chassis);
		return (-1);
	}
	hwinfo->hwi_chassis = chassis;

	return (0);
}

static int
topocb(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	int ret;

	cbarg->cb_nodename = topo_node_name(node);
	cbarg->cb_nodeinst = topo_node_instance(node);

	if (strcmp(cbarg->cb_nodename, CHASSIS) == 0) {
		ret = grok_chassis(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, BAY) == 0) {
		ret = grok_bay(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, CHIP) == 0) {
		ret = grok_chip(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, CORE) == 0) {
		ret = grok_core(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, DIMM) == 0) {
		ret = grok_dimm(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, DISK) == 0) {
		ret = grok_disk(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, FAN) == 0) {
		ret = grok_fan(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, MEMORYCONTROL) == 0) {
		ret = grok_mem_ctrl(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, MOTHERBOARD) == 0) {
		ret = grok_motherboard(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCI_DEVICE) == 0) {
		ret = grok_pcidev(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCIEX_DEVICE) == 0) {
		ret = grok_pcidev(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCI_FUNCTION) == 0) {
		ret = grok_pcifn(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCIEX_FUNCTION) == 0) {
		ret = grok_pcifn(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PSU) == 0) {
		ret = grok_psu(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, SP) == 0) {
		ret = grok_sp(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, STRAND) == 0) {
		ret = grok_strand(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, USB_DEVICE) == 0) {
		ret = grok_usbdev(thp, node, arg);
	} else {
		return (TOPO_WALK_NEXT);
	}
	return (ret == 0 ? TOPO_WALK_NEXT : TOPO_WALK_ERR);
}

int
main(int argc, char *argv[])
{
	topo_hdl_t *thp;
	topo_walk_t *twp;
	char c, *root = "/", *type = "all";
	int err, status = EXIT_FAILURE;
	hwg_info_t hwinfo = { 0 };
	hwg_cbarg_t cbarg = { 0 };

	pname = argv[0];

	while (optind < argc) {
		while ((c = getopt(argc, argv, optstr)) != -1) {
			switch (c) {
			case 'R':
				root = optarg;
				break;
			case 'v':
				(void) printf("%s\n", HWGROK_VERSION);
				exit(EXIT_SUCCESS);
			case 'h':
				/* FALLTHROUGH */
			default:
				usage();
				return (EXIT_USAGE);
			}
		}
	}

	if ((thp = topo_open(TOPO_VERSION, root, &err)) == NULL) {
		hwg_error("failed to get topo handle: %s\n",
		    topo_strerror(err));
		goto out;
	}
	if (topo_snap_hold(thp, NULL, &err) == NULL) {
		hwg_error("failed to take topo snapshot: %s\n",
		    topo_strerror(err));
		goto out;
	}

	cbarg.cb_hw_info = &hwinfo;
	if ((twp = topo_walk_init(thp, "hc", topocb, &cbarg, &err)) == NULL) {
		hwg_error("failed to init topo walker: %s\n",
		    topo_strerror(err));
		goto out;
	}
	if (topo_walk_step(twp, TOPO_WALK_CHILD) == TOPO_WALK_ERR) {
		hwg_error("failed to walk topology\n");
		topo_walk_fini(twp);
		goto out;
	}
	topo_walk_fini(twp);

	dump_hw_config_json(&hwinfo, type);
	status = EXIT_SUCCESS;
out:
	if (thp != NULL) {
		topo_snap_release(thp);
		topo_close(thp);
	}
	return (status);
}
