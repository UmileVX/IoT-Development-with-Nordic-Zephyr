#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/util.h>

#include "led.h"
#include "batterydisplay.h"
#include "value.h"

struct k_timer my_timer;
struct k_work my_work;

static int seconds = 0;

void my_work_handler(struct k_work *work)
{
        printk("Time: %d\n", seconds);

        // Display the time on the LED matrix
        led_on_seconds(seconds);

        // Display the time on the battery display
        if( seconds == 0){
                display_level(0);
        } else if(seconds > 0 && seconds < 10){
                display_level(1);
        } else if(seconds >= 10 && seconds < 20){
                display_level(2);
        } else if(seconds >= 20 && seconds < 30){
                display_level(3);
        } else if(seconds >= 30 && seconds < 40){
                display_level(4);
        } else if(seconds >= 40 && seconds < 50){
                display_level(5);
        } else if(seconds >= 50 && seconds < 56){
                display_level(6);
        } else {
                display_level(7);
        }


        // Increment the seconds
        seconds++;
        if(seconds > 59){
                seconds = 0;
        }
}

K_WORK_DEFINE(my_work, my_work_handler);

void my_timer_handler(struct k_timer *dummy)
{
        k_work_submit(&my_work);
}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

int main(void)
{
        printk("Seconds counter example 2\n");
        int ret = DK_OK;

        ret = batterydisplay_intit();
        if(ret != DK_OK){
                printk("Error initializing battery display\n");
                return DK_ERR;
        }

        ret = led_init();
        if(ret != DK_OK){
                printk("Error initializing LED\n");
                return DK_ERR;
        }

        // Start the timer
        k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));

        return DK_OK;
}
