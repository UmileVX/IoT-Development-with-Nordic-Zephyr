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

#if !defined(DEV_PWM_LED)
	int ret = check_and_configure_gpios();
	if (ret < 0) {
		return 0;
	}
#else
	int ret = check_and_configure_pwm();
	if (ret < 0) {
		return 0;
	}
#endif

	while (1) {
#if !defined(DEV_PWM_LED)
		check_button0();
		check_button1();
		check_button2();
		check_button3();
		k_msleep(SLEEP_TIME_MS);
#else
		int ret = run_pwm();
		if (ret < 0) {
			return 0;
		}
		k_msleep(SLEEP_TIME_MS);
#endif
	}
	return 0;
}
