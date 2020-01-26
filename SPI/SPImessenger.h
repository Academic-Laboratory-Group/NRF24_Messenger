/*//------------------------------------------

  SPI library
  --------------------------------
  description:
    Sets everything needed for SPI, 
		transmits and receives data
		from it.
*///------------------------------------------

#ifndef SPImessenger_h
#define SPImessenger_h

#include "MKL05Z4.h"

//brief\Sets ports and everything
//			else needed in SPI
void SPI_Init(void);

//brief\Transmits data to SPI
//param\char type data we want 
//			to send
void SPI_Transmit(char data);

//brief\Receives char data from SPI
//return\received char data
char SPI_Receive(void);

#endif
