#include <zephyr/sys/util.h>

#include "gpios.h"

static struct gpio_callback button0_cb_data;
static struct gpio_callback button1_cb_data;
static struct gpio_callback button2_cb_data;
static struct gpio_callback button3_cb_data;


void check_button0() {
    int val = gpio_pin_get_dt(&button0);
    int ret;
    if (val > 0) {
        ret = gpio_pin_toggle_dt(&led0);
        if (ret < 0) {
            return;
        }
    }
}

void check_button1() {
    int val = gpio_pin_get_dt(&button1);
    int ret;
    if (val > 0) {
        ret = gpio_pin_toggle_dt(&led1);
        if (ret < 0) {
            return;
        }
    }
}

void check_button2() {
    int val = gpio_pin_get_dt(&button2);
    int ret;
    if (val > 0) {
        ret = gpio_pin_toggle_dt(&led2);
        if (ret < 0) {
            return;
        }
    }
}

void check_button3() {
    int val = gpio_pin_get_dt(&button3);
    int ret;
    if (val > 0) {
        ret = gpio_pin_toggle_dt(&led3);
        if (ret < 0) {
            return;
        }
    }
}


int check_and_configure_gpios() {
    bool ret;
    int conf_ret;

    /* Check if all leds are ready */

    ret = gpio_is_ready_dt(&led0);
    if (!ret) {
        printk("GPIO LED0 is not ready\n");
		return -1;
	}
    conf_ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure LED0 pin\n", conf_ret);
        return -1;
    }

    ret = gpio_is_ready_dt(&led1);
    if (!ret) {
        printk("GPIO LED1 is not ready\n");
        return -1;
    }
    conf_ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure LED1 pin\n", conf_ret);
        return -1;
    }

    ret = gpio_is_ready_dt(&led2);
    if (!ret) {
        printk("GPIO LED2 is not ready\n");
        return -1;
    }
    conf_ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure LED2 pin\n", conf_ret);
        return -1;
    }

    ret = gpio_is_ready_dt(&led3);
    if (!ret) {
        printk("GPIO LED3 is not ready\n");
        return -1;
    }
    conf_ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure LED3 pin\n", conf_ret);
        return -1;
    }

    /* Check if all buttons are ready */

    ret = gpio_is_ready_dt(&button0);
    if (!ret) {
        printk("GPIO BUTTON0 is not ready\n");
        return -1;
    }
    conf_ret = gpio_pin_configure_dt(&button0, GPIO_INPUT);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure BUTTON0 pin\n", conf_ret);
        return -1;
    }

    ret = gpio_is_ready_dt(&button1);
    if (!ret) {
        printk("GPIO BUTTON1 is not ready\n");
        return -1;
    }
    conf_ret = gpio_pin_configure_dt(&button1, GPIO_INPUT);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure BUTTON1 pin\n", conf_ret);
        return -1;
    }

    ret = gpio_is_ready_dt(&button2);
    if (!ret) {
        printk("GPIO BUTTON2 is not ready\n");
        return -1;
    }
    conf_ret = gpio_pin_configure_dt(&button2, GPIO_INPUT);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure BUTTON2 pin\n", conf_ret);
        return -1;
    }

    ret = gpio_is_ready_dt(&button3);
    if (!ret) {
        printk("GPIO BUTTON3 is not ready\n");
        return -1;
    }
    conf_ret = gpio_pin_configure_dt(&button3, GPIO_INPUT);
    if (conf_ret < 0) {
        printk("Error %d: failed to configure BUTTON3 pin\n", conf_ret);
        return -1;
    }

    // if we get here, all gpios are ready and configured
    return 0;
}

#if defined(DEV_PWM_LED)
int check_and_configure_pwm(void) {
    // bool ret;
    int conf_ret;

    conf_ret = pwm_is_ready_dt(&pwm_led);
    if (conf_ret < 0) {
        printk("Error %d: failed to pwm_is_ready_dt PWM pin\n", conf_ret);
        return -1;
    }
    return 0;
}

#define ir_rate PWM_HZ(4)
#define FADE_DELAY_MS	10
#define FADE_DELAY	K_MSEC(FADE_DELAY_MS)

int run_pwm(void) {
    uint32_t period = ir_rate;
    // uint32_t pulse = ir_rate / 2U;
    int ret;

    for(uint32_t pulse = 1; pulse < pwm_led.period; pulse += 100000) {
        ret = pwm_set_pulse_dt(&pwm_led, pulse);
        if (ret < 0) {
            printk("Error %d: failed to set PWM\n", ret);
            return -1;
        }
        k_sleep(FADE_DELAY);
    }

    ret = pwm_set_dt(&pwm_led, 0, 0);
    if(ret < 0) {
        printk("Error %d: failed to set PWM\n", ret);
        return -1;
    }
    return 0;
}
#endif