#include "MKL05Z4.h"
#include "SPImessenger.h"

int main (void)
{
	SPIInit();

	while (1)
	{
		SPI_Transmit(3);
	}
}
