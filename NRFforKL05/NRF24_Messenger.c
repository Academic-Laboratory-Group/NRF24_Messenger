#include "NRF24_Messenger.h"
#include "../UART0/uart0messenger.h"

char * MISO[]{};
char * MOSI[]{};
	
void messengerInit()
{
	uart0Init(9600);
}

uint8_t messengerUpdate()
{
	//Check activity on UART side
	MOSI = UART0_Receive();
	return 1;
}
