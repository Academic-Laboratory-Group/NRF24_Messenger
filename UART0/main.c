#include "MKL05Z4.h"
#include "uart0messenger.h"

char data = 0;

void leds_Init(void)
{
	SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK);      
  PORTB->PCR[9] = PORT_PCR_MUX(1UL);                       

	PTB->PDDR |= 1UL << 9;
}

int main (void)
{
	leds_Init();
	UART0_Init(9600);
	
	char c = 0;
	
	for (c = 'a'; c <= 'g'; c++)
	{
		UART0_Transmit(c);
	}


	while (1)
	{
		data = UART0_Receive();

		if (data == 50) PTB->PTOR |= 1UL << 9; // green // kod ASCII
		UART0_Transmit(data);
	}
}
