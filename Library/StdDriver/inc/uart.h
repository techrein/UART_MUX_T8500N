#define UART0_Timer1  0
#define UART0_Timer3  1
#define UART1_Timer3  2
#define UART0 0
#define UART1 1


#define Delay_MAX				9//4
#define	Pause_MAX				9//4


#define WriteData_MAX 	128
#define PauseData_MAX 	32

#define ReadData_MAX 		256//224
#define UartNData_MAX		64//128//224
#define UartSData_MAX		16

#define Length_MAX  	256//224//0xFF

#define UART0_Blocking_LedON				P21 = 0
#define UART0_Blocking_LedOFF				P21 = 1

#define UART1_Blocking_LedON				P35 = 0
#define UART1_Blocking_LedOFF				P35 = 1

#define UART2_Blocking_LedON				P12 = 0
#define UART2_Blocking_LedOFF				P12 = 1

#define UART3_Blocking_LedON				P01 = 0
#define UART3_Blocking_LedOFF				P01 = 1

#define UART4_Blocking_LedON				P33 = 0
#define UART4_Blocking_LedOFF				P33 = 1

#define UARTALL_Blocking_LedOFF			P21 = 1; P35 = 1; P12 = 1; P01 = 1; P33=1


#define UART_0		0
#define UART_1		1
#define UART_2		2
#define UART_3		3
#define UART_4		4

#define CH_1		1
#define CH_2		2
#define CH_3		3
#define CH_4		4

#define LEVLE_1 0x1F
#define LEVLE_2 0x5E
#define LEVLE_3 0x97
#define LEVLE_4 0xD5

#define BaudRate_9600 	9600
#define BaudRate_19200 	19200
#define BaudRate_38400 	38400
#define BaudRate_57600 	57600
#define BaudRate_115200 	115200


extern 	 bit Write1Full_flg, Write2Full_flg, Write3Full_flg, Write4Full_flg;
extern 	 bit FirstData_flg;
extern   bit uart1_TX_flag;
extern 	 bit Read1Full_flg, Read2Full_flg, Read3Full_flg, Read4Full_flg, ReadNFull_flg, ReadSFull_flg, Uart1Full_flg;
extern 	 unsigned char  SpecialCH_flg;

extern   unsigned int ReadCNT;	
extern   xdata unsigned int ReadTopCNT;
extern   unsigned char CH_NUM, CHcnt;
extern   unsigned char Scan_CH[3];
extern   unsigned char step_count;

extern   xdata unsigned char   Uart1TXBuff[WriteData_MAX];
extern   xdata unsigned char   Uart1TXPauseBuff[PauseData_MAX];

extern   xdata unsigned char   Uart2TXBuff[WriteData_MAX];
extern   xdata unsigned char   Uart2TXPauseBuff[PauseData_MAX];

extern   xdata unsigned char   Uart3TXBuff[WriteData_MAX];
extern   xdata unsigned char   Uart3TXPauseBuff[PauseData_MAX];

extern   xdata unsigned char   Uart4TXBuff[WriteData_MAX];
extern   xdata unsigned char   Uart4TXPauseBuff[PauseData_MAX];

extern   xdata unsigned int   UartNBuff[UartNData_MAX][2];
extern   xdata unsigned int   UartSBuff[UartSData_MAX];

extern   xdata unsigned char   Uart1RXBuff[ReadData_MAX];
extern   xdata unsigned char   Uart2RXBuff[ReadData_MAX];
extern   xdata unsigned char   Uart3RXBuff[ReadData_MAX];
extern   xdata unsigned char   Uart4RXBuff[ReadData_MAX];

extern	 unsigned int 	  RX1Length,  RX2Length, RX3Length,   RX4Length;
extern	 unsigned char		PosiWriteData, PosiReadData;

extern	 xdata unsigned char	TopWriteData, TopReadData;
extern	 xdata unsigned int		RX1WriteData, RX1ReadData;
extern	 xdata unsigned int		RX2WriteData, RX2ReadData;
extern	 xdata unsigned int		RX3WriteData, RX3ReadData;
extern	 xdata unsigned int		RX4WriteData, RX4ReadData;


extern	 unsigned char   Write1Data, Read1Data, Write1Pause, Read1Pause;
extern	 unsigned char   Write2Data, Read2Data, Write2Pause, Read2Pause;
extern	 unsigned char   Write3Data, Read3Data, Write3Pause, Read3Pause;
extern	 unsigned char   Write4Data, Read4Data, Write4Pause, Read4Pause;



void UART_Open(unsigned long u32SysClock, unsigned char u8UARTPort,unsigned long u32Baudrate);
void UART2_Open(unsigned long u32SysClock, unsigned long u32Baudrate);
void UART3_Open(unsigned long u32SysClock, unsigned long u32Baudrate);
void UART4_Open(unsigned long u32SysClock, unsigned long u32Baudrate);


void UART_Send_Data(unsigned char UARTPort, unsigned char c);
void UART2_Send_Data(unsigned char c);
void UART3_Send_Data(unsigned char c);
void UART4_Send_Data(unsigned char c);



unsigned char Receive_Data(unsigned char UARTPort);
void UART_Interrupt_Enable(unsigned char u8UARTPort, unsigned char u8UARTINTStatus);













