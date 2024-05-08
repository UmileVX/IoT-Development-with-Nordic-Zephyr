#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_FAIL -1
#define GPIO_OK 0

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

#define LED1_NODE DT_ALIAS(led1)
#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

#define LED2_NODE DT_ALIAS(led2)
#if !DT_NODE_HAS_STATUS(LED2_NODE, okay)
#error "Unsupported board: led2 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

#define LED3_NODE DT_ALIAS(led3)
#if !DT_NODE_HAS_STATUS(LED3_NODE, okay)
#error "Unsupported board: led3 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);


// buttons
#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

#define SW1_NODE	DT_ALIAS(sw1)
#if !DT_NODE_HAS_STATUS(SW1_NODE, okay)
#error "Unsupported board: sw1 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);

#define SW2_NODE	DT_ALIAS(sw2)
#if !DT_NODE_HAS_STATUS(SW2_NODE, okay)
#error "Unsupported board: sw2 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET(SW2_NODE, gpios);

#define SW3_NODE	DT_ALIAS(sw3)
#if !DT_NODE_HAS_STATUS(SW3_NODE, okay)
#error "Unsupported board: sw3 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET(SW3_NODE, gpios);

int gpio_init(void);