#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include <msp430.h>
enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  char shifter_led = 0x001;
  char incdec_led = 0x000;
  int neg_flag = 0; //flag = 0 - incLEDs
  //volatile int PWM_flag = -1;
  PWMArr = 0x00;
  char colors[8] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70};
  sysConfig();
  enable_interrupts();
  
  while(1){
	switch(state){
	  case state0:
	        clrLEDs();
	        enable_interrupts();
	        enterLPM(lpm_mode);
        break;

	  case state1:
	      disable_interrupts();
	      riseAndFallLEDs(incdec_led, neg_flag);
	      neg_flag = ~neg_flag;
	      incdec_led = LEDsArrPort;
	      enable_interrupts();
	      state = state0;
		break;
		 
	  case state2:
	      disable_interrupts();
	      shifter(shifter_led);
	      shifter_led = LEDsArrPort;
	      enable_interrupts();
	      state = state0;
		break;
		 
	  case state3:
	    PWM();
		break;
		
	  case state4:
	      disable_interrupts();
	      BlinkColors(colors,8 ,250);
	      enable_interrupts();
	    break;

	}
  }
}
  
  
  
  
  
  
