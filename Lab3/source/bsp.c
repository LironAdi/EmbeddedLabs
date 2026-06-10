#include  "../header/bsp.h"    // private library - BSP layer
#include  <msp430xG46x.h>  // MSP430x4xx
//#include  <msp430g2553.h>  // MSP430x2xx

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // LEDs 8-bit Array Port configuration
  // LED Setup
  LEDsArrPortSel &= ~0xFF;  //GPIO capability for PORT9
  LEDsArrPortDir |= 0xFF;   // PORT9 output mode
  LEDsArrPort &= ~0xFF;     //clear output

  
  //Keypad Setup
  P10SEL &= ~0xFF; //PORT10 GPIO
  P10DIR = 0x0F; //p10.0-3 output, p10.4-7 input mode
  P10OUT = 0x00; // clear output
 // P10EN |= 0xF0;  // Enable resistors on P10.4–P10.7


  RGBsArrPortSel &= ~0xFF;  //GPIO capability for PORT3
  RGBsArrPortDir |= 0xFF;   // PORT3 output mode
  RGBsArrPort &= ~0xFF;     //clear output

 //Keypad Interapt

 P2SEL &= ~0x02; //GPIO capability for P2.1              //  change this back after fin
 P2DIR &= ~0x02; //P2.1 Input mode
 P2IES |= 0x02; // pull-down mode
 P2IE |= 0x02; // enable interrupts
 P2IFG &= ~0x02; // clear pending interrupts




  // PushButtons Setup
  PBsArrPortSel &= ~0x0F;
  PBsArrPortDir &= ~0x0F;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode
  PBsArrIntEn |= 0x0F;
  PBsArrIntPend &= ~0x0F;            // clear pending interrupts
  
  LCD_CTL_SEL &= ~0xE0;           //config the controll of the lcd
  _BIS_SR(GIE);                     // enable interrupts globally
}                             

//------------------------------------------------------------------------------------- 
//            Timer_A configuration
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
    TACTL |= TACLR; //reset the counter to 0 to aviod trash defult values at
    TACTL = (TimerSrcSMCLK | InputDiv_8 |TAIE); // choose 2^20 the SMCLK and divide it Bye 8 means 2^17

    TACCR0 = TBRmax; // in up mode                  dvir need to change it back to TIMER B!!!!!!!!!!!!!!!9*
}
//-------------------------------------------------------------------------------------
//            DMA configuration
//-------------------------------------------------------------------------------------
void DMAconfig(void) {
   //  for state 2
     DMACTL0 |= DMA1TSEL_0; //start transfer when receiving trigger from DMAREQ
     DMA1CTL |= (DMADT_1 | DMADSTINCR_3 | DMASRCINCR_2 | DMASRCBYTE | DMADSTBYTE | DMAIE); // block transfer, src address inc, dst address dcr, src and dst byte mode
     DMA1CTL &= ~DMAIFG; //clear pending interrupts

    // for state 3
     DMACTL0 |= DMA2TSEL_8; //start transfer each 500msec when receiving trigger from TBCCR0
     DMA2CTL |= (DMADT_4 | DMASRCINCR_3 | DMADSTBYTE | DMASRCBYTE | DMAIE); // repeated single transfer, increment byte source to byte destination, byte mode, interurpt enabled
     DMA2CTL &= ~DMAIFG; //clear pending interrupts
}
             

void TIMERBconfig(void){
    TBCTL |= TBCLR; //reset the counter to 0 to aviod trash defult values at
    TBCTL = (TimerSrcSMCLK | InputDiv_8 |TBIE); // choose 2^20 the SMCLK and divide it Bye 8 means 2^17
    TBCCR0 = TBRmax; // in up mode                  dvir need to change it back to TIMER B!!!!!!!!!!!!!!!9*
} 
 
             
             
            
  

