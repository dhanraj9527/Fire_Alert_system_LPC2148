
#ifndef _UART_lib_H
#define _UART_lib_H

#define PCLK 15000000

void UART1_Init(void);
void UART1_TX(char data);
char UART1_RX(void);
void UART1_String(char String[]);
void UART1_Char_Response(char data);
void TX1_Int(int INT);



void UART0_Init(void);
void UART0_TX(char data);
char UART0_RX(void);
void UART0_String(char String[]);
void UART0_Char_Response(char data);
void TX0_Int(int INT);

#endif
