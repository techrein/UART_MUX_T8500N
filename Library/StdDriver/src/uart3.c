/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jun/21/2019
//***********************************************************************************************************

#include "MS51_32K.h"

//unsigned char uart3rvbuffer=0;
//bit uart3rvflag;

void SMC1_ISR(void) interrupt 22          // Vector @  0x9B
{
    _push_(SFRS);
		set_SC1IE_RDAIEN;
 /* Since only enable receive interrupt, not add flag check */
		if(uart3_receive_flag == 0) 
		{
			uart3_receive_flag = 1;
//			P35 = ~P35;                     // GPIO1 toggle when interrupt			
			Current_TX3_Num = Save_TX_Num;
			TxBuff[Current_TX3_Num][0] = CH_0;
			TxBuff[Current_TX3_Num][2] = SC1DR;		

			TX3_Data = 3;
			Save_TX_Num++;
			if(Save_TX_Num > BuffNum_MAX)  
			{
				Save_TX_Num= 0;
			}						
		}
		else
		{
			
			TxBuff[Current_TX3_Num][TX3_Data] = SC1DR;

//			P35 = ~P35;                     // GPIO1 toggle when interrupt
			TX3_Data++;
			if(TX3_Data > BuffData_MAX)  
			{
				TX3_Data--;							
				BuffData3_OverFlg = 1;
			}	
		
		}
		
		Receive3_delayCnt = 0;
		Receive3_delayFlg = 1;		
//        uart3rvflag = 1;
//        uart3rvbuffer = SC1DR;
    _pop_(SFRS);
}
/**
 * @brief       UART3 transfer data without interrupt 
 * @param       u32SysClock , define Fsys value(value)
 * @param       u32Baudrate, Define baudrate (value)
 * @return      none
 * @details     none
 * @example:    UART3_Open(24000000,115200)
 */ 
void UART3_Open(unsigned long u32SysClock, unsigned long u32Baudrate)
{
    set_SC1CR0_SCEN;         /* Enable SC module */
    set_SC1CR1_UARTEN;       /* set as UART mode */
    set_SC1CR1_CLKKEEP;      
    clr_SC1CR0_CONSEL;       /* clr CONSEL and AUTOCEN*/
    clr_SC1CR0_AUTOCEN;
    SC1ETURD0 = LOBYTE(u32SysClock/8/u32Baudrate-1);  /* define baud rate low byte */
    SC1ETURD1 &= 0xF0;                                /* define baud rate high byte */
    SC1ETURD1 |= (HIBYTE(u32SysClock/8/u32Baudrate-1))&0x0F; 
    set_SC1CR1_PBOFF;     //parity bit disable
    SC1CR1&=0xCF;        //datalegth 8bit
    set_SC1CR0_NSB;      //stop bit = 1bit
	
		set_T3CON_TR3;             //Trigger Timer3
  }

/**
 * @brief       UART3 receive data without interrupt 
 * @param       baudrate value
 * @return      received data
 * @details     none
 * @example:    temp = UART3_Receive_Data()
 */ 
unsigned char UART3_Receive_Data()
{
    unsigned char c;

    clr_SC1CR0_RXOFF;
    while((SC1TSR&&SET_BIT1)==SET_BIT1);
    c = SC1DR;
  
    return (c);
}

/**
 * @brief       UART3 transfer data without interrupt 
 * @param       baudrate value
 * @return      none
 * @details     none
 * @example:    UART3_Send_Data(0x55)
 */ 
void UART3_Send_Data(unsigned char c)
{
    clr_SC1CR0_TXOFF;
    SC1DR = c;
    while(!(SC1TSR&SET_BIT3));
    clr_SC1CR0_TXOFF;
	
	  set_T3CON_TR3;                                    //Trigger Timer3
}


/**
 * @brief       UART interrupt enable setting 
 * @param       u8UARTPort: UART0/UART1/UART2/UART3 baudrate value
 * @param       u8UARTINTStatus: Disable/Enable
 * @return      none
 * @details     none
 * @note        max baud rate = 1.5MHz when Fsys = 24MHz
 * @example:    UART3_Interrupt(UART3_TXD,Enable)
 */
void UART3_Interrupt(unsigned char u8UART3INTSel,unsigned char u8UART3INTStatus)
{
        switch (u8UART3INTSel)
        {
          case UART3_TXD:
            switch(u8UART3INTStatus)
            {
              case Disable: clr_SC1IE_TBEIEN;   break;
              case Enable: set_SC1IE_TBEIEN;    break;
            }break;
          case UART3_RXD:
            switch(u8UART3INTStatus)
            {
              case Disable:  clr_SC1IE_RDAIEN;   break;
              case Enable:   set_SC1IE_RDAIEN;   break;
            }break;
        }

}


