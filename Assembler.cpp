#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef int Elem;
#define LOGS_TO_FILE

#include "Libs/PrintElem.h"
#include "Libs/Logging.h"
#include "Libs/FileWork/FileWork.h"
#include "Errors/Errors.h"
#include "Libs/ComandSystem.h"
#include "Libs/PrintElem.h"

const char PROGRAM[]     = "Program.txt";
const char EXECUTABLE[]  = "a.sy";

const int MAX_COMAND_LENGHT = 100;


int main()
{

    if (OpenLogFile("AssLogs.txt") != 0)
    {
        fprintf(stderr, "Error while logs open\n");
        return -1;
    }

    FILE* program_file = fopen(PROGRAM, "r");
    if (program_file == nullptr)
    {
        LogPrintf("Error while program file open\n");
        return -1;
    }
    LogPrintf("Normal open files\n");

    int   file_size = get_text_size(PROGRAM);
    char* all_text  = (char*) calloc(file_size, sizeof(char));

    if (all_text == nullptr)
    {
        LogPrintf("Error calloc memory to all text\n");
        return -1;
    }

    int lines_number  = gettext(all_text, file_size, program_file);
    LogPrintf("Took program\n");
    const char **text = (const char**)calloc(lines_number, sizeof(char*)); 

    getlines_from_text(text, all_text);
    LogPrintf("Splited by lines\n");

    LogPrintf("Line number = %d\n\n", lines_number);

    int* comands      = (int*)calloc(lines_number * 2 + 1, sizeof(int));
    int  comand_index = 0;
    for(int line = 0; line < lines_number; line++)
    {
        char cmd[MAX_COMAND_LENGHT] = "";

        int number_few_char = 0;
        sscanf(text[line], "%s%n", cmd, &number_few_char);
        LogPrintf("[%d] %s\n", line, text[line]);

        LogPrintf("cmd = <%s>\n", cmd);
        if (stricmp(cmd, "push") == 0)
        {
            LogPrintf("Do push\n");
            int val = 0;
            sscanf(text[line] + number_few_char, "%d", &val);
            comands[comand_index++] = CMD_PUSH;
            comands[comand_index++] = val; 
        }
        else if (stricmp(cmd, "add") == 0)
        {
            LogPrintf("Do add\n");
            comands[comand_index++] = CMD_ADD;
        }
        else if (stricmp(cmd, "div") == 0)
        {
            LogPrintf("Do div\n");
            comands[comand_index++] = CMD_DIV;
        }
        else if (stricmp(cmd, "sub") == 0)
        {
            LogPrintf("Do sub\n");
            comands[comand_index++] = CMD_SUB;
        }
        else if (stricmp(cmd, "mul") == 0)
        {
            LogPrintf("Do mul\n");
            comands[comand_index++] = CMD_MUL;
        }
        else if (stricmp(cmd, "out") == 0)
        {
            LogPrintf("Do out\n");
            comands[comand_index++] = CMD_OUT;
        }
        else if (stricmp(cmd, "hlt") == 0)
        {
            LogPrintf("Do hlt\n");
            comands[comand_index++] = CMD_HLT;
            break;
        }
        else
        {
            printf("Wrong comand\n");
            LogPrintf("Wrong comand in line %d", line);
            break;
        }
    }
    
    Header header        = {};
    header.signature        = SIGNATURE;
    header.version          = ASM_VERSION;
    header.comands_number   = comand_index;
    FILE* executable_file = fopen(EXECUTABLE, "wb");

    fwrite(&header, sizeof(header), 1, executable_file);
    fwrite(comands, sizeof(int), comand_index, executable_file);

    fclose(executable_file);
    free(comands);
    
    CloseLogFile();
}