/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include "gpios.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000


int main(void) {
	printk("Hello World! %s\n", CONFIG_BOARD);
	int ret = check_and_configure_gpios();
	if (ret < 0) {
		return 0;
	}

	while (1) {
		check_button0();
		check_button1();
		check_button2();
		check_button3();
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
