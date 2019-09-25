/*
 * main_ds_twr.c

 *
 *  Created on: 16 juil. 2019
 *      Author: Noolitic
 */
#include <stdio.h>
#include <string.h>
#include "deca_device_api.h"
#include "device.h"
#include "anch_dev.h"
#include "tag_dev.h"
#include "main.h"
#include "LoRadriver/LoRadriver.h"


int main_ds_twr (int init) {

	char debug[50];
	dw_device_t * dev = get_device();
	int role = ANCHOR;

#if LORA
	if (init == 1) {
		// if lora is connected, the device is a tag
		int lora_active = 0;
		lora_active = isLoraConnected();
		sprintf(debug, "[LORA] %s", lora_active ? "ACTIVATED" : "DISACTIVATED");
		println(debug);
		role = (lora_active ? TAG : ANCHOR);
	} else {
		role = ANCHOR;
	}
#else
	uint32 partid;
	partid = _dwt_otpread(PARTID_ADDRESS);
	if ((partid &0xffff) == 0x11B6) {
		role = TAG;
	} else {
		role = ANCHOR;
	}
#endif

	if (init == 1) {
		sprintf(debug, "********* %s *********", (role == TAG ? "TAG" : "ANCHOR"));
		println(debug);
		sprintf(debug, "[LPL] %s", (LPL_MODE == 1 ? "ACTIVATED" : "DISACTIVATED"));
		println(debug);
	}
	init_config(role, DWT_BR_110K, init);
	if (init) {
		println("init finished");
	}
	if (role == TAG) {
#if LORA
		init_lora();
#endif
		tag_dev();
	} else {
		print("a");
		anch_dev(init);
	}
	return 1;
}
