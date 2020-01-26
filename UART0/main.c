/*//------------------------------------------

  UART0 library test
  --------------------------------
  \description:
    Tests if data from our variable
		is transmitted to UART and console
		in connected computer. Putty was 
		used as our console to read from 
		COM where kl05 was connected.
		It's also testing receiving from 
		UART0 by the same way. 
*///------------------------------------------
#include "MKL05Z4.h"
#include "uart0messenger.h"

//brief\Char data used for testing
char data = 0;

//brief\Init for leds. It's easier 
//			to debug with leds.
void leds_Init(void)
{
	SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK);      
  PORTB->PCR[9] = PORT_PCR_MUX(1UL);                       

	PTB->PDDR |= 1UL << 9;
}

int main (void)
{
	//Needed inits
	leds_Init();
	UART0_Init(9600);

	//Sending form a to g letters one by one
	for (data = 'a'; data <= 'g'; data++)
	{
		UART0_Transmit(data);
	}


	while (1)
	{
		//Receiving from UART0 to data
		data = UART0_Receive();

		//Checking if data is ASCII '2'
		//If true green led should be turn on
		if (data == 50) PTB->PTOR |= 1UL << 9;
		
		//Transmitting to console what we received
		UART0_Transmit(data);
	}
}
