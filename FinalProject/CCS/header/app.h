#ifndef _app_H_
#define _app_H_


enum FSMstate{state0,state1,state2,state3,state4,state5,state6,state7}; // global variable
enum SYSmode{mode0,mode1,mode2,mode3,mode4}; // global variable



typedef enum {
    TEXT_FILE = 0,
    SCRIPT_FILE = 1
} FileType;

typedef struct {
    char name[13];
    FileType type;
    unsigned int size;
    char *flash_address;
} FileEntry;


#endif







