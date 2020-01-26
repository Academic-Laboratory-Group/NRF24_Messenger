/*//------------------------------------------

  NRF24 Messenger library
  --------------------------------
  description:
    Provide transmission from command line
		like Putty to radio and backwards. Can
		be used as messenger for two different 
		computers with KL05 and NRF24 radio 
		connected.
*///------------------------------------------

#ifndef NRF24_Messenger_h
#define NRF24_Messenger_h

//brief\Sets everything needed to communication
void messenger_Init(void);

//brief\Controls communication
//			When it is something to send
//			it sends, if not it is waiting
//			for message
void messenger_Update(void);

#endif // NRF24_Messenger_h
