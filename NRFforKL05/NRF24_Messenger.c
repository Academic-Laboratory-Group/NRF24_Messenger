#include "NRF24_Messenger.h"
#include "../UART0/uart0messenger.h"
#include "../SPI/SPImessenger.h"

char MISO;
char MOSI;
char c = 0;
void messenger_Init(void)
{
	UART0_Init(9600);
	SPI_Init();
	for (c = 'a'; c <= 'g'; c++)
	{
		UART0_Transmit(c);
	}
}

void messenger_Update(void)
{
	// Check activity on UART side
	MOSI = UART0_Receive();
	SPI_Transmit(MOSI);
	
	 
	// Check activity on SPI side 
	// (if UART has nothing to do)
	MISO = SPI_Receive();
	UART0_Transmit(MISO);

}
