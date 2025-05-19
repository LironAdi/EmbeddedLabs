#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
//#include  <msp430xG46x.h>  // MSP430x4xx


#define   debounceVal      250
//#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms
#define   LEDs_SHOW_RATE   0xCAA6
#define   RGB_SHOW_RATE   0x0067 //250*RGB_SHOW_RATE=250ms
#define   Shifter_LEDs_SHOW_RATE   0xA3B8

// LEDs abstraction
#define LEDsArrPort        P1OUT
#define LEDsArrPortDir     P1DIR
#define LEDsArrPortSel     P1SEL

// Switches abstraction
//#define SWsArrPort         P2IN
//#define SWsArrPortDir      P2DIR
//#define SWsArrPortSel      P2SEL
//#define SWmask             0x0F

// RGB abstraction
#define RGBsArrPort         P2OUT
#define RGBsArrPortDir      P2DIR
#define RGBsArrPortSel      P2SEL




// PushButtons abstraction
#define PBsArrPort	       P2IN
#define PBsArrIntPend	   P2IFG
#define PBsArrIntEn	       P2IE
#define PBsArrIntEdgeSel   P2IES
#define PBsArrPortSel      P2SEL
#define PBsArrPortDir      P2DIR
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08
// PWM abstraction
#define PWMArr             P2OUT

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);

#endif



