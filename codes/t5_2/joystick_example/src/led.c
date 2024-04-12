#include "led.h"

int led_init(void)
{
    if(!device_is_ready(led)){
        printk("LED device %s is not ready\n", led->name);
    }

    return 0;
}

void led_off_all(void)
{
    for(int i = 0; i< MAX_LED_NUM; i++){
        led_off(led, i);
    }
}

// * * * * * * * * * * * * * * * *  0 ~ 15
// * * * * * * * * * * * * * * * * 16 ~ 31
// * * * * * * x x x x * * * * * * 32 ~ 47 (38 ~ 41)
// * * * * * * x x x x * * * * * * 48 ~ 63 (54 ~ 57)
// * * * * * * x x x x * * * * * * 64 ~ 79 (70 ~ 73)
// * * * * * * x x x x * * * * * * 80 ~ 95 (86 ~ 89)
// * * * * * * * * * * * * * * * * 96 ~ 111
// * * * * * * * * * * * * * * * * 112 ~ 127

void led_on_center(void)
{
    // led_off_all();
    led_set_brightness(led, 0, 0);

    for(int i = 38; i < 95; i+=16){
        led_on(led, i);
        led_on(led, i+1);
        led_on(led, i+2);
        led_on(led, i+3);
    }

    k_sleep(K_MSEC(100));

    // led_on(led, 38); led_on(led, 39);led_on(led, 40); led_on(led, 41);
    // led_on(led, 54); led_on(led, 55); led_on(led, 56); led_on(led, 57);
    // led_on(led, 70); led_on(led, 71); led_on(led, 72); led_on(led, 73);
    // led_on(led, 86); led_on(led, 87); led_on(led, 88); led_on(led, 89);
}

// * * * * * * * * * * * * * * * *  0 ~ 15
// * * * * * * * * * * x * * * * * 16 ~ 31 (26)
// * * * * * * x x x x x x * * * * 32 ~ 47 (38 ~ 43)
// * * * * * * x x x x x x x x * * 48 ~ 63 (54 ~ 61)
// * * * * * * x x x x x x x x * * 64 ~ 79 (70 ~ 76)
// * * * * * * x x x x x x * * * * 80 ~ 95 (86 ~ 91)
// * * * * * * * * * * x * * * * * 96 ~ 111(106)
// * * * * * * * * * * * * * * * * 112 ~ 127

void led_on_right(void)
{
    // led_off_all();

    led_on(led, 26);
    led_on(led, 38); led_on(led, 39); led_on(led, 40); led_on(led, 41); led_on(led, 42); led_on(led, 43);
    led_on(led, 54); led_on(led, 55); led_on(led, 56); led_on(led, 57); led_on(led, 58); led_on(led, 59); led_on(led, 60); led_on(led, 61);
    led_on(led, 70); led_on(led, 71); led_on(led, 72); led_on(led, 73); led_on(led, 74); led_on(led, 75); led_on(led, 76); led_on(led, 77);
    led_on(led, 86); led_on(led, 87); led_on(led, 88); led_on(led, 89); led_on(led, 90); led_on(led, 91);
    led_on(led, 106);

    k_sleep(K_MSEC(100));
}


// * * * * * * * * * * * * * * * *  0 ~ 15
// * * * * * x * * * * * * * * * * 16 ~ 31 (21)
// * * * * x x x x x x * * * * * * 32 ~ 47 (36 ~ 41)
// * * x x x x x x x x * * * * * * 48 ~ 63 (50 ~ 57)
// * * x x x x x x x x * * * * * * 64 ~ 79 (66 ~ 73)
// * * * * x x x x x x * * * * * * 80 ~ 95 (84 ~ 89)
// * * * * * x * * * * * * * * * * 96 ~ 111 (101)
// * * * * * * * * * * * * * * * * 112 ~ 127

void led_on_left(void)
{
    // led_off_all();

    led_on(led, 21);
    led_on(led, 36); led_on(led, 37); led_on(led, 38); led_on(led, 39); led_on(led, 40); led_on(led, 41);
    led_on(led, 50); led_on(led, 51); led_on(led, 52); led_on(led, 53); led_on(led, 54); led_on(led, 55); led_on(led, 56); led_on(led, 57);
    led_on(led, 66); led_on(led, 67); led_on(led, 68); led_on(led, 69); led_on(led, 70); led_on(led, 71); led_on(led, 72); led_on(led, 73);
    led_on(led, 84); led_on(led, 85); led_on(led, 86); led_on(led, 87); led_on(led, 88); led_on(led, 89);
    led_on(led, 101);

    k_sleep(K_MSEC(100));
}


// * * * * * * * * * * * * * * * *  0 ~ 15
// * * * * * * * x x * * * * * * * 16 ~ 31 (23 ~ 24)
// * * * * * x x x x x x * * * * * 32 ~ 47 (37 ~ 42)
// * * * * x x x x x x x x * * * * 48 ~ 63 (52 ~ 59)
// * * * * * * x x x x * * * * * * 64 ~ 79 (70 ~ 73)
// * * * * * * x x x x * * * * * * 80 ~ 95 (86 ~ 89)
// * * * * * * * * * * * * * * * * 96 ~ 111
// * * * * * * * * * * * * * * * * 112 ~ 127

void led_on_up(void)
{
    // led_off_all();

    led_on(led, 23); led_on(led, 24);
    led_on(led, 37); led_on(led, 38); led_on(led, 39); led_on(led, 40); led_on(led, 41); led_on(led, 42);
    led_on(led, 52); led_on(led, 53); led_on(led, 54); led_on(led, 55); led_on(led, 56); led_on(led, 57); led_on(led, 58); led_on(led, 59);
    led_on(led, 70); led_on(led, 71); led_on(led, 72); led_on(led, 73);
    led_on(led, 86); led_on(led, 87); led_on(led, 88); led_on(led, 89);

    k_sleep(K_MSEC(100));
}

// * * * * * * * * * * * * * * * *  0 ~ 15
// * * * * * * * * * * * * * * * * 16 ~ 31
// * * * * * * x x x x * * * * * * 32 ~ 47 (38 ~ 41)
// * * * * * * x x x x * * * * * * 48 ~ 63 (54 ~ 57)
// * * * * x x x x x x x x * * * * 64 ~ 79 (68 ~ 75)
// * * * * * x x x x x x * * * * * 80 ~ 95 (85 ~ 90)
// * * * * * * * x x * * * * * * * 96 ~ 111(103 ~ 104)
// * * * * * * * * * * * * * * * * 112 ~ 127

void led_on_down(void)
{
    // led_off_all();

    led_on(led, 38); led_on(led, 39); led_on(led, 40); led_on(led, 41);
    led_on(led, 54); led_on(led, 55); led_on(led, 56); led_on(led, 57);
    led_on(led, 68); led_on(led, 69); led_on(led, 70); led_on(led, 71); led_on(led, 72); led_on(led, 73); led_on(led, 74); led_on(led, 75);
    led_on(led, 85); led_on(led, 86); led_on(led, 87); led_on(led, 88); led_on(led, 89); led_on(led, 90);
    led_on(led, 103); led_on(led, 104);

    k_sleep(K_MSEC(100));

}