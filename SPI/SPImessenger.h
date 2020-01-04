#ifndef SPImessenger_h
#define SPImessenger_h

#include "MKL05Z4.h"

void SPI_Init(void);
void SPI_Transmit(char * data);
uint8_t SPI_Receive(char * data);

#endif
