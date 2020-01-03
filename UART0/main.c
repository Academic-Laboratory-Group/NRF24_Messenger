#include "MKL05Z4.h"
#include "uart0messenger.h"

void ledsInit(void)
{
	SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK);      
  PORTB->PCR[9] = PORT_PCR_MUX(1UL);                       

	PTB->PDDR |= 1UL << 9;
}

void BlinkLED(void){
 uint32_t i = 0;

 PTB->PTOR = 1UL << 9;
 for(i = 0; i < 3000000; i++){};
}

int main (void)
{
	ledsInit();
	uart0Init(9600);
	
	uint8_t data = 0;
	uint8_t c = 0;
	
	//for (c = 1; c <= 10; c++)
	//{
		UART0_Transmit(c);
	//}

	while (1)
	{
		BlinkLED();
		//data = UART0_Receive();
		
		//if (data == 50) PTB->PTOR |= 1UL << 9; // green // kod ASCII
		
		//UART0_Transmit(data);
	}
}
