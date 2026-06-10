#include  "../header/halGPIO.h"     // private library - HAL layer
//#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
unsigned int N_ADC = 0;
int next = 0;
int type;

//variables for Servo angels
volatile unsigned int counter = 0;
unsigned int UpandDown = 1;
unsigned int count_servo_deg6 = 0;
//variables for distance samples
volatile int distance = 0;
volatile unsigned int samples[2];
//volatile unsigned int distances[100];
unsigned int PulseCounter = 0;
unsigned int angle = 0;
double c = 0;
unsigned int Temperature = 25;
double distance_cm = 0;

//Uart variables
volatile unsigned char rx = '0';
volatile unsigned char choice = '0';
volatile unsigned char UART_last = '0';
char max_dist[4];
char angle_array[4];
char file_size[4];
int run_script_request = 0;
unsigned int index_max_dist = 0;
unsigned int index_angle = 0;
unsigned int index_file_name = 0;
unsigned int index_file_size = 0;
unsigned int file_upload_finished = 0;
FileEntry my_script;
int count_file = 0;
int dist16;
char angle8;
int i = 0;
volatile char light_sample;
int clean_max = 0;

int max_num = 400;
int angle_num;
char *script_pointer = (char *)0xF600;
char *struct_pointer = (char *)0x1000;
unsigned char TX_active = 0;

unsigned int index = 0;
volatile unsigned char Left_Calib[50];
volatile unsigned char Right_Calib[50];
volatile unsigned char Average_Calib[50];
int count_finished = 0;

volatile unsigned int delay_target = 0;
volatile unsigned int delay_tick = 0;
char* struct_addresses[10];
char opcode[3];
char operand[10];

char counter_lcd[5] = "000";

int enter_to_file_mode = 0;

int switch_menu_choose = 0;
int switch_scroll = 0;
int top_index = 0;

int servo_deg_on = 0;
int servo_scan_on = 0;

int end = 0;
int begin = 0;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){
    GPIOconfig();
    //TIMERconfig();
    //ADCconfig();
}

int checkSW0(void){
    if ((SWsArrPort & SWmask) == 0x01)
        return 1;
    return 0;
}

void TurnOn_ADC10(void){
    ADC10CTL0 |= ADC10ON;//turn on ADC10
    ADC10CTL0 |= ENC | ADC10SC;//Enable + Start conversion
}


/*void N_Fout_to_TimerA0(void){
    unsigned int N_Fout;
    N_Fout = 2048 + ((6*N_ADC) >> 1);
    TA1CCR0 = SMCLK / N_Fout;
    TA1CCR1 = TA1CCR0 >> 1;
}*/

/*void enable_PWM(){
    TA1CTL |= MC_1; //start up mode
}

void disable_PWM(){
    TA1CTL &= ~MC_1; //stop up mode
}

void enable_Freq(){
    TA1CTL |= MC_1; //start up mode
}*/

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
  __interrupt void port1_handler(void){
      //delay(debounceVal);
      if(P1IFG & PB1){
            P1IFG &= ~PB1;
            if (state == state6) {
                if (switch_menu_choose == 0){
                    type = find_type(struct_addresses[top_index]);
                    if (type == 0){//Text
                        switch_scroll = 1;
                        lcd_show_page(struct_addresses[top_index]);
                        switch_menu_choose = 1;
                    }
                    else if(type == 1){//Script
                        lcd_clear();
                        run_script_request = 1;
                        switch_menu_choose = 1;
                        LPM0_EXIT;
                    }
                }
                else{
                    switch_menu_choose = 0;
                    lcd_cursor(0);
                    switch_scroll = 0;
                    top_index = 0;
                    display_scrolling_view();

                }
            }

        }
      P1IFG &= ~0xFF;
  }


#pragma vector=PORT2_VECTOR
  __interrupt void PBs_handler(void){
    delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if(P2IFG & PB0){
        P2IFG &= ~PB0;
        if (state == state3) {
            calibration();
            }


        else if(state == state6){




            //if scroll menu
            if (switch_scroll == 0){
                scroll_view_on_button_press();
            }
            //if scroll text
            else{
                scroll_text_on_button_press();
            }


        }

    }

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
    delay_tick++;
    if (delay_tick >= delay_target) {
        delay_tick = 0;
       //rra_lcd();

        if (strcmp(opcode, "01") == 0) {
            inc_lcd();
        } else if (strcmp(opcode, "02") == 0) {
            dec_lcd();
        } else if (strcmp(opcode, "03") == 0) {
            rra_lcd(operand);
        }
    }
    if (count_finished == 1){
        Timer_A0_intterupt_disable();
        count_finished = 0;
    }
}

void Trigger_on(void){
    TA1CCTL1 |= OUTMOD_7;
}
void Trigger_off(void){
    TA1CCTL1 &= ~OUTMOD_7;
}
void Capture_on(void){
    TA1CCTL2 |= CCIE;
}
void Capture_off(void){
    TA1CCTL2 &= ~CCIE;
}
void Timer_A1_intterupt_enable(void){
    TA1CTL |= TAIE;
}
void Timer_A1_intterupt_disable(void){
    TA1CTL &= ~TAIE;
}

void Timer_A0_intterupt_enable(void){
    TA0CCTL0 |= CCIE;
}
void Timer_A0_intterupt_disable(void){
    TA0CCTL0 &= ~CCIE;
}

void TX_enable(void){
    IE2 |= UCA0TXIE;
}
void TX_disable(void){
    IE2 &= ~UCA0TXIE;
}

void switch_to_adc(){
    P1REN &= ~PB1;            // Disable pull-up/down resistor
    P1OUT &= ~PB1;            // Clear output (just in case)
    ADC10AE0 |= PB1;          // Enable analog function
    ADC10CTL0 |= ADC10ON;      // Turn on ADC module
}

void switch_to_pushbutton(){
    ADC10AE0 &= ~PB1;         // Disable analog function on P1.0
    ADC10CTL0 &= ~ADC10ON;     // Optional: turn off ADC module
    P1DIR &= ~PB1;            // Set as input
    P1REN |= PB1;             // Enable pull-up/down resistor
    P1OUT |= PB1;             // Set pull-up
    P1IE |= PB1;
}

void Echo_meas(void){
    PulseCounter += 1;
    // save the data on rising and falling edge of the echo
    samples[PulseCounter - 1] = TA1CCR2;// associated to Echo pin
    TA1CCTL2 &= ~(COV + CCIFG);
    if (PulseCounter == 2){
        distance = abs((int)samples[1] - (int)samples[0]);
        c = 331.3 + 0.606 * Temperature;
        distance_cm = (double)(distance*c)/20000;
        PulseCounter = 0;

        if (state == state1) {
            Capture_off();
            //Timer_A1_intterupt_enable();

            if (!TX_active){
                TX_enable();
                TX_active = 1;
                angle8 = (char)angle;
                dist16 = (int)distance_cm;

                while (!(IFG2 & UCA0TXIFG));
                UCA0TXBUF = angle8;

                while (!(IFG2 & UCA0TXIFG));
                UCA0TXBUF = dist16 & 0xFF;

                while (!(IFG2 & UCA0TXIFG));
                UCA0TXBUF = (dist16 >> 8) & 0xFF;
                while (!(IFG2 & UCA0TXIFG));
                TX_disable();
                TX_active = 0;
            }
            Timer_A1_intterupt_enable();
        }
        else if(state == state5 || state == state6){
            Trigger_off();
            Capture_off();

            if (!TX_active){
                TX_enable();
                TX_active = 1;
                if (state == state6 && servo_scan_on == 1){
                    angle8 = (char)angle;
                    dist16 = (int)distance_cm;

                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF = 'S';

                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF = angle8;

                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF = dist16 & 0xFF;

                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF = (dist16 >> 8) & 0xFF;
                }
                else{
                    if (servo_deg_on == 1){
                        while (!(IFG2 & UCA0TXIFG));
                        UCA0TXBUF = 'D';
                    }
                    dist16 = (int)distance_cm;

                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF = dist16 & 0xFF;

                    while (!(IFG2 & UCA0TXIFG));
                    UCA0TXBUF = (dist16 >> 8) & 0xFF;
                }
               // while (!(IFG2 & UCA0TXIFG));
                TX_disable();
                TX_active = 0;
                if (count_servo_deg6 <= 15 && servo_deg_on == 1){
                    Timer_A1_intterupt_enable();
                    count_servo_deg6++;
                }
                else if (servo_scan_on == 1){
                    Timer_A1_intterupt_enable();
                }
                else{
                    servo_deg_on = 0;
                    next = 1;
                }
            }
        }
    }

}
//*********************************************************************
//           Timer A1 Interrupt Service Routine
//*********************************************************************
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void) {

    if (TA1IV == 4){
        Echo_meas();
    }
    else if (TA1CCTL0 & CCIFG){
        //if (UpandDown == 1){
        counter += 1;
        if(counter == 7){
            counter = 0;
            if (state == state1){
                if (TA0CCR1 < 2040){
    //                angle += 1;
    //                TA0CCR1 += 9;

                    angle += 1;
                    TA0CCR1 += 9;
                    Timer_A1_intterupt_disable();
                    Capture_on();
                }
                else{
                    Trigger_off();
                    Capture_off();
                    TX_disable();
                    TX_active = 0;
                    TA0CCR1 = 420;  // added - back to 0
                }
            }
            else if(state == state6){
                if (servo_deg_on == 1){
                    Trigger_on();
                    Capture_on();
                    Timer_A1_intterupt_disable();
                    TX_disable();
                    TX_active = 0;
                    Timer_A0_intterupt_disable();
                }
                else if (servo_scan_on == 1){
                    if (begin*9 + 420 <= TA0CCR1 && TA0CCR1 <= end*9 + 420){
                        angle += 1;
                        TA0CCR1 += 9;
                        Timer_A1_intterupt_disable();
                        Capture_on();
                        Trigger_on();
                    }
                    else{
                        Trigger_off();
                        Capture_off();
                        TX_disable();
                        TX_active = 0;
                        servo_scan_on = 0;
                        next = 1;
                    }
                }
            }
            else if (state == state7){      //dvir add this today
                if (TA0CCR1 < 2040){              //dvir add this today
                    angle += 2;                    //dvir add this today
                    TA0CCR1 += 18;                      //dvir add this today

                    Timer_A1_intterupt_disable();   //dvir add this today
                    light_sample=Light_detec_left() ;
                    //if (angle < 90){
                         //light_sample=Light_detec_right() ;              //dvir add this today
                    //}// else {
                          //   light_sample=Light_detec_left() ;              //dvir add this


                    if (!TX_active){                            //dvir changed it today
                        TX_enable();                            //dvir changed it today
                        TX_active = 1;                          //dvir changed it today
                        angle8 = (char)angle;                    //dvir changed it today
                        while (!(IFG2 & UCA0TXIFG));             //dvir changed it today
                        UCA0TXBUF = angle8;                      //dvir changed it today

                        while (!(IFG2 & UCA0TXIFG));             //dvir changed it today
                        UCA0TXBUF =light_sample;                 //dvir changed it today

                        while (!(IFG2 & UCA0TXIFG));             //dvir changed it today
                        TX_disable();                            //dvir changed it today
                        TX_active = 0;                            //dvir changed it today
                            }

                    Timer_A1_intterupt_enable();                //dvir add this today

                }
              }

        }

        TA1CCTL0 &= ~CCIFG;
    }
}


//*********************************************************************
void clear(char arr[]){
    int i = 0;
    for (i; i<3; i++){
        arr[i] = '\0';
    }

}
void clear_calib(char arr[]){    //dvir add this today
    int i = 0;
    for (i; i<50; i++){
        arr[i] = (char)0;           //dvir add this today
    }

}
//*********************************************************************
//           UARTRX Interrupt Service Routine
//*********************************************************************
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
     rx = UCA0RXBUF;
     if (rx == '@'){
         UART_last = rx;
        // state = state0;
     }
     else if (UART_last == '@'){
         choice = rx;
         UART_last = '0';
         enter_to_file_mode = 0;
         //clear(max_dist);
         clear(file_size);
         index_angle = 0;
         index_max_dist = 0;
         index_file_size = 0;
         index_file_name = 0;
         count_file = 0;
         count_servo_deg6 = 0;
     }

     switch (choice) {
         case 'S':
             state = state0;
             TX_disable();
             TX_active = 0;
             Trigger_off();
             Capture_off();
             TA0CCR1 = 420;


             break;
         case 'A':
             if (rx == 'A'){
                 TA0CCR0 = 21000; // Timer period
                 TA0CCR1 = 420;
                 TA0CCTL1 |= OUTMOD_7;
                 TA0CTL &= ~ID_3;
                 Timer_A0_intterupt_disable();
                 angle = 0;
                 Trigger_on();
                 Capture_on();
                 LPM0_EXIT;
             }
             state = state1;
             //TA0CCR1 = 420;
             TA1CCTL2 |= CCIE;
             break;

         case 'B'://send angle
             if (rx == 'B'){
                 clear(angle_array);
                 LPM0_EXIT; //dvir added this
             }
             state = state2;
             TX_disable();
             TX_active = 0;
             //TA1CCTL2 |= CCIE;
             if (index_angle <= 2 && rx <= 57 && rx >= 48 ) {
                 angle_array[index_angle] = rx;
                 index_angle++;
             }
             else {
                 index_angle = 0;
             }
             break;
         case 'C':
             state = state3;
             switch_to_adc();
             TA0CCR1 = 420 + 9*90;
             LPM0_EXIT;
             clear_calib(Left_Calib);
             clear_calib(Right_Calib);
             clear_calib(Average_Calib);
             index = 0;
             break;
         case 'D':
             state = state6;
             switch_to_pushbutton();
             if (count_file == 1){
                 if (rx == 'T'){
                     my_script.type = TEXT_FILE;
                 }
                 else if (rx == 'S'){
                     my_script.type = SCRIPT_FILE;
                 }
             }
             else if (count_file >= 3 && count_file <= 5){
                 if (index_file_size <= 2){
                     file_size[index_file_size] = rx;
                     index_file_size++;
                 }
             }
             else if(count_file >= 6 && count_file <= 17){
                 my_script.size = array_to_num(file_size);
                 if (index_file_name <= 11){
                     my_script.name[index_file_name] = rx;
                     index_file_name++;
                 }
                 my_script.flash_address = script_pointer;
             }
             else if(count_file >= 18){
                 if (my_script.size >= script_pointer - my_script.flash_address){
                     write_byte_to_flash(script_pointer, rx);
                     script_pointer++;
                 }
                 if (my_script.size == script_pointer - my_script.flash_address){
                     file_upload_finished = 1;
                 }
             }


             //enter_to_file_mode = 1;
             //counter_lcd[0] = '9';
             //counter_lcd[1] = '9';
             //counter_lcd[2] = '9';
             //Timer_A0_intterupt_enable();
             //set_delay();
             //TA0CCR0 = 65000; // delay
             //TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR;

             //TA0CCTL1 &= ~OUTMOD_7;
             //lcd_clear();
             count_file += 1;
             LPM0_EXIT;
             break;
         case 'L':                                                          //dvir add this today
             state = state7;
             TA0CCR0 = 21000; // Timer period
             TA0CCR1 = 420;
             TA0CCTL1 |= OUTMOD_7;
             TA0CTL &= ~ID_3;
             TX_active=0;
             Timer_A1_intterupt_enable();
       //      Timer_A0_intterupt_disable();
             angle = 0;
             LPM0_EXIT;
             break;

         case 'M':
             if (state == state6){
                 Trigger_on();
                 Capture_on();
                 Timer_A1_intterupt_disable();
             }
             else{
                 state = state5;
                 Trigger_on();
                 Capture_on();
                 Timer_A1_intterupt_disable();
             }
             break;
         case 'G':
             if (rx == 'G'){
                 clear(max_dist);
                 LPM0_EXIT;
             }
             state = state4;
             TX_disable();
             TX_active = 0;
             if (index_max_dist <= 2 && rx <= 57 && rx >= 48 ) {
                 max_dist[index_max_dist] = rx;
                 index_max_dist++;
             }
             else{
                 index_max_dist = 0;
             }
             break;


         default:
             break;
     }
}
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {
//    if (state == state6){
//        if (strcmp(opcode, "06") == 0) {
//            while (!(IFG2 & UCA0TXIFG));
//            UCA0TXBUF = 'D';
//            TX_disable();
//            TX_active = 0;
//        }
//        else if (strcmp(opcode, "07") == 0) {
//            while (!(IFG2 & UCA0TXIFG));
//            UCA0TXBUF = 'S';
//            TX_disable();
//            TX_active = 0;
//        }
//
//    }

}
//*********************************************************************
//           ADC10 Interrupt Service Routine
//*********************************************************************
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
    N_ADC = ADC10MEM;
}










/* - - - - - - - LCD interface - - - - - - - - -
 *  This code will interface to a standard LCD controller
 *  It uses it in 4 or 8 bit mode.
 */
#include "msp430g2553.h"
//#include  "LCD.h"

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
void lcd_clear_line(int line) {
    if (line == 1) {
        lcd_goto(0x00);  // Start of line 1
    } else if (line == 2) {
        lcd_goto(0x40);  // Start of line 2 (0xC0 in lcd_cmd)
    }
    int i = 0;
    for (i; i < 16; i++) {
        lcd_putchar(' ');  // Overwrite with spaces
    }

    // Return cursor to beginning of the line
    if (line == 1) {
        lcd_goto(0x00);
    } else if (line == 2) {
        lcd_goto(0x40);
    }
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

