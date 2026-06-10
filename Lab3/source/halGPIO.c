#include  "../header/halGPIO.h"     // private library - HAL layer



const char keypad_table[4][4][3] = {
    { {'1', 'G', '\0'}, {'2', 'H', '\0'}, {'3', 'I', '\0'}, {'C', 'J', '\0'} },
    { {'4', 'K', '\0'}, {'5', 'L', '\0'}, {'6', 'M', '\0'}, {'D', 'N', '\0'} },
    { {'7', 'O', '\0'}, {'8', 'P', '\0'}, {'9', 'Q', '\0'}, {'E', 'R', '\0'} },
    { {'A', 'S', 'T'}, {'0', 'U', 'V'}, {'B', 'W', 'X'}, {'F', 'Y', 'Z'} }};
	
static unsigned char led_array[9] = {128, 224, 32, 126, 83, 44, 253, 113, 160};
unsigned int index=0;
unsigned int cursor=0;
char cur_State='\0';
unsigned int old_col= 4;
unsigned int old_row = 4;
volatile int done_timer = 1;
unsigned int num_string = 11;
char num_string1 = 'E';



//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	lcd_init();
	TIMERconfig();
	DMAconfig();
	TIMERBconfig();			
	cursor_off;
}

//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}
void clrLEDs(void) {
   P9OUT= 0x00;
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

    delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if(PBsArrIntPend & PB0){
      state = state1;
      PBsArrIntPend &= ~PB0;
      lcd_clear();
        }

     else if(PBsArrIntPend & PB1){
      state = state2;
      PBsArrIntPend &= ~PB1;
      lcd_clear();
        }

    else if(PBsArrIntPend & PB2){
      state = state3;
      PBsArrIntPend &= ~PB2;
      lcd_clear();
        }
    else if(PBsArrIntPend & PB3){
          state = state3;
          PBsArrIntPend &= ~PB3;
          lcd_clear();
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
  //************** ISR OF TIMER
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB_config(void)
 
{
        }

  //************** ISR OF TIMER
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER_config(void)
{
    if (cursor<32){
        if (cursor==16){
            lcd_new_line;
            lcd_data(cur_State);
                cursor++;
                done_timer = 1;
            //TACTL |= TACLR;
         }else {    lcd_data(cur_State);
                    cursor++;
                    done_timer = 1;
                    }

                }
    TACTL &=~TAIFG;// clear pending interupts
    TACTL &= ~UpMode;/// Turn of the timer until next tap
}


#pragma vector=DMA_VECTOR
__interrupt void DMA_handler(void)
{
    switch(__even_in_range(DMAIV, DMAIV_DMA2IFG))
    {
        case DMAIV_NONE: break;
        case DMAIV_DMA0IFG:
            if (state == state2)
                LPM0_EXIT;
            DMA0CTL &= ~DMAIFG;
            break;
        case DMAIV_DMA1IFG:
            if (state == state3)
                DMA1CTL &= ~DMAEN;
            DMA1CTL &= ~DMAIFG;
            break;
        case DMAIV_DMA2IFG:
            // אם אתה משתמש גם ב-DMA2
            DMA2CTL &= ~DMAIFG;
            break;
    }
}
/// vector of the dma
/*#pragma vector=DMA_VECTOR
__interrupt void DMA_handler(void)
{
    if(state == state2){
            LPM0_EXIT;
            DMA0CTL &= ~DMAIFG;
        }
        else if(state == state3){
            DMA1CTL &= ~DMAEN;
            DMA1CTL &= ~DMAIFG;
        }
}*/
//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT2_VECTOR
  __interrupt void Keypad_handler(void){
      delay(750);

     if (state == state1){
         TACTL |= UpMode;
         P10OUT = 0x0E;
         if ((P10IN & 0x10) == 0)       check_state(3,3); //  F
         else if ((P10IN & 0x20) == 0)  check_state(3,2); //  B
         else if ((P10IN & 0x40) == 0)  check_state(3,1); //  0
         else if ((P10IN & 0x80) == 0)  check_state(3,0); //  A


        P10OUT = 0x0D;
        if ((P10IN & 0x10) == 0)      check_state(2,3);  // E
        else if ((P10IN & 0x20) == 0) check_state(2,2); // 9
        else if ((P10IN & 0x40) == 0) check_state(2,1); // 8
        else if ((P10IN & 0x80) == 0) check_state(2,0); // 7

        // Row 2
        P10OUT = 0x0B;
        if ((P10IN & 0x10) == 0)       check_state(1,3); // D
        else if ((P10IN & 0x20) == 0)  check_state(1,2); // 6
        else if ((P10IN & 0x40) == 0)  check_state(1,1); // 5
        else if ((P10IN & 0x80) == 0)  check_state(1,0); // 4

        // Row 3
        P10OUT = 0x07;
        if ((P10IN & 0x10) == 0)        check_state(0,3); // C
        else if ((P10IN & 0x20) == 0)   check_state(0,2); // 3
        else if ((P10IN & 0x40) == 0)   check_state(0,1); // 2
        else if ((P10IN & 0x80) == 0)   check_state(0,0); // 1

        P10OUT = 0x00;
        P2IFG &=~0x02;
        //check_state(); ///checks the state
  }
     else if (state == state2){
         delay(750);
         P10OUT = 0x0E;
         if ((P10IN & 0x40) == 0)  num_string = 0; //  0


         P10OUT = 0x0D;
         if ((P10IN & 0x20) == 0) num_string = 9; // 9
         else if ((P10IN & 0x40) == 0) num_string = 8; // 8
         else if ((P10IN & 0x80) == 0) num_string = 7; // 7

         // Row 2
         P10OUT = 0x0B;
         if ((P10IN & 0x20) == 0)  num_string = 6; // 6
         else if ((P10IN & 0x40) == 0)  num_string = 5; // 5
         else if ((P10IN & 0x80) == 0)  num_string = 4; // 4

         // Row 3
         P10OUT = 0x07;
         if ((P10IN & 0x20) == 0)   num_string = 3; // 3
         else if ((P10IN & 0x40) == 0)   num_string = 2; // 2
         else if ((P10IN & 0x80) == 0)   num_string = 1; // 1

         P10OUT = 0x00;
         P2IFG &=~0x02;
     }
}

     ///*******************************************************
     void check_state(row,col) {
         if (old_row == row && old_col == col && done_timer == 0) {
             if (row == 3){
                 index = (index + 1) % 3;
             }
             else {
                 index = (index + 1) % 2;
             }
         }
         else{
             index = 0;
             done_timer = 0;
         }
         old_row = row;
         old_col = col;
         cur_State = keypad_table[row][col][index];


  }


//***********************************************
void lcd_init(){
	char init_value;

	if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;
	
	LCD_RS_DIR(OUTPUT_PIN);
	LCD_EN_DIR(OUTPUT_PIN);
	LCD_RW_DIR(OUTPUT_PIN);
        LCD_DATA_DIR |= OUTPUT_DATA;
        LCD_RS(0);
	LCD_EN(0);
	LCD_RW(0);
        
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
	
}

void lcd_strobe(){
  LCD_EN(1);
  asm(" nop");
  asm(" nop");
  LCD_EN(0);
}

void DelayUs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) asm(" nop"); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){
  
	unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec
	
}

void lcd_puts(const char * s){
  
	while(*s)
		lcd_data(*s++);
}


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

void startDMA2() {
   // enterLPM(lpm_mode);
    DMA2SA = (void*)led_array;
    DMA2DA = (void*)&LEDsArrPort;
    DMA2SZ = 9;
    DMA2CTL = DMADT_4 | DMASRCINCR_3 | DMASRCBYTE | DMADSTBYTE | DMAIE| DMADSTINCR_0;
    DMA2CTL |= DMAEN;
}   ////dma starter


void startDMA4() {
   // enterLPM(lpm_mode);
    DMA0SA = (unsigned int)RBG_array;
    DMA0DA = (unsigned int)&RGBsArrPort;
    DMA0SZ = 1;
    //DMA0CTL = DMADT_5 | DMASRCINCR_3 | DMASRCBYTE | DMADSTBYTE | DMAIE| DMADSTINCR_0;
    DMA0CTL |= DMAEN;
    DMA0CTL |= DMAREQ;
}   ////dma starter






