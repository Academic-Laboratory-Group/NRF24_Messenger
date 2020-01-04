#include "NRF24_Messenger.h"
#include "../UART0/uart0messenger.h"
#include "../SPI/SPImessenger.h"

char * MISO;
char * MOSI;
	
enum STATES STATE;

void messengerInit(void)
{
	UART0_Init(9600);
	SPI_Init();
}

uint8_t messengerUpdate(void)
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

	return 1;
}
