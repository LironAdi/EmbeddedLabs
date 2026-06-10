#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;     // Stop WDT
   
  // Switches Setup
  //SWsArrPortSel &= ~0x0F;//on output mode
  //SWsArrPortDir &= ~0x0F; //INPUT MODE

  // PushButtons Setup
  PBsArrPortSel &= ~0x07;            // DECODE P1.3-P1.0 INTO O/I
  PBsArrPortDir &= ~0x07;            // DECODE THEM INTO INPUT MODE
  PBsArrIntEdgeSel |= 0x03;          // pull-up mode
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode
  PBsArrIntEn |= 0x07;
  PBsArrIntPend &= ~0x07;            // clear pending interrupts
  //PBsArrIntPend = 0x00;
  //PBsArrIntEn = 0x00;

// calibration button
  P2DIR &= ~PB0;
  P2SEL &= ~PB0;
  P2IES |= PB0;
  P2IE  |= PB0;
  P2IFG &= ~PB0;

  //LCD_CTL_Sel &= ~0xE0;           //config the controll of the lcd
  //_BIS_SR(GIE);                     // enable interrupts globally

//-------------------------------------------------------------
//            LCD setup
//-------------------------------------------------------------
  LCD_DATA_SEL &= ~0xF0 ;             // makes P1.4 - P1.7 IO mode
  LCD_DATA_DIR |= 0xF0 ;              // makes P1.4 - P1.7 Output mode
  LCD_DATA_WRITE &= ~0xF0 ;           // CLR P1.4 - P1.7

  LCD_CTL_Sel &= ~0xA2 ;             // makes P2.1, P2.5, P2.7 IO mode
  LCD_CTL_Dir |= 0xA2 ;              // makes P2.1, P2.5, P2.7 Output mode
  LCd_CTL_Write &= ~0xA2 ;           // CLR P2.1, P2.5, P2.7

  P1IFG &= ~0xF0;

//-------------------------------------------------------------------------------------
//           PWM & Ultra-sonic & LDR configuration
//-------------------------------------------------------------------------------------
  P2DIR |= PWM + Trigger;
  P2DIR &= ~Echo;
  P2SEL |= PWM + Trigger + Echo;
  P1DIR &= ~(LeftLDR + RightLDR);//outmode
  P1SEL &= ~(LeftLDR + RightLDR);//GPIO
  P2SEL &= ~0x80;
}
//------------------------------------------------------------------------------------- 
//            Timers configuration
//-------------------------------------------------------------------------------------
//Timer A0
void TIMER_A0_config(void){
  TA0CTL = TASSEL_2 + MC_1 ;//+ TAIE; // SMCLK, Up Mode, Interrupt enabled
  TA0CCTL1 |= OUTMOD_7; // Reset/set
  TA0CCR0 = 21000; // Timer period
  TA0CCR1 = 420; // Duty cycle - 0.6ms - 0 degrees - associated to PWM pin
  //TA0CCR1 = 1230; // Duty cycle - 0.6ms - 0 degrees - associated to PWM pin

}



void TIMER_A1_config(void){

 TA1CTL = TASSEL_2 + MC_1;// + TAIE;// SMCLK, Up Mode, Interrupt enabled
 TA1CCR0 = 0xFFFF;               // Timer period
 TA1CCR1 = 0x0020;               //10 Us  - associated to Trigger pin
 TA1CCTL2 = 0x00;               //clear
 TA1CCTL1 = OUTMOD_7;            // Reset/set
 TA1CCTL0 &= ~ CCIFG;           //Clr interrupt of TA1.2
 TA1CCTL0 &= ~ CCIE;
 TA1CCTL2 |= CM_3 + SCS + CAP;// + CCIE;  //capture on both rising and falling edges, sync cap, capture mode - for echo
 TA1CCTL2 &= ~ CCIFG ;           //Clr interrupt of TA1.2
}



//-------------------------------------------------------------------------------------
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADCconfig(void){
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;//Vcc to GND, 64x ADC10CLK, Turn on ADC
    //ADC10CTL1 = INCH_6 | ADC10DIV_1;
}











void UART_config(void){
    P1SEL |= RXTX;
    P1SEL2 |= RXTX;
    UCA0CTL1 |= UCSWRST;            //set the module to reset
    UCA0CTL1 |= UCSSEL_2;             //SMCLK
    UCA0CTL0 = 0;                   // 8-bit, LSB first, No parity, 1 stop

    UCA0BR0 = 104;                  //9600 baud rate
    UCA0BR1 = 0;

    UCA0MCTL = UCBRS_1 | UCBRF_0;   //make sure that the baud rate is accurate(start from the second stage)
    UCA0CTL1 &= ~UCSWRST;           //cancel reset
    IFG2 &= ~UCA0RXIFG;
    IE2 |= UCA0RXIE;                // enable interrupts RX
    //IE2 |= UCA0TXIE;                // enable interrupts TX

}






