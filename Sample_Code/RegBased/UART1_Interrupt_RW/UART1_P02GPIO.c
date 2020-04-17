/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : June/21/2019                                                                                   */
/***********************************************************************************************************/

/************************************************************************************************************/
/*  File Function: MS51 UART1 with interrupt demo                                                           */
/************************************************************************************************************/
#include "MS51_32K.h"

/****************************************************************************************************************
 * FUNCTION_PURPOSE: Main function 
 !!! MS51_8K UART1 pin also occupied by debug pin, 
 please remove Nu-link or not in debug mode to test UART1 function.
 External UART1 connect also disturb debug download
 ***************************************************************************************************************/
void main (void)
{
	unsigned int cnt = 0;
  /* Modify HIRC to 24MHz for UART baud rate deviation not over 1%*/
    MODIFY_HIRC(HIRC_24);
/*
    P36_PUSHPULL_MODE;                      // For I/O toggle display
    P37_INPUT_MODE;
    ENABLE_UART1_TXD_P36;
    ENABLE_UART1_RXD_P37;
*/
    P10_PUSHPULL_MODE;                      // For I/O toggle display
    P00_INPUT_MODE;
    ENABLE_UART1_TXD_P10;
    ENABLE_UART1_RXD_P00;
	
    UART_Open(24000000,UART1_Timer3,9600);
    ENABLE_UART1_INTERRUPT;                 //For interrupt enable , interrupt subroutine is in uart.c file
    ENABLE_GLOBAL_INTERRUPT;                //Enable global interrupt

    while(1)
    {
      if (uart1_receive_flag)
      {   
          uart1_receive_flag = 0;
          UART_Send_Data(UART1,uart1_receive_data);
				  for(cnt = 0; cnt < 20000; cnt++)
				{
				}
			  	UART_Send_Data(UART1,0xab);
				  UART_Send_Data(UART1,0xcc);

      }
    }
}
  
