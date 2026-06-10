#ifndef _bsp_H_
#define _bsp_H_

//#include  <msp430g2553.h>          // MSP430x2xx
 #include  <msp430xG46x.h>  // MSP430x4xx

#define   SMCLK  		   1048576 // 2^20
#define   debounceVal      250
#define   TBRmax             0xFFFF  //value for 500msec


// LEDs abstraction
#define LEDsArrPort        P9OUT
#define LEDsArrPortDir     P9DIR
#define LEDsArrPortSel     P9SEL

// RGB abstraction
#define RGBsArrPort        P3OUT
#define RGBsArrPortDir     P3DIR
#define RGBsArrPortSel     P3SEL


// PushButtons abstraction
#define PBsArrPort	   	   P1IN 
#define PBsArrIntPend	   P1IFG 
#define PBsArrIntEn	   	   P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL 
#define PBsArrPortDir      P1DIR 
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08





//Timer_B abstraction for TACTL
#define StopMode           MC_0
#define UpMode             MC_1
#define ContMode           MC_2
#define UpDownMode         MC_3
#define InputDiv_1         ID_0
#define InputDiv_2         ID_1
#define InputDiv_4         ID_2
#define InputDiv_8         ID_3
#define TimerSrcACLK       TBSSEL_1
#define TimerSrcSMCLK      TBSSEL_2


//LCD abstraction
#define LCD_CTL_SEL       P2SEL


extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void DMAconfig(void);

#endif



