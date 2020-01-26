/*//------------------------------------------

  SPI library test
  --------------------------------
  \description:
    Tests if data from our variable
		is transmitted by SPI. 
		Analog Discovery was used for 
		checking packages and on it's
		software all transmission was
		observed.
*///------------------------------------------
#include "MKL05Z4.h"
#include "SPImessenger.h"

//Enum with test types
enum TestType
{
	TRANSMITTING = 0,
	RECEIVING = 1
};

//Choose test type which You need
uint8_t test = TRANSMITTING;

int main (void)
{
	//init
	SPI_Init();
	//variable to send to SPI
	char data[4]= "TEST";
	
	//Transmitting
	while (test == TRANSMITTING)
	{	
		SPI_Transmit(data[0]);
		SPI_Transmit(data[1]);
		SPI_Transmit(data[2]);
		SPI_Transmit(data[3]);
	}
	
	//Receiving
	while (test == RECEIVING)
	{	
		data[0] = SPI_Receive();
		data[1] = SPI_Receive();
		data[2] = SPI_Receive();
		data[3] = SPI_Receive();
	}
}
