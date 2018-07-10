/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2018, Joyent, Inc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fm/libtopo.h>
#include <fm/topo_hc.h>
#include <sys/fm/protocol.h>
#include <sys/varargs.h>

#include "hwgrok.h"

extern int dump_hw_config_json(hwg_info_t *, char *);

#define	topo_list_next(elem)	((void *)(((topo_list_t *)(elem))->l_next))

static const char *pname;
static const char optstr[] = "dR:";
boolean_t enable_debug = B_FALSE;

static void
usage()
{
	(void) fprintf(stderr, "\n%s [-d][-R root]\n\n", pname);
}

void
hwg_debug(const char *format, ...)
{
	va_list ap;

	if (enable_debug != B_TRUE)
		return;

	va_start(ap, format);
	(void) vfprintf(stdout, format, ap);
	va_end(ap);
}

static void
*hwg_zalloc(ssize_t sz)
{
	void *buf;

	if ((buf = calloc(sizeof (char), sz)) == NULL) {
		return (NULL);
	}
	(void) memset(buf, 0, sz);
	return (buf);
}

static void
hwsen_free_sensor(hwg_sensor_t *hwsen)
{
	free(hwsen->hwsen_name);
	free(hwsen->hwsen_type);
	free(hwsen->hwsen_units);
	free(hwsen->hwsen_state_descr);
	free(hwsen);
}

static void
get_sensor_type(uint32_t reading_type, uint32_t units, char **typestr)
{
	char buf[64];

	if (reading_type != TOPO_SENSOR_TYPE_THRESHOLD_STATE) {
		topo_sensor_type_name(reading_type, buf, sizeof (buf));
		*typestr = strdup(buf);
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
		*typestr = strdup("temperature");
		break;
	case TOPO_SENSOR_UNITS_HZ:
	case TOPO_SENSOR_UNITS_RPM:
		*typestr = strdup("speed");
		break;
	case TOPO_SENSOR_UNITS_VOLTS:
		*typestr = strdup("voltage");
		break;
	default:
		*typestr = strdup("unknown");
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
	char *val, buf[64];
	topo_faclist_t sensorlist = { 0 }, ledlist = { 0 }, *fp;

	if (topo_node_resource(node, &fmri, &err) != 0 ||
	    topo_fmri_nvl2str(thp, fmri, &(cinfo->hwci_fmristr), &err) != 0) {
		goto out;
	}

	if (nvlist_lookup_string(fmri, FM_FMRI_HC_SERIAL_ID, &val) == 0 &&
	    (cinfo->hwci_serial = strdup(val)) == NULL)
		goto out;
	if (nvlist_lookup_string(fmri, FM_FMRI_HC_PART, &val) == 0 &&
	    (cinfo->hwci_part = strdup(val)) == NULL)
		goto out;
	if (nvlist_lookup_string(fmri, FM_FMRI_HC_REVISION, &val) == 0 &&
	    (cinfo->hwci_version = strdup(val)) == NULL)
		goto out;

	if (topo_prop_get_string(node, "protocol", "label",
	    &(cinfo->hwci_label), &err) != 0 && err != ETOPO_PROP_NOENT) {
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

		hwg_debug("Found sensor (%s)\n", topo_node_name(fp->tf_node));
		if ((sensor = hwg_zalloc(sizeof (hwg_sensor_t))) == NULL)
			goto out;

		/*
		 * Lookup the sensor reading type and sensor class. This tell
		 * us what type of sensor we're dealing with, which in turn
		 * informs what data we need to gather on it.
		 */
		if (topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    "type", &reading_type, &err) != 0 ||
		    topo_prop_get_string(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_CLASS, &sensor_class, &err) != 0) {
			free(sensor);
			goto out;
		}
		if (topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_STATE, &sensor->hwsen_state, &err) != 0) {
			if (err != ETOPO_PROP_NOENT) {
				free(sensor);
				goto out;
			}
		} else {
			sensor->hwsen_hasstate = B_TRUE;
		}

		if (sensor->hwsen_hasstate == B_TRUE) {
			topo_sensor_state_name(reading_type,
			    sensor->hwsen_state, buf, 64);
			sensor->hwsen_state_descr = strdup(buf);
		}
		sensor->hwsen_name = strdup(topo_node_name(fp->tf_node));

		llist_append(&(cinfo->hwci_sensors), sensor);

		/*
		 * If it's a discrete sensor, get the sensor type and then
		 * move on to the next sensor.
		 */
		if (strcmp(sensor_class, TOPO_SENSOR_CLASS_THRESHOLD) != 0) {
			get_sensor_type(reading_type, 0,
			    &(sensor->hwsen_type));
			continue;
		}

		sensor->hwsen_hasreading = B_TRUE;
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_READING, &(sensor->hwsen_reading), &err) != 0 ||
		    topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_SENSOR_UNITS, &units, &err) != 0) {
			free(sensor);
			goto out;
		}
		get_sensor_type(reading_type, units, &(sensor->hwsen_type));
		topo_sensor_units_name(units, buf, 64);
		sensor->hwsen_units = strdup(buf);

		/*
		 * Gather the upper and lower sensor reading thresholds, if
		 * available.
		 */
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_PROP_THRESHOLD_LNC, &(sensor->hwsen_thresh_lnc),
		    &err) != 0 && err != ETOPO_PROP_NOENT) {
			free(sensor);
			goto out;
		}
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_PROP_THRESHOLD_LCR, &(sensor->hwsen_thresh_lcr),
		    &err) != 0 && err != ETOPO_PROP_NOENT) {
			free(sensor);
			goto out;
		}
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_PROP_THRESHOLD_LNR, &(sensor->hwsen_thresh_lnr),
		    &err) != 0 && err != ETOPO_PROP_NOENT) {
			free(sensor);
			goto out;
		}
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_PROP_THRESHOLD_UNC, &(sensor->hwsen_thresh_unc),
		    &err) != 0 && err != ETOPO_PROP_NOENT) {
			free(sensor);
			goto out;
		}
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_PROP_THRESHOLD_UCR, &(sensor->hwsen_thresh_ucr),
		    &err) != 0 && err != ETOPO_PROP_NOENT) {
			free(sensor);
			goto out;
		}
		if (topo_prop_get_double(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_PROP_THRESHOLD_UNR, &(sensor->hwsen_thresh_unr),
		    &err) != 0 && err != ETOPO_PROP_NOENT) {
			free(sensor);
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

		hwg_debug("Found LED\n");
		if ((led = hwg_zalloc(sizeof (hwg_led_t))) == NULL)
			goto out;
		if (topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    TOPO_LED_MODE, &ledmode, &err) != 0 ||
		    topo_prop_get_uint32(fp->tf_node, TOPO_PGROUP_FACILITY,
		    "type", &ledtype, &err) != 0) {
			free(led);
			goto out;
		}

		topo_led_type_name(ledtype, buf, 64);
		led->hwled_type = strdup(buf);
		if (ledmode == TOPO_LED_STATE_ON)
			led->hwled_mode = strdup("on");
		else
			led->hwled_mode = strdup("off");

		llist_append(&(cinfo->hwci_leds), led);
	}
	ret = 0;
out:
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

	hwg_debug("Found disk bay\n");
	if ((bay = hwg_zalloc(sizeof (hwg_disk_bay_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(bay->hwdkb_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props\n");
		free(bay);
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
	char *capstr;
	uint32_t rpm;

	hwg_debug("Found disk\n");
	if ((disk = hwg_zalloc(sizeof (hwg_disk_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	cinfo = &(disk->hwdk_common_info);
	if (get_common_props(thp, node, cinfo) != 0) {
		(void) fprintf(stderr, "failure gathering common props\n");
		free(disk);
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_STORAGE, "model",
	    &(cinfo->hwci_model), &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(disk);
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_STORAGE, "manufacturer",
	    &(cinfo->hwci_manufacturer), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(disk);
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_STORAGE,
	    "capacity-in-bytes", &capstr, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(disk);
		return (-1);
	}
	if (topo_prop_get_uint32(node, TOPO_PGROUP_STORAGE, "speed-in-rpm",
	    &disk->hwdk_speed, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(disk);
		return (-1);
	}
	disk->hwdk_size = (uint64_t)strtoll(capstr, NULL, 10);
	topo_hdl_strfree(thp, capstr);

	llist_append(&(hwinfo->hwi_disks), disk);
	cbarg->cb_currbay->hwdkb_disk = disk;
	cbarg->cb_currbay->hwdkb_present = B_TRUE;
	return (0);
}

static int
grok_strand(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;

	cbarg->cb_currchip->hwpr_num_threads++;
}

static int
grok_core(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;

	cbarg->cb_currchip->hwpr_num_cores++;
}

static int
grok_chip(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_processor_t *processor;

	hwg_debug("Found processor\n");
	if ((processor = hwg_zalloc(sizeof (hwg_processor_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(processor->hwpr_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props on "
		    "node: %s=%d\n", cbarg->cb_nodename, cbarg->cb_nodeinst);
		free(processor);
		return (-1);
	}
	llist_append(&(hwinfo->hwi_processors), processor);
	cbarg->cb_currchip = processor;
	return (0);
}

static int
grok_psu(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_psu_t *psu;

	hwg_debug("Found power-supply\n");
	if ((psu = hwg_zalloc(sizeof (hwg_psu_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(psu->hwps_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props on "
		    "node: %s=%d\n", cbarg->cb_nodename, cbarg->cb_nodeinst);
		free(psu);
		return (-1);
	}
	llist_append(&(hwinfo->hwi_psus), psu);
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

	hwg_debug("Found dimm\n");
	if ((dimm = hwg_zalloc(sizeof (hwg_dimm_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	cinfo = &(dimm->hwdi_common_info);
	if (get_common_props(thp, node, cinfo) != 0) {
		(void) fprintf(stderr, "failure gathering common props\n");
		free(dimm);
		return (-1);
	}
	if (topo_prop_get_uint64(node, "dimm-properties", "size",
	    &(dimm->hwdi_size), &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(dimm);
		return (-1);
	}
	if (topo_prop_get_string(node, "dimm-properties", "manufacturer",
	    &(cinfo->hwci_manufacturer), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(dimm);
		return (-1);
	}
	if (topo_prop_get_string(node, "dimm-properties", "type",
	    &(dimm->hwdi_type), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(dimm);
		return (-1);
	}

	llist_append(&(hwinfo->hwi_dimms), dimm);
	cbarg->cb_currslot->hwds_dimm = dimm;
	cbarg->cb_currslot->hwds_present = B_TRUE;
	return (0);
}

static int
grok_fan(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_fan_t *fan;

	hwg_debug("Found fan\n");
	if ((fan = hwg_zalloc(sizeof (hwg_fan_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(fan->hwfa_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props on "
		    "node: %s=%d\n", cbarg->cb_nodename, cbarg->cb_nodeinst);
		free(fan);
		return (-1);
	}
	llist_append(&(hwinfo->hwi_fans), fan);
	return (0);
}

static int
grok_slot(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_dimm_slot_t *slot;

	hwg_debug("Found slot\n");
	if ((slot = hwg_zalloc(sizeof (hwg_dimm_slot_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(slot->hwds_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props\n");
		free(slot);
		return (-1);
	}
	llist_append(&(hwinfo->hwi_dimm_slots), slot);
	cbarg->cb_currslot = slot;
	return (0);
}

static int
grok_pcidev(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_pcidev_t *pcidev;

	hwg_debug("Found PCI(EX) device\n");
	if ((pcidev = hwg_zalloc(sizeof (hwg_pcidev_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(pcidev->hwpci_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props\n");
		free(pcidev);
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

	hwg_debug("Found PCI(EX) function\n");
	pcidev = cbarg->cb_currdev;
	if (topo_prop_get_string(node, TOPO_PGROUP_PCI, TOPO_PCI_VENDNM,
	    &(pcidev->hwpci_vendor), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_PCI, TOPO_PCI_DEVNM,
	    &(pcidev->hwpci_devnm), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_PCI, TOPO_PCI_SUBSYSNM,
	    &(pcidev->hwpci_subsysnm), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_IO, TOPO_IO_DEV,
	    &(pcidev->hwpci_devpath), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_string(node, TOPO_PGROUP_IO, TOPO_IO_DRIVER,
	    &(pcidev->hwpci_drivernm), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}
	if (topo_prop_get_uint32(node, TOPO_PGROUP_IO, TOPO_IO_INSTANCE,
	    &(pcidev->hwpci_driverinst), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		return (-1);
	}

	return (0);
}

static int
grok_sp(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_sp_t *sp;
	int err;

	hwg_debug("Found service-processor\n");
	if ((sp = hwg_zalloc(sizeof (hwg_sp_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(sp->hwsp_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props\n");
		free(sp);
		return (-1);
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-address",
	    &sp->hwsp_ipv4_addr, &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(sp);
		return (-1);
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-subnet",
	    &sp->hwsp_ipv4_subnet, &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(sp);
		return (-1);
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-gateway",
	    &sp->hwsp_ipv4_gateway, &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(sp);
		return (-1);
	}
	if (topo_prop_get_string(node, "network-config", "ipv4-config-type",
	    &sp->hwsp_ipv4_config_type, &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(sp);
		return (-1);
	}
	if (topo_prop_get_string(node, "network-config", "vlan-id",
	    &sp->hwsp_vlan_id, &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(sp);
		return (-1);
	}
	if (topo_prop_get_string(node, "network-config", "ipv6-address",
	    &sp->hwsp_ipv6_addr, &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(sp);
		return (-1);
	}
	if (topo_prop_get_string(node, "network-config", "ipv6-config-type",
	    &sp->hwsp_ipv6_config_type, &err) != 0 && err != ETOPO_PROP_NOENT) {
		free(sp);
		return (-1);
	}
	hwinfo->hwi_sp = sp;

	return (0);
}

static int
grok_motherboard(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_common_info_t *cinfo;
	hwg_motherboard_t *mb;
	int err;

	hwg_debug("Found motherboard\n");
	if ((mb = hwg_zalloc(sizeof (hwg_motherboard_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	cinfo = &(mb->hwmbo_common_info);
	if (get_common_props(thp, node, cinfo) != 0) {
		(void) fprintf(stderr, "failure gathering common props on "
		    "node: %s=%d\n", cbarg->cb_nodename, cbarg->cb_nodeinst);
		free(mb);
		return (-1);
	}
	if (topo_prop_get_string(node, "motherboard-properties", "manufacturer",
	    &(cinfo->hwci_manufacturer), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(mb);
		return (-1);
	}
	if (topo_prop_get_string(node, "motherboard-properties", "product",
	    &(cinfo->hwci_model), &err) != 0 &&
	    err != ETOPO_PROP_NOENT) {
		free(mb);
		return (-1);
	}
	hwinfo->hwi_motherboard = mb;

	return (0);
}

static int
grok_chassis(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;
	hwg_info_t *hwinfo = cbarg->cb_hw_info;
	hwg_chassis_t *chassis;

	hwg_debug("Found chassis\n");
	if ((chassis = hwg_zalloc(sizeof (hwg_chassis_t))) == NULL) {
		(void) fprintf(stderr, "alloc failed\n");
		return (-1);
	}
	if (get_common_props(thp, node, &(chassis->hwch_common_info)) != 0) {
		(void) fprintf(stderr, "failure gathering common props on "
		    "node: %s=%d\n", cbarg->cb_nodename, cbarg->cb_nodeinst);
		free(chassis);
		return (-1);
	}
	hwinfo->hwi_chassis = chassis;

	return (0);
}


static int
topocb(topo_hdl_t *thp, tnode_t *node, void *arg)
{
	hwg_cbarg_t *cbarg = (hwg_cbarg_t *)arg;

	cbarg->cb_nodename = topo_node_name(node);
	cbarg->cb_nodeinst = topo_node_instance(node);

	if (strcmp(cbarg->cb_nodename, CHASSIS) == 0) {
		grok_chassis(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, BAY) == 0) {
		grok_bay(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, CHIP) == 0) {
		grok_chip(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, CORE) == 0) {
		grok_core(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, DIMM) == 0) {
		grok_dimm(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, DISK) == 0) {
		grok_disk(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, FAN) == 0) {
		grok_fan(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, MOTHERBOARD) == 0) {
		grok_motherboard(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCI_DEVICE) == 0) {
		grok_pcidev(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCIEX_DEVICE) == 0) {
		grok_pcidev(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCI_FUNCTION) == 0) {
		grok_pcifn(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PCIEX_FUNCTION) == 0) {
		grok_pcifn(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, PSU) == 0) {
		grok_psu(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, SLOT) == 0) {
		grok_slot(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, SP) == 0) {
		grok_sp(thp, node, arg);
	} else if (strcmp(cbarg->cb_nodename, STRAND) == 0) {
		grok_strand(thp, node, arg);
	}
	return (TOPO_WALK_NEXT);
}

int
main(int argc, char *argv[])
{
	topo_hdl_t *thp;
	topo_walk_t *twp;
	char c, *root = "/", *type = "all";
	int err;
	hwg_info_t hwinfo = { 0 };
	hwg_cbarg_t cbarg = { 0 };

	pname = argv[0];

	while (optind < argc) {
		while ((c = getopt(argc, argv, optstr)) != -1) {
			switch (c) {
			case 'd':
				enable_debug = B_TRUE;
				break;
			case 'R':
				root = optarg;
				break;
			default:
				usage();
				return (2);
			}
		}
	}

	if ((thp = topo_open(TOPO_VERSION, root, &err)) == NULL) {
		(void) fprintf(stderr, "failed to get topo handle: %s\n",
		    topo_strerror(err));
		goto out;
	}
	if (topo_snap_hold(thp, NULL, &err) == NULL) {
		(void) fprintf(stderr, "failed to take topo snapshot: %s\n",
		    topo_strerror(err));
		goto out;
	}

	cbarg.cb_hw_info = &hwinfo;
	if ((twp = topo_walk_init(thp, "hc", topocb, &cbarg, &err)) == NULL) {
		(void) fprintf(stderr, "failed to init topo walker: %s\n",
		    topo_strerror(err));
		goto out;
	}
	if (topo_walk_step(twp, TOPO_WALK_CHILD) == TOPO_WALK_ERR) {
		(void) fprintf(stderr, "failed to walk topology\n");
		topo_walk_fini(twp);
		goto out;
	}
	topo_walk_fini(twp);

	dump_hw_config_json(&hwinfo, type);
out:
	if (thp != NULL) {
		topo_snap_release(thp);
		topo_close(thp);
	}
	return (0);
}
