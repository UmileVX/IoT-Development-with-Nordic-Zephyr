
/**
 *  @brief CO2 Service UUID value
 */
#define BT_UUID_CO2_VAL 0x1c02
/**
 *  @brief CO2 Service
 */
#define BT_UUID_CO2 \
	BT_UUID_DECLARE_16(BT_UUID_CO2_VAL)

/**
 *  @brief CTS Characteristic Current Time UUID value
 */
#define BT_UUID_CO2_CURRENT_VAL 0x2c02
/**
 *  @brief CTS Characteristic Current Time
 */
#define BT_UUID_CO2_CURRENT \
	BT_UUID_DECLARE_16(BT_UUID_CO2_CURRENT_VAL)


int co2_init(void);
void serial_write(void);
