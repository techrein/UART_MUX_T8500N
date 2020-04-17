/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jun/21/2019
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: MS51_8K SPI in Master mode demo code
//***********************************************************************************************************

#include "MS51_32K.h"

#define SS  P15


unsigned char code SPIBUFF[5]={0x4E,0x55,0x56,0x4F,0x54};
unsigned char spi_rv_ct;

bit  SPIERRFG=0;
//***********************************************************************************************************
//  Application: SPI Function 
//  Master send 0x90 and recevie 0x4E
//  Master send 0x01 and recevie 0x55
//  Master send 0x02 and recevie 0x56
//  Master send 0x03 and recevie 0x4F
//  Master send 0x04 and recevie 0x54
//
//  Master recevie 0x4E and 0x4F form slave after transmitting
//
//  Output : 
//           UART show result on hyper-terminal
//           P1.7 flash when SPI error
//***********************************************************************************************************

//-----------------------------------------------------------------------------------------------------------
void SPI_Error(void)
{
    printf ("\nSPI error.\n");
    while(1)                                    // SPI error and P1.7 flash/
    {
        P35 = 1;
        Timer0_Delay(24000000,200,1000);
        P35 = 0;
        Timer0_Delay(24000000,200,1000);
    }
  }
//-----------------------------------------------------------------------------------------------------------
void SPI_Initial(void)
{              
    P05_QUASI_MODE;                                // P05 (SS) Quasi mode
    P10_QUASI_MODE;                                // P10 (SPCLK) Quasi mode
    P00_QUASI_MODE;                                // P00 (MOSI) Quasi mode
    P01_QUASI_MODE;                                // P22 (MISO) Quasi mode
            
    set_SPSR_DISMODF;                              // SS General purpose I/O ( No Mode Fault ) 
    clr_SPCR_SSOE;
   
    clr_SPCR_LSBFE;                                // MSB first         

    clr_SPCR_CPOL;                                  // The SPI clock is low in idle mode
    set_SPCR_CPHA;                                  // The data is sample on the second edge of SPI clock 
    
    set_SPCR_MSTR;                                  // SPI in Master mode 
     
    SPICLK_FSYS_DIV8;                              // Select SPI clock
    ENABLE_SPI0_INTERRUPT;                          // Enable SPI interrupt
    set_SPCR_SPIEN;                                // Enable SPI function 
}
//-----------------------------------------------------------------------------------------------------------
void Start_Sending_SPI(UINT8 *pu8MID,UINT8 *pu8DID)
{
    SS = 0;

    SPDR = 0x90;                                // Send 0x90 to Slave 
    while(SPSR|CLR_BIT7);                       // Wait flag
    if(SPIERRFG)                            // Receive slave 1st DATA 
       SPI_Error(); 
    printf ("\nSlave Return %c!\n",SPDR);
                                          
    SPDR = 0x01;                                // Send 0x01 to Slave 
    PCON |= SET_BIT0;                           // Enter idle mode
    if(SPIERRFG)                            // Receive slave 2nd DATA  
       SPI_Error();
    printf ("\nSlave Return %c!\n",SPDR);

    SPDR = 0x02;                                // Send 0x02 to Slave 
    PCON |= SET_BIT0;                           // Enter idle mode
    if(SPIERRFG)                            // Receive slave 3rd DATA 
       SPI_Error();
    printf ("\nSlave Return %c!\n",SPDR);

    SPDR = 0x03;                                // Send 0x03 to Slave 
    PCON |= SET_BIT0;                           // Enter idle mode
    if(SPIERRFG)                            // Receive slave 4th DATA
       SPI_Error();
    printf ("\nSlave Return %c!\n",SPDR);

    SPDR = 0x04;                                // Send 0x04 to Slave 
    PCON |= SET_BIT0;                           // Enter idle mode
    if(SPIERRFG)                            // Receive slave 5th DATA 
       SPI_Error();
    printf ("\nSlave Return %c!\n",SPDR);

    SPDR = 0x4F;                   
    PCON |= SET_BIT0;                           // Enter idle mode
    *pu8MID = SPDR;                             // Receive Slave 1st DATA fron Slave       
    printf ("\nSlave Return %c!\n",SPDR);

    SPDR = 0x4E;                   
    PCON |= SET_BIT0;                           // Enter idle mode             
    *pu8DID = SPDR;                             // Receive Slave 2nd DATA from Slave 
    printf ("\nSlave Return %c!\n",SPDR);

    SS = 1;    
}
//-----------------------------------------------------------------------------------------------------------
void main(void)
{      
    unsigned char  u8MID,u8DID;

  
    MODIFY_HIRC(HIRC_24);
    P17_PUSHPULL_MODE;
    UART_Open(24000000,UART0_Timer3,115200);             /* 115200 Baud Rate*/

    SPI_Initial();
    ENABLE_UART0_PRINTF;
    printf ("\nSPI Start Transmit...\n");

    Start_Sending_SPI(&u8MID,&u8DID);
        
    if((u8MID != 0x4F)&&(u8DID != 0x4E))
        SPI_Error();

    printf ("\nSPI Test OK!\n");

    DISABLE_UART0_PRINTF;
    while(1)                                    // SPI transmission finish
    {
        P35 = 1;
        Timer0_Delay(24000000,200,1000);
        P35 = 0;
        Timer0_Delay(24000000,200,1000);
    }
}
//-----------------------------------------------------------------------------------------------------------
void SPI_ISR(void) interrupt 9                  // Vecotr @  0x4B
{
    _push_(SFRS);
  
     if(spi_rv_ct<6)
   {
      if (SPDR!= SPIBUFF[spi_rv_ct])
      {
        SPIERRFG = 1;
      }
   }
	 spi_rv_ct++;
   clr_SPSR_SPIF;
  
    _pop_(SFRS);
}
//-----------------------------------------------------------------------------------------------------------
