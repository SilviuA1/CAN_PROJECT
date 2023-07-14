

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
	for(;;)
	{
		Board_LED_Set(0, ledState);
		ledState = (bool)!ledState;
		puts("blindLed task loop");

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
