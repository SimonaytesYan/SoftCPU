#include <stdio.h>
#include <stdlib.h>

#define LOGS_TO_FILE

#include "Libs\ComandSystem.h"
#include "Libs\Logging.h"

const char original_program[] = "DisAssebledProgram.txt";
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

    int* comands = (int*)calloc(header.comands_number, sizeof(int));
    int  pc = 0;

    fread(comands, sizeof(int) ,header.comands_number, executable_file);

    printf("Start comand\n");

    FILE* original_text_fp = fopen(original_program, "w");

    while (pc < header.comands_number)
    {
        switch(comands[pc])
        {
            case CMD_PUSH:
                if (pc < header.comands_number - 1)
                {
                    fprintf(original_text_fp, "push %d\n", comands[pc + 1]);
                    pc += 2;
                }
                else
                {
                    LogPrintf("Not enought comands");
                    return 0;
                }
            break;

            case CMD_ADD:
                fprintf(original_text_fp, "add\n");
                pc++;
            break;

            case CMD_DIV:
                fprintf(original_text_fp, "div\n");
                pc++;
            break;

            case CMD_MUL:
                fprintf(original_text_fp, "mul\n");
                pc++;
            break;

            case CMD_SUB:
                fprintf(original_text_fp, "sub\n");
                pc++;
            break;

            case CMD_HLT:
                fprintf(original_text_fp, "hlt\n");
                pc++;
            break;

            case CMD_OUT:
                fprintf(original_text_fp, "out\n");
                pc++;
            break;

            default:
                LogPrintf("Wrong comand");
                return 0;
            break;
        }
    }
}