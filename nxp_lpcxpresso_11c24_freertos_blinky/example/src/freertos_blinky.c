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
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/

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

/**
 * @brief	Main UART program body
 * @return	Always returns 1
 */
int main(void)
{
	prvSetupHardware();

	/* LED1 toggle thread */
	xTaskCreate(blinkLed, (signed char *) "vTaskLed1",
				2*configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	/* Send initial messages */
	puts("Start scheduler");

	/* Start the scheduler */
	vTaskStartScheduler();


	// should never arrive here
	return 1;
}
