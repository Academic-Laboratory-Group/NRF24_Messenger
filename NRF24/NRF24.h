/*//------------------------------------------

  NRF24 library
  --------------------------------
  description:
    Sets everything needed for NRF24, 
		transmits and receives data
		from it. Has some other internal
        needed functions.
*///------------------------------------------

#ifndef NRF24
#define NRF24

#include "MKL05Z4.h"

//brief\edging bites
#define _BV(bit) (1<<(bit))

//brief\Max of two variables
#define rf24_max(a,b) (a>b?a:b)

//brief\Min of two variables
#define rf24_min(a,b) (a<b?a:b)

//brief\Delay for ... 
void delay(void);

//brief\Delay for ... 
void delay1(void);

//brief\Delay for ... 
void delay11(void);

//brief\Delay for 210 us
void delay2(void);

//brief\Sets proper values on
//			registers to begin transaction
void beginTransaction(void);

//brief\Sets proper values on
//			registers to finish transaction
void endTransaction(void);

//brief\Sets Clock Enable register low
void CE_LOW(void);

//brief\Sets Clock Enable register high
void CE_HIGH(void);

//brief\Writes on registers
//param\Address where we want to write
//param\Value which we want to write on
void write_register(uint8_t reg, char value);

//brief\Reads from registers
//param\Address from which we want to read
//returns\Value which we have read
char read_register(uint8_t reg);

//brief\Sets number of retries
//param\Address where we want to write
//param\Value which we want to write on
void setRetries(uint8_t delay, uint8_t count);

//brief\Sets default data rate
void setDataRate(void);

//brief\Sets channel of transmission
//param\Chosen channel
void setChannel(uint8_t channel);

//brief\Transmit commands to radio by SPI
//param\Command to transmit
void spiTrans(uint8_t cmd);

//brief\Cleans rx fifo
void flush_rx(void);

//brief\Cleans tx fifo
void flush_tx(void);

//brief\Turns PWR_UP register on (HIGH)
void powerUp(void);

//brief\CGives info about connected/
//			disconnected radio
//return\true or false in uint8_t
uint8_t isChipConnected(void);

//brief\Gives info about data rate
//returns\Value of data rate. If
//			  250 - it is 250KBs
//				1 - 1MBs
//        2 - 2MBs
uint8_t getDataRate( void );

//brief\Writes buffor to registers
//param\Adress to start from
//param\Length of buffor
void write_register2(uint8_t reg, uint8_t len);

//brief\Sets everything needed to stop 
//			reading and start writing
void stopListening(void);

//brief\Writes all payload to radio
//param\Buffor to write
//param\Length of data to write
//param\Type of writting. Look on radio spec.
void write_payload(const uint8_t buf, uint8_t data_len, const uint8_t writeType);

//brief\Starts fast writting
//param\Buffor to send
//param\Length of data to send
void startFastWrite(const uint8_t buf, uint8_t len) ;

//brief\Writes to radio
//param\Buffor to send
//param\Length of buffor to send
void write(const uint8_t buf, uint8_t len);

//brief\Closes reading pipe
//param\Pipe we want to close
void closeReadingPipe( uint8_t pipe );

//brief\Sets everything needed to start reading
void startListening(void);

//brief\Reads all payload
//param\Pointer to buffer to read to
//param\Length of data to read
void read_payload( void* buf, uint8_t data_len);

//brief\Reads
//param\Pointer to buffer to read to
//param\Length of data to read
void read( void* buf, uint8_t len );

//brief\Inits RF24. Sets all needed 
//			registers required for proper
//			radio work.
void RF24_Init(void);

#endif // NRF24
