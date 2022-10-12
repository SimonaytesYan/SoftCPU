#ifndef __COMAND_SYSTEM_SYM__
#define __COMAND_SYSTEM_SYM__

const int REG_N = 4;

enum REGISTERS
{
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4, 
};

enum CMD_MASKS
{
    CMD_MASK  = 0xFF,
    ARG_IMMED = 0x100,
    ARG_REG   = 0x200,
    ARG_MEM   = 0x400
};

const int SIGNATURE   = 'S' * 256 + 'Y';
const int ASM_VERSION = 2;

struct Header
{
    int signature      = -1;
    int version        = -1;
    int comands_number = -1;
};

int InitHeader(Header* header, int comands_number);

#endif //__COMAND_SYSTEM_SYM__