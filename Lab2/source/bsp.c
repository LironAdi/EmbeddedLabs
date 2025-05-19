#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // Switches Setup
  SWsArrPortSel &= ~0x0F;//on output mode
  SWsArrPortDir &= ~0x0F; //INPUT MODE

  // PushButtons Setup
  PBsArrPortSel &= ~0x07;            // DECODE P1.3-P1.0 INTO O/I
  PBsArrPortDir &= ~0x07;            // DECODE THEM INTO INPUT MODE
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode
  PBsArrIntEn |= 0x07;
  PBsArrIntPend &= ~0x07;            // clear pending interrupts
  //PBsArrIntPend = 0x00;
  //PBsArrIntEn = 0x00;




  
  _BIS_SR(GIE);                     // enable interrupts globally

//-------------------------------------------------------------
//            LCD setup
//-------------------------------------------------------------
  LCD_DATA_SEL &= ~0xF0 ;             // makes P1.4 - P1.7 IO mode
  LCD_DATA_DIR |= 0xF0 ;              // makes P1.4 - P1.7 Output mode
  LCD_DATA_WRITE &= ~0xF0 ;           // CLR P1.4 - P1.7

  LCD_CTL_Sel &= ~0xE0 ;             // makes P2.5 - P2.7 IO mode
  LCD_CTL_Dir |= 0xE0 ;              // makes P2.5 - P2.7 Output mode
  LCd_CTL_Write &= ~0xE0 ;           // CLR P2.5 - P2.7

//-------------------------------------------------------------
//            InputSignal setup
//-------------------------------------------------------------
//  LCD_CTL_Sel |= 0x10 ;              // makes P2.4  IO mode
// LCD_CTL_Dir &= 0x10 ;              // makes P2.4  Input mode
//LCd_CTL_Write &= ~0x10 ;           // CLR P2.4

}




/// DVIR ADD HERE THE TIMERS CONFIGURATION


//------------------------------------------------------------------------------------- 
//            Timers configuration
//-------------------------------------------------------------------------------------
void TIMER_A0_config(void){
  TA0CTL |= TACLR;                      // clear TA0
  TA0CTL = TASSEL_2 + TAIE + ID_3 + MC_3 + TAIE  ;     //TASSEL_2 choose - SMCLK , enable interrupts , ID_3 - divider by 8, UpDown mode
  TA0CTL &= ~TAIFG ;                     // clear interrupts
  TA0CCR0 = 0xFFFF ;                   // makes TA CCR to FFFF - ******need to check if this is 1 sec
  TA0CCTL0 = CCIE;

  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}
void TIMER_A1_config(void){
 TA1CTL = TASSEL_2 + MC_2 + TACLR;           //choose the SMCLK
 TimerA1SEL |=0x14 ;                         // set the 2.5/2.2 Pin into I/O mode
 TimerA1DIR &= ~0x10;                        //define P2.4 to be input mode
 TimerA1DIR |=0x04;                          //define P2.2 to be output mode
 TA1CCTL2 |= CM_1 + CAP + CCIE + CCIS_0 ;    // CM :1 - Rising Edge , SCS :1 - Sync Cap , CAP :1 - Capture Mode , CCIE :1 - enable
 TA1CCTL2 &= ~CCIFG;                         //clear pending interrupts
 TA1CCTL1 |= OUTMOD_2;                       // Create PWM signal
}




//-------------------------------------------------------------------------------------
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADCconfig(void){
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + MSC + ADC10IE;
    ADC10CTL0 &= ~ADC10IFG;//clear pending interrupts
    ADC10CTL1 = INCH_3 + SHS_0 + ADC10DIV_1 + ADC10SSEL_3 + CONSEQ_2;
    ADC10AE0 = 0x08;//enable P1.3 to input
}




// // LEDs 8-bit Array Port configuration
// LEDsArrPortSel &= ~0xFF;            // GPIO capability
// LEDsArrPortDir |= 0xFF;             // output dir
// LEDsArrPort = 0x00;               // clear all LEDs












