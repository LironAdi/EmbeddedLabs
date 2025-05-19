#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
// #include  <msp430xG46x.h>  // MSP430x4xx

#define debounceVal        250
#define SMCLK              1048576
// Switches abstraction
#define SWsArrPort         P2IN
#define SWsArrPortDir      P2DIR
#define SWsArrPortSel      P2SEL
#define SWmask             0x0F

// PushButtons abstraction
#define PBsArrPort         P1IN
#define PBsArrIntPend      P1IFG
#define PBsArrIntEn        P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL
#define PBsArrPortDir      P1DIR
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04



// LCD Data
#define LCD_DATA_SEL    P1SEL
#define LCD_DATA_WRITE  P1OUT
#define LCD_DATA_DIR    P1DIR
#define LCD_DATA_READ   P1IN

// LCD Control
#define LCD_CTL_Sel     P2SEL
#define LCD_CTL_Dir     P2DIR
#define LCd_CTL_Write   P2OUT


#define RS 0X00     // P1.0
#define EN 0X01     // P1.1
#define D4 BIT2     // P1.4
#define D5 BIT3     // P1.5
#define D6 BIT4     // P1.6
#define D7 BIT5     // P1.7


#define BuzzerOUT     P2OUT
#define Buzzer        0x04
/////define TIMER_A1 sel and dir ext (dvir)
#define TimerA1DIR    P2DIR
#define TimerA1SEL    P2SEL

extern void GPIOconfig(void);
extern void TIMER_A0_config(void);
extern void TIMER_A1_config(void);
extern void ADCconfig(void);


//#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms
//// LEDs abstraction
//#define LEDsArrPort        P1OUT
//#define LEDsArrPortDir     P1DIR
//#define LEDsArrPortSel     P1SEL





#endif



