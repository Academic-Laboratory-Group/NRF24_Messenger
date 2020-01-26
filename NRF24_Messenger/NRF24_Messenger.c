#include "MKL05Z4.h"
#include "NRF24_Messenger.h"
#include "../NRF24/NRF24.h"
#include "../UART0/uart0messenger.h"
#include <cstring>

char MISO = 0;
char MOSI = 0;
char full = 0;
void messenger_Init(void)
{
	UART0_Init(9600);
	NRF24_Init();
	UART0_Transmit_word("NRF24L01 Messenger\n\r");
}

void messenger_Update(void)
{
	//Normally it is a receiver
	startListening();
	CE_HIGH();
	delay1();
	delay1();
	CE_LOW();
	read(&MISO, sizeof(MISO));
	if (MISO != 0)
	{
		UART0_Transmit(MISO);
	}
	delay1();
	//But if we want to send sth
	//It is a transmiter
	MOSI = UART0_Receive();
	if (MOSI != 0)
	{
		stopListening();
		write(MOSI, sizeof(MOSI));
		CE_HIGH();
		delay1();
		delay1();
		CE_LOW();
	}
	delay2();
}
