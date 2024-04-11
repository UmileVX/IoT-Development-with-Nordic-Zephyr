/*
 * Copyright (c) 2020 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include "led.h"

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

// add for joystick
int32_t preX = 0 , perY = 0;
static const int AXIS_DEVIATION = 511;
static const int ADC_MAX = 1023;
static const int ADC_MIN = 0;
int mid = ADC_MAX / 2;
int32_t nowX = 0;
int32_t nowY = 0;

bool isChange(void)
{
	if((nowX < (preX - 50)) || nowX > (preX+50)){
		preX = nowX;
		return true;
	}

	if((nowY < (perY - 50)) || nowY > (perY+50)){
		perY = nowY;
		return true;
	}
}
//endif
int main(void)
{
	int err;
	uint32_t count = 0;
	uint16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

	/* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!adc_is_ready_dt(&adc_channels[i])) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}

	led_init();

	while (1) {
		printk("ADC reading[%u]: ", count++);

		(void)adc_sequence_init_dt(&adc_channels[0], &sequence);
		err = adc_read(adc_channels[0].dev, &sequence);
		if (err < 0) {
			printk("Could not read (%d)\n", err);
			continue;
		}

		nowX = (int32_t)buf;

		(void)adc_sequence_init_dt(&adc_channels[1], &sequence);
		err = adc_read(adc_channels[1].dev, &sequence);
		if (err < 0) {
			printk("Could not read (%d)\n", err);
			continue;
		}

		nowY = (int32_t)buf;

        printk("Joy X: %" PRIu32 ", ", nowX);
		printk("Joy Y: %" PRIu32 ", ", nowY);

		if (nowX >= 65500 || nowY >= 65500){
			printk("Out of Range\n");
			k_sleep(K_MSEC(100));
			continue;
		}

		bool checkFlag = isChange();
		if(!checkFlag){
                        printk("No Change\n");
			k_sleep(K_MSEC(100));
			continue;
		} else {
			led_off_all();
		}

		if (nowX == ADC_MAX && nowY == ADC_MAX){
			led_on_center();
			printk("Center");
		} else if (nowX < mid && nowY == ADC_MAX){
			led_on_left();
			printk("Left");
		} else if (nowX > mid && nowY == ADC_MAX){
			led_on_right();
			printk("Right");
		} else if (nowY > mid && nowX == ADC_MAX){
			led_on_up();
			printk("Up");
		} else if (nowY < mid && nowX == ADC_MAX){
			led_on_down();
			printk("Down");
		}

                printk("\n");

		k_sleep(K_MSEC(100));
	}
	return 0;
}
