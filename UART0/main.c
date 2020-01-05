#include "MKL05Z4.h"
#include "uart0messenger.h"

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
	
	char * send = "TEST";
	char * receive = "";
	
	UART0_Transmit(send);

	while (1)
	{
		UART0_Receive(receive);
		
		if (receive == send) PTB->PTOR |= 1UL << 9; // green // kod ASCII
		
		UART0_Transmit(send);
	}
}
