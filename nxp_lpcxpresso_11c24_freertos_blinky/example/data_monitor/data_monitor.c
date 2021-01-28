#include "data_monitor.h"

#include "FreeRTOS.h"
#include "task.h"

static uint8_t CAN_ID_to_POS[CAN_ID_Sensor_max_ID] =
{
		[CAN_ID_Temperature]           = DB_ID_Temperature,
		[CAN_ID_Uart_messages]         = DB_ID_Uart_messages,
		[CAN_ID_Buttons_potentiometer] = DB_ID_Buttons_potentiometer,
		[CAN_ID_Humidity_sensor]       = DB_ID_Humidity_sensor,
		[CAN_ID_Proximity_sensor]      = DB_ID_Proximity_sensor
};

typedef struct
{
	uint8_t sensor_id;
	uint8_t sensor_value;
}sensors_database;

static sensors_database inst_sensor_database[DB_ID_Sensor_max_ID];


void update_database(CAN_ID_MAPP can_message_id, uint8_t sensor_value)
{
	uint8_t sensor_type = CAN_ID_to_POS[can_message_id];

	if (sensor_type < DB_ID_Sensor_max_ID)
	{
		taskDISABLE_INTERRUPTS();
		inst_sensor_database[sensor_type].sensor_id = sensor_type;
		inst_sensor_database[sensor_type].sensor_value = sensor_value;
		taskENABLE_INTERRUPTS();
	}
}


uint8_t get_sensor_value(SENSOR_ID_MAPP sensor_id)
{
	uint8_t return_value = 0u;


	if (sensor_id < DB_ID_Sensor_max_ID)
	{
		taskDISABLE_INTERRUPTS();
		return_value = inst_sensor_database[sensor_id].sensor_value;
		taskENABLE_INTERRUPTS();
	}

	return return_value;
}


void init_database()
{
	for(uint8_t i= 0 ; i < sizeof(inst_sensor_database)/sizeof(inst_sensor_database[0]); i++)
	{
		inst_sensor_database[i].sensor_id = 0u;
		inst_sensor_database[i].sensor_value = 0u;
	}
}
