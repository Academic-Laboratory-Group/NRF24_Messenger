#include "NRF24_Messenger.h"

int main(void)
{
	//SETUP
	messenger_Init();
	
	//LOOP
	while(1)
	{
		messenger_Update();
	}
		
	
	//EXIT
	return 0;
}
