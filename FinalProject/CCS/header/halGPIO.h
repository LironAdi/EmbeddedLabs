#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"         // private library - BSP layer
#include  "../header/app.h"         // private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void delay(unsigned int); //for debounce

extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char* s);
extern void lcd_init();
extern void lcd_strobe();

extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);

extern int checkSW0(void);

extern void TurnOn_ADC10(void);
//extern void N_Fout_to_TimerA0(void);

//extern void enable_PWM(void);
//extern void disable_PWM(void);

//extern void enable_Freq(void);

//extern char TimeC[6];
//extern volatile unsigned int captureValues[2];


extern void Echo_meas(void);

extern void Trigger_on(void);
extern void Trigger_off(void);
extern void Capture_on(void);
extern void Capture_off(void);

extern void Timer_A1_intterupt_enable(void);
extern void Timer_A1_intterupt_disable(void);

extern void TX_disable(void);
extern void TX_enable(void);

extern unsigned int count_servo_deg6;
extern int servo_deg_on;
extern int servo_scan_on;


extern int count_finished;
extern char opcode[3];
extern char operand[10];
extern int next;
extern char max_dist[4];
extern char angle_array[4];
extern int clean_max;
extern unsigned int index_angle;
extern int max_num;
extern int angle_num;
extern unsigned int angle;
extern volatile unsigned char Left_Calib[50];
extern volatile unsigned char Right_Calib[50];
extern volatile unsigned char Average_Calib[50];
extern unsigned int index;
extern unsigned char TX_active;
extern char counter_lcd[5];
extern volatile unsigned int delay_target;
extern volatile unsigned int delay_tick;
extern char *start_main_memory;
extern unsigned int file_upload_finished;
extern char *struct_pointer;
extern int top_index;
extern char* struct_addresses[10];
extern int type;
extern FileEntry my_script;
extern int run_script_request;


//---------------------------------------------------------------------
//            LCD Configuration
//---------------------------------------------------------------------


#ifdef CHECKBUSY
    #define LCD_WAIT lcd_check_busy()
#else
    #define LCD_WAIT DelayMs(5)
#endif

/*----------------------------------------------------------
  CONFIG: change values according to your port pin selection
------------------------------------------------------------*/
#define LCD_EN(a)   (!a ? (P2OUT&=~0X02) : (P2OUT|=0X02)) // P2.1 is lcd enable pin
#define LCD_EN_DIR(a)   (!a ? (P2DIR&=~0X02) : (P2DIR|=0X02)) // P2.1 pin direction

#define LCD_RS(a)   (!a ? (P2OUT&=~0X20) : (P2OUT|=0X20)) // P2.5 is lcd RS pin
#define LCD_RS_DIR(a)   (!a ? (P2DIR&=~0X20) : (P2DIR|=0X20)) // P2.5 pin direction

#define LCD_RW(a)   (!a ? (P2OUT&=~0X80) : (P2OUT|=0X80)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)   (!a ? (P2DIR&=~0X80) : (P2DIR|=0X80)) // P2.7 pin direction

#define LCD_DATA_OFFSET 0x04 //data pin selection offset for 4 bit mode, variable range is 0-4, default 0 - Px.0-3, no offset
/*---------------------------------------------------------
  END CONFIG
-----------------------------------------------------------*/

#define FOURBIT_MODE    0x0
#define EIGHTBIT_MODE   0x1
#define LCD_MODE        FOURBIT_MODE


#define OUTPUT_PIN      1
#define INPUT_PIN       0
#define OUTPUT_DATA     (LCD_MODE ? 0xFF : (0x0F << LCD_DATA_OFFSET))
#define INPUT_DATA      0x00


#define lcd_cursor(x)       lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()         lcd_cmd(0x01)
#define lcd_putchar(x)      lcd_data(x)
#define lcd_goto(x)         lcd_cmd(0x80+(x))
#define lcd_cursor_right()  lcd_cmd(0x14)
#define lcd_cursor_left()   lcd_cmd(0x10)
#define lcd_display_shift() lcd_cmd(0x1C)
#define lcd_home()          lcd_cmd(0x02)
#define cursor_off          lcd_cmd(0x0C)
#define cursor_on           lcd_cmd(0x0F)
#define lcd_function_set    lcd_cmd(0x3C) // 8bit,two lines,5x10 dots
#define lcd_new_line        lcd_cmd(0xC0)

extern void lcd_clear_line(int line);

/*
#define LCD_STROBE_READ(value)  LCD_EN(1), \
                asm("nop"), asm("nop"), \
                value=LCD_DATA_READ, \
                LCD_EN(0)
*/


#endif







#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"         // private library - BSP layer
#include  "../header/app.h"         // private library - APP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void delay(unsigned int); //for debounce

extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char* s);
extern void lcd_init();
extern void lcd_strobe();

extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);

extern int checkSW0(void);

extern void TurnOn_ADC10(void);
//extern void N_Fout_to_TimerA0(void);

//extern void enable_PWM(void);
//extern void disable_PWM(void);

//extern void enable_Freq(void);

//extern char TimeC[6];
//extern volatile unsigned int captureValues[2];


extern void Echo_meas(void);

extern void Trigger_on(void);
extern void Trigger_off(void);
extern void Capture_on(void);
extern void Capture_off(void);

extern void Timer_A1_intterupt_enable(void);
extern void Timer_A1_intterupt_disable(void);

extern void TX_disable(void);
extern void TX_enable(void);


extern int servo_deg_on;
extern int servo_scan_on;


extern int count_finished;
extern char opcode[3];
extern char operand[10];
extern int next;
extern char max_dist[4];
extern char angle_array[4];
extern int clean_max;
extern unsigned int index_angle;
extern int max_num;
extern int angle_num;
extern unsigned int angle;
extern volatile unsigned char Left_Calib[50];
extern volatile unsigned char Right_Calib[50];
extern volatile unsigned char Average_Calib[50];
extern unsigned int index;
extern unsigned char TX_active;
extern char counter_lcd[5];
extern volatile unsigned int delay_target;
extern volatile unsigned int delay_tick;
extern char *start_main_memory;
extern unsigned int file_upload_finished;
extern char *struct_pointer;
extern int top_index;
extern char* struct_addresses[10];
extern int type;
extern FileEntry my_script;
extern int run_script_request;


//---------------------------------------------------------------------
//            LCD Configuration
//---------------------------------------------------------------------


#ifdef CHECKBUSY
    #define LCD_WAIT lcd_check_busy()
#else
    #define LCD_WAIT DelayMs(5)
#endif

/*----------------------------------------------------------
  CONFIG: change values according to your port pin selection
------------------------------------------------------------*/
#define LCD_EN(a)   (!a ? (P2OUT&=~0X02) : (P2OUT|=0X02)) // P2.1 is lcd enable pin
#define LCD_EN_DIR(a)   (!a ? (P2DIR&=~0X02) : (P2DIR|=0X02)) // P2.1 pin direction

#define LCD_RS(a)   (!a ? (P2OUT&=~0X20) : (P2OUT|=0X20)) // P2.5 is lcd RS pin
#define LCD_RS_DIR(a)   (!a ? (P2DIR&=~0X20) : (P2DIR|=0X20)) // P2.5 pin direction

#define LCD_RW(a)   (!a ? (P2OUT&=~0X80) : (P2OUT|=0X80)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)   (!a ? (P2DIR&=~0X80) : (P2DIR|=0X80)) // P2.7 pin direction

#define LCD_DATA_OFFSET 0x04 //data pin selection offset for 4 bit mode, variable range is 0-4, default 0 - Px.0-3, no offset
/*---------------------------------------------------------
  END CONFIG
-----------------------------------------------------------*/

#define FOURBIT_MODE    0x0
#define EIGHTBIT_MODE   0x1
#define LCD_MODE        FOURBIT_MODE


#define OUTPUT_PIN      1
#define INPUT_PIN       0
#define OUTPUT_DATA     (LCD_MODE ? 0xFF : (0x0F << LCD_DATA_OFFSET))
#define INPUT_DATA      0x00


#define lcd_cursor(x)       lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()         lcd_cmd(0x01)
#define lcd_putchar(x)      lcd_data(x)
#define lcd_goto(x)         lcd_cmd(0x80+(x))
#define lcd_cursor_right()  lcd_cmd(0x14)
#define lcd_cursor_left()   lcd_cmd(0x10)
#define lcd_display_shift() lcd_cmd(0x1C)
#define lcd_home()          lcd_cmd(0x02)
#define cursor_off          lcd_cmd(0x0C)
#define cursor_on           lcd_cmd(0x0F)
#define lcd_function_set    lcd_cmd(0x3C) // 8bit,two lines,5x10 dots
#define lcd_new_line        lcd_cmd(0xC0)

extern void lcd_clear_line(int line);

/*
#define LCD_STROBE_READ(value)  LCD_EN(1), \
                asm("nop"), asm("nop"), \
                value=LCD_DATA_READ, \
                LCD_EN(0)
*/


#endif







