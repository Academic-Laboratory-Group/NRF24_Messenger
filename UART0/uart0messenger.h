#ifndef uart0messenger_h
#define uart0messenger_h

#include "MKL05Z4.h"

void uart0Init(uint32_t naud_rate);
void UART0_Transmit(uint8_t data);
uint8_t UART0_Receive(void);

#endif
