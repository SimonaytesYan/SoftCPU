#ifndef __CPU_SYM__
#define __CPU_SYM__

#include "Libs\PrintElem.h"
#include "Libs\Stack.h"
#include "Libs\ComandSystem.h"

struct CPU
{
    int*  code           = nullptr;
    int   number_comands = 0; 
    int   pc             = 0;
    Stack stk            = {};
};

int  GetCPUFromFile(CPU* cpu, int comands_number, FILE* executable_file);

int  OpenFileAndCheckHeader(Header *header, FILE** executable_file);

int  GetExecFileFromCMDArgs(FILE** fp, int argc, char* argv[]);

void Run(CPU* cpu);

int ExecProgramFromCL(int argc, char* argv[]);

#endif  //__CPU_SYM__