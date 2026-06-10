#include  "../header/halGPIO.h"     // private library - HAL layer
#include <string.h>
char counter[4] = "000";
volatile unsigned char captureCount = 0;
unsigned int N_ADC = 0;
 char pot_string[4];
int tone_series[7] = {1024, 820, 682, 584, 512, 456, 412};
int buz_index = 0;
int len = 0;
int pb_pressed=0;
volatile unsigned char rx = 0;
//volatile uint8_t X_delay;
char X_delay[5] = "0500";
char X_temp[5] = "0500";
char print_pot[4];
volatile unsigned int captureValues[2] = {0};
const char quote[] = "I love my Negev";
char strings[] = "                                    ";
int w = 0;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){
	GPIOconfig();
	//TIMERconfig();
	//ADCconfig();
}

/*int checkSW0(void){
    if ((SWsArrPort & SWmask) == 0x01)
        return 1;
    return 0;
}*/

void TurnOn_ADC10(void){
    ADC10CTL0 |= ADC10ON;//turn on ADC10
    ADC10CTL0 |= ENC | ADC10SC;//Enable + Start conversion
}

void N_Fout_to_TimerA0(void){
    unsigned int N_Fout;
    N_Fout = 2048 + ((6*N_ADC) >> 1);
    TA1CCR0 = SMCLK / N_Fout;
    TA1CCR1 = TA1CCR0 >> 1;
}

void enable_PWM(){
    TA1CTL |= MC_1; //start up mode
}

void disable_PWM(){
    TA1CTL &= ~MC_1; //stop up mode
}

void enable_Freq(){
    TA1CTL |= MC_1; //start up mode
}

//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;

	for(i=t; i>0; i--);
}
//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00)
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03)
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
   
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	//if(PBsArrIntPend & PB0){
	   // lcd_clear();
	    //state = state1;
	    //PBsArrIntPend &= ~PB0;
	//}
     if(PBsArrIntPend & PB1){
           if (state == state5) {                       // when we press pb we check if it rlly state5 , else do nothing
               IE2 |= UCA0TXIE;                          // Enable USCI_A0 TX interrupt
               pb_pressed =1;                            // flag that signal that the key is pressed
               PBsArrIntPend &= ~PB1;
        }
       // lcd_clear();A
      //  state = state2;
       // PBsArrIntPend &= ~PB1;

    }
//	else if(PBsArrIntPend & PB2){
      //  lcd_clear();
	//    state = state3;
	   // PBsArrIntPend &= ~PB2;


//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------
        switch(lpm_mode){
		case mode0:
		 LPM0_EXIT; // must be called from ISR only
		 break;

		case mode1:
		 LPM1_EXIT; // must be called from ISR only
		 break;

		case mode2:
		 LPM2_EXIT; // must be called from ISR only
		 break;
                 
         case mode3:
		 LPM3_EXIT; // must be called from ISR only
		 break;
                 
         case mode4:
		 LPM4_EXIT; // must be called from ISR only
		 break;
	}
        
}
//*********************************************************************
//           Timer A0 Interrupt Service Routine
//*********************************************************************
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_ISR(void) {
    if (state==state1) {
                // Check if tens of seconds reached 5
                if (counter[2] == '9') {
                    // Check if units of minutes reached 9
                    if (counter[1] == '9') {
                        // Check if tens of minutes reached 5
                        if (counter[0] == '9') {
                            // Reset all to 00:00
                            counter[0] = '0';
                            counter[1] = '0';
                            counter[2] = '0';
                        } else {
                            // Increment tens of minutes
                            counter[0]++;
                            counter[1] = '0';
                            counter[2] = '0';
                        }
                    } else {
                        // Increment units of minutes
                        counter[1]++;
                        counter[2] = '0';
                    }
                } else {
                    // Increment tens of seconds
                    counter[2]++;
                }
            }
    if (state==state2) {
        buz_index = (buz_index +1) % 7 ;
        TA1CCR0 = tone_series[buz_index];
        TA1CCR2 = TA1CCR0 >> 1 ;
    }
}




//*********************************************************************
//           Timer A1 Interrupt Service Routine
//*********************************************************************
/*#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void) {
    switch (__even_in_range(TA1IV, 14)) {
        case 4:  // TA1CCR2 interrupt
            if (state == state1){
                TA1CCTL2 &= ~CCIFG; // clear interrupt flag

                captureValues[captureCount] = TA1CCR2;
                captureCount++;
                if (captureCount >= 2) {
                    captureCount = 0;
                }
            }
            else if (state == state3) {
                BuzzerOUT ^= Buzzer;
            }
            else if (state == state2 ){
             break;
            }
            break;

        default:
            break;
    }
}*/
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void){
    if (TA1CCTL2 & CCIFG) {
           TA1CCTL2 &= ~CCIFG;   // clear interrupt flag
       }
    __bic_SR_register_on_exit(LPM1_bits);
}



//******************************************************************
// UART_VECTOR
//******************************************************************

//UART TRANSMITER
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {

    static unsigned int i;
       if(quote[i] != '\0') {    //checks if we fin to print
           UCA0TXBUF = quote[i]; //print the char
           i++;
       } else if (quote[i] == '\0') { //we fin to print
           pb_pressed = 0;            //reset the pb for the next press
           i = 0;                    //reset the index
           IE2 &= ~UCA0TXIE;
       }


}


//UART RECEIVER
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    rx = UCA0RXBUF;
    if (state == state3){
        len = 0;
        memset(X_temp, '0', sizeof(X_temp));
        memset(X_delay, '0', sizeof(X_delay));
        state = state0;
        rx = 0;
    }
    else if (state == state9) {
        if (rx != '@' && w <= 37){
            strings[w++] = rx;
            //lcd_data(strings);
        }
        else {
            lcd_puts(strings);
            state = state0;
            memset(strings, ' ', sizeof(strings));

            }
        }

switch(UCA0RXBUF){
    case '1':
       // LCD_clear();
        lcd_goto(0);
        lcd_clear();
        state = state1;
        BuzzerOUT &=~Buzzer;

        break;
    case '2':
        lcd_goto(0);
        lcd_clear();
        lcd_goto(0);
        lcd_puts("Music:");
        state = state2;
        break;
    case '3':
        lcd_goto(0);
        lcd_clear();
        len = 0;
        memset(X_temp, '0', sizeof(X_temp));
        memset(X_delay, '0', sizeof(X_delay));
        state = state3;
        rx = 0;
        break;
    case '4':
        lcd_goto(0);
        lcd_clear();
            lcd_puts("POT vol :");
        BuzzerOUT &=~Buzzer;

        state = state4;
        break;
    case '5':
        lcd_clear();
        state = state5;
        break;
    case '6':
        lcd_clear();

        state = state6;
        counter[0] = '0';
        counter[1] = '0';
        counter[2] = '0';
        counter[3] = '\0';
        break;
    case '7':
        state =state7;

    case '8':
        state = state8;
        break;
    case '9':
        lcd_clear();
        state = state9;
        w = 0;

        }

if (len <= 4){
            X_temp[len++] = rx;
            convertor();
        }
    __bic_SR_register_on_exit(LPM3_bits);
}


//*********************************************************************
//           ADC10 Interrupt Service Routine
//*********************************************************************
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
    N_ADC = ADC10MEM;
}




/* - - - - - - - LCD interface - - - - - - - - -
 *	This code will interface to a standard LCD controller
 *  It uses it in 4 or 8 bit mode.
 */
#include "msp430g2553.h"
//#include	"LCD.h"

//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){

	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	if (LCD_MODE == FOURBIT_MODE)
	{
		LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
    		LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){

	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_RS(1);
	if (LCD_MODE == FOURBIT_MODE)
	{
    		LCD_DATA_WRITE &= ~OUTPUT_DATA;
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
                LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}

	LCD_RS(0);
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){

	while(*s)
		lcd_data(*s++);
}
//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){

	char init_value;

	if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;
    disable_interrupts();
	LCD_RS_DIR(OUTPUT_PIN);
	LCD_EN_DIR(OUTPUT_PIN);
	LCD_RW_DIR(OUTPUT_PIN);
        LCD_DATA_DIR |= OUTPUT_DATA;
        LCD_RS(0);
        LCD_EN(0);
        LCD_RW(0);

    //DelayUs(1);//just for test
	DelayMs(15);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayMs(5);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayUs(200);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();

	if (LCD_MODE == FOURBIT_MODE){
		LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
		lcd_strobe();
		lcd_cmd(0x28); // Function Set
	}
        else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots

	lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
	lcd_cmd(0x1); //Display Clear
	lcd_cmd(0x6); //Entry Mode
	lcd_cmd(0x80); //Initialize DDRAM address to zero
    enable_interrupts();

}



//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){

	unsigned char i;
        for(i=cnt ; i>0 ; i--) asm("nop"); // tha command asm("nop") takes raphly 1usec

}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){

	unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec

}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
  asm(" nop");
  asm(" nop");
  LCD_EN(0);
}



char* return_potentiometer(void) {
    unsigned int potentiometer = (N_ADC*0.326);

    pot_string[0] = '0' + (potentiometer/100);         //1v-3v
    pot_string[1] = '.';
    pot_string[2] = '0' + ((potentiometer/10)%10);   // 0.1v-0.9v
    pot_string[3] = '0' + (potentiometer % 10);      // 0.01v-0.09v

    return pot_string;
}

