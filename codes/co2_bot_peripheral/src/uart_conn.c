#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>


#include "uart_conn.h"

#define MSG_SIZE 9
#define CO2_MULTIPLIER 256

static const struct device *const uart_serial = DEVICE_DT_GET(DT_N_ALIAS_myserial);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

static uint8_t co2_data[4];
static unsigned co2;
static uint8_t co2_update;

struct bt_conn *myconn = NULL;

void send_co2_data_via_ble();


char getCheckSum(char *packet) {
	char i, checksum;
	checksum = 0;
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
	// if (checksum_ok) {
	// 	printk("Checksum OK (%d == %d, index=%d)\n", checksum, rx_buf[8], rx_buf_pos);
	// } else {
	// 	printk("Checksum failed (%d == %d, index=%d)\n", checksum, rx_buf[8], rx_buf_pos);
	// }

	// check if we received all data and checksum is OK
	value_calc_flag = rx_buf_pos == MSG_SIZE && checksum_ok;
	if (value_calc_flag) {
		high = rx_buf[2];
		high = fromHexadecimalToDecimal(high);
		low = rx_buf[3];
		low = fromHexadecimalToDecimal(low);
		co2 = (high * CO2_MULTIPLIER) + low;
		// printk("CO2: %d ppm (high = %d, low = %d)\n", co2, high, low);

		send_co2_data_via_ble();
	}
}

/**
 * Write via serial port.
 */
void serial_write() {
	uint8_t tx_buf[MSG_SIZE] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
	for (int i = 0; i < MSG_SIZE; i+=1) {
		uart_poll_out(uart_serial, tx_buf[i]);
	}
}

int serial_init() {
	if (!device_is_ready(uart_serial)) {
		printk("UART device not found!");
		return -1;
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
		return ret;
	}
	uart_irq_rx_enable(uart_serial);

	return 0;
}

// ------------------------------
// Bluetooth

static void co2_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
	/* TODO: Handle value */
}

static ssize_t read_co2(
	struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset
) {
	const char *value = attr->user_data;
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(co2_data));
}

static ssize_t write_co2(
	struct bt_conn *conn, const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len, uint16_t offset,
	uint8_t flags
) {
	int rc;
	if (co2 == 0) {
		return -EINVAL;
	}
	// co2_data[0] = co2;
	memcpy((uint8_t *)co2_data,(uint8_t *)&buf,len);
	printk("co2 write!!\n");

	rc = bt_gatt_notify(NULL, attr, co2_data, sizeof(co2_data));
	return rc == -ENOTCONN ? 0 : rc;
}


// Declare the CO2 service
BT_GATT_SERVICE_DEFINE(co2_cvs,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_CO2),
	BT_GATT_CHARACTERISTIC(BT_UUID_CO2_CURRENT, BT_GATT_CHRC_READ |
			       BT_GATT_CHRC_NOTIFY | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
			       read_co2, write_co2, co2_data),
	BT_GATT_CCC(co2_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);


int co2_init(void) {
	int ret = serial_init();
	if (ret < 0) {
		printk("Error initializing serial port: %d\n", ret);
	}
	return ret;
}

void send_co2_data_via_ble() {
	memcpy(co2_data, &co2, sizeof(co2));
	// snprintk(co2_data, sizeof(co2_data), "%d", co2);
	bt_gatt_notify(myconn, &co2_cvs.attrs[1], &co2, sizeof(co2));
	printk("CO2 data ( %d ppm) sent via BLE\n", co2);
	printk("value (bytes): [%d, %d, %d, %d ]\n", co2_data[0], co2_data[1], co2_data[2], co2_data[3]);
	printk("value   (str): %s\n", co2_data);
}
