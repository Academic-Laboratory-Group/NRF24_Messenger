#include "MKL05Z4.h"
#include "SPImessenger.h"

int main (void)
{
	SPI_Init();
	char * data = "TEST";
	
	while (1)
	{
		SPI_Transmit(data);
	}
}
