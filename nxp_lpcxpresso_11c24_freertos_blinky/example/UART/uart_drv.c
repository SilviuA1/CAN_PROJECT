#include <uart_drv.h>



static const char inst1[] = "LPC11xx UART is initialized!\r\n";


/* Ring buffer size */
#define UART_RB_SIZE 128

/* Transmitnd receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RB_SIZE], txbuff[UART_RB_SIZE];


void UART_IRQHandler2(void)
{
 /* Want to handle any errors? Do it here. */

 /* Use default ring buffer handler. Override this with your own
    code if you need more capability. */
 Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}


/***************** Static function implementation********************/
static void Init_UART_PinMux(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));/* RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));/* TXD */
}


void uart_init()
{
  // Board_UART_Init();
   Init_UART_PinMux();

  /* Setup UART for 115.2K8N1 */
  Chip_UART_Init(LPC_USART);
  Chip_UART_SetBaud(LPC_USART, 115200);
  Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
  Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
  Chip_UART_TXEnable(LPC_USART);

  /* Before using the ring buffers, initialize them using the ring
     buffer init function */
  RingBuffer_Init(&rxring, rxbuff, 1, UART_RB_SIZE);
  RingBuffer_Init(&txring, txbuff, 1, UART_RB_SIZE);

  /* Enable receive data and line status interrupt */
  Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));

  /* preemption = 1, sub-priority = 1 */
  NVIC_SetPriority(UART0_IRQn, 1);
  NVIC_EnableIRQ(UART0_IRQn);

  Chip_UART_SendBlocking(LPC_USART, inst1, sizeof(inst1) - 1);
//  Chip_UART_SendRB(LPC_USART, &txring, inst1, sizeof(inst1) - 1);
}
