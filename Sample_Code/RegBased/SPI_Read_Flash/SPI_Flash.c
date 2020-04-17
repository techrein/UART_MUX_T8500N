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
//  File Function: MS51_8K Access SPI Flash (W25Q16BV) demo code
//***********************************************************************************************************

#include "MS51_32K.h"

#define SS              P15
#define WRITE_ENABLE    0x06
#define WRITE_DISABLE   0x04
#define READ_DATA       0x03
#define PAGE_PROGRAM    0x02
#define CHIP_ERASE      0xC7
#define READ_STATUS1    0x05

//-----------------------------------------------------------------------------------------------------------
void SPI_Error(void)
{
    printf ("\n*  SPI Error, please check the connection between MCU and SPI Flash");
    while(1);
}
//-----------------------------------------------------------------------------------------------------------
void SPI_Initial(void)
{

    /* /SS General purpose I/O ( No Mode Fault ) */
    set_SPSR_DISMODF;
    clr_SPCR_SSOE;

    /* SPI in Master mode */
    set_SPCR_MSTR;

    /* MSB first */
    clr_SPCR_LSBFE;

    clr_SPCR_CPOL;
    clr_SPCR_CPHA;
    
    /* Enable SPI function */
    set_SPCR_SPIEN;
}
//-----------------------------------------------------------------------------------------------------------
void Read_MID_DID(UINT8 *pu8MID,UINT8 *pu8DID)
{
    SS = 0;
    SPDR = 0x90;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0xFF;
    while((SPSR&0x80)==0x00);
    *pu8MID = SPDR;
    clr_SPSR_SPIF;

    SPDR = 0xFF;
    while((SPSR&0x80)==0x00);
    *pu8DID = SPDR;
    clr_SPSR_SPIF;
    SS = 1;    
}
//-----------------------------------------------------------------------------------------------------------
void Flash_Write_Enable(void)
{
    SS = 0;
    SPDR = WRITE_ENABLE;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;
    SS = 1;    
}
//-----------------------------------------------------------------------------------------------------------
void Flash_Write_Disable(void)
{
    SS = 0;
    SPDR = WRITE_DISABLE;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;
    SS = 1;    
}
//-----------------------------------------------------------------------------------------------------------
void Flash_Chip_Erase(void)
{
    SS = 0;
    SPDR = CHIP_ERASE;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;
    SS = 1;    
}
//-----------------------------------------------------------------------------------------------------------
void Flash_Read_Status(void)
{
    UINT8 u8Status;

    SS = 0;
    do{
        SPDR = READ_STATUS1;
        while((SPSR&0x80)==0x00);
        clr_SPSR_SPIF;
    
        SPDR = 0xFF;
        while((SPSR&0x80)==0x00);
        u8Status = SPDR;
        clr_SPSR_SPIF;
    }while((u8Status&0x01)==0x01);
    SS = 1;    
}
//-----------------------------------------------------------------------------------------------------------
void Flash_Erase_Verify(void)
{
    UINT16 u16CNT;    
    UINT8  u8Data;

    SS = 0;

    SPDR = READ_DATA;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    /* 24-bit Address */
    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    for(u16CNT=0;u16CNT<256;u16CNT++)
    {
        SPDR = 0x00;
        while((SPSR&0x80)==0x00);
        u8Data = SPDR;
        clr_SPSR_SPIF;
        if(u8Data != 0xFF)
        {
            SPI_Error();
        }
    }

    SS = 1; 
}
//-----------------------------------------------------------------------------------------------------------
void Flash_Program(void)
{
    UINT16 u16CNT; 
    
    SS = 0;

    SPDR = PAGE_PROGRAM;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    /* 24-bit Address */
    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    /* Send the data to SPI_Flash buffer */
    for(u16CNT=0;u16CNT<256;u16CNT++)
    {
        SPDR = (UINT8)u16CNT;
        while((SPSR&0x80)==0x00);
        clr_SPSR_SPIF;
    }
    
    SS = 1;    
}
//-----------------------------------------------------------------------------------------------------------
void Flash_Program_Verify(void)
{
    UINT16 u16CNT;    
    UINT8  u8Data;

    SS = 0;

    SPDR = READ_DATA;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    /* 24-bit Address */
    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    SPDR = 0x00;
    while((SPSR&0x80)==0x00);
    clr_SPSR_SPIF;

    for(u16CNT=0;u16CNT<256;u16CNT++)
    {
        SPDR = 0x00;
        while((SPSR&0x80)==0x00);
        u8Data = SPDR;
        clr_SPSR_SPIF;
        if(u8Data != (UINT8)u16CNT)
        {
            SPI_Error();
        }
    }

    SS = 1; 
}
//-----------------------------------------------------------------------------------------------------------
void main(void)
{      
    UINT8 u8MID,u8DID;

    /* Note
       MCU power on system clock is HIRC (22.1184MHz), so Fsys = 22.1184MHz
    */
    MODIFY_HIRC(HIRC_24);
    ALL_GPIO_QUASI_MODE;
    UART_Open(240000000,UART0_Timer3,115200);             /* 9600 Baud Rate*/
    ENABLE_UART0_PRINTF;


    SPI_Initial();
    Read_MID_DID(&u8MID,&u8DID);

    if((u8MID != 0xEF)&&(u8DID != 0x14))
    {
        SPI_Error();
    }

    /* The procedure of SPI Flash at erase mode */
    Flash_Write_Enable();
    Flash_Chip_Erase();
    Flash_Read_Status();
    Flash_Write_Disable();
    
    Flash_Erase_Verify();

    /* The procedure of SPI Flash at program mode */
    Flash_Write_Enable();
    Flash_Program();
    Flash_Read_Status();
    Flash_Write_Disable();

    /* Program verify */
    Flash_Program_Verify();
    
    printf("\nFinished the SPI Demo Code and test pass!!!\n");
    while(1);
}
//-----------------------------------------------------------------------------------------------------------
