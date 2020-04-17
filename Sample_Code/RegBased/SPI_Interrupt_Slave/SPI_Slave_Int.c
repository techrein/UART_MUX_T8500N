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
//  File Function: MS51_8K SPI in Slave mode demo code
//***********************************************************************************************************
#include "MS51_32K.h"


//***********************************************************************************************************
//  Application: SPI Function 
//  Slave receive 0x90 and return 0x4E
//  Slave receive 0x01 and return 0x55
//  Slave receive 0x02 and return 0x56
//  Slave receive 0x03 and return 0x4F
//  Slave receive 0x04 and return 0x54
//
//  Slave send 0x4F and 0x4E to Master after receiving
//  
//  Output : 
//           P3.5 flash when SPI error 
//***********************************************************************************************************

unsigned char   u8Receive_Data[5];
unsigned int  u16CNT = 0;
bit     SPI_Send_Flag = 0;

//-----------------------------------------------------------------------------------------------------------
void SPI_Error(void)
{
    while(1)                                    // SPI error and P0.7 flash/
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
    P05_QUASI_MODE;                            //P15 (SS) Quasi mode
    P10_QUASI_MODE;                            //P10(SPCLK) Quasi mode
    P00_QUASI_MODE;                            //P00 (MOSI) Quasi mode
    P01_QUASI_MODE;                            //P01 (MISO) Quasi mode;

    clr_SPCR_MSTR;                                   // SPI in Slave mode 
    clr_SPCR_LSBFE;                                  // MSB first 

    clr_SPCR_CPOL;                                   // The SPI clock is low in idle mode
    set_SPCR_CPHA;                                   // The data is sample on the second edge of SPI clock     
       
    ENABLE_SPI0_INTERRUPT;                           // Enable SPI interrupt
    set_SPCR_SPIEN;                                  // Enable SPI function
    
    clr_SPSR_SPIF;                                   // Clear SPI flag
}
//-----------------------------------------------------------------------------------------------------------
void Slave_Receive_Data(void)
{
    SPDR = 0x4E;                             
    PCON |= SET_BIT0;                           // Enter idle mode 
    if(u8Receive_Data[0] != 0x90)               // Receive 1st DATA form master 
        SPI_Error(); 
          
    SPDR = 0x55;
    PCON |= SET_BIT0;                           // Enter idle mode                       
    if(u8Receive_Data[1] != 0x01)               // Receive 2nd DATA form master 
        SPI_Error();

    SPDR = 0x56;
    PCON |= SET_BIT0;                           // Enter idle mode
    if(u8Receive_Data[2] != 0x02)               // Receive 3rd DATA form master 
        SPI_Error();

    SPDR = 0x4F;
    PCON |= SET_BIT0;                           // Enter idle mode
    if(u8Receive_Data[3] != 0x03)               // Receive 4th DATA form master 
        SPI_Error();

    SPDR = 0x54;
    PCON |= SET_BIT0;                           // Enter idle mode
    if(u8Receive_Data[4] != 0x04)               // Receive 5th DATA form master 
        SPI_Error();
}
//-----------------------------------------------------------------------------------------------------------
void Slave_Transmit_Data(void)
{
    SPI_Send_Flag = 1;
    SPDR = 0x4F;                                // Send 1st data (04F) to Master
    PCON |= SET_BIT0;                           // Enter idle mode       

    SPI_Send_Flag = 1;
    SPDR = 0x4E;                                // Send 2nd data (0x4E) to Master 
    PCON |= SET_BIT0;                           // Enter idle mode
}
//-----------------------------------------------------------------------------------------------------------
void main(void)
{   
    MODIFY_HIRC(HIRC_24);
		P17_PUSHPULL_MODE;
    SPI_Initial();

    Slave_Receive_Data();                       // Slave receive data from master
    Slave_Transmit_Data();                      // Slave transmit data to master

    clr_SPSR_SPIF;
    SPDR = 0x00;

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
  
    clr_SPSR_SPIF;                                   
    if(!SPI_Send_Flag)
    {       
        u8Receive_Data[u16CNT] = SPDR;
        u16CNT ++;
    }
    SPI_Send_Flag = 0; 
  
    _pop_(SFRS);
}
//-----------------------------------------------------------------------------------------------------------