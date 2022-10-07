
typedef int Elem;
#define LOGS_TO_FILE

#include "Libs\PrintElem.h"
#include "Libs\Logging.h"
#include "Libs\Stack.h"
#include "Libs\ComandSystem.h"

struct CPU
{
    int*  code           = nullptr;
    int   number_comands = 0; 
    int   pc             = 0;
    Stack stk            = {};
};

const int CMD_MASK = 0xFF;

int GetCPUFromFile(CPU* cpu, int comands_number, FILE* executable_file)
{
    cpu->stk = {};
    StackCtor(&(cpu->stk), 0);

    cpu->code = (int*)calloc(comands_number, sizeof(int));
    if (cpu->code == nullptr)
    {
        LogPrintf("\nError during allocation memory for comands array\n");
        return 1;
    }
    fread(cpu->code, sizeof(int) ,comands_number, executable_file);
    
    cpu->number_comands = comands_number;
    cpu->pc             = 0;
    
    return 0;
}

int OpenFileAndCheckHeader(Header *header, FILE** executable_file)
{
    if (executable_file == nullptr)
    {
        LogPrintf("\nDamaged executable file\n");
        return 1;
    }

    if (executable_file == nullptr)
    {
        LogPrintf("\nExecutable file = nullptr\n");
        return 1;
    }

    fread(header, sizeof(*header), 1, *executable_file);

    if (header->signature != SIGNATURE)
    {
        LogPrintf("\nFile isn`t executable\n");
        return 1;
    }

    if (header->version != ASM_VERSION)
    {
        LogPrintf("\nWrong version of compiler\n");
        return 1;
    }

    return 0;
}

int GetExecutableFilePointerFromCMDArgs(FILE** fp, int argc, char** argv[])
{
    if (fp == nullptr)
    {
        LogPrintf("\nFp damaged\n");
        return 1;
    }

    if (argv == nullptr)
    {
        LogPrintf("\nArgv damaged\n");
        return 1;
    }

    if (*argv == nullptr)
    {
        LogPrintf("\nargv = nullptr\n");
        return 1;
    }
    
    if (argc != 2)
    {
        LogPrintf("\nWrong number of cmd arguments\n");
        return 1;
    }

    char* executable_file_name = (*argv)[1];

    *fp = fopen(executable_file_name, "rb");
    if (*fp == nullptr)
    {
        LogPrintf("\nError during executable file open");
        return 1;
    }

    return 0;
}

void Run(CPU* cpu)
{
    Elem a1 = 0;
    Elem a2 = 0;
    while (cpu->pc < cpu->number_comands)
    {
        int cmd = cpu->code[cpu->pc++] & CMD_MASK;
        //printf("cmd = %d\n", cmd);
        switch(cmd)
        {
            case CMD_PUSH:
                //printf("Push\n");
                if (cpu->pc < cpu->number_comands - 1)
                {
                    StackPush(&cpu->stk, cpu->code[cpu->pc++]);
                }
                else
                {
                    LogPrintf("\nWrong arguments in push\n");
                    return;
                }
            break;

            case CMD_ADD:
                StackPush(&cpu->stk, StackPop(&cpu->stk) + StackPop(&cpu->stk));
            break;

            case CMD_DIV:
                a1 = StackPop(&cpu->stk);
                a2 = StackPop(&cpu->stk);
                StackPush(&cpu->stk, a2 / a1);
            break;

            case CMD_MUL:
                StackPush(&cpu->stk, StackPop(&cpu->stk) * StackPop(&cpu->stk));
            break;

            case CMD_SUB:
                a1 = StackPop(&cpu->stk);
                a2 = StackPop(&cpu->stk);
                StackPush(&cpu->stk, a2 - a1); 
            break;

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

int main(int argc, char* argv[])
{
    FILE* executable_file = nullptr;
    if (GetExecutableFilePointerFromCMDArgs(&executable_file, argc, &argv) != 0)
        return 0;

    Header header = {};
    if (OpenFileAndCheckHeader(&header, &executable_file) != 0)
        return 0;

    CPU cpu = {};
    if (GetCPUFromFile(&cpu, header.comands_number, executable_file) != 0)
        return 0;

    Run(&cpu);
    int k = 0;
    system("Pause");
}