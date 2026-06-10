#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

void LCD_Counter(){
  //  lcd_puts(TimeC);
 //   lcd_cursor(0);

     lcd_puts(counter);
     lcd_cursor(0);
}

void PWM_Buzzer(){
    TurnOn_ADC10();
    N_Fout_to_TimerA0();
    enable_PWM();
}

void LCD_Freq(){
    //enable_Freq();
    int T = (captureValues[1]-captureValues[0]);
    int F = SMCLK/ T;
    int F_int = (int)(F*1.05);
    char num_str[6];
    intToStr(F_int, num_str);
    lcd_cursor(4);
    lcd_puts(num_str);
}

void intToStr(int num,char* str) {
    int i = 0;

    if (num == 0) {
           str[i++] = '0';
           str[i] = '\0';
           return;
       }
    while (num > 0 && i < 5) {
           str[4-i] = (num % 10) + '0';
           i++;
           num = num / 10;
       }
    str[i] = '\0';
}

void convertor(){
    int i = 0;
    int X;
    int Y;
    for (i = 0 ;i <= len;i++){
        X_delay[4 - i] = X_temp[len - i];
    }
    //X = 131*((100)*(X_delay[2] - '0') + (10)*(X_delay[3] - '0') + (X_delay[4] - '0'));
    X = 131*((1000)*(X_delay[1] - '0') + (100)*(X_delay[2] - '0') + (10)*(X_delay[3] - '0') + (X_delay[4] - '0'));
    Y = 0xFFFF;
    //TA0CCR0 = X;
    TA0CCR0 = ( ( (X) > (Y) ) ? (Y) : (X) );

}

void potent2LCD(void) {
    char* potentiometer;

    potentiometer = return_potentiometer();
    lcd_goto(11);
    lcd_puts(potentiometer);
}

void printstrings() {
    while (rx != '@'){
        lcd_puts(strings);
    }
    if (rx == '@'){
        state = state0;
    }
}





/*//-------------------------------------------------------------
//            Print SWs value onto LEDs
//-------------------------------------------------------------
void printSWs2LEDs(void){
	unsigned char ch;
	
	ch = readSWs();
	print2LEDs(ch);
}
//--------------------------------------------------------------------
//            Print array to LEDs array with rate of LEDs_SHOW_RATE
//--------------------------------------------------------------------            
void printArr2SWs(char Arr[], int size, unsigned int rate){
	unsigned int i;
	
	for(i=0; i<size; i++){
		print2LEDs(Arr[i]);
		delay(rate);
	}
}*/


 
  

