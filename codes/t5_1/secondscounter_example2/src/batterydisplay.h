#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define CLK_NODE DT_NODELABEL(gpioclk)
#if !DT_NODE_HAS_STATUS(CLK_NODE, okay)
#error "Unsupported board: gpioclk devicetree alias is not defined or enabled"
#endif
static const struct gpio_dt_spec clk = GPIO_DT_SPEC_GET(CLK_NODE, gpios);

#define DIO_NODE DT_NODELABEL(gpiodio)
#if !DT_NODE_HAS_STATUS(DIO_NODE, okay)
#error "Unsupported board: gpiodio devicetree alias is not defined or enabled"
#endif
static const struct gpio_dt_spec dio = GPIO_DT_SPEC_GET(DIO_NODE, gpios);


#define ADDR_FIXED 0x44
#define ADDR_AUTO  0x40

#define ADDR_CMD_00H 0xc0
#define ADDR_CMD_01H 0xc1
#define ADDR_CMD_02H 0xc2
#define ADDR_CMD_03H 0xc3

#define FRAME_OFF false
#define FRAME_ON true

#define DISPLAY_BRIGHTEST 0x88

#define BRIGHTNESS_LEVEL0 0
#define BRIGHTNESS_LEVEL1 1
#define BRIGHTNESS_LEVEL2 2
#define BRIGHTNESS_LEVEL3 3
#define BRIGHTNESS_LEVEL4 4
#define BRIGHTNESS_LEVEL5 5
#define BRIGHTNESS_LEVEL6 6
#define BRIGHTNESS_LEVEL7 7


int batterydisplay_intit(void);
void set_brightness(int brightness);
void set_level(int level);
void write_byte(int8_t wr_data);

void start(void);
void stop(void);

int display_level(uint8_t level);
void display_frame(bool framgeflag);
void display_clear(void);

void bit_delay(void);