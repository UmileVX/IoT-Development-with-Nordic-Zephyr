/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#include <string.h>
#include <errno.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/drivers/gpio.h>
#include <soc.h>

#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "../services/my_service.h"

static uint8_t mfg_data[] = { 0xff, 0xff, 0x00 };

static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 3),
};

static const struct bt_data sd[] =
{
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, MY_SERVICE_UUID),
};

struct bt_conn *my_connection;

struct k_sem ble_init_ok;

static void connected(struct bt_conn *conn, uint8_t err)
{
	struct bt_conn_info info;
	char addr[BT_ADDR_LE_STR_LEN];

	my_connection = conn;

	if (err)
	{
		printk("Connection failed (err %u)\n", err);
		return;
	}
	else if(bt_conn_get_info(conn, &info))
	{
		printk("Could not parse connection info\n");
	}
	else
	{
		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		printk("Connection established!		\n\
		Connected to: %s					\n\
		Role: %u							\n\
		Connection interval: %u				\n\
		Slave latency: %u					\n\
		Connection supervisory timeout: %u	\n"
		, addr, info.role, info.le.interval, info.le.latency, info.le.timeout);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);
}

static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
	//If acceptable params, return true, otherwise return false.
	return true;
}

static void le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout)
{
	struct bt_conn_info info;
	char addr[BT_ADDR_LE_STR_LEN];

	if(bt_conn_get_info(conn, &info))
	{
		printk("Could not parse connection info\n");
	}
	else
	{
		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		printk("Connection parameters updated!	\n\
		Connected to: %s						\n\
		New Connection Interval: %u				\n\
		New Slave Latency: %u					\n\
		New Connection Supervisory Timeout: %u	\n"
		, addr, info.le.interval, info.le.latency, info.le.timeout);
	}
}

static struct bt_conn_cb conn_callbacks =
{
	.connected			= connected,
	.disconnected   		= disconnected,
	.le_param_req			= le_param_req,
	.le_param_updated		= le_param_updated
};

static void bt_ready(int err)
{
	if (err)
	{
		printk("BLE init failed with error code %d\n", err);
		return;
	}

	//Configure connection callbacks
	bt_conn_cb_register(&conn_callbacks);

	//Initalize services
	err = my_service_init();
	if (err)
	{
		printk("Failed to init LBS (err:%d)\n", err);
		return;
	}

	//Start advertising
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");

	k_sem_give(&ble_init_ok);
}


static void error(void)
{
	while (true) {
		printk("Error!\n");
		/* Spin for ever */
		k_sleep(K_MSEC(1000)); //1000ms
	}
}


int main(void)
{
	int err = 0;
	uint32_t number = 0;

	printk("Starting Nordic BLE peripheral tutorial\n");

	k_sem_init(&ble_init_ok, 0, 1);
	err = bt_enable(bt_ready);

	if (err)
	{
		printk("BLE initialization failed\n");
		error(); //Catch error
	}

	/* 	Bluetooth stack should be ready in less than 100 msec. 								\
																							\
		We use this semaphore to wait for bt_enable to call bt_ready before we proceed 		\
		to the main loop. By using the semaphore to block execution we allow the RTOS to 	\
		execute other tasks while we wait. */
	err = k_sem_take(&ble_init_ok, K_MSEC(500));

	if (!err)
	{
		printk("Bluetooth initialized\n");
	} else
	{
		printk("BLE initialization did not complete in time\n");
		error(); //Catch error
	}

	err = my_service_init();

	for (;;)
	{
		// Main loop
		my_service_send(my_connection, (uint8_t *)&number, sizeof(number));
		number++;
		k_sleep(K_MSEC(1000)); // 1000ms
	}
}
