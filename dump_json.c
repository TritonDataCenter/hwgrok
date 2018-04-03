/*
 * Copyright (c) 2018, Joyent, Inc.
 */
#include "hwgrok.h"
static int
dump_sensor_json(llist_t *node, void *arg)
{
	hwg_sensor_t *sensor = (hwg_sensor_t *)node;
	char buf[64];
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"name\":\"%s\",", sensor->hwsen_name);
	(void) printf("\"type\":\"%s\"", sensor->hwsen_type);
	if (sensor->hwsen_hasstate == B_TRUE) {
		(void) printf(",\"state\":%u,", sensor->hwsen_state);
		(void) printf("\"state-description\":\"%s\"",
		    sensor->hwsen_state_descr);
	}
	if (sensor->hwsen_hasreading == B_TRUE) {
		(void) printf(",\"reading\":%lf,", sensor->hwsen_reading);
		(void) printf("\"units\":\"%s\"", sensor->hwsen_units);
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

	(void) printf("{ \"type\":\"%s\",", led->hwled_type);
	(void) printf("\"mode\":\"%s\"}", led->hwled_mode);
	return (LL_WALK_NEXT);
}

static void
dump_facilities_json(hwg_common_info_t *cinfo)
{
	boolean_t firstelem;

	firstelem = B_TRUE;
	(void) printf("\"sensors\": [");
	if (cinfo->hwci_sensors.ll_next != NULL &&
	    llist_walker(&(cinfo->hwci_sensors), dump_sensor_json,
	    &firstelem) != 0) {
		(void) fprintf(stderr, "error walking disk sensors\n");
		return;
	}
	(void) printf("],");

	(void) printf("\"leds\": [");
	firstelem = B_TRUE;
	if (cinfo->hwci_leds.ll_next != NULL &&
	    llist_walker(&(cinfo->hwci_leds), dump_led_json,
	    &firstelem) != 0) {
		(void) fprintf(stderr, "error walking disk leds\n");
		return;
	}
	(void) printf("]");
}

static void
dump_sp_json(hwg_sp_t *sp)
{
	char *rev = "unknown";
	hwg_common_info_t *cinfo = &(sp->hwsp_common_info);

	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf("\"service-processor\":{");
	(void) printf("\"firmware-revision\":\"%s\",", rev);

	(void) printf("\"ipv4-address\":\"%s\",",
	    sp->hwsp_ipv4_addr != NULL ?
	    sp->hwsp_ipv4_addr : "");

	(void) printf("\"ipv4-subnet\":\"%s\",",
	    sp->hwsp_ipv4_subnet != NULL ?
	    sp->hwsp_ipv4_subnet : "");

	(void) printf("\"ipv4-gateway\":\"%s\",",
	    sp->hwsp_ipv4_gateway != NULL ?
	    sp->hwsp_ipv4_gateway : "");

	(void) printf("\"ipv4-config-type\":\"%s\"",
	    sp->hwsp_ipv4_config_type != NULL ?
	    sp->hwsp_ipv4_config_type : "");

	(void) printf("},\n\n");
}

static void
dump_motherboard_json(hwg_motherboard_t *mb)
{
	char *model = "unknown", *manuf = "unknown", *rev = "unknown";
	hwg_common_info_t *cinfo = &(mb->hwmbo_common_info);

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf("\"motherboard\":{");
	(void) printf("\"manufacturer\":\"%s\",", manuf);
	(void) printf("\"model\":\"%s\",", model);
	(void) printf("\"firmware-revision\":\"%s\",", rev);

	dump_facilities_json(cinfo);

	(void) printf("},\n\n");
}

static void
dump_chassis_json(hwg_chassis_t *chassis)
{
	char *model = "unknown", *manuf = "unknown";
	hwg_common_info_t *cinfo = &(chassis->hwch_common_info);

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;

	(void) printf("\"chassis\":{");
	(void) printf("\"manufacturer\":\"%s\",", manuf);
	(void) printf("\"model\":\"%s\",", model);

	dump_facilities_json(cinfo);

	(void) printf("},\n\n");
}

static int
dump_fan_json(llist_t *node, void *arg)
{
	hwg_fan_t *fan = (hwg_fan_t *)node;
	hwg_common_info_t *cinfo = &(fan->hwfa_common_info);
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"label\":\"%s\",", cinfo->hwci_label);

	dump_facilities_json(cinfo);

	(void) printf("}\n");

	return (LL_WALK_NEXT);
}

static int
dump_psu_json(llist_t *node, void *arg)
{
	char *model = "unknown", *manuf = "unknown", *rev = "unknown";
	hwg_psu_t *psu = (hwg_psu_t *)node;
	hwg_common_info_t *cinfo = &(psu->hwps_common_info);
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"label\":\"%s\",", cinfo->hwci_label);

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf("\"manufacturer\":\"%s\",", manuf);
	(void) printf("\"model\":\"%s\",", model);
	(void) printf("\"firmware-revision\":\"%s\",", rev);

	dump_facilities_json(cinfo);

	(void) printf("}\n");

	return (LL_WALK_NEXT);
}

static int
dump_processor_json(llist_t *node, void *arg)
{
	char *model = "unknown", *manuf = "unknown";
	hwg_processor_t *chip = (hwg_processor_t *)node;
	hwg_common_info_t *cinfo = &(chip->hwpr_common_info);
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{\"label\":\"%s\",", cinfo->hwci_label);

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;

	(void) printf("\"manufacturer\":\"%s\",", manuf);
	(void) printf("\"model\":\"%s\",", model);

	(void) printf("\"num-cores\":%u,", chip->hwpr_num_cores);
	(void) printf("\"num-threads-per-core\":%u,",
	    (chip->hwpr_num_threads / chip->hwpr_num_cores));

	dump_facilities_json(cinfo);

	(void) printf("}\n");

	return (LL_WALK_NEXT);
}

static void
dump_disk_json(hwg_disk_t *disk)
{
	char *model = "unknown", *manuf = "unknown", *rev = "unknown";
	char *serial = "unknown";
	hwg_common_info_t *cinfo = &(disk->hwdk_common_info);
	boolean_t firstelem;

	if (cinfo->hwci_model != NULL)
		model = cinfo->hwci_model;
	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_serial != NULL)
		serial = cinfo->hwci_serial;
	if (cinfo->hwci_version != NULL)
		rev = cinfo->hwci_version;

	(void) printf(",\"disk\": {");
	(void) printf("\"manufacturer\": \"%s\",", manuf);
	(void) printf("\"model\":\"%s\",", model);
	(void) printf("\"serial\":\"%s\",", serial);
	(void) printf("\"firmware-revision\":\"%s\",", rev);
	(void) printf("\"size-in-bytes\":%u,", disk->hwdk_size);
	(void) printf("\"speed-in-rpm\":%u,", disk->hwdk_speed);

	dump_facilities_json(cinfo);

	(void) printf("}\n");
}

static int
dump_bay_json(llist_t *node, void *arg)
{
	hwg_disk_bay_t *bay = (hwg_disk_bay_t *)node;
	hwg_common_info_t *cinfo = &(bay->hwdkb_common_info);
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{ \"label\":\"%s\",",
	    bay->hwdkb_common_info.hwci_label);

	dump_facilities_json(cinfo);

	if (bay->hwdkb_present == B_TRUE)
		dump_disk_json(bay->hwdkb_disk);

	(void) printf("}\n");
	return (LL_WALK_NEXT);
}

static void
dump_dimm_json(hwg_dimm_t *dimm)
{
	char *manuf = "unknown", *part = "unknown", *serial = "unknown";
	hwg_common_info_t *cinfo = &(dimm->hwdi_common_info);
	boolean_t firstelem;

	if (cinfo->hwci_manufacturer != NULL)
		manuf = cinfo->hwci_manufacturer;
	if (cinfo->hwci_part != NULL)
		part = cinfo->hwci_part;
	if (cinfo->hwci_serial != NULL)
		serial = cinfo->hwci_serial;

	(void) printf("\"dimm\":{");
	(void) printf("\"manufacturer\":\"%s\",", manuf);
	(void) printf("\"part\":\"%s\",", part);
	(void) printf("\"serial\":\"%s\",", serial);
	(void) printf("\"type\":\"%s\",", dimm->hwdi_type);
	(void) printf("\"size-in-bytes\":\"%llu\",", dimm->hwdi_size);

	dump_facilities_json(cinfo);

	(void) printf("}");
}

static int
dump_dimm_slot_json(llist_t *node, void *arg)
{
	hwg_dimm_slot_t *slot = (hwg_dimm_slot_t *)node;
	boolean_t firstelem = *(boolean_t *)arg;

	if (firstelem == B_FALSE)
		(void) printf(",");
	*(boolean_t *)arg = B_FALSE;

	(void) printf("{ \"label\":\"%s\",",
	    slot->hwds_common_info.hwci_label);

	if (slot->hwds_present == B_TRUE)
		dump_dimm_json(slot->hwds_dimm);
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

	(void) printf("\n\"processors\": [\n");

	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_processors), dump_processor_json,
	    &firstelem) != 0) {
		(void) fprintf(stderr, "error walking processors\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"memory\": [\n");

	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_dimm_slots), dump_dimm_slot_json,
	    &firstelem) != 0) {
		(void) fprintf(stderr, "error walking slots\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"drive-bays\": [\n");
	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_disk_bays), dump_bay_json,
	    &firstelem) != 0) {
		(void) fprintf(stderr, "error walking bays\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"power-supplies\": [\n");
	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_psus), dump_psu_json,
	    &firstelem) != 0) {
		(void) fprintf(stderr, "error walking psus\n");
		return (-1);
	} else {
		(void) printf("],\n");
	}

	(void) printf("\n\"fans\": [\n");
	firstelem = B_TRUE;
	if (llist_walker(&(hwinfo->hwi_fans), dump_fan_json,
	    &firstelem) != 0) {
		(void) fprintf(stderr, "error walking fans\n");
		return (-1);
	} else {
		(void) printf("]\n");
	}

	(void) printf("}\n");
}
