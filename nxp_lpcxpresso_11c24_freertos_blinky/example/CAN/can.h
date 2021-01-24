#include "board.h"


#define TEST_CCAN_BAUD_RATE 500000


void CAN_rx(uint8_t msg_obj_num);
void CAN_tx(uint8_t msg_obj_num);
void CAN_init(void);
