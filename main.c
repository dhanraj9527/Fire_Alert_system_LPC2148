//	Header Files
#include <lpc214x.h>
#include "UART_lib.h"
#include "DHT11.h"


//	Intialize input and output 
char Sensor[5] = {0};
#define bit(x) (1<<x)
#define delay_1 for(i=0;i<7000;i++);
#define GAS (IO0PIN & (1<<14))
#define FLAME (IO0PIN & (1<<15))
#define WATER_PUMP 10
unsigned int i,k,j,z;
unsigned int adc(int,int);
unsigned int val;


////////// LCD FUNCTIONS    /////////////

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++) {
        for(j = 0; j < 10000; j++) {
            // Adjust the loop counts according to your clock frequency and desired delay
        }
    }
}

void callen() {
    IOSET1 = (1 << 24); // Set pin 24 high to trigger the enable signal.
    delay_ms(1);
    IOCLR1 = (1 << 24); // Set pin 24 low to end the enable signal.
}

void lcd_cmd(unsigned int cmdcode) {
    IOCLR1 = (1 << 25); 		// Clear the register select (RS) pin to indicate a command.
    IOCLR1 = (255 << 16); 	// Clear bits 16-23 (8 bits) to prepare for sending command data
    IOSET1 = (cmdcode << 16); // Set bits 16-23 according to the command code.
    callen();  								// Trigger the enable signal to send the command.
}

void lcd_data(unsigned int datacode) {
    IOSET1 = (1 << 25);  // Set the RS pin to indicate data.
    IOCLR1 = (255 << 16); // Clear bits 16-23 to prepare for sending data.
    IOSET1 = (datacode << 16); // Set bits 16-23 according to the data code. 
    callen();  // Trigger the enable signal to send the data.
}

void lcd_init() {
    lcd_cmd(0x38); // Initialize 2-line 8-bit mode
    lcd_cmd(0x01); // Clear display
    lcd_cmd(0x06); // Set entry mode (left entry)
    lcd_cmd(0x0E); // Display on, cursor on
    lcd_cmd(0x80); // Set cursor to the first line, first character
}

void lcd_datawr(char *str) {
    int i = 0;
    while (str[i] != '\0') { // Loop until null terminator
        lcd_data((unsigned int)str[i]);
        i++;
        if (i >= 16) {
            break; // Exit the loop after displaying 16 characters
        }
    }
}


/// LCD FUNCTIONS END
// Motor Function 
			void initializeWaterPump()
			{
				IODIR0 |= (1 << WATER_PUMP);
			}

			void turnOnWaterPump()
			{
				IOSET0 = (1 << WATER_PUMP);
			}

			void turnOffWaterPump()
			{
				IOCLR0 = (1 << WATER_PUMP);
			}

void Flame(){
	delay_ms(100);
	lcd_cmd(0x01);
if(FLAME) {
										lcd_datawr(" Flame Detected");
                    UART0_String("Flame Detected " );
										UART0_String("Water Pump Started  " );
										UART0_String("\r\n");
										turnOnWaterPump();
							
            }
						else{
										lcd_datawr("No Flame Detected");
						        UART0_String(" No Flame Detected");
							      UART0_String("\r\n");
										turnOffWaterPump();
							}
						delay_ms(500);
}

void Gas(){
		if(GAS) {						
								UART0_String("Gas Detected " );
								delay_ms(100);
								UART0_String("\r\n");
								Flame();
								UART0_String("\r\n");
								
            }
						else{
										Flame();
						        UART0_String(" No Gas Detected");
							      UART0_String("\r\n");
						}
}
int main()
{
	initializeWaterPump();
	PINSEL2 = 0; // Set the port as GPIO
  IODIR1 = 0x0FFF0000; // Configure pins as outputs for LCD
	lcd_init();
	char fire[] = "Temperature Limit Excedded";
	char action[] = "Checking Flame Sensor";
	initDHT();//DHT pin is connected at P0.30
	UART0_Init();//UART 0 Initialization
	UART0_String("DATA");
	for(k=0;k<20;k++)
	{
			
			if(fetchData(Sensor))
			{
				if(Sensor[2]>40){
					
				UART0_String("Temperature:");
				
				TX0_Int(Sensor[2]);//temp
		    UART0_String("\r\n");
				UART0_String("Temperature Limit Excedded \r\n");
				UART0_String("\r\n");
				lcd_cmd(0x80);	
				lcd_datawr(fire); // Display on lcd
				delay_ms(2);

		lcd_cmd(0xC0); // Move cursor to the second line
    lcd_datawr(action); // Display the roll
    delay_ms(10);
		Flame();
				}
				else if(Sensor[2]<=40){
							UART0_String("Temperature:");				
							TX0_Int(Sensor[2]);//temp
							UART0_String("\r\n");
							Gas();
				}

      }
			else
			{
			 UART0_String("ERROR ");
			}
		}	
}
