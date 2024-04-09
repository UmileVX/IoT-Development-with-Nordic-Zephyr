/*
 * Copyright (c) 2022 Valerio Setti <valerio.setti@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

#include "led.h"

#if !DT_NODE_EXISTS(DT_ALIAS(qdec0))
#error "Unsupported board: qdec0 devicetree alias is not defined"
#endif

#define SW_NODE DT_NODELABEL(gpiosw)
#if !DT_NODE_HAS_STATUS(SW_NODE, okay)
#error "Unsupported board: gpiosw devicetree alias is not defined or enabled"
#endif
static const struct gpio_dt_spec sw = GPIO_DT_SPEC_GET(SW_NODE, gpios);

static bool sw_led_flag = false;
static int rotary_idx = 0;

static struct gpio_callback sw_cb_data;

void sw_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("SW pressed\n");
	sw_led_flag = !sw_led_flag;
}

void display_rotary_led(int32_t rotary_val)
{
	if(rotary_val > 0){
		rotary_idx++;
	} else if(rotary_val < 0){
		rotary_idx--;
	}

	if(rotary_idx < 0){
		rotary_idx = MAX_ROTARY_IDX -1;
	} else if (rotary_idx > MAX_ROTARY_IDX -1) {
		rotary_idx = 0;
	}

	led_on_idx(rotary_idx);
}

int main(void)
{
	struct sensor_value val;
	int rc;
	const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(qdec0));

	if (!device_is_ready(dev)) {
		printk("Qdec device is not ready\n");
		return 0;
	}

	if(!gpio_is_ready_dt(&sw)) {
		printk("SW GPIO is not ready\n");
		return 0;
	}

	int err = gpio_pin_configure_dt(&sw, GPIO_INPUT);
	if(err < 0){
		printk("Error configuring SW GPIO pin %d\n", err);
		return 0;
	}

	err = gpio_pin_interrupt_configure_dt(&sw, GPIO_INT_EDGE_RISING);
	if(err != 0){
		printk("Error configuring SW GPIO interrupt %d\n", err);
		return 0;
	}

	gpio_init_callback(&sw_cb_data, sw_callback, BIT(sw.pin));
	gpio_add_callback(sw.port, &sw_cb_data);

	printk("Quadrature decoder sensor test\n");

	led_on_idx(rotary_idx);

	while (true) {
		rc = sensor_sample_fetch(dev);
		if (rc != 0) {
			printk("Failed to fetch sample (%d)\n", rc);
			return 0;
		}

		rc = sensor_channel_get(dev, SENSOR_CHAN_ROTATION, &val);
		if (rc != 0) {
			printk("Failed to get data (%d)\n", rc);
			return 0;
		}

		// int sw_val = gpio_pin_get_dt(&sw);

		if(!sw_led_flag){
			display_rotary_led(val.val1);
		} else{
			led_off_all();
		}

		printk("Position = %d degrees \n", val.val1);

		k_msleep(100);
	}
	return 0;
}
