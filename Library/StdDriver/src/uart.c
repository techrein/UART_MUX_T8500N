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


bit Write1Full_flg = 0, Write2Full_flg = 0, Write3Full_flg = 0, Write4Full_flg = 0;
bit FirstData_flg = 1;
bit uart1_TX_flag = 0;
bit Read1Full_flg = 0, Read2Full_flg = 0, Read3Full_flg = 0, Read4Full_flg = 0, ReadNFull_flg, ReadSFull_flg = 0;

xdata unsigned char   Uart1TXBuff[WriteData_MAX] = 0;
xdata unsigned char   Uart1TXPauseBuff[PauseData_MAX] = 0;
xdata unsigned char   Uart2TXBuff[WriteData_MAX] = 0;
xdata unsigned char   Uart2TXPauseBuff[PauseData_MAX] = 0;
xdata unsigned char   Uart3TXBuff[WriteData_MAX] = 0;
xdata unsigned char   Uart3TXPauseBuff[PauseData_MAX] = 0;
xdata unsigned char   Uart4TXBuff[WriteData_MAX] = 0;
xdata unsigned char   Uart4TXPauseBuff[PauseData_MAX] = 0;

xdata unsigned int   UartNBuff[UartNData_MAX][2] = 0;
xdata unsigned int   UartSBuff[UartSData_MAX] = 0;

xdata unsigned char   Uart1RXBuff[ReadData_MAX] = 0;
xdata unsigned char   Uart2RXBuff[ReadData_MAX] = 0;
xdata unsigned char   Uart3RXBuff[ReadData_MAX] = 0;
xdata unsigned char   Uart4RXBuff[ReadData_MAX] = 0;

unsigned char   Blocking_data = 0;
unsigned int 		RX1Length = 0,  RX2Length = 0, RX3Length = 0,   RX4Length = 0;
unsigned char 	step_count = 0;
unsigned char		PosiWriteData = 0, PosiReadData = 0;

xdata unsigned char		TopWriteData = 0, TopReadData = 0;
xdata unsigned int		RX1WriteData = 0, RX1ReadData = 0;
xdata unsigned int		RX2WriteData = 0, RX2ReadData = 0;
xdata unsigned int		RX3WriteData = 0, RX3ReadData = 0;
xdata unsigned int		RX4WriteData = 0, RX4ReadData = 0;


unsigned char 	Write1Data = 0, Read1Data = 0, Write1Pause = 0, Read1Pause = 0;
unsigned char 	Write2Data = 0, Read2Data = 0, Write2Pause = 0, Read2Pause = 0;
unsigned char 	Write3Data = 0, Read3Data = 0, Write3Pause = 0, Read3Pause = 0;
unsigned char 	Write4Data = 0, Read4Data = 0, Write4Pause = 0, Read4Pause = 0;

unsigned int ReadCNT = 0;
xdata unsigned int ReadTopCNT = 0;
unsigned char CH_NUM, CHcnt = 0;
unsigned char Scan_CH[3] = {0,0,0};


void Serial_ISR(void) interrupt 4   /*UART0*/
{
    _push_(SFRS);
		SFRS=0;

    if (RI) /* 0 --> 1,2,3,4 */
    {

				Receive0_delayCnt = 0;
				Receive0_delayFlg = 1;
			  if(FirstData_flg)
				{
					if((Scan_CH[0] == Scan_CH[1]) || (Scan_CH[0] == Scan_CH[2]))
					{
						CH_NUM = Scan_CH[0];
					}
					else if(Scan_CH[1] == Scan_CH[2])
					{
						CH_NUM = Scan_CH[1];
					}

				}
				
				switch(CH_NUM)
				{
					case CH_1:

						if(!Write1Full_flg)
						{
							SFRS=0;
							Uart1TXBuff[Write1Data] = SBUF;
							
							Write1Data = (++Write1Data) % WriteData_MAX;			
							
							if(Write1Data == Read1Data) 
							{
								Write1Full_flg = 1;
							}
							else if(!uart1_TX_flag)
							{
								set_SCON_1_TI_1;
							}
						}

//						P01 = 1;
//						P05 = 1;

					break;
						
					case CH_2:
						if(!Write2Full_flg)
						{				
							SFRS=0;
							Uart2TXBuff[Write2Data] = SBUF;
							set_SC0IE_TBEIEN;
							SFRS=0;
							
							Write2Data = (++Write2Data) % WriteData_MAX;
							
							if(Write2Data == Read2Data) // Buffer Full?? 
							{
								Write2Full_flg = 1;
							}

						}
				
//						P01 = 1;
//						P05 = 0;						

					break;
				
					case CH_3:
						if(!Write3Full_flg)
						{					
							SFRS=0;
							Uart3TXBuff[Write3Data] = SBUF;

							set_SC1IE_TBEIEN;
							SFRS=0;
											
							Write3Data = (++Write3Data) % WriteData_MAX;
							
							if(Write3Data == Read3Data) // Buffer Full?? //
							{
								Write3Full_flg = 1;
							}	
					
						}	
					
//						P01 = 0;
//						P05 = 1;					
					break;

				case CH_4:					
						if(!Write4Full_flg)
						{					
							SFRS=0;
							Uart4TXBuff[Write4Data] = SBUF;

							set_SC2IE_TBEIEN;
							SFRS=0;
											
							Write4Data = (++Write4Data) % WriteData_MAX;
							
							if(Write4Data == Read4Data) // Buffer Full?? 
							{
								Write4Full_flg = 1;
							}	
							
						}		
			
//						P01 = 0;
//						P05 = 0;					
				
				break;
			}
				
			FirstData_flg = 0;
			
			clr_SCON_RI;                                         // Clear RI (Receive Interrupt).

    }


    if (TI) /* 1,2,3,4 --> 0 */
    {
			clr_SCON_TI;	

			if(!Pause0Flg)
			{
				Pause0Cnt = 0;
				
				if((ReadCNT==0)&&(UartSBuff[TopReadData] != 0)) /*UART_4*/
				{
						if(ReadTopCNT	< UartSBuff[TopReadData])
						{
//							P35 = 0;
							ReadTopCNT++;
							SFRS = 0;
							SBUF = Uart4RXBuff[RX4ReadData];		
							RX4ReadData = (++RX4ReadData) % ReadData_MAX; 				
							Read4Full_flg = 0;
						}
						else if(ReadTopCNT == UartSBuff[TopReadData])
						{
							ReadTopCNT = 0;
							UartSBuff[TopReadData] = 0;
							TopReadData = (++TopReadData) % UartSData_MAX;

							ReadSFull_flg = 0;
							Pause0Flg = 1;
							
						}						
				}
				else
				{
					switch(UartNBuff[PosiReadData][0])
					{
						
						case UART_0:
						break;
						
						case UART_1:

							if(ReadCNT	< UartNBuff[PosiReadData][1])
							{
								ReadCNT++;
								
								SFRS = 0;
								SBUF = Uart1RXBuff[RX1ReadData];		
								RX1ReadData = (++RX1ReadData) % ReadData_MAX; 				
								Read1Full_flg = 0;
								
							}
							else if(ReadCNT == UartNBuff[PosiReadData][1])
							{
								ReadCNT = 0;
								UartNBuff[PosiReadData][0] = 0;
								PosiReadData = (++PosiReadData) % UartNData_MAX;

								ReadNFull_flg = 0;
								Pause0Flg = 1;
						
							}
						break;
							
						case UART_2:
							
							if(ReadCNT	< UartNBuff[PosiReadData][1])
							{
								ReadCNT++;
								SFRS = 0;
								SBUF = Uart2RXBuff[RX2ReadData];		
								RX2ReadData = (++RX2ReadData) % ReadData_MAX; 				
								Read2Full_flg = 0;
							}
							else if(ReadCNT == UartNBuff[PosiReadData][1])
							{
								ReadCNT = 0;
								UartNBuff[PosiReadData][0] = 0;
								PosiReadData = (++PosiReadData) % UartNData_MAX;

								ReadNFull_flg = 0;
								Pause0Flg = 1;
								
							}						
						break;
							
						case UART_3:

							if(ReadCNT	< UartNBuff[PosiReadData][1])
							{
								ReadCNT++;
								SFRS = 0;
								SBUF = Uart3RXBuff[RX3ReadData];		
								RX3ReadData = (++RX3ReadData) % ReadData_MAX; 				
								Read3Full_flg = 0;
							}
							else if(ReadCNT == UartNBuff[PosiReadData][1])
							{
								ReadCNT = 0;
								UartNBuff[PosiReadData][0] = 0;
								PosiReadData = (++PosiReadData) % UartNData_MAX;
								
								ReadNFull_flg = 0;
								Pause0Flg = 1;
				
							}						
						break;

		/*				case UART_4:
				
							if(ReadCNT	< UartNBuff[PosiReadData][1])
							{
								ReadCNT++;
								SFRS = 0;
								SBUF = Uart4RXBuff[RX4ReadData];		
								RX4ReadData = (++RX4ReadData) % ReadData_MAX; 				
								Read4Full_flg = 0;
							}
							else if(ReadCNT == UartNBuff[PosiReadData][1])
							{
								ReadCNT = 0;
								UartNBuff[PosiReadData][0] = 0;
								PosiReadData = (++PosiReadData) % UartNData_MAX;

								ReadNFull_flg = 0;
								Pause0Flg = 1;
				
							}						
						break;				*/		
					}
				}
			}
		}

    _pop_(SFRS);
}	



void SerialPort1_ISR(void) interrupt 15			/*UART1*/
{
    _push_(SFRS);
		SFRS=0;
    if (RI_1 == 1)
    {
			Receive1_delayCnt = 0;
			Receive1_delayFlg = 1;

				if((!Read1Full_flg)&&(!ReadNFull_flg))
				{
					SFRS=0;
					Uart1RXBuff[RX1WriteData] = SBUF_1;
					RX1WriteData = (++RX1WriteData) % ReadData_MAX;
					
					RX1Length = (++RX1Length) % Length_MAX;
					
					if((RX1WriteData == RX1ReadData) || (RX1Length == 0))
					{
						Read1Full_flg = 1;
						if(!RX1Length) 
						{
							RX1Length = Length_MAX;
						}
						
					}
				}

			clr_SCON_1_RI_1;                             /* clear reception flag for next reception */
		
		}
		
    if (TI_1 == 1)
    {
			uart1_TX_flag = 0;
			clr_SCON_1_TI_1;			
			if((Write1Pause != Read1Pause)&&(!Pause1Flg))
			{
				if(Uart1TXPauseBuff[Read1Pause] == Read1Data)
				{
					Read1Pause = (++Read1Pause) % PauseData_MAX;
					
					Pause1Cnt = 0;
					Pause1Flg = 1;
					
				}

			}

			if((Write1Full_flg || (Read1Data != Write1Data))&&(!Pause1Flg))
			{
				uart1_TX_flag = 1;
				SFRS = 0;
				SBUF_1 = Uart1TXBuff[Read1Data];

				Read1Data = (++Read1Data) % WriteData_MAX;
				
				Write1Full_flg = 0;
			}			
			
    }
		
    _pop_(SFRS);
}	

void SMC0_ISR(void) interrupt 21          // Vector @  0x9B  /*UART2*/
{
			
    _push_(SFRS);
		SFRS = 2;
	 if(SC0IS&0x01)
	 {
			Receive2_delayCnt = 0;
			Receive2_delayFlg = 1;	
		 
			if((!Read2Full_flg)&&(!ReadNFull_flg))
			{
				SFRS=2;
				Uart2RXBuff[RX2WriteData] = SC0DR;
				RX2WriteData = (++RX2WriteData) % ReadData_MAX;
				
				RX2Length = (++RX2Length) % Length_MAX;
				
				if((RX2WriteData == RX2ReadData) || (RX2Length == 0))
				{
					Read2Full_flg = 1;

					if(!RX2Length)
					{
						RX2Length = Length_MAX;
					}
				}
			}
			else
			{
				SFRS=2;		
				Blocking_data = SC0DR;			 
			}				

		}
	 
		if(SC0IS&0x02)
		{
			clr_SC0CR0_TXOFF;	
			clr_SC0IE_TBEIEN;
			
			if((Write2Pause != Read2Pause)&&(!Pause2Flg))
			{
				if(Uart2TXPauseBuff[Read2Pause] == Read2Data)
				{
					Read2Pause = (++Read2Pause) % PauseData_MAX;
					
					Pause2Cnt = 0;
					Pause2Flg = 1;
				}
				set_SC0IE_TBEIEN;
			}
			
			if((Write2Full_flg || (Read2Data != Write2Data))&&(!Pause2Flg))
			{
				
				SFRS=2;
				SC0DR = Uart2TXBuff[Read2Data];
				Read2Data = (++Read2Data) % WriteData_MAX;
				
				Write2Full_flg = 0;
				set_SC0IE_TBEIEN;
			}
			
		}		
		
		SFRS = 0;
    _pop_(SFRS);
		
	
}


void SMC1_ISR(void) interrupt 22          // Vector @  0x9B  /*UART3*/
{
    _push_(SFRS);
		SFRS=2;
 /* Since only enable receive interrupt, not add flag check */

		if(SC1IS&0x01)
		{
			Receive3_delayCnt = 0;
			Receive3_delayFlg = 1;
	
			if((!Read3Full_flg)&&(!ReadNFull_flg))
			{
				SFRS=2;
				Uart3RXBuff[RX3WriteData] = SC1DR;
				RX3WriteData = (++RX3WriteData) % ReadData_MAX;
				
				RX3Length = (++RX3Length) % Length_MAX;
				
				if((RX3WriteData == RX3ReadData) || (RX3Length == 0))
				{
					Read3Full_flg = 1;

					if(!RX3Length)
					{
						RX3Length = Length_MAX;//-1;
					}
				}
			}
			else
			{
				SFRS=2;		
				Blocking_data = SC1DR;			 

			}			

		}
		if(SC1IS&0x02)
		{
			clr_SC1CR0_TXOFF;	
			clr_SC1IE_TBEIEN;
			if((Write3Pause != Read3Pause)&&(!Pause3Flg))
			{
				if(Uart3TXPauseBuff[Read3Pause] == Read3Data)
				{
					Read3Pause = (++Read3Pause) % PauseData_MAX;
					
					Pause3Cnt = 0;
					Pause3Flg = 1;

				}
				set_SC1IE_TBEIEN;
			}
			
			if((Write3Full_flg || (Read3Data != Write3Data))&&(!Pause3Flg))
			{

				SFRS=2;
				SC1DR = Uart3TXBuff[Read3Data];				
				Read3Data = (++Read3Data) % WriteData_MAX;
				
				Write3Full_flg = 0;
				set_SC1IE_TBEIEN;
			}
		}

		SFRS = 0;
    _pop_(SFRS);
}


void SMC2_ISR(void) interrupt 23          // Vector @  0x9B /*UART4*/
{
    _push_(SFRS);
		SFRS=2;
 /* Since only enable receive interrupt, not add flag check */
		if(SC2IS&0x01)
		{	
			Receive4_delayCnt = 0;
			Receive4_delayFlg = 1;	
	 
			if((!Read4Full_flg)&&(!ReadSFull_flg))
			{
				SFRS=2;
				Uart4RXBuff[RX4WriteData] = SC2DR;
				RX4WriteData = (++RX4WriteData) % ReadData_MAX;
				
				RX4Length = (++RX4Length) % Length_MAX;
				
				if((RX4WriteData == RX4ReadData) || (RX4Length == 0))
				{
					Read4Full_flg = 1;

					if(!RX4Length)
					{
						RX4Length = Length_MAX;//-1;
					}
				}
			}
			else
			{
				SFRS=2;		
				Blocking_data = SC2DR;			 

			}				
		}
		
		if(SC2IS&0x02)
		{
			clr_SC2CR0_TXOFF;	
			clr_SC2IE_TBEIEN;
			if((Write4Pause != Read4Pause)&&(!Pause4Flg))
			{
				if(Uart4TXPauseBuff[Read4Pause] == Read4Data)
				{
					Read4Pause = (++Read4Pause) % PauseData_MAX;
					
					Pause4Cnt = 0;
					Pause4Flg = 1;
				}
				set_SC2IE_TBEIEN;
			}
			
			if((Write4Full_flg || (Read4Data != Write4Data))&&(!Pause4Flg))
			{
				SFRS=2;
				SC2DR = Uart4TXBuff[Read4Data];					
				Read4Data = (++Read4Data) % WriteData_MAX; 
				
				Write4Full_flg = 0;
				set_SC2IE_TBEIEN;				
			} 
		}
		
		SFRS = 0;
    _pop_(SFRS);
}




void UART_Send_Data(unsigned char UARTPort, unsigned char c)
{
    switch (UARTPort)
    {
        case UART0:
//						while(TX0_Busy);
						SFRS = 0;
            SBUF = c;
//						TX0_Busy = 1;
        break;

        case UART1:
//						while(TX1_Busy);				
						SFRS = 0;
            SBUF_1 = c;			
//						TX1_Busy = 1;
        break;
    }
}


/**
 * @brief       UART2 transfer data without interrupt 
 * @param       baudrate value
 * @return      none
 * @details     none
 * @example:    UART2_Send_Data(0x55)
 */ 
void UART2_Send_Data(unsigned char c)
{
	SFRS=2;
	SC0DR = c;

	while(!(SC0TSR&SET_BIT3));

	clr_SC0CR0_TXOFF;	

	SFRS=0;
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

		SFRS = 2;
    SC1DR = c;
    while(!(SC1TSR&SET_BIT3));
		clr_SC1CR0_TXOFF;
		SFRS=0;
}

/**
 * @brief       UART3 transfer data without interrupt 
 * @param       baudrate value
 * @return      none
 * @details     none
 * @example:    UART3_Send_Data(0x55)
 */ 
void UART4_Send_Data(unsigned char c)
{

		SFRS = 2;
    SC2DR = c;
    while(!(SC2TSR&SET_BIT3));
		clr_SC2CR0_TXOFF;
	  SFRS=0;
}




/*MS51 new version buadrate */
void UART_Open(unsigned long u32SysClock, unsigned char u8UARTPort, unsigned long u32Baudrate)
{
     SFRS = 0;
     switch (u8UARTPort)
    {
        case UART0_Timer1:
            SCON = 0x50;            //UART0 Mode1,REN=1,TI=1
            TMOD |= 0x20;           //Timer1 Mode1
            set_PCON_SMOD;          //UART0 Double Rate Enable
            set_CKCON_T1M;
            clr_T3CON_BRCK;          //Serial port 0 baud rate clock source = Timer1
            TH1 = 256 - (u32SysClock / 16 / u32Baudrate);
				
            set_TCON_TR1;
            set_IE_ES;
            break;

        case UART0_Timer3:
            SCON = 0x50;          //UART0 Mode1,REN=1,TI=1
            set_PCON_SMOD;        //UART0 Double Rate Enable
            T3CON &= 0xF8;        //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
            set_T3CON_BRCK;        //UART0 baud rate clock source = Timer3
            RH3    = HIBYTE(65536 - (u32SysClock / 16 / u32Baudrate));
            RL3    = LOBYTE(65536 - (u32SysClock / 16 / u32Baudrate));
            set_T3CON_TR3;         //Trigger Timer3
				    clr_EIE1_ET3;
            set_IE_ES;
            break;

        case UART1_Timer3:
            SCON_1 = 0x50;          //UART1 Mode1,REN_1=1,TI_1=1
				    T3CON &= 0xF8;        //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
            RH3    = HIBYTE(65536 - (u32SysClock/16/u32Baudrate));  
            RL3    = LOBYTE(65536 - (u32SysClock/16/u32Baudrate));    
						clr_EIE1_ET3; 			
            set_T3CON_TR3;             //Trigger Timer3
            set_EIE1_ES_1;
            break;
    }
 }
/**
 * @brief       UART2 transfer data without interrupt 
 * @param       u32SysClock , define Fsys value(value)
 * @param       u32Baudrate, Define baudrate (value)
 * @return      none
 * @details     none
 * @example:    UART2_Open(24000000,115200)
 */ 
void UART2_Open(unsigned long u32SysClock, unsigned long u32Baudrate)
{
    set_SC0CR0_SCEN;         /* Enable SC module */
    set_SC0CR1_UARTEN;       /* set as UART mode */
    set_SC0CR1_CLKKEEP;      
    clr_SC0CR0_CONSEL;       /* clr CONSEL and AUTOCEN*/
    clr_SC0CR0_AUTOCEN;
 
    SC0ETURD0 = LOBYTE((u32SysClock/8/u32Baudrate)-1);  /* define baud rate low byte */
    SC0ETURD1 &= 0xF0;                                /* define baud rate high byte */
    SC0ETURD1 |= (HIBYTE(u32SysClock/8/u32Baudrate-1))&0x0F; 
    set_SC0CR1_PBOFF;     //parity bit disable
    SC0CR1&=0xCF;        //datalegth 8bit
    set_SC0CR0_NSB;      //stop bit = 1bit
	
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
 * @brief       UART3 transfer data without interrupt 
 * @param       u32SysClock , define Fsys value(value)
 * @param       u32Baudrate, Define baudrate (value)
 * @return      none
 * @details     none
 * @example:    UART3_Open(24000000,115200)
 */ 
void UART4_Open(unsigned long u32SysClock, unsigned long u32Baudrate)
{
    set_SC2CR0_SCEN;         /* Enable SC module */
    set_SC2CR1_UARTEN;       /* set as UART mode */
    set_SC2CR1_CLKKEEP;      
    clr_SC2CR0_CONSEL;       /* clr CONSEL and AUTOCEN*/
    clr_SC2CR0_AUTOCEN;
    SC2ETURD0 = LOBYTE(u32SysClock/8/u32Baudrate-1);  /* define baud rate low byte */
    SC2ETURD1 &= 0xF0;                                /* define baud rate high byte */
    SC2ETURD1 |= (HIBYTE(u32SysClock/8/u32Baudrate-1))&0x0F; 
    set_SC2CR1_PBOFF;     //parity bit disable
    SC2CR1&=0xCF;        //datalegth 8bit
    set_SC2CR0_NSB;      //stop bit = 1bit
	
		set_T3CON_TR3;             //Trigger Timer3
  }

	
unsigned char Receive_Data(unsigned char UARTPort)
{
    UINT8 c;
    SFRS = 0;
    switch (UARTPort)
    {
        case UART0:
            while (!RI);

            c = SBUF;
            RI = 0;
            break;

        case UART1:
            while (!RI_1);

            c = SBUF_1;
            RI_1 = 0;
            break;
    }

    return (c);
}


