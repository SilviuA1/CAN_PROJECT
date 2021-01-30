

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
}

static void blinkLed(void *pvParameters)
{
	bool ledState = false;
	char my_message[30];
	for(;;)
	{
//		sprintf(my_message, "Hello world\r\n");

		get_sensor_(DB_ID_Uart_messages, my_message);
		Board_LED_Set(0, ledState);
		ledState = (bool)!ledState;

		puts(my_message);

		Board_UARTPutSTR(my_message);

		vTaskDelay(configTICK_RATE_HZ * 1);
	}
}

static void transmitPeriodic(void *pvParameters)
{
	CCAN_MSG_OBJ_T msg_obj;

    /* Send a simple one time CAN message */
    msg_obj.msgobj  = 0;
    msg_obj.mode_id = 0x345;
    msg_obj.mask    = 0x7F0;
    msg_obj.dlc     = 4;
    msg_obj.data[0] = 'T';    // 0x54
    msg_obj.data[1] = 'E';    // 0x45
    msg_obj.data[2] = 'S';    // 0x53
    msg_obj.data[3] = 'T';    // 0x54

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
		LPC_CCAN_API->can_transmit(&msg_obj);

//		update_database(CAN_ID_Temperature, test_value);
		update_database(CAN_ID_Uart_messages, (uint8_t*)("sadasda"));
		vTaskDelay(configTICK_RATE_HZ * 2);
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
