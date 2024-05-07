#include "batterydisplay.h"
#include "value.h"

static int8_t leveltab[9] = {0x00,0x40,0x60,0x70,0x78,0x7c,0x7e,0x7f};//Level 0~7
static uint8_t cmd_dispctrl = DISPLAY_BRIGHTEST + BRIGHTNESS_LEVEL1;
static int setlevel = 0;

// Function to initialize the battery display
int batterydisplay_intit(void)
{
    if (!gpio_is_ready_dt(&clk)) {
		printk("clk GPIO is not ready\n");
		return DK_ERR;
	}

	if(!gpio_is_ready_dt(&dio)) {
		printk("dio GPIO is not ready\n");
		return DK_ERR;
	}

    printk("batterydisplay_init success\n");

    return DK_OK;
}

void set_brightness(int brightness)
{
    cmd_dispctrl = DISPLAY_BRIGHTEST + brightness;
}

void set_level(int level)
{
    setlevel = level;
}

void write_byte(int8_t wr_data)
{
    uint8_t data = wr_data;

    for(uint8_t i = 0; i < 8; i++){
        gpio_pin_configure_dt(&clk, GPIO_OUTPUT);
        bit_delay();

        if(data & 0x01)
            gpio_pin_configure_dt(&dio, GPIO_INPUT);
        else
            gpio_pin_configure_dt(&dio, GPIO_OUTPUT);

        bit_delay();

        gpio_pin_configure_dt(&clk, GPIO_INPUT);
        bit_delay();

        data >>= 1;
    }

    gpio_pin_configure_dt(&clk, GPIO_OUTPUT);
    gpio_pin_configure_dt(&dio, GPIO_INPUT);
    bit_delay();

    gpio_pin_configure_dt(&clk, GPIO_INPUT);
    bit_delay();
    uint8_t ack = gpio_pin_get_dt(&dio);
    if(ack == 0){
        gpio_pin_configure_dt(&dio, GPIO_OUTPUT);
    }

    bit_delay();
    gpio_pin_configure_dt(&clk, GPIO_OUTPUT);
    bit_delay();
}

void start(void)
{
    gpio_pin_configure_dt(&dio, GPIO_OUTPUT);
    bit_delay();
}

void stop(void)
{
    gpio_pin_configure_dt(&dio, GPIO_OUTPUT);
    bit_delay();

    gpio_pin_configure_dt(&clk, GPIO_INPUT);
    bit_delay();

    gpio_pin_configure_dt(&dio, GPIO_INPUT);
    bit_delay();
}

int display_level(uint8_t level)
{
    if(level > 7){
        printk("Invalid level\n");
        return DK_ERR;
    }

    if( setlevel == level){
        // printk("Same level\n");
        return DK_OK;
    }

    set_level(level);
    printk("display_level: %d\n", level);

    start();
    write_byte(ADDR_FIXED);
    stop();

    start();
    write_byte(ADDR_CMD_00H);
    write_byte(leveltab[level]);
    stop();

    start();
    write_byte(cmd_dispctrl);
    stop();

    printk("display_level success\n");

    return DK_OK;
}

void display_frame(bool framgeflag)
{
    int8_t segdata;
    if(framgeflag == FRAME_ON)
        segdata = 0x40;
    else
        segdata = 0x00;

    start();
    write_byte(ADDR_AUTO);
    stop();

    start();
    write_byte(ADDR_CMD_01H);
    for(uint8_t i = 0; i < 3; i++){
        write_byte(segdata);
    }
    stop();

    start();
    write_byte(cmd_dispctrl);
    stop();
}

void display_clear(void)
{
    display_level(0);
    display_frame(FRAME_OFF);
}

void bit_delay(void)
{
    k_sleep(K_TICKS(1));
}