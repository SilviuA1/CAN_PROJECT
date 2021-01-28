/*
 * @brief UART interrupt example with ring buffers
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"
#include "board.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"

#include "can.h"
#include "uart_drv.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

//void HardFault_Handler(void)
//{
//
//}
//
//void IntDefaultHandler(void)
//{
//
//}

// init
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	Board_LED_Set(0, false);

	CAN_init();
//	uart_init();
}

static void blinkLed(void *pvParameters)
{
	bool ledState = false;
	int my_count = 0u;
	char my_message[15];
	for(;;)
	{
		my_count ++;
		if (my_count > 9)
		{
			my_count = 0;
		}
		sprintf(my_message, "Hello %d\r\n", my_count);
		Board_LED_Set(0, ledState);
		ledState = (bool)!ledState;
		puts(my_message);
//		uart_transmit();
//		uart_send_msg(my_message);
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

	for(;;)
	{
		LPC_CCAN_API->can_transmit(&msg_obj);
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
				configMINIMAL_STACK_SIZE + 100, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	xTaskCreate(transmitPeriodic, (signed char *) "periodicCanTransmit",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
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
