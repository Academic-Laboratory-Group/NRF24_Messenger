#include "NRF24_Messenger.h"

int main(void)
{
	//SETUP
	messengerInit();
	
	//LOOP
	while(messengerUpdate());
	
	//EXIT
	return 0;
}