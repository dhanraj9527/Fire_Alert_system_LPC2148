
#include<lpc214x.h>
#include <stdio.h>
#include "UART_lib.h"
#include "DHT11.h"


unsigned int  data [5];
unsigned int cnt, check;
int i1,j1;
void initDHT(void)
{
    /* According to the DHT11's datasheet, the sensor needs about
       1-2 seconds to get ready when first getting power, so we
       wait
     */
    //_delay_ms(2000);
	PINSEL1 |= 0x00400000;
	T1PR = 14;
	T1TC = 0;
}


char fetchData(char* arr)
{
    /******************* Sensor communication start *******************/
    /* Set data pin as output first */
	   DHT11_OUTPUT();
    
    /* First we need milliseconds delay*/
	   T1TCR = 0x01;
	   T1TC = 0;
    
    /* Clear bit for 20 ms */
    DHT11_CLR();
    /* Wait about 18~20 ms */
     while(T1TC<18000);
		
		T1TCR = 0x01;
	  T1TC = 0;
    
    /* Pull high again */
    DHT11_SET();
		/* Wait about 40 us */
		while(T1TC<40);  
    /* And set data pin as input */
    DHT11_INPUT();
		
		
		
		T1TC = 0;
    /* Wait for response from sensor, 80?s according to datasheet */
		while(!DHT11_READ())
		{
			if(T1TC>=100) return 0;
    }
		
		T1TC = 0;
		while(DHT11_READ())
		{
			if(T1TC>=100) { return 0;}
    }
    /********************* Data transmission start **********************/
    
    for (i1 = 0; i1 < 5; ++i1)
    {
        for(j1 = 7; j1 >= 0; --j1)
        {
            T1TC = 0;
            /* First there is always a 50?s low period */
            while(!DHT11_READ())
            {if (T1TC >= 60) {return 0;} }
            T1TC = 0;
						
            /* Then the data signal is sent. 26 to 28?s (ideally)
             indicate a low bit, and around 70?s a high bit */
            while(DHT11_READ())
            {if (T1TC >= 100) return 0;}
            /* Store the value now so that the whole checking doesn't
             move the TCNT0 forward by too much to make the data look
             bad */
						cnt = T1TC;
            
            if (cnt >= 20 && cnt <= 35)//20-35
            { CLEAR_BIT(data[i1],j1);}
            
            else if (cnt >= 60 && cnt <= 80)//60-80
            { SET_BIT(data[i1],j1); }
            
            else return 0;
						
        }
    }
    /********************* Sensor communication end *********************/
    check = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
    if (check != data[4]) { return 0;};
    
    for(i1 = 0; i1 < 4; ++i1)
    { arr[i1] = data[i1]; }
    return 1;
}
