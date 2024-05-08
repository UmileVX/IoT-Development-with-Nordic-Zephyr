#include <zephyr/kernel.h>

#include "gpios.h"

int main(void)
{
        printk("pwmled_example\n");
        pwm_init();
        return 0;
}
