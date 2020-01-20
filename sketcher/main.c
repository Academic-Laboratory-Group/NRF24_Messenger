#include "MKL05Z4.h"
#include "uart0messenger.h"
#include "nRF24L01.h"

#define _BV(bit) (1<<(bit))
#define rf24_max(a,b) (a>b?a:b)
#define rf24_min(a,b) (a<b?a:b)

void UART0_Transmit2(uint8_t data)
{
	while (!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = data;
}


void delay(void)
{
	int i = 0;
	for (i =0 ; i <= 100; i++) {}
	i = 0;
}

void delay2(void)
{
	int i = 0;
	for (i =0 ; i <= 750; i++) {}
	i = 0;
}
void delay1(void)
{
	int i = 0;
	for (i =0 ; i <= 10000000; i++) {}
	i = 0;
}

void ledsInit(void)
{
	SIM->SCGC5 |=  (SIM_SCGC5_PORTB_MASK);      
  PORTB->PCR[9] = PORT_PCR_MUX(1UL); 
	
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB -> PCR[11] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[11] |= PORT_PCR_MUX(1UL);
	PTB->PDDR |= 1UL << 11;
	PTB -> PSOR |= 1UL << 11;
	
	PTB->PDDR |= 1UL << 9;
}

void SPI_Init ( void ) 
{
   
  SIM -> SCGC4 |= SIM_SCGC4_SPI0_MASK ;       // wlacz zegar do modulu spi0
   
	SIM -> SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK);     // wlacz zegar do portu A.
	
	SPI0 -> C1 &=~ SPI_C1_SPE_MASK ;         // Wylacz system SPI
	PORTA -> PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTA -> PCR[5] |= PORT_PCR_MUX(1)|PORT_PCR_DSE_MASK;			  //SS
	
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
	
	PTA -> PDDR |= 1UL << 5;
	PTA -> PSOR |= 1UL << 5;
	
	SPI0->C1 |= SPI_C1_SPE_MASK;			//wlacz System SPI
}

char SPI_Transmit (char data)
{
	while(!(SPI0->S & SPI_S_SPTEF_MASK));
	SPI0->D = data;
	
	return SPI0 -> D;
}

char SPI_Receive(void)
{
	while (!(SPI0->S & SPI_S_SPRF_MASK));
	return SPI0->D;
}

void beginTransaction(void) {
	//csn
	PTB->PCOR |= 1UL << 11;
	//ss
	PTA -> PCOR |= 1UL << 5;
}

void endTransaction(void) {
	//csn
	PTB->PSOR |= 1UL << 11;
	//ss
	PTA -> PSOR |= 1UL << 5;
}

char write_register(uint8_t reg, char value)
{
	char status;

	beginTransaction();
	SPI_Transmit(W_REGISTER | (REGISTER_MASK & reg));
	status = SPI_Transmit(value);
	delay2();
	endTransaction();

	return status;
}

char read_register(uint8_t reg)
{
	char result;

	beginTransaction();
	SPI_Transmit(R_REGISTER | (REGISTER_MASK & reg));
	SPI_Transmit(0xFF);
	result = SPI_Receive();
	result = SPI_Receive();
	delay2();
	endTransaction();
	
	return result;
}

void print_byte_register(const char* name, uint8_t reg, uint8_t qty)
{
	UART0_Transmit_word(name);
  UART0_Transmit(read_register(reg));
}

void setRetries(uint8_t delay, uint8_t count)
{
	write_register(SETUP_RETR, (delay & 0xf) << ARD | (count & 0xf) << ARC);
}

void setDataRate(void)
{
	char setup = read_register(RF_SETUP) ;
	//uint8_t data = 0;
	setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));
	
	setup |= _BV(RF_DR_LOW);

	delay();
	
	write_register(RF_SETUP, setup);
	delay();
}

void setChannel(uint8_t channel)
{
	const uint8_t max_channel = 125;
	write_register(RF_CH, rf24_min(channel, max_channel));
}

void toggle_features(void)
{
	beginTransaction();
	SPI_Transmit(ACTIVATE);
	SPI_Transmit(0x73);
	delay();
	endTransaction();
}

uint8_t spiTrans(uint8_t cmd) {

	uint8_t status;

	beginTransaction();
	SPI_Transmit(cmd);
	status = SPI_Receive();
	delay();
	endTransaction();

	return status;
}

uint8_t flush_rx(void)
{
	return spiTrans(FLUSH_RX);
}

uint8_t flush_tx(void)
{
	return spiTrans(FLUSH_TX);
}

void powerUp(void)
{
	uint8_t cfg = read_register(NRF_CONFIG);

	if (!(cfg & _BV(PWR_UP))) 
	{
		write_register(NRF_CONFIG, cfg | _BV(PWR_UP));
		delay();	
		UART0_Transmit_word("powerUP\n\r");
	}
}

void isChipConnected(void)
{
	uint8_t setup = read_register(SETUP_AW);
	if (setup >= 1 && setup <= 3)
	{
		UART0_Transmit_word("ChipisConnected\n\r");
	}

}

void available(void)
{
	if (!(read_register(FIFO_STATUS) & _BV(RX_EMPTY))) 
	{
		UART0_Transmit_word("ChipisAvailable\n\r");
	}

}

void getDataRate( void )
{
  uint8_t dr = read_register(RF_SETUP) & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

  // switch uses RAM (evil!)
  // Order matters in our case below
  if ( dr == _BV(RF_DR_LOW) )
  {
    UART0_Transmit_word("250KBPS");
  }
  else if ( dr == _BV(RF_DR_HIGH) )
  {
    // '01' = 2MBPS
    UART0_Transmit_word("2MPS");
  }
  else
  {
    // '00' = 1MBPS
    UART0_Transmit_word("1MBPS");
  }
}


uint8_t write_register2(uint8_t reg, const uint8_t* buf, uint8_t len)
{
	uint8_t status;

	beginTransaction();
	status = SPI_Transmit(W_REGISTER | (REGISTER_MASK & reg));
	while (len--)
		SPI_Transmit(*buf++);
		delay();
	endTransaction();

	return status;
}

void openWritingPipe(const uint8_t* address)
{
	// Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
	// expects it LSB first too, so we're good.
	
	write_register2(RX_ADDR_P0, address, 5);
	delay();
	
	write_register2(TX_ADDR, address, 5);
	delay();
	
	//const uint8_t max_payload_size = 32;
	//write_register(RX_PW_P0,rf24_min(payload_size,max_payload_size));
	write_register(RX_PW_P0, 32);
	delay();
	uint8_t dane = read_register(RX_PW_P0);
	
	//UART0_Transmit2(dane);
}

void stopListening(void)
{
	uint8_t read1 = 0;
	uint8_t read2 = 0;
	
	//ce low
	PTA -> PCOR |= 1UL << 5;

	delay();

	if (read_register(FEATURE) & _BV(EN_ACK_PAY)) {
		delay(); //200
		flush_tx();
	}
	//flush_rx();
	delay();
	
	read1 = read_register(NRF_CONFIG);
	delay();
	
	write_register(NRF_CONFIG, (read1) & ~_BV(PRIM_RX));
	delay();
	
	read2 = read_register(EN_RXADDR);
	delay();
	
	write_register(EN_RXADDR, read2 | _BV(ERX_P0)); // Enable RX on pipe0
	delay();

}

uint8_t write_payload(const uint8_t* buf, uint8_t data_len, const uint8_t writeType)
{
	uint8_t status;

	data_len = rf24_min(data_len, 32);

	beginTransaction();
	status = SPI_Transmit(writeType);
	while (data_len--) 
	{
		SPI_Transmit(*buf++);
		delay();
	}
	endTransaction();

	return status;
}

void startFastWrite(const uint8_t* buf, uint8_t len) 
{ 
	write_payload( buf, len, W_TX_PAYLOAD);
	//ce high
	PTA -> PSOR |= 1UL << 5;
}

void write(const uint8_t* buf, uint8_t len)
{
	//Start Writing
	startFastWrite(buf, len);
	delay();
	//ce low
	PTA -> PCOR |= 1UL << 5;
	uint8_t status = 0;
	status = write_register(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
	delay();

	//Max retries exceeded
	if (status & _BV(MAX_RT)) {
		flush_tx(); //Only going to be 1 packet int the FIFO at a time using this method, so just flush
	}
	//TX OK 1 or 0

}

void RF24Init(void)
{
	uint8_t setup = 0;
	uint8_t data = 0;

	delay();
	
	write_register(NRF_CONFIG, 0x0C);			//1
	delay();
	
	setRetries(15, 15);						//1
	delay();
	
	setDataRate(); //2MBPS				//2
	delay();
	
  setup = read_register(RF_SETUP);			//1
	delay();
	
	toggle_features();	
  delay();
																	//????
	write_register(FEATURE, 0);	
	delay();  											//????
	
	write_register(DYNPD,0);
	delay();
	
	write_register(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
	delay();
	
	setChannel(76);
	delay();

	flush_rx();				//?????
	delay();
	
	flush_tx();				//??????
	delay();

	powerUp(); 
	delay();
	
	data = read_register(NRF_CONFIG);
	delay();
	
	write_register(NRF_CONFIG, (data) & ~_BV(PRIM_RX));
	delay();
	
	if ( setup != 0 && setup != 0xff )
	{
		UART0_Transmit_word("Radio is configured\n\r");
	}
}


int main (void)
{
	char c = 0;
	
	ledsInit();
	uart0Init(9600);
	SPI_Init();

	UART0_Transmit('2');
	//RF24Init();


		
	write_register(NRF_CONFIG, 0x02);			//1
	delay();
	read_register(NRF_CONFIG);			//1
	delay();

	
	c = read_register(NRF_CONFIG);
	UART0_Transmit('a');
	UART0_Transmit(c+48);
	

	//available();
	//const uint8_t rxAddr[6] = "00010";
	//openWritingPipe(rxAddr);
	//stopListening();

	
	while (1)
	{

		//uint8_t text = 128;
		//write(&text, sizeof(text));
	
		
	}
}
