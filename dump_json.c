/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2018, Joyent, Inc.
 */
#include "hwgrok.h"

/*
 * JSON property names
 */
#define	CHASSIS			"chassis"
#define	DEVICE_PATH		"device-path"
#define	DIMM			"dimm"
#define	DISK			"disk"
#define	DRIVE_BAYS		"drive-bays"
#define	DRIVERNM		"device-driver-name"
#define	DRIVERINST		"device-driver-instance"
#define	FANS			"fans"
#define	FIRMWARE_REV		"firmware-revision"
#define	HC_FMRI			"hc-fmri"
#define	IPV4_ADDR		"ipv4-address"
#define	IPV4_SUBNET		"ipv4-subnet"
#define	IPV4_GATEWAY		"ipv4-gateway"
#define	IPV4_CFG_TYPE		"ipv4-config-type"
#define	LABEL			"label"
#define	LEDS			"leds"
#define	MANUF			"manufacturer"
#define	MEMORY			"memory"
#define	MODE			"mode"
#define	MODEL			"model"
#define	MOTHERBOARD		"motherboard"
#define	NAME			"name"
#define	NUM_CORES		"number-of-cores"
#define	NUM_THREADS		"number-of-threads-per-core"
#define	PARTNO			"part-number"
#define	PCI_DEVICES		"pci-devices"
#define	PCI_DEVICE_NAME		"pci-device-name"
#define	PCI_SUBSYS_NAME		"pci-subsystem-name"
#define	PCI_VENDOR_NAME		"pci-vendor-name"
#define	PROCESSORS		"processors"
#define	PROC_BRAND		"processor-brand"
#define	PROC_FAMILY		"processor-family"
#define	PROC_MODEL		"processor-model"
#define	PROC_STEPPING		"processor-stepping"
#define	PSUS			"power-supplies"
#define	READING			"reading"
#define	SENSORS			"sensors"
#define	SERIAL			"serial-number"
#define	SERVICE_PROCESSOR	"service-processor"
#define	SPEED_RPM		"speed-in-rpm"
#define	SZ_BYTES		"size-in-bytes"
#define	STATE			"state"
#define	STATE_DESC		"state-description"
#define	THRESH_LCR		"threshold-lower-critical"
#define	THRESH_LNC		"threshold-lower-non-critical"
#define	THRESH_LNR		"threshold-lower-non-recoverable"
#define	THRESH_UCR		"threshold-upper-critical"
#define	THRESH_UNC		"threshold-upper-non-critical"
#define	THRESH_UNR		"threshold-upper-non-recoverable"
#define	TYPE			"type"
#define	UNITS			"units"

/*
 * Used as default property value for the case where the property is present
 * but the value is not known for some reason.
 */
#define	UNKNOWN			"unknown"

static int
dump_sensor_json(llist_t *node, void *arg)
{
	hwg_sensor_t *sensor = (hwg_sensor_t *)node;
	char buf[64];
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"%s\":\"%s\",", NAME, sensor->hwsen_name);
	(void) printf("\"%s\":\"%s\"", TYPE, sensor->hwsen_type);
	if (sensor->hwsen_hasstate == B_TRUE) {
		(void) printf(",\"%s\":%u,", STATE, sensor->hwsen_state);
		(void) printf("\"%s\":\"%s\"", STATE_DESC,
		    sensor->hwsen_state_descr);
	}
	if (sensor->hwsen_hasreading == B_TRUE) {
		(void) printf(",\"%s\":%lf,", READING, sensor->hwsen_reading);
		(void) printf("\"%s\":\"%s\"", UNITS, sensor->hwsen_units);
		if (sensor->hwsen_thresh_lnc.hnp_is_set)
			(void) printf(",\"%s\":%lf", THRESH_LNC,
			    sensor->hwsen_thresh_lnc.hnp_dbl);
		if (sensor->hwsen_thresh_lnc.hnp_is_set)
			(void) printf(",\"%s\":%lf", THRESH_LCR,
			    sensor->hwsen_thresh_lcr.hnp_dbl);
		if (sensor->hwsen_thresh_lnc.hnp_is_set)
			(void) printf(",\"%s\":%lf", THRESH_LNR,
			    sensor->hwsen_thresh_lnr.hnp_dbl);
		if (sensor->hwsen_thresh_lnc.hnp_is_set)
			(void) printf(",\"%s\":%lf", THRESH_UNC,
			    sensor->hwsen_thresh_unc.hnp_dbl);
		if (sensor->hwsen_thresh_lnc.hnp_is_set)
			(void) printf(",\"%s\":%lf", THRESH_UCR,
			    sensor->hwsen_thresh_ucr.hnp_dbl);
		if (sensor->hwsen_thresh_lnc.hnp_is_set)
			(void) printf(",\"%s\":%lf", THRESH_UNR,
			    sensor->hwsen_thresh_unr.hnp_dbl);
	}
	(void) printf("}");

	return (LL_WALK_NEXT);
}

static int
dump_led_json(llist_t *node, void *arg)
{
	hwg_led_t *led = (hwg_led_t *)node;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{ \"%s\":\"%s\",", TYPE, led->hwled_type);
	(void) printf("\"%s\":\"%s\"}", MODE, led->hwled_mode);
	return (LL_WALK_NEXT);
}

static void
dump_facilities_json(hwg_common_info_t *cinfo)
{
	boolean_t firstelem;

	firstelem = B_TRUE;
	(void) printf("\"%s\": [", SENSORS);
	if (cinfo->hwci_sensors.ll_next != NULL &&
	    llist_walker(&(cinfo->hwci_sensors), dump_sensor_json,
	    &firstelem) != 0) {
		hwg_error("error walking disk sensors\n");
		return;
	}
	(void) printf("],");

	(void) printf("\"%s\": [", LEDS);
	firstelem = B_TRUE;
	if (cinfo->hwci_leds.ll_next != NULL &&
	    llist_walker(&(cinfo->hwci_leds), dump_led_json,
	    &firstelem) != 0) {
		hwg_error("error walking disk leds\n");
		return;
	}
	(void) printf("]");
}

static void
dump_sp_json(hwg_sp_t *sp)
{
	char *rev = UNKNOWN;
	hwg_common_info_t *cinfo = &sp->hwsp_common_info;

	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf("\"%s\":{", SERVICE_PROCESSOR);
	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\":\"%s\",", FIRMWARE_REV, rev);

	(void) printf("\"%s\":\"%s\",", IPV4_ADDR,
	    sp->hwsp_ipv4_addr != NULL ?
	    sp->hwsp_ipv4_addr : "");

	(void) printf("\"%s\":\"%s\",", IPV4_SUBNET,
	    sp->hwsp_ipv4_subnet != NULL ?
	    sp->hwsp_ipv4_subnet : "");

	(void) printf("\"%s\":\"%s\",", IPV4_GATEWAY,
	    sp->hwsp_ipv4_gateway != NULL ?
	    sp->hwsp_ipv4_gateway : "");

	(void) printf("\"%s\":\"%s\"", IPV4_CFG_TYPE,
	    sp->hwsp_ipv4_config_type != NULL ?
	    sp->hwsp_ipv4_config_type : "");

	(void) printf("},\n\n");
}

static void
dump_motherboard_json(hwg_motherboard_t *mb)
{
	char *model = UNKNOWN, *manuf = UNKNOWN, *rev = UNKNOWN;
	hwg_common_info_t *cinfo = &mb->hwmbo_common_info;

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf("\"%s\":{", MOTHERBOARD);
	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\":\"%s\",", MANUF, manuf);
	(void) printf("\"%s\":\"%s\",", MODEL, model);
	(void) printf("\"%s\":\"%s\",", FIRMWARE_REV, rev);

	dump_facilities_json(cinfo);

	(void) printf("},\n\n");
}

static void
dump_chassis_json(hwg_chassis_t *chassis)
{
	char *model = UNKNOWN, *manuf = UNKNOWN;
	hwg_common_info_t *cinfo = &chassis->hwch_common_info;

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;

	(void) printf("\"%s\":{", CHASSIS);
	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\":\"%s\",", MANUF, manuf);
	(void) printf("\"%s\":\"%s\",", MODEL, model);

	dump_facilities_json(cinfo);

	(void) printf("},\n\n");
}

static int
dump_fan_json(llist_t *node, void *arg)
{
	hwg_fan_t *fan = (hwg_fan_t *)node;
	hwg_common_info_t *cinfo = &fan->hwfa_common_info;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"%s\":\"%s\",", LABEL, cinfo->hwci_label);
	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);

	dump_facilities_json(cinfo);

	(void) printf("}\n");

	return (LL_WALK_NEXT);
}

static int
dump_psu_json(llist_t *node, void *arg)
{
	char *model = UNKNOWN, *manuf = UNKNOWN, *rev = UNKNOWN;
	hwg_psu_t *psu = (hwg_psu_t *)node;
	hwg_common_info_t *cinfo = &psu->hwps_common_info;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"%s\":\"%s\",", LABEL, cinfo->hwci_label);

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\":\"%s\",", MANUF, manuf);
	(void) printf("\"%s\":\"%s\",", MODEL, model);
	(void) printf("\"%s\":\"%s\",", FIRMWARE_REV, rev);

	dump_facilities_json(cinfo);

	(void) printf("}\n");

	return (LL_WALK_NEXT);
}

static int
dump_processor_json(llist_t *node, void *arg)
{
	char *model = UNKNOWN, *manuf = UNKNOWN, *brand = UNKNOWN;
	char *label = UNKNOWN;
	hwg_processor_t *chip = (hwg_processor_t *)node;
	hwg_common_info_t *cinfo = &chip->hwpr_common_info;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	if (cinfo->hwci_label != NULL)
		label = cinfo->hwci_label;

	(void) printf("{\"%s\":\"%s\",", LABEL, label);

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (chip->hwpr_brand != NULL)
		brand = chip->hwpr_brand;

	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\":\"%s\",", MANUF, manuf);
	(void) printf("\"%s\":\"%s\",", MODEL, model);

	(void) printf("\"%s\":\"%s\",", PROC_BRAND, brand);
	(void) printf("\"%s\":\"%u\",", PROC_FAMILY, chip->hwpr_family);
	(void) printf("\"%s\":\"%u\",", PROC_MODEL, chip->hwpr_model);
	(void) printf("\"%s\":\"%u\",", PROC_STEPPING, chip->hwpr_stepping);
	(void) printf("\"%s\":%u,", NUM_CORES, chip->hwpr_num_cores);
	(void) printf("\"%s\":%u,", NUM_THREADS,
	    (chip->hwpr_num_threads / chip->hwpr_num_cores));

	dump_facilities_json(cinfo);

	(void) printf("}\n");

	return (LL_WALK_NEXT);
}

static int
dump_pcidev_json(llist_t *node, void *arg)
{
	char *vendor = UNKNOWN, *devnm = UNKNOWN, *subsysnm = UNKNOWN;
	hwg_pcidev_t *pcidev = (hwg_pcidev_t *)node;
	hwg_common_info_t *cinfo = &pcidev->hwpci_common_info;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"%s\":\"%s\",", LABEL, cinfo->hwci_label);

	if (pcidev->hwpci_vendor != NULL)
		vendor = pcidev->hwpci_vendor;
	if (pcidev->hwpci_devnm != NULL)
		devnm = pcidev->hwpci_devnm;
	if (pcidev->hwpci_subsysnm != NULL)
		subsysnm = pcidev->hwpci_subsysnm;

	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\":\"%s\",", PCI_VENDOR_NAME, vendor);
	(void) printf("\"%s\":\"%s\",", PCI_DEVICE_NAME, devnm);
	(void) printf("\"%s\":\"%s\",", PCI_SUBSYS_NAME, subsysnm);
	(void) printf("\"%s\":\"%s\",", DRIVERNM, pcidev->hwpci_drivernm);
	if (pcidev->hwpci_driverinst.hnp_is_set)
		(void) printf("\"%s\":%u,", DRIVERINST,
		    pcidev->hwpci_driverinst.hnp_u32);
	(void) printf("\"%s\":\"%s\"", DEVICE_PATH, pcidev->hwpci_devpath);

	(void) printf("}\n");

	return (LL_WALK_NEXT);
}

static void
dump_disk_json(hwg_disk_t *disk)
{
	char *model = UNKNOWN, *manuf = UNKNOWN, *rev = UNKNOWN;
	char *serial = UNKNOWN;
	hwg_common_info_t *cinfo = &disk->hwdk_common_info;
	boolean_t firstelem;

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_serial != NULL)
		serial = cinfo->hwci_serial;
	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf(",\"%s\": {", DISK);
	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\": \"%s\",", MANUF, manuf);
	(void) printf("\"%s\":\"%s\",", MODEL, model);
	(void) printf("\"%s\":\"%s\",", SERIAL, serial);
	(void) printf("\"%s\":\"%s\",", FIRMWARE_REV, rev);
	(void) printf("\"%s\":%u,", SZ_BYTES, disk->hwdk_size);
	if (disk->hwdk_speed.hnp_is_set)
		(void) printf("\"%s\":%u,", SPEED_RPM,
		    disk->hwdk_speed.hnp_u32);

	dump_facilities_json(cinfo);

	(void) printf("}\n");
}

static int
dump_bay_json(llist_t *node, void *arg)
{
	hwg_disk_bay_t *bay = (hwg_disk_bay_t *)node;
	hwg_common_info_t *cinfo = &bay->hwdkb_common_info;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{ \"%s\":\"%s\",", LABEL,
	    bay->hwdkb_common_info.hwci_label);
	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);

	dump_facilities_json(cinfo);

	if (bay->hwdkb_present == B_TRUE)
		dump_disk_json(bay->hwdkb_disk);

	(void) printf("}\n");
	return (LL_WALK_NEXT);
}

static void
dump_dimm_json(hwg_dimm_t *dimm)
{
	char *manuf = UNKNOWN, *part = UNKNOWN, *serial = UNKNOWN;
	hwg_common_info_t *cinfo = &dimm->hwdi_common_info;
	boolean_t firstelem;

	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_part != NULL)
		part = cinfo->hwci_part;
	if (cinfo->hwci_serial != NULL)
		serial = cinfo->hwci_serial;

	(void) printf("\"%s\":{", DIMM);
	(void) printf("\"%s\":\"%s\",", HC_FMRI, cinfo->hwci_fmri);
	(void) printf("\"%s\":\"%s\",", MANUF, manuf);
	(void) printf("\"%s\":\"%s\",", PARTNO, part);
	(void) printf("\"%s\":\"%s\",", SERIAL, serial);
	(void) printf("\"%s\":\"%s\",", TYPE, dimm->hwdi_type);
	(void) printf("\"%s\":\"%llu\",", SZ_BYTES, dimm->hwdi_size);

	dump_facilities_json(cinfo);

	(void) printf("}");
}

static int
dump_dimm_slot_json(llist_t *node, void *arg)
{
	hwg_dimm_slot_t *slot = (hwg_dimm_slot_t *)node;
	hwg_common_info_t *cinfo = &slot->hwds_common_info;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{ \"%s\":\"%s\",", LABEL,
	    slot->hwds_common_info.hwci_label);
	(void) printf("\"%s\":\"%s\"", HC_FMRI, cinfo->hwci_fmri);

	if (slot->hwds_present == B_TRUE) {
		(void) printf(",");
		dump_dimm_json(slot->hwds_dimm);
	}
	(void) printf(" }\n");

	return (LL_WALK_NEXT);
}

int
dump_hw_config_json(hwg_info_t *hwinfo, char *type)
{
	boolean_t firstelem;

	(void) printf("{\n");

	dump_chassis_json(hwinfo->hwi_chassis);

	if (hwinfo->hwi_sp != NULL)
		dump_sp_json(hwinfo->hwi_sp);

	dump_motherboard_json(hwinfo->hwi_motherboard);

	(void) printf("\n\"%s\": [\n", PROCESSORS);

	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_processors), dump_processor_json,
	    &firstelem) != 0) {
		hwg_error("error walking processors\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"%s\": [\n", MEMORY);

	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_dimm_slots), dump_dimm_slot_json,
	    &firstelem) != 0) {
		hwg_error("error walking slots\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"%s\": [\n", PCI_DEVICES);

	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_pcidevs), dump_pcidev_json,
	    &firstelem) != 0) {
		hwg_error("error walking slots\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"%s\": [\n", DRIVE_BAYS);
	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_disk_bays), dump_bay_json,
	    &firstelem) != 0) {
		hwg_error("error walking bays\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"%s\": [\n", PSUS);
	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_psus), dump_psu_json,
	    &firstelem) != 0) {
		hwg_error("error walking psus\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"%s\": [\n", FANS);
	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_fans), dump_fan_json,
	    &firstelem) != 0) {
		hwg_error("error walking fans\n");
		return (-1);
	} else {
		(void) printf("]\n");
	}

	(void) printf("}\n");
}
