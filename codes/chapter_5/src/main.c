/*
 * Copyright (c) 2022 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <string.h>


#define RECEIVE_TIMEOUT 1000

#define MSG_SIZE 9
#define CO2_MULTIPLIER 256

/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

static const struct device *const uart_serial = DEVICE_DT_GET(DT_N_ALIAS_myserial);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;


char getCheckSum(char *packet) {
	char i, checksum;
	for( i = 1; i < 8; i++) {
		checksum += packet[i];
	}
	checksum = 0xff - checksum;
	checksum += 1;
	return checksum;
}

uint8_t fromHexadecimalToDecimal(uint8_t hexadecimalValue) {
	uint8_t decimalValue = 0;
	decimalValue += hexadecimalValue / 10 * 16;
	decimalValue += hexadecimalValue % 10;
	return decimalValue;
}

/**
 * Read data via UART IRQ.
 *
 * @param dev UART device struct
 * @param user_data Pointer to user data (NULL in this practice)
 */
void serial_callback(const struct device *dev, void *user_data) {
	uint8_t c, high, low;
	char checksum_ok, value_calc_flag;
	int checksum;

	if (!uart_irq_update(uart_serial)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_serial)) {
		printk("No data\n");
		return;
	}

	/* read until FIFO empty */
	while (uart_fifo_read(uart_serial, &c, 1) == 1) {
		if (rx_buf_pos >= MSG_SIZE) {
			rx_buf_pos = 0;
		}
		rx_buf[rx_buf_pos++] = c;
	}

	// calculate checksum, and compare with received checksum
	checksum = getCheckSum(rx_buf);
	checksum_ok = checksum == rx_buf[8];
	if (checksum_ok) {
		printk("Checksum OK (%d == %d, index=%d)\n", checksum, rx_buf[8], rx_buf_pos);
	} else {
		printk("Checksum failed (%d == %d, index=%d)\n", checksum, rx_buf[8], rx_buf_pos);
	}

	// check if we received all data and checksum is OK
	value_calc_flag = rx_buf_pos == MSG_SIZE && checksum_ok;
	if (value_calc_flag) {
		high = rx_buf[2];
		high = fromHexadecimalToDecimal(high);
		low = rx_buf[3];
		low = fromHexadecimalToDecimal(low);
		printk("CO2: %d ppm (high = %d, low = %d)\n", (high * CO2_MULTIPLIER) + low, high, low);
		// print message buffer
		for (int i = 0; i < MSG_SIZE; i+=1) {
			printk("%x ", rx_buf[i]);
		}
		printk("\n");
	}
}

void serial_write() {
	uint8_t tx_buf[MSG_SIZE] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
	for (int i = 0; i < MSG_SIZE; i+=1) {
		uart_poll_out(uart_serial, tx_buf[i]);
	}
}

int main(void) {
	if (!device_is_ready(uart_serial)) {
		printk("UART device not found!");
		return 0;
	}

	/* configure interrupt and callback to receive data */
	int ret = uart_irq_callback_user_data_set(uart_serial, serial_callback, NULL);

	if (ret < 0) {
		if (ret == -ENOTSUP) {
			printk("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			printk("UART device does not support interrupt-driven API\n");
		} else {
			printk("Error setting UART callback: %d\n", ret);
		}
		return 0;
	}
	uart_irq_rx_enable(uart_serial);

	while (1) {
		k_sleep(K_MSEC(5000));
		serial_write();
	}
}
