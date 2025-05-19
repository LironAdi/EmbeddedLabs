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
  TIMER_A1_config();
  ADCconfig();



  while(1){
    switch(state){
      case state0:
        ///printSWs2LEDs();
        enterLPM(lpm_mode);
        break;

      case state1:
          lcd_puts("fin=");
          LCD_Freq();
          lcd_cursor(9);
          lcd_puts("Hz");
          lcd_cursor(0);
        break;

      case state2:
          LCD_Counter();
        break;

      case state3:
          lcd_clear();
          PWM_Buzzer();
        break;


    }
  }
}





