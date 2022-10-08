#include <stdio.h>
#include <stdlib.h>

#define LOGS_TO_FILE

#include "Processor.h"

const char original_program[] = "DisAssebledProgram.txt";

int RecoverProgram(CPU* cpu)
{
    FILE* original_text_fp = fopen(original_program, "w");

    while (cpu->pc < cpu->number_comands)
    {
        int cmd = cpu->code[cpu->pc++];
        int arg = 0;

        switch(cmd & CMD_MASK)
        {
            case CMD_PUSH:
                fprintf(original_text_fp, "push");
                if ((cmd & ARG_IMMED) != 0)
                {
                    fprintf(original_text_fp, " %d", cpu->code[cpu->pc++]);
                }
                
                if ((cmd & ARG_REG) != 0)
                {
                    const char* reg = "";
                    switch (cpu->code[cpu->pc++])
                    {
                    case RAX:
                        reg = "rax";
                    break;
                    
                    case RBX:
                        reg = "rbx";
                    break;
                    
                    case RCX:
                        reg = "rcx";
                    break;
                    
                    case RDX:
                        reg = "rdx";
                    break;
                    default:
                        break;
                    }

                    fprintf(original_text_fp, " %s", cpu->code[cpu->pc++], reg);
                }
                
                fprintf(original_text_fp, "\n");
            break;

            case CMD_POP:

            break;

            case CMD_ADD:
                fprintf(original_text_fp, "add\n");
            break;

            case CMD_SUB:
                fprintf(original_text_fp, "sub\n");
            break;
            
            case CMD_MUL:
                fprintf(original_text_fp, "mul\n");
            break;
            
            case CMD_DIV:
                fprintf(original_text_fp, "div\n");
            break;
            
            case CMD_HLT:
                fprintf(original_text_fp, "hlt\n");
            break;

            case CMD_OUT:
                fprintf(original_text_fp, "out\n");
            break;

            default:
                LogPrintf("\nWrong comand\n");
            break;
        }
    }

    fclose(original_text_fp);
}

int main(int argc, char* argv[])
{
    FILE *fp = nullptr;
    GetExecFileFromCLArgs(&fp, argc, argv);

    Header header = {};
    CheckHeaderFromFile(&header, &fp);

    CPU cpu           = {};

    GetCPUFromFile(&cpu, header.comands_number, fp);
    
    if (RecoverProgram(&cpu) != 0)
        return -1;
}