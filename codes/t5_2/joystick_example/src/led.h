#include <zephyr/drivers/led.h>
#include <zephyr/drivers/kscan.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>

#define LED_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(holtek_ht16k33)
#define KEY_NODE DT_CHILD(LED_NODE, keyscan)

#define MAX_LED_NUM 128

static const struct device *const led = DEVICE_DT_GET(LED_NODE);

int led_init(void);

void led_off_all(void);
void led_on_center(void);
void led_on_right(void);
void led_on_left(void);
void led_on_up(void);
void led_on_down(void);