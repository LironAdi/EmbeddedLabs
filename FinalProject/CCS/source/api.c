#include  "../header/api.h"         // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
char* last_erased_segment = 0;
char X = 'A';
unsigned int units_of_10ms = 50;
int size_from_flash;
char *dest;
//FileEntry file_table[MAX_FILES];
int file_count = 0;
int num_files = 0;


//const char *content_ptr;

/*void list_files() {
    int i = 0;
    for (i; i < file_count; i++) {
        lcd_puts(file_table[i].name);
        lcd_data(' ');
    }
}*/



void read_struct_from_flash(FileEntry *entry, char *src_flash) {
    dest = (char *)entry;
    int i = 0;
    for (i; i < sizeof(FileEntry); i++) {
        dest[i] = src_flash[i];
    }
}



/*int add_file(const char *name, FileType type, unsigned int size, char *flash_addr) {
    if (file_count >= MAX_FILES)
        return -1;

    FileEntry *f = &file_table[file_count++];
    strncpy(f->name, name, 11);
    f->name[11] = '\0';
    f->type = type;
    f->size = size;
    f->flash_address = flash_addr;

    return 0;
}*/

void init_count_up(const char* operand){
    counter_lcd[0] = '0';
    counter_lcd[1] = '0';
    counter_lcd[2] = '0';
    counter_lcd[3] = '\0';
    //lcd_clear();
}

void inc_lcd(){
    int value = (int)strtol(operand, NULL, 16);
    if (counter_lcd[0] == ('0' + (value / 100)) && counter_lcd[1] == ('0' + ((value / 10) % 10)) && counter_lcd[2] == ('0' + (value % 10))) {
        lcd_cursor(0);
        lcd_puts(counter_lcd);
        count_finished = 1;
        next = 1;
        return;
    }

    lcd_cursor(0);
    lcd_puts(counter_lcd);
    if (counter_lcd[2] == '9') {
        // Check if units of minutes reached 9
        if (counter_lcd[1] == '9') {
            // Check if tens of minutes reached 5
            if (counter_lcd[0] == '9') {
                // Reset all to 00:00
                counter_lcd[0] = '0';
                counter_lcd[1] = '0';
                counter_lcd[2] = '0';
            }
            else {
                // Increment tens of minutes
                counter_lcd[0]++;
                counter_lcd[1] = '0';
                counter_lcd[2] = '0';
            }
        }
        else {
            // Increment units of minutes
            counter_lcd[1]++;
            counter_lcd[2] = '0';
        }
    }
    else {
        // Increment tens of seconds
        counter_lcd[2]++;
    }
    counter_lcd[3] = '\0';
    //TA0CCTL0 &= ~CCIFG;

}

void init_count_down(const char* operand){
    int value = (int)strtol(operand, NULL, 16);
    counter_lcd[0] = '0' + (value / 100);
    counter_lcd[1] = '0' + ((value / 10) % 10);
    counter_lcd[2] = '0' + (value % 10);
    counter_lcd[3] = '\0';
    //lcd_clear();
}


void dec_lcd(){
    if (counter_lcd[0] == '0' && counter_lcd[1] == '0' && counter_lcd[2] == '0') {
        lcd_cursor(0);
        lcd_puts(counter_lcd);
        count_finished = 1;
        next = 1;
        return;
    }
    lcd_cursor(0);
    lcd_puts(counter_lcd);

    if (counter_lcd[2] == '0') {
        // Check if units of minutes reached 9
        if (counter_lcd[1] == '0') {
            // Check if tens of minutes reached 5
            if (counter_lcd[0] == '0') {
                // Reset all to 00:00
                counter_lcd[0] = '0';
                counter_lcd[1] = '0';
                counter_lcd[2] = '0';
            }
            else {
                // Increment tens of minutes
                counter_lcd[0]--;
                counter_lcd[1] = '9';
                counter_lcd[2] = '9';
            }
        }
        else {
            // Increment units of minutes
            counter_lcd[1]--;
            counter_lcd[2] = '9';
        }
    }
    else {
        // Increment tens of seconds
        counter_lcd[2]--;
    }
    counter_lcd[3] = '\0';
    //TA0CCTL0 &= ~CCIFG;

}



//char print_operand_char(const char* operand) {
//    char ch = '\0';
//    int i = 0;
//    for (i; operand[i] != '\0'; i++) {
//        // skip leading '0', '\n', and space
//        if (operand[i] != '0' && operand[i] != '\n' && operand[i] != ' ') {
//            ch = operand[i];
//            break;
//        }
//    }
//
//    if (ch != '\0') {
//        return ch;  // print the character on LCD
//    }
//}


char print_operand_char(const char* operand) {
    unsigned int value = 0;
    char c;
    while ((c = *operand++)) {
        if (c >= '0' && c <= '9') {
            value = (value << 4) + (c - '0');
        } else if (c >= 'A' && c <= 'F') {
            value = (value << 4) + (c - 'A' + 10);
        } else if (c >= 'a' && c <= 'f') {
            value = (value << 4) + (c - 'a' + 10);
        } else {
            continue; // skip spaces/newlines
        }
    }
    return (char)value;
}

void rra_lcd(const char* operand){
    static int i = 0;
    X = print_operand_char(operand);

    if (i >= 32) {
        lcd_clear();
        count_finished = 1;
        next = 1; // added
        return;
    }
    lcd_clear();
    if (i < 16) {
        lcd_cmd(0x80 + i);  // first row
    }
    else {
        lcd_cmd(0xC0 + (i - 16));  // second row
    }

    lcd_data(X);
    i++;
}

/*void rra_lcd(){
    static int i = 0;

    lcd_clear();

    if (i < 16) {
        lcd_cmd(0x80 + i);
    }
    else if (i < 32) {
        lcd_cmd(0xC0 + (i - 16));
    }
    else {
        i = 0;
        lcd_cmd(0x80);
    }

    lcd_data(X);
    i++;
}*/

void set_delay(const char* operand){
    units_of_10ms = (int)strtol(operand, NULL, 16);
    delay_target = units_of_10ms;
    TA0CCR0 = 1250;
    TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR;
    TA0CCTL0 |= CCIE;
}

char read_from_flash(char *addr) {
    return *addr;
}




void write_struct_to_flash(FileEntry *entry, char *dest_flash) {
    // Cast the struct to a byte-wise pointer
    char *src = (char *)entry;
    int i = 0;
    for (i; i < sizeof(FileEntry); i++) {
        write_byte_to_flash(dest_flash + i, src[i]);
    }
}






void write_byte_to_flash(char *addr, char data) {

    char* segment_start = get_flash_segment_start(addr);
    if (segment_start == NULL) return;

    //If the byte is taken erase it
    if (*addr != 0xFF && segment_start != last_erased_segment) {
            erase_flash_segment_auto(addr);
            last_erased_segment = segment_start;
        }

    // 1. Wait if Flash Controller is busy with a previous write/erase
    while (FCTL3 & BUSY);

    // 2. Set Flash timing:
    // FWKEY - Flash write key (must be included to allow access)
    // FSSEL_2 - Use SMCLK as the source clock
    // +3 - Divide SMCLK by 8 for safe Flash write timing
    FCTL2 = FWKEY + FSSEL_2 + 3;

    // 3. Unlock Flash by clearing the lock bit
    FCTL3 = FWKEY;

    // 4. Enable Flash write mode
    FCTL1 = FWKEY + WRT;


    // 5. Write a single byte to the target address in Flash
    *addr = data;

    // 4. Disable Flash write mode
    FCTL1 = FWKEY;

    // 7. Lock the Flash to prevent further unintended modifications
    FCTL3 = FWKEY + LOCK;
}

char* get_flash_segment_start(char* addr) {
    unsigned int address = (unsigned int)addr;

    if (address >= 0x1000 && address <= 0x10BF)
        return (char*)(address & 0xFFC0);  // Info segment
    else if (address >= 0xF600 && address <= 0xFDFF)
        return (char*)(address & 0xFE00);  // Main segment
    else
        return NULL;  // invalid
}



void erase_flash_segment_auto(char *addr) {
    char* segment_start = get_flash_segment_start(addr);
    if (segment_start == NULL)
        return;
    // 1. Wait if Flash is busy
    while (FCTL3 & BUSY);

    // 2. Unlock flash for erasing
    FCTL2 = FWKEY + FSSEL_2 + 3;     // Use SMCLK, divide by 8
    FCTL1 = FWKEY + ERASE;           // Set erase bit
    FCTL3 = FWKEY;                   // Clear LOCK

    // 3. Dummy write to trigger erase
    *segment_start = 0;

    // 4. Lock flash again
    FCTL1 = FWKEY;
    FCTL3 = FWKEY + LOCK;
}

void erase_flash_segment(char *address) {
    FCTL3 = FWKEY;           // Unlock flash memory for writing
    FCTL1 = FWKEY + ERASE;   // Set flash controller to erase mode

    *address = 0;            // Dummy write to trigger the erase operation

    FCTL1 = FWKEY;           // Clear erase mode
    FCTL3 = FWKEY + LOCK;    // Lock flash memory again to prevent accidental writes
}

void erase_all_file_segments() {
    erase_flash_segment((char *)0xFC00);  // Segment 1
    erase_flash_segment((char *)0xFA00);  // Segment 2
    erase_flash_segment((char *)0xF800);  // Segment 3
    erase_flash_segment((char *)0xF600);  // Segment 4
    erase_flash_segment((char *)0x1000);  // Segment D
    erase_flash_segment((char *)0x1040);  // Segment C
    erase_flash_segment((char *)0x1080);  // Segment B
    //script_pointer = (char *)0xF600;
    //struct_pointer = (char *)0x1000;

}

void print_struct_names_from_flash(void) {
    int i = 0;
    for (i; i < num_files; i++) {
        char *name_ptr = struct_addresses[i];  // Pointer to the start of the struct (where 'name' begins)
        int j = 0;
        for (j; j < 12; j++) {
            char ch = name_ptr[j];
            if (ch == '\0') break;
            lcd_data(ch);  // Print one character at a time from flash

        }
        lcd_cmd(0xC0);
    }
}

void script_command_machine(const char* command){
    parse_command(command, opcode, operand);
    execute_command(opcode,operand);

    while (next == 0);
    next = 0;
}

const char * find_content_pointer(const char* struct_pointer){
    unsigned int offset_to_pointer = 18;
    unsigned int addr_low = struct_pointer[offset_to_pointer] & 0xFF;
    unsigned int addr_high = struct_pointer[offset_to_pointer + 1] & 0xFF;
    unsigned int content_address = (addr_high << 8) | addr_low;
    const char *content_ptr = (const char *)content_address;
    return content_ptr;
}

int find_type(const char* struct_pointer){
    unsigned int offset_to_pointer = 14;
    unsigned int type = struct_pointer[offset_to_pointer] & 0xFF;
    return type;
}
int find_size(const char* struct_pointer){
    unsigned int size;
    size = ((unsigned char)struct_pointer[16]) |
           (((unsigned char)struct_pointer[17]) << 8);
    return size;
}

static const char* g_flash_cursor = 0;

void show_text(const char* scipt_ptr, const char** out_next_ptr){
    const char *content_ptr = scipt_ptr;
    int line = 1;
    int col  = 0;
    int cells_written = 0;

    lcd_clear_line(1);
    lcd_clear_line(2);
    lcd_goto(0x00);

    while (cells_written < 32 && *content_ptr) {
        char ch = *content_ptr++;

        if (ch == '\r') {
            continue;
        }
        if (ch == '\n') {
            while (cells_written < 32 && col < 16) {
                lcd_putchar(' ');
                col++;
                cells_written++;
            }
            if (line == 1) {
                line = 2;
                col = 0;
                lcd_goto(0x40);
            }
            continue;
        }

        lcd_putchar(ch);
        col++;
        cells_written++;

        if (col == 16) {
            if (line == 1) {
                line = 2;
                col = 0;
                lcd_goto(0x40);
            }
        }
    }

    while (cells_written < 32) {
        if (line == 1 && col == 0) {
            lcd_goto(0x00);
        } else if (line == 2 && col == 0) {
            lcd_goto(0x40);
        }
        lcd_putchar(' ');
        col++;
        cells_written++;
        if (col == 16 && line == 1) {
            line = 2;
            col = 0;
        }
    }

    // Remember where we stopped so you can continue later
    g_flash_cursor = content_ptr;
    if (out_next_ptr) {
        *out_next_ptr = content_ptr;
    }
}

void lcd_show_page(const char* flash_ptr){
    const char *content_ptr = find_content_pointer(flash_ptr);

    show_text(content_ptr, NULL);
}

void scroll_text_on_button_press(void){
    if (g_flash_cursor && *g_flash_cursor) {
        show_text(g_flash_cursor, NULL);
    }
}


void run_script_mode(const char* struct_pointer){
    int size = find_size(struct_pointer);
    const char *content_ptr = find_content_pointer(struct_pointer);
    set_delay("32");
    char line_buffer[MAX_LINE_LENGTH];
    unsigned int i = 0;
    unsigned int index = 0;
    for (index ; index < size; index++) {
        char ch = *content_ptr++;

        if (ch == 0x0A) {  // Newline character
            line_buffer[i] = '\n';     // Replace with newline
            line_buffer[i+1] = '\0';   // Null-terminate
            script_command_machine(line_buffer);
            i = 0;                     // Reset buffer
        } else {
            if (i < MAX_LINE_LENGTH - 2) {
                line_buffer[i++] = ch;
            } else {
                // Buffer overflow protection: skip line
                i = 0;
            }
        }
    }


    // Handle last line if no newline at the end
    if (i > 0) {
        line_buffer[i] = '\n';
        line_buffer[i+1] = '\0';
        script_command_machine(line_buffer);
    }
}



void display_scrolling_view(void) {
    lcd_cmd(0x80);  // Line 1
    lcd_clear_line(1);

    if (top_index < num_files) {
        char *name1 = struct_addresses[top_index];
        int i = 0;
        for (i; i < 12 && name1[i] != '\0'; i++) {
            lcd_data(name1[i]);
        }
    }

    lcd_cmd(0xC0);  // Line 2
    lcd_clear_line(2);

    if ((top_index + 1) < num_files) {
        char *name2 = struct_addresses[top_index + 1];
        int j = 0;
        for (j; j < 12 && name2[j] != '\0'; j++) {
            lcd_data(name2[j]);
        }

    }
}

void scroll_view_on_button_press(void) {
    if ((top_index + 1) < num_files) {
        top_index++;  // Advance the window by 1
        display_scrolling_view();
    }
}








// Parses a single command string into opcode and operand.
// Expects the format: "OPoperand\n", e.g., "07003C\n"
void parse_command(const char* string, char* opcode, char* operand) {
    int i = 0, j = 0;

    // Extract the opcode (first 2 characters)
    opcode[0] = string[0];
    opcode[1] = string[1];
    opcode[2] = '\0';  // Null-terminate the opcode string

    // Extract the operand (characters from index 2 until '\n' or end of string)
    i = 2;
    while (string[i] != '\n' && string[i] != '\0') {
        operand[j++] = string[i++];
    }
    operand[j] = '\0';  // Null-terminate the operand string
}

void execute_command(const char* opcode, const char* operand) {
    if (strcmp(opcode, "01") == 0) {
        //set_delay();
        //lcd_clear();
        init_count_up(operand);
        Timer_A0_intterupt_enable();
        TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR;
        TA0CCTL1 &= ~OUTMOD_7;
        TA0CCR0 = 1250;
        //inc_lcd();
    } else if (strcmp(opcode, "02") == 0) {
        //set_delay(operand);
        //lcd_clear();
        init_count_down(operand);
        Timer_A0_intterupt_enable();
        TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR;
        TA0CCTL1 &= ~OUTMOD_7;
        lcd_clear();
        TA0CCR0 = 1250;

        //dec_lcd();
        //Timer_A0_intterupt_disable();
    } else if (strcmp(opcode, "03") == 0) {
        //set_delay();
        Timer_A0_intterupt_enable();
        TA0CTL = TASSEL_2 + MC_1 + ID_3 + TACLR;
        TA0CCTL1 &= ~OUTMOD_7;
        TA0CCR0 = 1250;

        //rra_lcd(operand);
    } else if (strcmp(opcode, "04") == 0) {
        set_delay(operand);
        next = 1;
    } else if (strcmp(opcode, "05") == 0) {
        lcd_clear();
        next = 1;
    } else if (strcmp(opcode, "06") == 0) {
        servo_deg(operand);

    } else if (strcmp(opcode, "07") == 0) {
        servo_scan(operand);

    } else if (strcmp(opcode, "08") == 0) {
        Timer_A0_intterupt_disable();
        Timer_A1_intterupt_disable();
        lcd_cursor(0x00);
        lcd_puts("sleep");
        next = 1;
        enterLPM(lpm_mode);
    }
}


void servo_deg(const char* operand){
    int angle = (int)strtol(operand, NULL, 16);
    servo_deg_on = 1;
    servo_scan_on = 0;
    count_servo_deg6 = 0;
    TX_disable();
    TX_active = 0;
    TA0CCR1 =  420 + 9 * angle;
    TA0CCR0 = 21000;
    TA0CCTL1 |= OUTMOD_7;
    TA0CTL &= ~ID_3;
    Timer_A0_intterupt_enable();
    Trigger_on();
    Timer_A1_intterupt_disable();
    Capture_on();
}

void servo_scan(const char* operand){
    servo_deg_on = 0;
    servo_scan_on = 1;

    char begin_str[3];  // 2 digits + null terminator
    char end_str[3];    // 2 digits + null terminator

    // Copy first two characters to begin_str
    begin_str[0] = operand[0];
    begin_str[1] = operand[1];
    begin_str[2] = '\0';  // Null-terminate

    // Copy next two characters to end_str
    end_str[0] = operand[2];
    end_str[1] = operand[3];
    end_str[2] = '\0';  // Null-terminate

    // Convert strings to integers
    begin = (int)strtol(begin_str, NULL, 16);
    angle = begin;
    end = (int)strtol(end_str, NULL, 16);



    TA0CCR0 = 21000;
    TA0CCTL1 |= OUTMOD_7;
    TA0CTL &= ~ID_3;
    Timer_A0_intterupt_enable();
    Timer_A1_intterupt_enable();
    TA0CCR1 = begin*9 + 420;

}


int array_to_num(char arr[]){
    unsigned int max_distance_num;
    if (arr[2] == '\0' && arr[1] == '\0'){
        max_distance_num = arr[0] - 48;
    }
    else if (arr[2] == '\0'){
        max_distance_num = 10*(arr[0] - 48) + arr[1] - 48;
    }
    else{
        max_distance_num = 100*(arr[0] - 48) + 10*(arr[1] - 48) + arr[2] - 48;
    }
    return max_distance_num;

}




void calibration(void){
    if (index < 50){
        ADC10CTL1 = INCH_0;             //choose left LDR       //dvir changed it today
        ADC10CTL0 |= ENC + ADC10SC;
        while (ADC10CTL1 & ADC10BUSY); //wait to the convertion to finish
        unsigned int adc_val_left = ADC10MEM;                   // dvir add this today
        unsigned char result8 = adc_val_left >> 2;              // dvir add this today
        Left_Calib[index] = result8;                             // dvir add this today
        ADC10CTL0 &= ~ENC;
        ADC10CTL1 = INCH_3;              //choose right LDR     //dvir changed it today

        ADC10CTL0 |= ENC + ADC10SC;
        while (ADC10CTL1 & ADC10BUSY); //wait to the convertion to finish
        unsigned int adc_val_right = ADC10MEM;                   // dvir add this today
        unsigned char result8R = adc_val_right >> 2;              // dvir add this today
        Right_Calib[index] = result8R;                            //dvir add this
        ADC10CTL0 &= ~ENC;
        index += 5;
    }
    else{
        Fill_calib_Linearization();
    }
}

void Fill_calib_Linearization(void){
    char i = 0;         //dvir add this today
    char j = 1;        //dvir add this today
    char m = 0;      //dvir add this today
    char b = 0;      //dvir add this today
    int k = 0;
    for (i; i < 10; i++){
        if (i == 9){
            m = m;
            b = b;
        }
        else{
            b = ((char)Right_Calib[(i+1)*5] - (char)Right_Calib[i*5]) / 5;      //dvir add this today
            m = ((char)Left_Calib[(i+1)*5] - (char)Left_Calib[i*5]) / 5;        //dvir add this today
        }
        for (j; j < 5; j++){
            Right_Calib[i*5 + j] = b*j + Right_Calib[i*5];
            Left_Calib[i*5 + j] = m*j + Left_Calib[i*5];
        }
        j = 1;
    }
    for (k; k<50; k++){
        Average_Calib[k] = (Right_Calib[k] + Left_Calib[k])/2;
    }
    TX_enable();
    TX_active = 1;
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = 1;

    calibration_to_pc(Left_Calib);
    calibration_to_pc(Right_Calib);

    TA0CCR1 = 420 ;
    TX_disable();
    TX_active = 0;
}

char Light_detec_left(void){                                           //dvir changed it today

    ADC10CTL1 = INCH_0;             //choose left LDR       //dvir changed it today
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & ADC10BUSY); //wait to the convertion to finish
        return (char)(ADC10MEM >> 2);
}

char Light_detec_right(void){             //dvir changed it today

    ADC10CTL1 = INCH_3;             //choose left LDR       //dvir changed it today
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & ADC10BUSY); //wait to the convertion to finish
        return ADC10MEM;
}                            //dvir changed it today


void calibration_to_pc(char arr[]){                        //dvir add this
    int i ;
for ( i = 0; i < 50; i++) {
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = arr[i];
}
}
