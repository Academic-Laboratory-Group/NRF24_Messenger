#ifndef uart0messenger_h
#define uart0messenger_h

#include "MKL05Z4.h"

void uart0Init(uint32_t naud_rate);
void UART0_Transmit(char data);
void UART0_Transmit_word(const char *str);
char UART0_Receive(void);

#endif
