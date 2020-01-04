#ifndef uart0messenger_h
#define uart0messenger_h

#include "MKL05Z4.h"

void UART0_Init(uint32_t naud_rate);
void UART0_Transmit(char * data);
uint8_t UART0_Receive(char * data);

#endif
