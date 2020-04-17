/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : Jan/21/2019                                                                                   */
/***********************************************************************************************************/

/***********************************************************************************************************
//  File Function: MS51 UART0 receive and transmit loop test
/***********************************************************************************************************/
#include "MS51_32K.H"
#include <math.h>
/************************************************************************************************************/
/*  Main function                                                                                           */
/************************************************************************************************************/
void Buffer_Full_Check(void)
{
		/*UART0 Read flg ---  UART1 Read flg*/
		if(Write1Full_flg || Read1Full_flg)
		{
			UART1_Blocking_LedON;
		}
		else
		{
			UART1_Blocking_LedOFF;
		}
		
		/*UART0 Read flg ---  UART2 Read flg*/
		if(Write2Full_flg || Read2Full_flg)
		{
			UART2_Blocking_LedON;
		}
		else
		{
			UART2_Blocking_LedOFF;
		}
		
		/*UART0 Read flg ---  UART3 Read flg*/
		if(Write3Full_flg || Read3Full_flg)
		{
			UART3_Blocking_LedON;
		}
		else
		{
			UART3_Blocking_LedOFF;
		}
		
		/*UART0 Read  flg ---  UART4 Read flg*/
		if(Write4Full_flg || Read4Full_flg)
		{
			UART4_Blocking_LedON;
		}
		else
		{
			UART4_Blocking_LedOFF;
		}		
		
		/*UART1,2,3,4 Position Read */
		if(ReadNFull_flg || ReadSFull_flg)
		{
			UART0_Blocking_LedON;
		}
		else
		{
			UART0_Blocking_LedOFF;
		}
}

void BaudRate_Select(void)
{
	unsigned char  i;
	xdata unsigned int	 temp[3]; 

	/*ADC LEVEL:  3.3K / 6.8K ---  0x00(0V) / 0x53(1.6V) / 0xAC(3.36V) / 0xF4(4.96V) */
	ENABLE_ADC_CH4;  /* UART0 */
	for(i=0; i<3; i++)
	{
		Timer2_Delay500us(2); 
		clr_ADCCON0_ADCF;
		set_ADCCON0_ADCS;                  // ADC start trig signal
		while(ADCF == 0);
		temp[i]=ADCRH;					
	}
	temp[0] = (temp[0]+temp[1]+temp[2])/3;
	
	if(temp[0] < LEVLE_1)//20)
	{
		UART_Open(24000000, UART0_Timer1, BaudRate_9600);
	}
	else if(temp[0] < LEVLE_2)//73)
	{
		UART_Open(24000000, UART0_Timer1, BaudRate_19200);
	}
	else if(temp[0] < LEVLE_3)//cc)
	{
		UART_Open(24000000, UART0_Timer1, BaudRate_38400);
	}
	else
	{
		UART_Open(24000000, UART0_Timer1, BaudRate_57600);
	}

	ENABLE_ADC_CH7;  /* UART1 */
	for(i=0; i<3; i++)
	{
		Timer2_Delay500us(2);
		clr_ADCCON0_ADCF;
		set_ADCCON0_ADCS;                  // ADC start trig signal
		while(ADCF == 0);
		temp[i]=ADCRH;					
	}
	temp[0] = (temp[0]+temp[1]+temp[2])/3;
	
	if(temp[0] < LEVLE_1)
	{
		UART_Open(24000000, UART1_Timer3, BaudRate_9600);
	}
	else if(temp[0] < LEVLE_2)
	{
		UART_Open(24000000, UART1_Timer3, BaudRate_19200);
	}
	else if(temp[0] < LEVLE_3)
	{
		UART_Open(24000000, UART1_Timer3, BaudRate_38400);
	}
	else
	{
		UART_Open(24000000, UART1_Timer3, BaudRate_57600);
	}				

	ENABLE_ADC_CH13; /* UART2 */
	for(i=0; i<3; i++)
	{
		Timer2_Delay500us(2);
		clr_ADCCON0_ADCF;
		set_ADCCON0_ADCS;                  // ADC start trig signal
		while(ADCF == 0);
		temp[i]=ADCRH;					
	}			
	temp[0] = (temp[0]+temp[1]+temp[2])/3;
	
	if(temp[0] < LEVLE_1)
	{
		UART2_Open(24000000,BaudRate_9600); 
	}
	else if(temp[0] < LEVLE_2)
	{
		UART2_Open(24000000,BaudRate_19200); 
	}
	else if(temp[0] < LEVLE_3)
	{
		UART2_Open(24000000,BaudRate_38400); 
	}
	else
	{
		UART2_Open(24000000,BaudRate_57600); 
	}	
	
	ENABLE_ADC_CH14; /* UART3 */
	for(i=0; i<3; i++)
	{
		Timer2_Delay500us(2);
		clr_ADCCON0_ADCF;
		set_ADCCON0_ADCS;                  // ADC start trig signal
		while(ADCF == 0);
		temp[i]=ADCRH;					
	}		
	temp[0] = (temp[0]+temp[1]+temp[2])/3;
	
	if(temp[0] < LEVLE_1)
	{
		UART3_Open(24000000,BaudRate_9600); 
	}
	else if(temp[0] < LEVLE_2)
	{
		UART3_Open(24000000,BaudRate_19200); 
	}
	else if(temp[0] < LEVLE_3)
	{
		UART3_Open(24000000,BaudRate_38400); 
	}
	else
	{
		UART3_Open(24000000,BaudRate_57600); 
	}	
	
	ENABLE_ADC_CH12;  /* UART4 */
	for(i=0; i<3; i++)
	{
		Timer2_Delay500us(2);
		clr_ADCCON0_ADCF;
		set_ADCCON0_ADCS;                  // ADC start trig signal
		while(ADCF == 0);
		temp[i]=ADCRH;					
	}				
	temp[0] = (temp[0]+temp[1]+temp[2])/3;
	
	if(temp[0] < LEVLE_1)
	{
		UART4_Open(24000000,BaudRate_9600); 
	}
	else if(temp[0] < LEVLE_2)
	{
		UART4_Open(24000000,BaudRate_19200); 
	}
	else if(temp[0] < LEVLE_3)
	{
		UART4_Open(24000000,BaudRate_38400); 
	}
	else
	{
		UART4_Open(24000000,BaudRate_57600); 
	}	
	
}

void Buff_Init(void)
{
	unsigned int i;
	
	for(i=0; i< WriteData_MAX; i++) /*128*/
	{
		Uart1TXBuff[i] = 0;
		Uart2TXBuff[i] = 0;
		Uart3TXBuff[i] = 0;
		Uart4TXBuff[i] = 0;
	}
	
	for(i=0; i< PauseData_MAX; i++) /*32*/
	{
		Uart1TXPauseBuff[i] = 0;		
		Uart2TXPauseBuff[i] = 0;
		Uart3TXPauseBuff[i] = 0;
		Uart4TXPauseBuff[i] = 0;
	}
	
	for(i=0; i< ReadData_MAX; i++) /*256*/
	{
		Uart1RXBuff[i] = 0;
		Uart2RXBuff[i] = 0;
		Uart3RXBuff[i] = 0;
		Uart4RXBuff[i] = 0;		
	}
	for(i=0; i< UartNData_MAX; i++) /*64*/
	{	
		UartNBuff[i][0] = 0;
		UartNBuff[i][1] = 0;
	}
	
	for(i=0; i< UartSData_MAX; i++) /*16*/
	{	
		UartSBuff[i] = 0;
	}
}


 void main(void)
{

	MODIFY_HIRC(HIRC_24); 	
	
/* 	GPIO MODE SETTING */
	P05_INPUT_MODE;
	P11_INPUT_MODE;
	P13_INPUT_MODE;	
	P14_INPUT_MODE;
	P24_INPUT_MODE;
	
	P34_INPUT_MODE;
	P17_INPUT_MODE;
	P30_INPUT_MODE;

	P21_PUSHPULL_MODE;	
	P35_PUSHPULL_MODE;	
	P12_PUSHPULL_MODE;	
	P01_PUSHPULL_MODE;	
	P33_PUSHPULL_MODE;

	UARTALL_Blocking_LedOFF; 


/* UART INITIAL START*/
	P06_PUSHPULL_MODE;
	P07_INPUT_MODE;

	P10_PUSHPULL_MODE;
	P00_INPUT_MODE;

	P03_PUSHPULL_MODE;                          
	P04_INPUT_MODE;         

	P15_PUSHPULL_MODE;
	P25_INPUT_MODE;

	P23_PUSHPULL_MODE;                         
	P22_INPUT_MODE;                            
	
	
	ENABLE_UART1_TXD_P10;
	ENABLE_UART1_RXD_P00;

	ENABLE_UART2_TXD_P03;
	ENABLE_UART2_RXD_P04;
	
	ENABLE_UART3_TXD_P15;
	ENABLE_UART3_RXD_P25;

	ENABLE_UART4_TXD_P23;
	ENABLE_UART4_RXD_P22;


//	UART_Open(24000000, UART1_Timer3, 9600);
//	UART2_Open(24000000,9600);                 /* Open UART2 use timer1 as baudrate generate and baud rate = 115200*/
//	UART3_Open(24000000,9600);                 /* Open UART3 use timer1 as baudrate generate and baud rate = 115200*/	
//	UART4_Open(24000000,9600);                 /* Open UART3 use timer1 as baudrate generate and baud rate = 115200*/

	BaudRate_Select();
	Buff_Init();
	FirstData_flg = 1;
	
	ENABLE_UART0_INTERRUPT;                                   /* Enable UART0 interrupt */
	ENABLE_UART1_INTERRUPT;                 //For interrupt enable , interrupt subroutine is in uart.c file	
	ENABLE_SC0_RECEIVE_DATA_REACH_INTERRUPT;
	ENABLE_SC1_RECEIVE_DATA_REACH_INTERRUPT;
	ENABLE_SC2_RECEIVE_DATA_REACH_INTERRUPT;

/*TIMER0 MODE1 INITIAL */ 
	ENABLE_TIMER0_MODE1;
	TIMER0_FSYS_DIV12;

	TH0 = TH0_INIT;
	TL0 = TL0_INIT;
	
	ENABLE_TIMER0_INTERRUPT;                       //enable Timer0 interrupt
	set_TCON_TR0;                                  //Timer0 run

	set_SC0IE_RDAIEN;
	set_SC1IE_RDAIEN;
	set_SC2IE_RDAIEN;

	clr_SC0CR0_TXOFF;
	clr_SC1CR0_TXOFF;
	clr_SC2CR0_TXOFF;
	
	 
	SFRS=0;
  ENABLE_GLOBAL_INTERRUPT;                                 /* Global interrupt enable */
	
	while(1)
  {
		Buffer_Full_Check();
	}
}