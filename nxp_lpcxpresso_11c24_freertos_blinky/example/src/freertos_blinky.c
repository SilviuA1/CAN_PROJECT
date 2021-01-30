

#include "chip.h"
#include "board.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"

#include "can.h"
#include "data_monitor.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

// init
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	Board_LED_Set(0, false);

	CAN_init();
	init_database();

	update_database(CAN_ID_Uart_messages, (uint8_t*)("Hello\n"));
}

static void blinkLed(void *pvParameters)
{
	bool ledState = false;
	char my_message[30];

	uint8_t what_to_send_cnt = 0u;
	for(;;)
	{
//		sprintf(my_message, "Hello world\r\n");

		if (what_to_send_cnt == 0u)
		{
			get_sensor_(DB_ID_Uart_messages, my_message);
			Board_UARTPutSTR(my_message);

		}
		else if (what_to_send_cnt == 1u)
		{
			get_sensor_(DB_ID_Buttons_potentiometer, my_message);
			Board_UARTPutSTR(my_message);
		}
		else if(what_to_send_cnt == 2u)
		{
			get_sensor_(DB_ID_Humidity_sensor, my_message);
			Board_UARTPutSTR(my_message);
		}
		else if(what_to_send_cnt == 3u)
		{
			get_sensor_(DB_ID_Temperature, my_message);
			Board_UARTPutSTR(my_message);
		}
		else
		{
			get_sensor_(DB_ID_Proximity_sensor, my_message);
			Board_UARTPutSTR(my_message);
		}
		Board_LED_Set(0, ledState);
		ledState = (bool)!ledState;

		what_to_send_cnt ++;
		what_to_send_cnt = what_to_send_cnt % 5;
		puts("-----");
		puts(my_message);
		vTaskDelay(600 * 1);

	}
}

static void transmitPeriodic(void *pvParameters)
{

    static uint8_t test_value[8];
    int i = 0;
    test_value[i] = 0;

	for(;;)
	{
		test_value[i] ++;
		if (test_value[i] > 9u )
		{
			test_value[i] = 0u;
		}
//		LPC_CCAN_API->can_transmit(&msg_obj);

//		send_TEMP_val_over_CAN(2u);
//		vTaskDelay(configTICK_RATE_HZ);
//
//		send_HUMIDITY_val_over_CAN(100u);
//		vTaskDelay(configTICK_RATE_HZ);
//
//		send_PROXIMITY_val_over_CAN(130u);
//		vTaskDelay(configTICK_RATE_HZ);
//
//		send_UART_msg_over_CAN("heya\r\n", strlen("heya\r\n"));
//		vTaskDelay(configTICK_RATE_HZ);
//
//		send_buttons_potentiometer_over_CAN(true, false, 250u);
//		vTaskDelay(configTICK_RATE_HZ);
//
//
//		update_database(CAN_ID_Temperature, test_value);
//		update_database(CAN_ID_Uart_messages, (uint8_t*)("sadasda\n"));
		vTaskDelay(configTICK_RATE_HZ * 3);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/


int main(void)
{
	prvSetupHardware();

	xTaskCreate(blinkLed, (signed char *) "blinkLed",
				configMINIMAL_STACK_SIZE + 80, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	xTaskCreate(transmitPeriodic, (signed char *) "periodicCanTransmit",
				configMINIMAL_STACK_SIZE + 30, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* Send initial messages */
	puts("Start scheduler");

	Board_LED_Set(0, true);

	/* Start the scheduler */
	vTaskStartScheduler();

	while(1)
	{
		;
	}

	// should never arrive here
	return 1;
}
