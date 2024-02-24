
#include<lpc214x.h>
#include "UART_lib.h"


void UART1_Init(void)
{
  PINSEL0|=0x50300;      //setting rx and tx pin
	U1LCR=0x83;
	U1DLL=0x61;
	U1DLM=0x00;
	U1LCR=0x03;
}
void UART1_TX(char data)
{
	while(!(U1LSR&0x20));		//line stats register
    U1THR=data;		//Transmit Holding Circuit
}
char UART1_RX(void)
{
    char p;
	while(!(U1LSR&0x01));
	p=U1RBR;		//Reciever Buffer Register
	return p;
}
void UART1_String(char String[])
{
    unsigned char i;
	for(i=0;String[i]!='\0';i++)
	{
		UART1_TX(String[i]);
	}
}
void UART1_Char_Response(char data)
{
	char Received_char;
	Received_char = UART1_RX();
	while(Received_char != data)
	{
		Received_char = UART1_RX();
	}
}



void UART0_Init(void)
{
    PINSEL0|=0x5;
	U0LCR=0x83;
	U0DLL=0x61;
	U0DLM=0x00;
	U0LCR=0x03;
}
void UART0_TX(char data)
{
	while(!(U0LSR&0x20));
    U0THR=data;
}
char UART0_RX(void)
{
    char p;
	while(!(U0LSR&0x01));
	p=U0RBR;
	return p;
}
void UART0_String(char String[])
{
    unsigned char i;
	for(i=0;String[i]!='\0';i++)
	{
		UART0_TX(String[i]);
	}
}
void UART0_Char_Response(char data)
{
	char Received_char;
	Received_char = UART0_RX();
	while(Received_char != data)
	{
		Received_char = UART0_RX();
	}
}
void TX0_Int(int INT)
{
	UART0_TX(((INT%1000)%100)/10 + 48);
	UART0_TX(((INT%1000)%100)%10 + 48);
}
