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


void update_database(CAN_ID_MAPP can_message_id, uint8_t sensor_value[8]);
void get_sensor_ (SENSOR_ID_MAPP sensor_id, char sensor_state[30]);

void init_database();


void send_TEMP_val_over_CAN(uint8_t sensor_val);
void send_HUMIDITY_val_over_CAN(uint8_t sensor_val);
void send_PROXIMITY_val_over_CAN(uint8_t sensor_val);
void send_UART_msg_over_CAN(char uart_message[8], uint8_t msg_len);
void send_buttons_potentiometer_over_CAN(bool button1, bool button2, uint8_t potentiometer);



