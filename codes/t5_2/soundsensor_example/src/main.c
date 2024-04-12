#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include "led.h"

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

#define MAX_SENSORVALUE 1000
#define MIN_SENSORVALUE 8
#define SENSOR_INVALID_VALUE 65500

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
	int err;
	uint32_t count = 0;
	uint16_t buf;
        uint32_t sound_value = 0;

	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
                // .oversampling = 0,
                // .calibrate = 0,
	};

        // printk("Sound Sensor : array_size [%d]\n", ARRAY_SIZE(adc_channels));
	/* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!adc_is_ready_dt(&adc_channels[i])) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}

	// led_init();
	// led_set_brightness(led, 0, 0);

	while (1) {
		printk("ADC reading[%u]:\n", count++);

		(void)adc_sequence_init_dt(&adc_channels[0], &sequence);
		err = adc_read(adc_channels[0].dev, &sequence);
		if (err < 0) {
			printk("Could not read (%d)\n", err);
			k_sleep(K_MSEC(100));
			continue;
		}

		sound_value = (int32_t)buf;
                if(sound_value >= SENSOR_INVALID_VALUE){
			printk("sound_value: invalid data %" PRIu32 "\n", sound_value);
			k_sleep(K_MSEC(100));
			continue;
		}
		int sound_level = map(sound_value, 0, MAX_SENSORVALUE, 0, MIN_SENSORVALUE);
		printk("sound_value: %" PRIu32 " sound_level : %d\n", sound_value, sound_level);


		// if(sound_level > 7){
		// 	led_off_all();
		// } else {
		// 	led_on_idx(sound_level);
		// }

		k_sleep(K_MSEC(100));
	}
	return 0;
}
