#include "chip.h"
#include "board.h"

typedef enum
{
	CAN_ID_Temperature = 1,
	CAN_ID_Uart_messages = 3,
	CAN_ID_Buttons_potentiometer = 5,
	CAN_ID_Humidity_sensor = 7,
	CAN_ID_Proximity_sensor = 9,
	CAN_ID_Sensor_max_ID
}CAN_ID_MAPP;

typedef enum
{
	DB_ID_Temperature = 0,
	DB_ID_Uart_messages,
	DB_ID_Buttons_potentiometer,
	DB_ID_Humidity_sensor,
	DB_ID_Proximity_sensor,
	DB_ID_Sensor_max_ID
}SENSOR_ID_MAPP;


void update_database(CAN_ID_MAPP can_message_id, uint8_t sensor_value);
uint8_t get_sensor_value(SENSOR_ID_MAPP sensor_id);

void init_database();
