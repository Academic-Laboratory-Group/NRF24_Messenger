#include "MKL05Z4.h"

void ledsInit(void)
{
	SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK);      
  PORTB->PCR[9] = PORT_PCR_MUX(1UL);                       

	PTB->PDDR |= 1UL << 9;
	PTB->PCOR = 1UL << 9;
}

void BlinkLED(void){
 uint32_t i = 0;

 PTB->PTOR = 1UL << 9;
 for(i = 0; i < 3000000; i++){};
}


void spiInit ( void ) {
   
    SIM -> SCGC4 |= SIM_SCGC4_SPI0_MASK ;       // wlacz zegar do modulu spi0
   
    SIM -> SCGC5 |= SIM_SCGC5_PORTA_MASK ;     // wlacz zegar do portu A.
		SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK ;
	  
    PORTA -> PCR [7] |= PORT_PCR_MUX (2) ;       // pin7 w porcie A to SPI0 MOSI
	
		PORTA -> PCR [6] |= PORT_PCR_MUX (2);					// pin6 w porcie A to SPI0 MISO
   
    PORTB -> PCR [0] |= PORT_PCR_MUX (2) ;       // pin0 w porcie B to SPI0 SCLK
   
    PORTA -> PCR [ 10 ] |= PORT_PCR_MUX (1) ;       // pin10 w porcie A to SS
 
	  SPI0 -> C1 |= SPI_C1_SPE_MASK ;         // Wlacz system SPI
   
    SPI0 -> C1 |= SPI_C1_MSTR_MASK ;       // Wybrano tryb glówny
   
    SPI0 -> C1 |= SPI_C1_CPOL_MASK ;       // Biegunowosc zegara - aktywny wysoki  //
   
    SPI0 -> C1 &=~ SPI_C1_CPHA_MASK ;       // srodek transmisji										
   
    //SPI0 -> C1 &=~ SPI_C1_LSBFE_MASK ;     // startsccc transmisji z msb
   
    SPI0 -> BR |= SPI_BR_SPPR ( 0b000 ) ;     // predkosci transmisji ..
   
    SPI0 -> BR |= SPI_BR_SPR ( 0b0110 ) ;     // ...
   
    SPI0 -> C2 |= SPI_C2_MODFEN_MASK ;         // slave select									//
   
    SPI0 -> C1 |= SPI_C1_SSOE_MASK ;       // is gpio
		

}

void sendData (int8_t data)
{
	while(!(SPI0->S & SPI_S_SPTEF_MASK)) SPI0->D = data;
}

int main (void)
{
	ledsInit();
	spiInit();

	while (1)
	{
		//BlinkLED();
		sendData(23);
		
	}
}
