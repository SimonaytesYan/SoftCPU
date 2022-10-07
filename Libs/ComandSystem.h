#ifndef __COMAND_SYSTEM_SYM__
#define __COMAND_SYSTEM_SYM__

enum COMANDS{
    CMD_HLT  = 0,
    CMD_PUSH = 1,
    CMD_ADD  = 2,
    CMD_SUB  = 3,
    CMD_MUL  = 4,
    CMD_DIV  = 5,
    CMD_OUT  = 6,
    CMD_IN   = 7,
};

struct Header
{
    int signature      = -1;
    int version        = -1;
    int comands_number = -1; 
};

const int SIGNATURE   = 'S' * 256 + 'Y';
const int ASM_VERSION = 0;



#endif //__COMAND_SYSTEM_SYM__