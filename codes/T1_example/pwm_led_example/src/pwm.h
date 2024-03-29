#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

#define PWM_NODE    DT_ALIAS(pwm_led0)
#if !DT_NODE_HAS_STATUS(PWM_NODE, okay)
#error "Unsupported board: pwm_led0 devicetree alias is not defined"
#endif
static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

int check_and_configure_pwm(void);
int run_pwm(void);