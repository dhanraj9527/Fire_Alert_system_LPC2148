
#ifndef _DHT_h
#define _DHT_h

#define DHT_PIN 25 // PORT0 


#define SET_BIT(byte, bit) ((byte) |= (1 << (bit)))
#define CLEAR_BIT(byte,bit) ((byte) &= ~(1 << (bit)))
#define DHT11_INPUT() 	(IODIR0 &= ~(1<<DHT_PIN)) 
#define DHT11_OUTPUT() 	(IODIR0 |= (1<<DHT_PIN)) 
#define DHT11_SET()			(IOSET0 = (1<<DHT_PIN))
#define DHT11_CLR() 		(IOCLR0 = (1<<DHT_PIN)) 
#define DHT11_READ()		(IOPIN0&(1<<DHT_PIN))	


#include <stdint.h>

void initDHT(void);

char fetchData(char* arr);

#endif
