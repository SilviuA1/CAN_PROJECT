#include "data_monitor.h"

#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

#include "can.h"

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
	memset(sensor_state, 0, 30);
	sensor_state[0] = '\0';
	switch(sensor_id)
	{
	case DB_ID_Temperature:
		sprintf(sensor_state, "Temp=%u\n", sensor_value[0]);
		break;
	case DB_ID_Uart_messages:
		sprintf(sensor_state, "%s", sensor_value);
		break;
	case DB_ID_Buttons_potentiometer:
		sprintf(sensor_state, "B1=%u  B2=%u \n PotMtr = %u\n", (uint8_t)(sensor_value[2]& 0x01),(uint8_t)(sensor_value[2]& 0x02),
				                                                    sensor_value[3]);
		break;
	case DB_ID_Humidity_sensor:
		sprintf(sensor_state, "Humidity = %u\n", sensor_value[4]);
		break;
	case DB_ID_Proximity_sensor:
		sprintf(sensor_state, "Proximity = %u\n", sensor_value[0]);
		break;
	default:
		break;
	}
}

void send_TEMP_val_over_CAN(uint8_t sensor_val)
{
	CCAN_MSG_OBJ_T msg_obj;

	/* Send a simple one time CAN message */
	msg_obj.msgobj  = 0;
	msg_obj.mode_id = CAN_ID_Temperature;
	msg_obj.mask    = 0x000;
	msg_obj.dlc     = 1;
	memset(msg_obj.data, 0u, 8);
	msg_obj.data[0] = sensor_val;

	LPC_CCAN_API->can_transmit(&msg_obj);
}

void send_HUMIDITY_val_over_CAN(uint8_t sensor_val)
{
	CCAN_MSG_OBJ_T msg_obj;

	/* Send a simple one time CAN message */
	msg_obj.msgobj  = 0;
	msg_obj.mode_id = CAN_ID_Humidity_sensor;
	msg_obj.mask    = 0x000;
	msg_obj.dlc     = 5;
	memset(msg_obj.data, 0u, 8);
	msg_obj.data[4] = sensor_val;

	LPC_CCAN_API->can_transmit(&msg_obj);
}

void send_PROXIMITY_val_over_CAN(uint8_t sensor_val)
{
	CCAN_MSG_OBJ_T msg_obj;

	/* Send a simple one time CAN message */
	msg_obj.msgobj  = 0;
	msg_obj.mode_id = CAN_ID_Proximity_sensor;
	msg_obj.mask    = 0x000;
	msg_obj.dlc     = 1;
	memset(msg_obj.data, 0u, 8);
	msg_obj.data[0] = sensor_val;

	LPC_CCAN_API->can_transmit(&msg_obj);
}

void send_UART_msg_over_CAN(char uart_message[8], uint8_t msg_len)
{
	CCAN_MSG_OBJ_T msg_obj;

	/* Send a simple one time CAN message */
	msg_obj.msgobj  = 0;
	msg_obj.mode_id = CAN_ID_Uart_messages;
	msg_obj.mask    = 0x000;
	msg_obj.dlc     = msg_len;
	memcpy(msg_obj.data, (uint8_t*)(uart_message), 8);

	LPC_CCAN_API->can_transmit(&msg_obj);
}

void send_buttons_potentiometer_over_CAN(bool button1, bool button2, uint8_t potentiometer)
{
	CCAN_MSG_OBJ_T msg_obj;

	/* Send a simple one time CAN message */
	msg_obj.msgobj  = 0;
	msg_obj.mode_id = CAN_ID_Buttons_potentiometer;
	msg_obj.mask    = 0x000;
	msg_obj.dlc     = 4;
	memset(msg_obj.data, 0u, 8);

	if (button1 == true)
	{
		msg_obj.data[2] |= 1u << 0u;
	}
	if (button2 == true)
	{
		msg_obj.data[2] |= 1u << 1u;
	}
	msg_obj.data[3] = potentiometer;

	LPC_CCAN_API->can_transmit(&msg_obj);
}



void init_database()
{
	for(uint8_t i= 0 ; i < sizeof(inst_sensor_database)/sizeof(inst_sensor_database[0]); i++)
	{
		inst_sensor_database[i].sensor_id = 0u;
		memset(inst_sensor_database[i].sensor_value, 1, 8);
	}
}
