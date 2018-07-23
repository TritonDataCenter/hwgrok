/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2018, Joyent, Inc.
 */
#ifndef	_HWGROK_H
#define	_HWGROK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fm/libtopo.h>
#include <fm/topo_hc.h>
#include <fm/topo_list.h>
#include <sys/fm/protocol.h>
#include <sys/varargs.h>

#include "llist.h"

extern void hwg_debug(const char *, ...);
extern void hwg_error(const char *, ...);

typedef struct hwg_common_info {
	char *hwci_manufacturer;
	char *hwci_model;
	char *hwci_part;
	char *hwci_serial;
	char *hwci_label;
	char *hwci_fmristr;
	char *hwci_version;
	llist_t hwci_sensors;
	llist_t hwci_leds;
	llist_t hwci_active_cases;
} hwg_common_info_t;

typedef struct hwg_sensor {
	struct llist *ll_prev;
	struct llist *ll_next;
	char *hwsen_name;
	char *hwsen_type;
	char *hwsen_units;
	uint32_t hwsen_state;
	char *hwsen_state_descr;
	boolean_t hwsen_hasreading;
	boolean_t hwsen_hasstate;
	double hwsen_reading;
	double hwsen_thresh_lnc;
	double hwsen_thresh_lcr;
	double hwsen_thresh_lnr;
	double hwsen_thresh_unc;
	double hwsen_thresh_ucr;
	double hwsen_thresh_unr;
} hwg_sensor_t;

typedef struct hwg_led {
	struct llist *ll_prev;
	struct llist *ll_next;
	char *hwled_type;
	char *hwled_mode;
} hwg_led_t;

typedef struct hwg_processor {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwpr_common_info;
	uint_t hwpr_num_cores;
	uint_t hwpr_num_threads;
	uint_t hwpr_speed;
} hwg_processor_t;

typedef struct hwg_dimm {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwdi_common_info;
	uint64_t hwdi_size;
	char *hwdi_type;
} hwg_dimm_t;

typedef struct hwg_dimm_slot {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwds_common_info;
	char *hwds_formfactor;
	hwg_dimm_t *hwds_dimm;
	boolean_t hwds_present;
} hwg_dimm_slot_t;

typedef struct hwg_disk {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwdk_common_info;
	uint64_t hwdk_size;
	uint32_t hwdk_speed;
} hwg_disk_t;

typedef struct hwg_disk_bay {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwdkb_common_info;
	hwg_disk_t *hwdkb_disk;
	boolean_t hwdkb_present;
} hwg_disk_bay_t;

typedef struct hwg_pcidev {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwpci_common_info;
	char *hwpci_vendor;
	char *hwpci_devnm;
	char *hwpci_subsysnm;
	char *hwpci_devpath;
	char *hwpci_drivernm;
	uint_t hwpci_driverinst;
	boolean_t hwpci_is_onboard;
} hwg_pcidev_t;

typedef struct hwg_psu {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwps_common_info;
} hwg_psu_t;

typedef struct hwg_fan {
	struct llist *ll_prev;
	struct llist *ll_next;
	hwg_common_info_t hwfa_common_info;
} hwg_fan_t;

typedef struct hwg_motherboard {
	hwg_common_info_t hwmbo_common_info;
} hwg_motherboard_t;

typedef struct hwg_sp {
	hwg_common_info_t hwsp_common_info;
	char *hwsp_macaddr;
	char *hwsp_ipv4_addr;
	char *hwsp_ipv4_subnet;
	char *hwsp_ipv4_gateway;
	char *hwsp_ipv4_config_type;
	char *hwsp_vlan_id;
	char *hwsp_ipv6_addr;
	char *hwsp_ipv6_config_type;
} hwg_sp_t;


typedef struct hwg_chassis {
	hwg_common_info_t hwch_common_info;
} hwg_chassis_t;

typedef struct hwg_info {
	llist_t hwi_dimm_slots;
	llist_t hwi_disk_bays;
	llist_t hwi_processors;
	llist_t hwi_pcidevs;
	llist_t hwi_dimms;
	llist_t hwi_disks;
	llist_t hwi_psus;
	llist_t hwi_fans;
	hwg_motherboard_t *hwi_motherboard;
	hwg_chassis_t *hwi_chassis;
	hwg_sp_t *hwi_sp;
} hwg_info_t;

typedef struct hwg_cbarg {
	hwg_info_t *cb_hw_info;
	char *cb_nodename;
	int cb_nodeinst;
	hwg_disk_bay_t *cb_currbay;
	hwg_dimm_slot_t *cb_currslot;
	hwg_processor_t *cb_currchip;
	hwg_pcidev_t *cb_currdev;
} hwg_cbarg_t;

#endif	/* _HW_GROK_H */
