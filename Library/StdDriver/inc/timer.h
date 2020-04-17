

void Timer0_Interrupt_Init(unsigned long u32SYSCLK, unsigned int u16CNT, unsigned int u16DLYUnit);
void Timer0_Interrupt_Disable(void);
void Channel_Scan(void);
void Timer2_Delay500us(unsigned int u8CNT);
/* if define TIMER0_FSYS_DIV12, timer = (0xFFFF-0x1000)*12/24MHz = 36.58ms */
/* if define TIMER0_FSYS, timer = (0xFFFF-0x1000)/24MHz = 2.548ms */
/* if define TIMER0_FSYS, timer = (0xFFFF-0xF82F)/24MHz = 1ms */
/* if define TIMER0_FSYS, timer = (0xFFFF-0xFC17)/24MHz = 0.5ms */
#define TH0_INIT        0xFC//F8//0x10 
#define TL0_INIT        0x17//2F//0x00



extern bit Receive0_delayFlg, Receive1_delayFlg, Receive2_delayFlg, Receive3_delayFlg, Receive4_delayFlg;
extern	bit Pause0Flg, Pause1Flg, Pause2Flg, Pause3Flg, Pause4Flg;

extern	unsigned char 	Pause0Cnt, Pause1Cnt, Pause2Cnt, Pause3Cnt, Pause4Cnt;
extern  unsigned char 	Receive0_delayCnt, Receive1_delayCnt, Receive2_delayCnt, Receive3_delayCnt, Receive4_delayCnt;



