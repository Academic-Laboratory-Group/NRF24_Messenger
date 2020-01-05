#include "NRF24_Messenger.h"
#include "../UART0/uart0messenger.h"
#include "../SPI/SPImessenger.h"

char * MISO;
char * MOSI;

void messenger_Init(void)
{
	UART0_Init(9600);
	SPI_Init();
}

void messenger_Update(void)
{
	// Check activity on UART side
	if (UART0_Receive(MISO))
	{
		SPI_Transmit(MISO);
	}
	 
	// Check activity on SPI side 
	// (if UART has nothing to do)
	else if (SPI_Receive(MOSI))
	{
		UART0_Transmit(MOSI);
	}
}
