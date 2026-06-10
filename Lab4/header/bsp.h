#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
//#include  <msp430xG46x.h>  // MSP430x4xx

#define debounceVal        250
#define SMCLK              1048576


// PushButtons abstraction

//PB0 - to P1.0 - state 5
#define PBsArrPort         P1IN
#define PBsArrIntPend      P1IFG
#define PBsArrIntEn        P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL
#define PBsArrPortDir      P1DIR
#define PB1                0x01
//#define PB1                0x02
//#define PB2                0x04


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
//#define EN 0X01     // P1.1 --------------------
#define D4 BIT2     // P1.4
#define D5 BIT3     // P1.5
#define D6 BIT4     // P1.6
#define D7 BIT5     // P1.7

//Buzzer - state 2
#define BuzzerOUT     P2OUT
#define Buzzer        0x10

//potentiometer - state 4



// UART
#define UART_R_BUFFER    UCA0RXBUF
#define UART_T_BUFFER    UCA0TXBUF

/////define TIMER_A1 sel and dir ext (dvir)
#define TimerA1DIR    P2DIR
#define TimerA1SEL    P2SEL

extern void GPIOconfig(void);
extern void TIMER_A0_config(void);
extern void TIMER_A1_config(void);
extern void ADCconfig(void);
extern void UART_config(void);
#endif



