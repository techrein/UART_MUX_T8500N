/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: ML51 simple GPIO toggle out demo code
//***********************************************************************************************************

#include "MS51_32K.h"

/**
 * @brief       UART2 TXD output demo
 * @param       None
 * @return      None
 * @details     conned UART2 and UART0 to loop check.
 */

void main (void) 
{
    unsigned char temp=0x30;
		MODIFY_HIRC(HIRC_24); 
    UART2_Open(24000000,9600);                 /* Open UART2 use timer1 as baudrate generate and baud rate = 115200*/
    P03_QUASI_MODE;                              /* Set UART2_TXD pin P3.0 as Quasi mode */
    P04_INPUT_MODE;                              /* Set UART2_RXD pin P1.7 as Input mode */
    ENABLE_UART2_TXD_P03;
    ENABLE_UART2_RXD_P04;
    ENABLE_SC0_RECEIVE_DATA_REACH_INTERRUPT;
    ENABLE_GLOBAL_INTERRUPT;

   
/* while receive data from RXD, send this data to TXD */
  while(1)
  {
    if (uart2rvflag)
    {
      uart2rvflag = 0;
      UART2_Send_Data(uart2rvbuffer);
    }
  }

  }
