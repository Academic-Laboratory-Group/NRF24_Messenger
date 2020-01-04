#include "NRF24_Messenger.h"

int main(void)
{
	//SETUP
	messenger_Init();
	
	//LOOP
	while(messenger_Update());
	
	//EXIT
	return 0;
}
