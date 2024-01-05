/*
 * Copyright (c) 2018 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

static uint32_t count;

/*Technonia*/
#define LCDBL_NODE	

static struct gpio_dt_spec sd_lcd_bl = GPIO_DT_SPEC_GET(DT_ALIAS(backlight), gpios);
static struct gpio_dt_spec spi_clk = GPIO_DT_SPEC_GET(DT_ALIAS(test_spi), gpios);

#ifdef CONFIG_GPIO
static struct gpio_dt_spec button_gpio = GPIO_DT_SPEC_GET_OR(
		DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_callback;

static void button_isr_callback(const struct device *port,
				struct gpio_callback *cb,
				uint32_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	count = 0;
}
#endif

/*
static void ofs_y_anim(void * img, int32_t v)
{
    lv_img_set_offset_y(img, v);
}
*/

/**
 * Image styling and offset
 */
/*
void lv_example_img_4(void)
{
    LV_IMG_DECLARE(img_skew_strip);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_style_set_img_recolor(&style, lv_color_black());

    lv_obj_t * img = lv_img_create(lv_scr_act());
    lv_obj_add_style(img, &style, 0);
    lv_img_set_src(img, &img_skew_strip);
    lv_obj_set_size(img, 150, 100);
    lv_obj_center(img);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);
    lv_anim_set_exec_cb(&a, ofs_y_anim);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, 3000);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);

}
*/
void lv_example_style_6(void)
{
    static lv_style_t style;
    lv_style_init(&style);

    /*Set a background color and a radius*/
    lv_style_set_radius(&style, 5);
    lv_style_set_bg_opa(&style, LV_OPA_COVER);
    lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_GREY, 3));
    lv_style_set_border_width(&style, 2);
    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_img_recolor(&style, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_img_recolor_opa(&style, LV_OPA_50);
    lv_style_set_transform_angle(&style, 300);

    /*Create an object with the new style*/
    lv_obj_t * obj = lv_img_create(lv_scr_act());
    lv_obj_add_style(obj, &style, 0);

    LV_IMG_DECLARE(img_cogwheel_argb);
    lv_img_set_src(obj, &img_cogwheel_argb);

    lv_obj_center(obj);
}

void main(void)
{
	int err;
	char count_str[11] = {0};
	const struct device *display_dev;
	lv_obj_t *hello_world_label;
	lv_obj_t *count_label;
	
	/*
	static lv_style_t style;

    lv_style_init(&style);

	style.text.font = &lv_font_montserrat_42;
	*/

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return;
	}
	display_blanking_off(display_dev);

	if(device_is_ready(sd_lcd_bl.port)) {
		// err = gpio_pin_configure_dt(&sd_lcd_bl, GPIO_OUTPUT_ACTIVE);
		err = gpio_pin_configure_dt(&sd_lcd_bl, GPIO_OUTPUT);
		if (err) {
			LOG_ERR("failed to configure lcd backlight gpio: %d", err);
			return;
		}
		// gpio_pin_set_dt(&sd_lcd_bl, 1);
		gpio_pin_set_dt(&sd_lcd_bl, 0);
	}

	if (device_is_ready(spi_clk.port)) {
		err = gpio_pin_configure_dt(&spi_clk, GPIO_OUTPUT);
		if (err) {
			LOG_ERR("failed to configure spi clk gpio: %d", err);
			return;
		}
		gpio_pin_set_dt(&spi_clk, 1);
	}

#ifdef CONFIG_GPIO
	if (device_is_ready(button_gpio.port)) {
		err = gpio_pin_configure_dt(&button_gpio, GPIO_INPUT);
		if (err) {
			LOG_ERR("failed to configure button gpio: %d", err);
			return;
		}

		gpio_init_callback(&button_callback, button_isr_callback,
				   BIT(button_gpio.pin));

		err = gpio_add_callback(button_gpio.port, &button_callback);
		if (err) {
			LOG_ERR("failed to add button callback: %d", err);
			return;
		}

		err = gpio_pin_interrupt_configure_dt(&button_gpio,
						      GPIO_INT_EDGE_TO_ACTIVE);
		if (err) {
			LOG_ERR("failed to enable button callback: %d", err);
			return;
		}
	}
#endif

	if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN)) {
		lv_obj_t *hello_world_button;

		hello_world_button = lv_btn_create(lv_scr_act());
		lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
		hello_world_label = lv_label_create(hello_world_button);
	} else {
		hello_world_label = lv_label_create(lv_scr_act());
	}

	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x0041df), LV_PART_MAIN);
	lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
	lv_obj_set_style_text_font(hello_world_label, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_label_set_text(hello_world_label, "Hello world!");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	count_label = lv_label_create(lv_scr_act());
	/*
	lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);
	*/
	lv_obj_set_pos(count_label, 30 ,100);
	lv_obj_set_style_text_font(count_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

	int res = gpio_pin_set_dt(&sd_lcd_bl, 1);
	printk("gpio_pin_set_dt: %d\n", res);

	lv_task_handler();

	int value = 0;

	while (1) {
		if ((count % 10) == 0U) {
			sprintf(count_str, "%d", count/10U);
			lv_label_set_text(count_label, count_str);
			gpio_pin_set_dt(&sd_lcd_bl, value);
			value = !value;
		}
		lv_task_handler();
		++count;
		k_sleep(K_MSEC(100));
	}


	// lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x0041df), LV_PART_MAIN);
	// lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
	// lv_obj_set_style_text_font(hello_world_label, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_DEFAULT);

	// lv_label_set_text(hello_world_label, "Hello world!");
	// lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	// count_label = lv_label_create(lv_scr_act());

	// // lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	// lv_obj_set_pos(count_label, 30 ,100);
	// lv_obj_set_style_text_font(count_label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

	// lv_task_handler();
	// display_blanking_off(display_dev);

	// gpio_pin_set_dt(&sd_lcd_bl, 1);

	// while (1) {
	// 	if ((count % 100) == 0U) {
	// 		sprintf(count_str, "%d", count/100U);
	// 		printk("count: %s\n", count_str);
	// 		lv_label_set_text(count_label, count_str);
	// 	}
	// 	lv_task_handler();
	// 	++count;
	// 	k_sleep(K_MSEC(10));
	// }
}
