#include "uart0messenger.h"
#include "MKL05Z4.h"

char * received_uart_data;

void ASCII_Encoder(uint8_t received)
{
	*received_uart_data = (char)((uint32_t)received);
}

void UART0_Init(uint32_t naud_rate)
{
	uint32_t divisor;
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	PORTB -> PCR[1] |= PORT_PCR_MUX(2);
	PORTB -> PCR[2] |= PORT_PCR_MUX(2);
	
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
	
	UART0 -> C4 |= UART0_C4_OSR(31); //31
	
	divisor = 41940000/(32*9600);
	UART0->BDH = UART0_BDH_SBR(divisor >> 8);
	UART0->BDL = UART0_BDL_SBR(divisor);
	
	UART0->C1 = UART0->S2 = UART0->C3 = 0;
	
	UART0 ->BDH |= UART0_BDH_SBNS_MASK;

	UART0->C2 = UART0_C2_TE_MASK | UART0_C2_RE_MASK;
}

void UART0_Transmit(char * data)
{
	while (!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = *data;
}

uint8_t UART0_Receive(char * data)
{
	while (!(UART0->S1 & UART0_S1_RDRF_MASK));
	ASCII_Encoder(UART0->D);
	if (*received_uart_data != *data)
	{
		*data = *received_uart_data;
		return 1;
	}
	return 0;
}
