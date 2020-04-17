/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/*  Website: http://www.nuvoton.com                                                                        */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : June/21/2019                                                                                   */
/***********************************************************************************************************/
#include "MS51_32K.h"

bit Receive0_delayFlg = 0, Receive1_delayFlg = 0, Receive2_delayFlg = 0, Receive3_delayFlg = 0, Receive4_delayFlg = 0;
bit Pause0Flg = 0, Pause1Flg = 0, Pause2Flg = 0, Pause3Flg = 0, Pause4Flg  = 0;

unsigned char Receive0_delayCnt = 0, Receive1_delayCnt = 0, Receive2_delayCnt = 0, Receive3_delayCnt = 0, Receive4_delayCnt = 0;
unsigned char Pause0Cnt = 0, Pause1Cnt = 0, Pause2Cnt = 0, Pause3Cnt = 0, Pause4Cnt = 0;


void Channel_Scan(void)
{
	Scan_CH[CHcnt] = (((P1 & 0x80)>>6) | (P3 & 0x01)) + 1;
	
	CHcnt++;
	if(CHcnt > 2) CHcnt = 0;

}



/************************************************************************************************************
*    TIMER 0 interrupt subroutine
************************************************************************************************************/

void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B  /* 500us Timer */
{
    _push_(SFRS);
 		SFRS = 0;
	
    TH0 = TH0_INIT;
    TL0 = TL0_INIT;    
    TF0 = 0;
		
		if(Receive0_delayFlg) 
		{
			Receive0_delayCnt++;
			if(Receive0_delayCnt > Pause_MAX)
			{
				Receive0_delayFlg = 0;
				Receive0_delayCnt = 0;			
				
				switch(CH_NUM)
				{
					case CH_1:
						Uart1TXPauseBuff[Write1Pause] = Write1Data;
						Write1Pause = (++Write1Pause) % PauseData_MAX;
					  if(!uart1_TX_flag)
						{
							set_SCON_1_TI_1;
						}
					break;
					
					case CH_2:
						Uart2TXPauseBuff[Write2Pause] = Write2Data;
						Write2Pause = (++Write2Pause) % PauseData_MAX;
						set_SC0IE_TBEIEN;
					break;
					
					case CH_3:
						Uart3TXPauseBuff[Write3Pause] = Write3Data;
						Write3Pause = (++Write3Pause) % PauseData_MAX;
						set_SC1IE_TBEIEN;
					break;
					
					case CH_4:
						Uart4TXPauseBuff[Write4Pause] = Write4Data;
						Write4Pause = (++Write4Pause) % PauseData_MAX;
						set_SC2IE_TBEIEN;
					break;
				}
				SFRS = 0;
				FirstData_flg = 1;
			}
		}
		
		if(Pause1Flg)
		{
			Pause1Cnt++;
			if(Pause1Cnt>Pause_MAX) //500us x 4 = 2ms
			{
				Pause1Cnt = 0;
				Pause1Flg = 0;

				if(!uart1_TX_flag)	
				{
					set_SCON_1_TI_1;
				}
			}
		}

		if(Pause2Flg)
		{
			Pause2Cnt++;
			if(Pause2Cnt>Pause_MAX) //500us x 4 = 2ms
			{
				Pause2Cnt = 0;
				Pause2Flg = 0;
		
				set_SC0IE_TBEIEN;
				SFRS = 0;
			}
		}		

		if(Pause3Flg)
		{
			Pause3Cnt++;
			if(Pause3Cnt>Pause_MAX) //500us x 4 = 2ms
			{
				Pause3Cnt = 0;
				Pause3Flg = 0;
				set_SC1IE_TBEIEN;
				SFRS = 0;
			}
		}

		if(Pause4Flg)
		{
			Pause4Cnt++;
			if(Pause4Cnt>Pause_MAX) //500us x 4 = 2ms
			{
				Pause4Cnt = 0;
				Pause4Flg = 0;
				set_SC2IE_TBEIEN;
				SFRS = 0;
			}
		}

		if(Receive1_delayFlg) 
		{

			Receive1_delayCnt++;
			if(Receive1_delayCnt > Delay_MAX)
			{
				Receive1_delayCnt = 0;
				
				if(!ReadNFull_flg)
				{
					Receive1_delayFlg = 0;
					if(((PosiWriteData == PosiReadData)&&(UartNBuff[PosiReadData][0] == 0)) && ((TopWriteData == TopReadData)&&(UartSBuff[TopReadData] == 0)))//First Data?
					{
						set_SCON_TI;
					}
				
					UartNBuff[PosiWriteData][0] = UART_1; 
					UartNBuff[PosiWriteData][1] = RX1Length;
					
					PosiWriteData = (++PosiWriteData) % UartNData_MAX;
					
					if(PosiWriteData == PosiReadData) //ReadNbuff Full?
					{
						ReadNFull_flg = 1;
					}
					RX1Length = 0;
				}
				
			}
		}	

		if(Receive2_delayFlg) 
		{
			Receive2_delayCnt++;
			if(Receive2_delayCnt > Delay_MAX)
			{
				Receive2_delayCnt = 0;

				if(!ReadNFull_flg)
				{
					Receive2_delayFlg = 0;
					if(((PosiWriteData == PosiReadData)&&(UartNBuff[PosiReadData][0] == 0)) && ((TopWriteData == TopReadData)&&(UartSBuff[TopReadData] == 0)))//First Data?
					{
						set_SCON_TI;
					}			
					
					UartNBuff[PosiWriteData][0] = UART_2; 
					UartNBuff[PosiWriteData][1] = RX2Length;
					
					PosiWriteData = (++PosiWriteData) % UartNData_MAX;
					
					if(PosiWriteData == PosiReadData) //ReadNbuff Full?
					{
						ReadNFull_flg = 1;
					}
					RX2Length = 0;	
				}
							
			}
		}

		if(Receive3_delayFlg) 
		{
			Receive3_delayCnt++;
			if(Receive3_delayCnt > Delay_MAX)
			{
				Receive3_delayCnt = 0;

				if(!ReadNFull_flg)
				{
					Receive3_delayFlg = 0;
					if(((PosiWriteData == PosiReadData)&&(UartNBuff[PosiReadData][0] == 0)) && ((TopWriteData == TopReadData)&&(UartSBuff[TopReadData] == 0))) //First Data?
					{
						set_SCON_TI;
					}
					
					UartNBuff[PosiWriteData][0] = UART_3; 
					UartNBuff[PosiWriteData][1] = RX3Length;

					PosiWriteData = (++PosiWriteData) % UartNData_MAX;
					
					if(PosiWriteData == PosiReadData) //ReadNbuff Full?
					{
						ReadNFull_flg = 1;
					}
					RX3Length = 0;
				}
								
			}
		}

		if(Receive4_delayFlg) 
		{
			Receive4_delayCnt++;
			if(Receive4_delayCnt > Delay_MAX)
			{
				Receive4_delayCnt = 0;
				if(!ReadSFull_flg)
				{
					Receive4_delayFlg = 0;
					if(((TopWriteData == TopReadData)&&(UartSBuff[TopReadData] == 0))&&((PosiWriteData == PosiReadData)&&(UartNBuff[PosiReadData][0] == 0))) //First Data?
					{
						set_SCON_TI;
					} 

					UartSBuff[TopWriteData] = RX4Length;

					TopWriteData = (++TopWriteData) % UartSData_MAX;
					
					if(TopWriteData == TopReadData) //ReadSbuff Full?
					{
						ReadSFull_flg = 1;
					}
					RX4Length = 0;	
				}				
				
/* //original
				if(!ReadNFull_flg)
				{
					Receive4_delayFlg = 0;
					if((PosiWriteData == PosiReadData)&&(UartNBuff[PosiReadData][0] == 0))//First Data?
					{
						set_SCON_TI;
					} 
					
					UartNBuff[PosiWriteData][0] = UART_4; 
					UartNBuff[PosiWriteData][1] = RX4Length;

					PosiWriteData = (++PosiWriteData) % UartNData_MAX;
					
					if(PosiWriteData == PosiReadData) //ReadNbuff Full?
					{
						ReadNFull_flg = 1;
					}
					RX4Length = 0;	
				}
*/					
			}
		}
		
		if(Pause0Flg)
		{
			Pause0Cnt++;
				
			if(Pause0Cnt>Pause_MAX) //500us x 4 = 2ms
			{
				Pause0Cnt = 0;
				Pause0Flg = 0;
				
				if((ReadNFull_flg)||(PosiReadData != PosiWriteData)||(ReadSFull_flg)||(TopReadData != TopWriteData))
				{
					set_SCON_TI;
				}					
			}
		}		

		Channel_Scan();
		
    _pop_(SFRS);
}



//-------------------------------------------------------------------------
void Timer0_Interrupt_Init(unsigned long u32SYSCLK, unsigned int u16CNT, unsigned int u16DLYUnit)
{
    unsigned int i;
    BIT_TMP = EA;
    ENABLE_TIMER0_MODE1;
    TIMER0_FSYS_DIV12;
    ENABLE_TIMER0_INTERRUPT;
    ENABLE_GLOBAL_INTERRUPT;

    for (i = 0; i < u16CNT; i++)
    {
        TL0 = LOBYTE(65535 - ((u32SYSCLK / 1000000) * u16DLYUnit / 12));
        TH0 = HIBYTE(65535 - ((u32SYSCLK / 1000000) * u16DLYUnit / 12));
        set_TCON_TR0;                                    //Start Timer0

        while (!TF0);

        clr_TCON_TR0;
    }

    DISABLE_TIMER0_INTERRUPT;
    EA = BIT_TMP;
}

void Timer0_Interrupt_Disable(void)
{
    clr_TCON_TR0;
    DISABLE_TIMER0_INTERRUPT;
}


void Timer2_Delay500us(unsigned int u8CNT)
{
    SFRS = 0;
    T2MOD&=0x8F;T2MOD|=0x10;                                   //Timer2 Clock = Fsys/4
    clr_T2CON_CMRL2;
    set_T2MOD_LDEN;
    set_T2MOD_CMPCR;

    while (u8CNT != 0)
    {
        TL2 = LOBYTE(TIMER_DIV4_VALUE_500us_FOSC_240000);    //Find  define in "Function_define.h" "TIMER VALUE"
        TH2 = HIBYTE(TIMER_DIV4_VALUE_500us_FOSC_240000);
        set_T2CON_TR2;
        while (TF2 == 0);                       //Check Timer2 Time-Out Flag
        clr_T2CON_TF2;
        u8CNT --;
    }
    clr_T2CON_TR2;                                    //Stop Timer2
		
}

/************************************************************************************************************
*    TIMER 1 interrupt subroutine
************************************************************************************************************/
/*
void Timer1_ISR (void) interrupt 3  //interrupt address is 0x000B
{
    _push_(SFRS);
 
    TH1 = TH1_INIT;
    TL1 = TL1_INIT;    
    TF0 = 0;
    P35 = ~P35;                     // GPIO1 toggle when interrupt
  
    _pop_(SFRS);
}

*/
/*
//------------------------------------------------------------------------------
void Timer0_Delay1ms(UINT32 u32CNT)
{
    clr_CKCON_T0M;                                    //T0M=0, Timer0 Clock = Fsys/12
    TMOD |= 0x01;                               //Timer0 is 16-bit mode

    while (u32CNT != 0)
    {
        TL0 = LOBYTE(TIMER_DIV12_VALUE_1ms);     //Find  define in "Function_define.h" "TIMER VALUE"
        TH0 = HIBYTE(TIMER_DIV12_VALUE_1ms);
         set_TCON_TR0;                                    //Start Timer0
        while (TF0 != 1);                       //Check Timer0 Time-Out Flag
        clr_TCON_TF0;
        u32CNT --;
    }
    clr_TCON_TR0;                                    //Stop Timer0
}
//------------------------------------------------------------------------------
void Timer1_Delay10ms(UINT32 u32CNT)
{
    clr_CKCON_T1M;                                    //T1M=0, Timer1 Clock = Fsys/12
    TMOD |= 0x10;                                //Timer1 is 16-bit mode
    set_TCON_TR1;                                    //Start Timer1
    while (u32CNT != 0)
    {
        TL1 = LOBYTE(TIMER_DIV12_VALUE_10ms);    //Find  define in "Function_define.h" "TIMER VALUE"
        TH1 = HIBYTE(TIMER_DIV12_VALUE_10ms);
        while (TF1 != 1);                        //Check Timer1 Time-Out Flag
        clr_TCON_TF1;
        u32CNT --;
    }
    clr_TCON_TR1;                                     //Stop Timer1
}
//------------------------------------------------------------------------------

void Timer2_Delay1ms(unsigned char u8CNT)
{
    SFRS = 0;
    T2MOD&=0x8F;T2MOD|=0x10;                                   //Timer2 Clock = Fsys/4
    clr_T2CON_CMRL2;
    set_T2MOD_LDEN;
    set_T2MOD_CMPCR;

    while (u8CNT != 0)
    {        TL2 = LOBYTE(TIMER_DIV4_VALUE_1ms);    //Find  define in "Function_define.h" "TIMER VALUE"
        TH2 = HIBYTE(TIMER_DIV4_VALUE_1ms);
        set_T2CON_TR2;
        while (TF2 == 0);                       //Check Timer2 Time-Out Flag
        clr_T2CON_TF2;
        u8CNT --;
    }
    clr_T2CON_TR2;                                    //Stop Timer2
}
//------------------------------------------------------------------------------
void Timer3_Delay100ms(UINT32 u32CNT)
{
    T3CON = 0x07;                               //Timer3 Clock = Fsys/128
    set_T3CON_TR3;                                    //Trigger Timer3
    while (u32CNT != 0)
    {
        RL3 = LOBYTE(TIMER_DIV128_VALUE_100ms); //Find  define in "Function_define.h" "TIMER VALUE"
        RH3 = HIBYTE(TIMER_DIV128_VALUE_100ms);
        while ((T3CON&SET_BIT4) != SET_BIT4);    //Check Timer3 Time-Out Flag
        clr_T3CON_TF3;
        u32CNT --;
    }
    clr_T3CON_TR3;                                    //Stop Timer3
}
//------------------------------------------------------------------------------
void Timer3_Delay10us(UINT32 u32CNT)
{
    T3CON = 0x02;                               //Timer3 Clock = Fsys/4
    set_T3CON_TR3;                                    //Trigger Timer3
    while (u32CNT != 0)
    {
        RL3 = LOBYTE(TIMER_DIV4_VALUE_10us); //Find  define in "Function_define.h" "TIMER VALUE"
        RH3 = HIBYTE(TIMER_DIV4_VALUE_10us);
        while ((T3CON&SET_BIT4) != SET_BIT4);    //Check Timer3 Time-Out Flag
        clr_T3CON_TF3;
        u32CNT --;
    }
    clr_T3CON_TR3;                                    //Stop Timer3
}
*/