#include  "../header/api.h"         // private library - API layer
#include  "../header/app.h"         // private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
void main(void){
  state = state0;       // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  lcd_init();          // config the LCD
  lcd_clear();         // CLR the LCD
  TIMER_A0_config();
  UART_config();
  TIMER_A1_config();
  ADCconfig();
 // convertor();
  cursor_off;



  while(1){
    switch(state){
      case state0:
          BuzzerOUT &=~Buzzer;
        ///printSWs2LEDs();
        enterLPM(LPM3_bits);
        break;

      case state1:
         // BuzzerOUT &=~0x10;
          TA1CTL &=~MC_1;
          TA0CTL |= MC_1;
          LCD_Counter();
        break;

      case state2:
          TA0CTL |= MC_1;
          TA1CTL |= MC_1;
        break;

      case state3:
          TA1CTL &=~MC_1;

        break;

      case state4:
          TA1CTL &=~MC_1;

          ADC10CTL0 |= ADC10ON;    //start the convert
          ADC10CTL0 |= ENC | ADC10SC; ///start the convert
         // char* print_pot = return_potentiometer();   //function the convert the POT into volt
          //need to print here :"the volate" is ....:
         // lcd_cursor(0);
         // lcd_puts(print_pot);
          potent2LCD();
        break;

      case state5:
          TA1CTL &=~MC_1;

        break;

      case state6:
          TA1CTL &=~MC_1;

          lcd_cursor(0);
          lcd_clear();
        break;
      case state7:
          TA1CTL &=~MC_1;
          TA0CTL &=~MC_1;
         break;
      case state8:
          TA1CTL &=~MC_1;
          state = state0;
        break;
      case state9:
          //printstrings();
        break;


    }
  }
}





