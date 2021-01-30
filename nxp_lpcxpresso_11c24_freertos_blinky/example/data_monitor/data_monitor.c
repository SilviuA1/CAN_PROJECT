#include "data_monitor.h"

#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

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
	uint8_t sensor_value[8];
}sensors_database;

static sensors_database inst_sensor_database[DB_ID_Sensor_max_ID];


void update_database(CAN_ID_MAPP can_message_id, uint8_t sensor_value[8])
{
	uint8_t sensor_type = CAN_ID_to_POS[can_message_id];

	if (sensor_type < DB_ID_Sensor_max_ID)
	{
		taskDISABLE_INTERRUPTS();
		inst_sensor_database[sensor_type].sensor_id = sensor_type;
		memcpy(inst_sensor_database[sensor_type].sensor_value, sensor_value, 8u);
		taskENABLE_INTERRUPTS();
	}
}


static uint8_t* get_sensor_value(SENSOR_ID_MAPP sensor_id)
{
	uint8_t* return_value = 0u;


	if (sensor_id < DB_ID_Sensor_max_ID)
	{
		taskDISABLE_INTERRUPTS();
		return_value = inst_sensor_database[sensor_id].sensor_value;
		taskENABLE_INTERRUPTS();
	}

	return return_value;
}


// face 1
/**************************
 * Temp = val
 * Uart
 *************************/

// face 2
/**************************
 * Buttons = val
 * Ptmeter = val
 *************************/

// face 3
/**************************
 * Humidity = val
 * Proximity = val
 *************************/

void get_sensor_ (SENSOR_ID_MAPP sensor_id, char sensor_state[30])
{
	uint8_t* sensor_value = get_sensor_value(sensor_id);
	switch(sensor_id)
	{
	case DB_ID_Temperature:
		sprintf(sensor_state, "Temp=%u\r\n", sensor_value[0]);
		break;
	case DB_ID_Uart_messages:
		sprintf(sensor_state, "%s\r\n", sensor_value);
		break;
	case DB_ID_Buttons_potentiometer:
		sprintf(sensor_state, "B1=%u  B2=%u \r\n Potmeter = %u\r\n", (uint8_t)(sensor_value[2]& 0x01),(uint8_t)(sensor_value[2]& 0x02),
				                                                    sensor_value[3]);
		break;
	case DB_ID_Humidity_sensor:
		sprintf(sensor_state, "Humidity = %u\r\n", sensor_value[4]);
		break;
	case DB_ID_Proximity_sensor:
		sprintf(sensor_state, "Proximity = %u\r\n", sensor_value[4]);
		break;
	default:
		break;
	}
}


void init_database()
{
	for(uint8_t i= 0 ; i < sizeof(inst_sensor_database)/sizeof(inst_sensor_database[0]); i++)
	{
		inst_sensor_database[i].sensor_id = 0u;
	}
}
