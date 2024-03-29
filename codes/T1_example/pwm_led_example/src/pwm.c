#include <zephyr/sys/util.h>

#include "pwm.h"

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
    // uint32_t period = ir_rate;
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