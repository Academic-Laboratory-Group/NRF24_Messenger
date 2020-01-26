/*//------------------------------------------

  UART0 library
  --------------------------------
  description:
    Sets everything needed for UART0, 
		transmits and receives data
		from it.
*///------------------------------------------
#ifndef uart0messenger_h
#define uart0messenger_h

#include "MKL05Z4.h"

//brief\Sets ports and everything
//			else needed in UART0
void UART0_Init(uint32_t naud_rate);

//brief\Transmits data to UART0
//param\char type data we want 
//			to send
void UART0_Transmit(char data);

//brief\Receives char data from UART0
//return\received char data
char UART0_Receive(void);

#endif
