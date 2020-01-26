#include "nRF24L01.h"
#include "NRF24.h"
#include "../SPI/SPImessenger.h"
#include <cstring>

#define _BV(bit) (1<<(bit))
#define rf24_max(a,b) (a>b?a:b)
#define rf24_min(a,b) (a<b?a:b)

void delay(void)
{
	int i = 0;
	for (i =0 ; i <= 100; i++) {}
	i = 0;
}

void delay1(void)
{
	int i = 0;
	for (i =0 ; i <= 750; i++) {}
	i = 0;
}
void delay11(void)
{
	int i = 0;
	for (i =0 ; i <= 375; i++) {}
	i = 0;
}
void delay2(void)
{
	int i = 0;
	for (i =0 ; i <= 10000000; i++) {}
	i = 0;
}

void ledsInit(void)
{
	SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK);      
  PORTB->PCR[9] = PORT_PCR_MUX(1UL); 
	
	PTB->PDDR |= 1UL << 9;
}

void beginTransaction(void) {
	//csn
	PTB->PCOR |= 1UL << 11;
}

void endTransaction(void) {
	//csn
	PTB->PSOR |= 1UL << 11;
}

void CE_LOW(void) {
	//ce
	PTA->PCOR |= 1UL << 5;
}

void CE_HIGH(void) {
	//ce
	PTA->PSOR |= 1UL << 5;
}

void write_register(uint8_t reg, char value)
{
	beginTransaction();
	SPI_Transmit(W_REGISTER | reg);
	SPI_Transmit(value);
	delay1();
	endTransaction();
}

char read_register(uint8_t reg)
{
	char result;

	beginTransaction();
	SPI_Transmit(R_REGISTER |  reg);
	SPI_Transmit(0xFF);
	result = SPI_Receive();
	result = SPI_Receive();
	delay1();
	endTransaction();
	delay();
	result = SPI_Receive();

	
	return result;
}

void setRetries(uint8_t delay, uint8_t count)
{
	write_register(SETUP_RETR, (delay & 0xf) << ARD | (count & 0xf) << ARC);
}

void setDataRate(void)
{
	char setup = read_register(RF_SETUP) ;
	
	setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));
	
	setup |= _BV(RF_DR_HIGH);  //LOW 250kBS  HIGH 2MBS

	delay();
	
	write_register(RF_SETUP, setup);
}

void setChannel(uint8_t channel)
{
	write_register(RF_CH, channel);
}

void toggle_features(void)
{
	beginTransaction();
	SPI_Transmit(ACTIVATE);
	SPI_Transmit(0x73);
	delay1();
	endTransaction();
}

void spiTrans(uint8_t cmd) 
{
	beginTransaction();
	SPI_Transmit(cmd);
	delay11();
	endTransaction();
}

void flush_rx(void)
{
	spiTrans(FLUSH_RX);
}

void flush_tx(void)
{
	spiTrans(FLUSH_TX);
}

void powerUp(void)
{
	char cfg = read_register(NRF_CONFIG);
	
	if (!(cfg & _BV(PWR_UP))) 
	{
		write_register(NRF_CONFIG, cfg | _BV(PWR_UP));
		delay();	
		//UART0_Transmit_word("powerUP\n\r");
	}
}

uint8_t isChipConnected(void)
{
	uint8_t setup = read_register(SETUP_AW);
	if (setup >= 1 && setup <= 3)
	{
		return 1;
	}
	return 0;
}

uint8_t getDataRate( void )
{
  uint8_t dr = read_register(RF_SETUP) & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

  if ( dr == _BV(RF_DR_LOW) )
  {
    return 250;
  }
  else if ( dr == _BV(RF_DR_HIGH) )
  {
    return 2;
  }
  else
  {
    return 1;
  }
}


void write_register2(uint8_t reg, const uint8_t* buf, uint8_t len)
{
	beginTransaction();
	SPI_Transmit(W_REGISTER | (REGISTER_MASK & reg));
	while (len--)
		SPI_Transmit(*buf++);
		delay1();
	endTransaction();
}

void openWritingPipe(const uint8_t* address)
{
	delay();
	write_register2(RX_ADDR_P0, address, 5);
	delay();
	
	write_register2(TX_ADDR, address, 5);
	delay();
	
	write_register(RX_PW_P0, 32);
	delay();
}

void stopListening(void)
{
	delay1();
	delay1();
	
	delay1();
	delay1();
	//flush_tx();
	delay();
	
	//write_register(NRF_CONFIG, read_register(NRF_CONFIG) & ~_BV(PRIM_RX));
	//delay();
	write_register(NRF_CONFIG,0x1F);
	//write_register(EN_RXADDR, read_register(EN_RXADDR) | _BV(ERX_P0)); // Enable RX on pipe0
	//delay();
}

void write_payload(const uint8_t buf, uint8_t data_len, const uint8_t writeType)
{
  const uint8_t current = (buf);

   data_len = rf24_min(data_len, 32);

	beginTransaction();
	SPI_Transmit(writeType);
	while (data_len--) 
	{
		SPI_Transmit(current);
		delay();
	}
	endTransaction();
	

}

void startFastWrite(const uint8_t buf, uint8_t len) 
{ 
	write_payload( buf, len,  W_TX_PAYLOAD);
	//ce high
	PTA -> PSOR |= 1UL << 5;
	delay1();
	delay1();
	delay1();
	//ce low
	PTA->PCOR |= 1UL << 5;
	delay();
	//write_register(NRF_STATUS,0x1E);
	//delay();
	
	
	////
	//write_register(NRF_STATUS,0x50);
	//delay();
	//spiTrans(0xFF);
	//delay();
	/////
}

void write(const uint8_t buf, uint8_t len)
{
	flush_tx(); 
	delay();
	
	startFastWrite(buf, len);
	delay();
	
	//write_register(NRF_STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );
	//delay();
	


}

void openReadingPipe(const uint8_t *address)
{
	/////child = 0////
	delay();
	memcpy(pipe0_reading_address,&address,5);
	delay();
	
	write_register2(RX_ADDR_P0, address, 5);
	delay();
	
	write_register(RX_PW_P0,32);
	delay();
	
	write_register(EN_RXADDR,read_register(EN_RXADDR) | _BV(ERX_P0));
	delay();
}

void closeReadingPipe( uint8_t pipe )
{
  write_register(EN_RXADDR,read_register(EN_RXADDR) & ~_BV(ERX_P1));
	delay();
}

void startListening(void)
{
	//delay();
	write_register(NRF_CONFIG, 0x1E);
  //write_register(NRF_CONFIG, read_register(NRF_CONFIG) | _BV(PRIM_RX));
	//delay();
	
	//write_register(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );
	//delay();

  //write_register2(RX_ADDR_P0, pipe0_reading_address, 5);	
	//delay();
	//PTA->PSOR |= 1UL << 5;
	//flush_tx();
}

void read_payload( void* buf, uint8_t data_len)
{
	 uint8_t* current = buf;
	
  if(data_len > 32) data_len = 32;
  char blank_len = dynamic_payloads_enabled ? 0 : 32 - data_len;
	
	beginTransaction();
  SPI_Transmit( R_RX_PAYLOAD );
  while ( data_len-- ) 
	{
		SPI_Transmit(0xFF);
		SPI_Receive();
		SPI_Receive();
		delay11();
		*current++ = SPI_Receive();
  }
  while ( blank_len-- ) 
	{
    SPI_Transmit(0xff);
		delay11();
	}
  endTransaction();
}


void read( void* buf, uint8_t len )
{
  read_payload( buf, len );
	delay();
}

void RF24Init(void)
{
	
	pipe0_reading_address[0]=0;
	
	//ce
	PTA -> PCOR |= 1UL << 5;
	delay();
	
	//csn 
	PTA -> PSOR |= 1UL << 11;
	delay();
	
	write_register(NRF_CONFIG, 0x0C);			//1
	delay();
	
	setDataRate(); //2MBPS				//2
	delay();

	setChannel(50);
	delay();

	flush_rx();	
	delay();
	
	flush_tx();
	delay();
	
	write_register(EN_AA,1);
	delay();
	
	write_register2(RX_ADDR_P0, rxAddr, 5);
	delay();
	
	write_register2(TX_ADDR, rxAddr, 5);
	delay();
	
	write_register(RX_PW_P0, 32);
	delay();
	
	write_register(EN_RXADDR,read_register(EN_RXADDR) | _BV(ERX_P0));
	delay();
	
	write_register(SETUP_AW, 0x03);
	delay();
	
	write_register(RF_SETUP,1<<3|1<<2);
	delay();
	
	powerUp(); 
	delay();
}

