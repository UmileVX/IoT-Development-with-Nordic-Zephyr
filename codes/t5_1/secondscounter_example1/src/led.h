#include <zephyr/drivers/led.h>
#include <zephyr/drivers/kscan.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>

#define MAX_LED_MATRIX_IDX 10
#define MAX_LED_NUM 128
#define MAX_LED_MATRIX_NUM 64

#define TENS_START_INDEX 0
#define UNITS_START_INDEX 8

#define LED_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(holtek_ht16k33)
#define KEY_NODE DT_CHILD(LED_NODE, keyscan)

static const struct device *const led = DEVICE_DT_GET(LED_NODE);

int led_init(void);
void led_on_seconds(int seconds);