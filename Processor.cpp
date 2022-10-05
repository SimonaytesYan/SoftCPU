
typedef int Elem;
#define LOGS_TO_FILE

#include "Libs\PrintElem.h"
#include "Libs\Stack.h"
#include "Libs\ComandSystem.h"
#include "Libs\Logging.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        LogPrintf("Wrong number of cmd arguments\n");
        return 0;
    }    

    char* executable_file_name = argv[1];

    FILE* executable_file = fopen(executable_file_name, "rb");

    if (executable_file == nullptr)
    {
        LogPrintf("Error during executable file open");
        return 0;
    }
    Header header = {};
    
    fread(&header, sizeof(header), 1, executable_file);

    if (header.signature != SIGNATURE)
    {
        LogPrintf("File isn`t executable");
        return 0;
    }

    if (header.version != ASM_VERSION)
    {
        LogPrintf("Wrong version of compiler");
        return 0;
    }

    Stack stk = {};
    StackCtor(&stk, 0);

    int* comands = (int*)calloc(header.comands_number, sizeof(int));
    int  pc = 0;

    fread(comands, sizeof(int) ,header.comands_number, executable_file);

    printf("Start comand\n");

    Elem a1 = 0;
    Elem a2 = 0;
    while (pc < header.comands_number)
    {
        switch(comands[pc])
        {
            case CMD_PUSH:
                if (pc < header.comands_number - 1)
                {
                    StackPush(&stk, comands[pc + 1]);
                    pc += 2;
                }
                else
                {
                    LogPrintf("Not enought comands");
                    return 0;
                }
            break;

            case CMD_ADD:
                StackPush(&stk, StackPop(&stk) + StackPop(&stk));
                pc++;
            break;

            case CMD_DIV:
                a1 = StackPop(&stk);
                a2 = StackPop(&stk);
                StackPush(&stk, a2 / a1); 
                pc++;
            break;

            case CMD_MUL:
                StackPush(&stk, StackPop(&stk) * StackPop(&stk));
                pc++;
            break;

            case CMD_SUB:
                a1 = StackPop(&stk);
                a2 = StackPop(&stk);
                StackPush(&stk, a2 - a1); 
                pc++;
            break;

            case CMD_HLT:
                return 0;
            break;

            case CMD_OUT:
                printf("%d\n", StackPop(&stk));
                pc++;
            break;

            default:
                LogPrintf("Wrong comand");
                return 0;
            break;
        }
    }
}