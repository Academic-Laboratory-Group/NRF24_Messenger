#include "MKL05Z4.h"
#include "NRF24_Messenger.h"
#include "NRF24.h"
#include "../UART0/uart0messenger.h"
#include "../SPI/SPImessenger.h"
#include <cstring>

char MISO;
char MOSI;
void messenger_Init(void)
{
	UART0_Init(9600);
	SPI_Init();
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
