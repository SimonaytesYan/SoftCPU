#include "Processor.h"

int GetCPUFromFile(CPU* cpu, int comands_number, FILE* executable_file)
{
    cpu->stk = {};
    StackCtor(&(cpu->stk), 0);

    cpu->code = (int*)calloc(comands_number, sizeof(int));

    CHECK(cpu->code == nullptr, "\nError during allocation memory for comands array\n", -1);

    fread(cpu->code, sizeof(int) ,comands_number, executable_file);
    
    cpu->number_comands = comands_number;
    cpu->pc             = 0;
    
    return 0;
}

int CheckHeaderFromFile(Header *header, FILE* executable_file)
{
    CHECK(executable_file == nullptr, "\nExecutable file = nullptr\n", -1); 

    fread(header, sizeof(*header), 1, executable_file);

    CHECK(header->signature != SIGNATURE,   "\nFile isn`t executable\n",     -1); 
    CHECK(header->version   != ASM_VERSION, "\nWrong version of compiler\n", -1);
    return 0;
}

int GetExecFileFromCLArgs(FILE** fp, int argc, char* argv[])
{
    CHECK(fp   == nullptr, "\nFp damaged\n",                    -1);
    CHECK(argv == nullptr, "\nArgv damaged\n",                  -1);
    CHECK(argc != 2,       "\nWrong number of cmd arguments\n", -1);

    char* executable_file_name = argv[1];
    
    *fp = fopen(executable_file_name, "rb");
    CHECK(*fp == nullptr, "\nError during executable file open", -1);   

    return 0;
}

#define CaseCMD(CMD, oper)                       \
    case CMD:                                    \
        a1 = StackPop(&(cpu->stk));                \
        a2 = StackPop(&(cpu->stk));                \
        StackPush(&(cpu->stk), a2 oper a1);        \
    break;

void Run(CPU* cpu)
{
    Elem a1 = 0;
    Elem a2 = 0;
    int regs[REG_N] = {};

    while (cpu->pc < cpu->number_comands)
    {
        int cmd = cpu->code[cpu->pc++];
        int arg = 0;

        switch(cmd & CMD_MASK)
        {
            case CMD_PUSH:
                if ((cmd & ARG_IMMED) != 0)
                {
                    arg += cpu->code[cpu->pc++];
                }

                if ((cmd & ARG_REG) != 0)
                {
                    arg += regs[cpu->code[cpu->pc++]];
                    printf("Arg = %d\n", arg);
                }

                if (cpu->pc < cpu->number_comands - 1)
                {
                    StackPush(&cpu->stk, arg);
                }
                else
                {
                    LogPrintf("\nWrong arguments in push\n");
                    return;
                }
            break;

            case CMD_POP:

            break;

            CaseCMD(CMD_ADD, +);
            CaseCMD(CMD_SUB, -);
            CaseCMD(CMD_MUL, *);
            CaseCMD(CMD_DIV, /);
            
            case CMD_HLT:
                return;
            break;

            case CMD_OUT:
                printf("%d\n", StackPop(&cpu->stk));
            break;

            default:
                LogPrintf("\nWrong comand\n");
            break;
        }
    }
}
#undef CaseCMD

int ExecProgramFromCL(int argc, char* argv[])
{
    OpenLogFile("CPULogs.txt");

    FILE* executable_file = nullptr;
    if (GetExecFileFromCLArgs(&executable_file, argc, argv) != 0)
        return -1;

    Header header = {};
    if (CheckHeaderFromFile(&header, executable_file) != 0)
        return -1;

    CPU cpu = {};
    if (GetCPUFromFile(&cpu, header.comands_number, executable_file) != 0)
        return -1;

    Run(&cpu);

    CloseLogFile();
}

int main(int argc, char* argv[])
{
    CHECK(ExecProgramFromCL(argc, argv) != 0, "", -1);
    system("Pause");
}