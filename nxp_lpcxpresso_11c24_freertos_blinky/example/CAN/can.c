#include "can.h"

#include "data_monitor.h"

CCAN_MSG_OBJ_T msg_obj;
CCAN_MSG_OBJ_T gCANRxObj;

void baudrateCalculate(uint32_t baud_rate, uint32_t *can_api_timing_cfg)
{
 uint32_t pClk, div, quanta, segs, seg1, seg2, clk_per_bit, can_sjw;
 Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_CAN);
 pClk = Chip_Clock_GetMainClockRate();

 clk_per_bit = pClk / baud_rate;

 for (div = 0; div <= 15; div++) {
  for (quanta = 1; quanta <= 32; quanta++) {
   for (segs = 3; segs <= 17; segs++) {
    if (clk_per_bit == (segs * quanta * (div + 1))) {
     segs -= 3;
     seg1 = segs / 2;
     seg2 = segs - seg1;
     can_sjw = seg1 > 3 ? 3 : seg1;
     can_api_timing_cfg[0] = div;
     can_api_timing_cfg[1] =
      ((quanta - 1) & 0x3F) | (can_sjw & 0x03) << 6 | (seg1 & 0x0F) << 8 | (seg2 & 0x07) << 12;
     return;
    }
   }
  }
 }
}



/* CAN receive callback */
/* Function is executed by the Callback handler after
    a CAN message has been received */
void CAN_rx(uint8_t msg_obj_num) {
	puts("<< RX CALLBACK");

 /* Determine which CAN message has been received */
 msg_obj.msgobj = msg_obj_num;
 /* Now load up the msg_obj structure with the CAN message */
 LPC_CCAN_API->can_receive(&msg_obj);

//
// if (msg_obj_num == 1) {
//  /* Simply transmit CAN frame (echo) with with ID +0x100 via buffer 2 */
//  msg_obj.msgobj = 2;
//  msg_obj.mode_id += 0x100;
//  LPC_CCAN_API->can_transmit(&msg_obj);
// }

 update_database((CAN_ID_MAPP)(msg_obj.mode_id) , msg_obj.data);

 // where is this suppose to be called
 puts((char*)(msg_obj.data));
 LPC_CCAN_API->config_rxmsgobj(&gCANRxObj);
}

/* CAN transmit callback */
/* Function is executed by the Callback handler after
    a CAN message has been transmitted */
void CAN_tx(uint8_t msg_obj_num) {
	puts(">> TX CALLBACK");
}

/* CAN error callback */
/* Function is executed by the Callback handler after
    an error has occured on the CAN bus */
void CAN_error(uint32_t error_info) {}

void CAN_IRQHandler(void) {
 LPC_CCAN_API->isr();
}

/* INIT CAN */
void CAN_init(void)
{
  uint32_t CanApiClkInitTable[2];
  /* Publish CAN Callback Functions */
  CCAN_CALLBACKS_T callbacks = {
   CAN_rx,
   CAN_tx,
   CAN_error,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
  };
  baudrateCalculate(TEST_CCAN_BAUD_RATE, CanApiClkInitTable);

  LPC_CCAN_API->init_can(&CanApiClkInitTable[0], TRUE);
  /* Configure the CAN callback functions */
  LPC_CCAN_API->config_calb(&callbacks);
  /* Enable the CAN Interrupt */
  NVIC_EnableIRQ(CAN_IRQn);

  /* Configure message object 1 to receive all 29-bit messages for servo outputs */
  gCANRxObj.msgobj = 1;
  gCANRxObj.mode_id = 0x0000000FUL;
  gCANRxObj.mask = 0x00000000UL;
  gCANRxObj.dlc = 8;
  LPC_CCAN_API->config_rxmsgobj(&gCANRxObj);
}
