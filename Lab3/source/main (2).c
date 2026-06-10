#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
char* data_base[] = {"An apple a day keeps the doctor away",
                        "Give someone the benefit of the doubt",
                        "Time flies when you're having fun",
                        "He who pays the piper calls the tune",
                        "The pen is mightier than the sword",
                        "The pot calling the kettle black",
                        "Wrap your head around something",
                        "We'll cross that bridge when we come to it",
                        "A bird in hand is worth two in the bush",
                        "Do unto others as you would have then unto you",
                        "You can catch more flies with honey than you can with vinegar",
                        "eod"};

void main(void){
  sysConfig();
  lpm_mode = mode0;     // start in idle state on RESET
  state = state0;  // start in idle state on RESET
  
  while(1){
	switch(state){
	  case state0:
		  DMA2CTL &= ~DMAEN;			 
	      enterLPM(lpm_mode);
		break;
		 
	  case state1:
	  //add here the key_pad 
		 // TACTL |= UpMode;/// if we are on state 1 enable the clock

		break;
		 
	  case state2:
	      lcd_clear();
	      lcd_home();
	      lcd_puts("choose 0<=i<11");
	      TBCTL &= ~UpMode; ////if we are on state 2 disable the clock
	      TACTL &= ~UpMode; ////if we are on state 2 disable the clock
	      P1IFG &= ~BIT3;
	      P1IFG &= ~BIT2;
	      P1IFG &= ~BIT3;
	      reverse_string(data_base);
		break;
	  case state3:
	      TBCTL |= UpMode;
	      startDMA2(); // start the dma		
	  case state4:
	            TBCTL |= UpMode;
	            startDMA2(); // start the dma
	}
  }
}
  
  
  
  
  
  
