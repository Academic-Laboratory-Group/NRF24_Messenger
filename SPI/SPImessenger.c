#include "SPImessenger.h"
#include "MKL05Z4.h"

char * received_data;

void SPI_Init ( void ) 
{
   
  SIM -> SCGC4 |= SIM_SCGC4_SPI0_MASK ;       // wlacz zegar do modulu spi0
   
	SIM -> SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK);     // wlacz zegar do portu A.
	
	SPI0 -> C1 &=~ SPI_C1_SPE_MASK ;         // Wylacz system SPI
	PORTA -> PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTA -> PCR[5] |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //SS
	
  PORTA -> PCR[6] &= ~PORT_PCR_MUX_MASK;
	PORTA -> PCR[6] |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //MISO 
	
  PORTA -> PCR[7] &= ~PORT_PCR_MUX_MASK;
	PORTA -> PCR[7] |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //MOSI
	
  PORTB -> PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[0] = PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			    //SCK
	
	
	SPI0->C1 |= SPI_C1_MSTR_MASK;		// ustawienie jak master
	SPI0->BR = 0x06;								//bate  rate
	SPI0->C1 |= SPI_C1_SSOE_MASK;      //slave
	SPI0->C2 |= SPI_C2_MODFEN_MASK;		//ustawienie slave

	SPI0->C1 |= SPI_C1_CPHA_MASK;
	SPI0->C1 &= (~SPI_C1_CPHA_MASK);		
	SPI0->C1 |= SPI_C1_CPOL_MASK;
	SPI0->C1 &= (~SPI_C1_CPOL_MASK);		
	SPI0->C1 &= (~SPI_C1_LSBFE_MASK);
		
	SPI0->C1 |= SPI_C1_SPE_MASK;			//wlacz System SPI
}

void SPI_Transmit (char * data)
{
	while(!(SPI0->S & SPI_S_SPTEF_MASK));
	SPI0->D = *data;
}

uint8_t SPI_Receive(char * data)
{
	if (*received_data != *data)
	{
		*data = *received_data;
		return 1;
	}
	return 0;
}
