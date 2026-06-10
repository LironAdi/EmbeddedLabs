#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

//extern void intToStr(int num,char* str);


extern int array_to_num(char arr[]);
extern void calibration(void);
extern void Fill_calib_Linearization(void);
extern void inc_lcd(void);
extern void dec_lcd(void);
extern void rra_lcd(const char* operand);
extern void servo_deg(const char* operand);
extern void servo_scan(const char* operand);
extern void set_delay(const char* operand);
extern void write_byte_to_flash(char *addr, char data);
extern char read_from_flash(char *addr);
#define MAX_LINE_LENGTH 10
extern char Light_detec_left(void);                                 //dvir add this
extern char Light_detec_right(void);                               //dvir add this
extern void  calibration_to_pc(char arr[]);                       //dvir add this

extern int size_from_flash;
extern unsigned int units_of_10ms;
extern void init_count_down(const char* operand);
extern void init_count_up(const char* operand);
extern char print_operand_char(const char* operand);
extern void script_command_machine(const char* command);
extern int end;
extern int begin;
extern const char * find_content_pointer(const char* struct_pointer);
extern void run_script_mode(const char* struct_pointer);
extern int file_count;
extern int num_files;
extern void write_struct_to_flash(FileEntry *entry, char *dest_flash);
//extern int add_file(const char *name, FileType type, unsigned int size, char *flash_addr);
extern void read_struct_from_flash(FileEntry *entry, char *src_flash);
extern void erase_flash_segment_auto(char *addr);
extern char* get_flash_segment_start(char* addr);
extern void erase_flash_segment(char *address);
extern void erase_all_file_segments();
extern void print_struct_names_from_flash(void);
extern void scroll_view_on_button_press(void);
extern void display_scrolling_view(void);
extern void parse_command(const char* string, char* opcode, char* operand);
extern void execute_command(const char* opcode, const char* operand);
extern int find_type(const char* struct_pointer);
extern void lcd_show_page(const char* flash_ptr);
extern void scroll_text_on_button_press(void);
extern void show_text(const char* scipt_ptr, const char** out_next_ptr);




#endif







