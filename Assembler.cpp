#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef int Elem;
#define LOGS_TO_FILE

#include "Assembler.h"
#include "Libs/Logging.h"
#include "Libs/FileWork/FileWork.h"
#include "Libs/ComandSystem.h"

const char EXECUTABLE[]  = "a.sy";

const int MAX_COMAND_LENGHT = 100;

int GetProgramText(const char* program, int* number_lines, const char*** text, char** original_text)
{
    CHECK(number_lines  == nullptr, "&number_lines = nullptr\n", -1);
    CHECK(text          == nullptr, "&text = nullptr\n",         -1);
    CHECK(original_text == nullptr, "original_text = nullptr\n", -1);

    FILE* program_file = fopen(program, "r");
    CHECK(program_file == nullptr, "Error while logs open\n", -1);

    int   file_size = get_text_size(program);
    *original_text  = (char*) calloc(file_size + 1, sizeof(char));
    CHECK(*original_text == nullptr, "Error calloc memory to all text\n", -1); 

    *number_lines  = gettext(*original_text, file_size, program_file);

    *text = (const char**)calloc(*number_lines, sizeof(char*)); 

    getlines_from_text(*text, *original_text);

    return 0;
}

int ParseArgs(const char* args, int** comands, int* comand_index)
{
    char arg[MAX_COMAND_LENGHT] = {};
    sscanf(args, "%s", arg);

    int comand = CMD_PUSH;
    int val    = 0;

    if (0 <= arg[0] - '0' && arg[0] - '0' <= 9)
    {
        comand |= ARG_IMMED;
        val     = atoi(arg);
    }
    else if (stricmp(arg, "rax") == 0)
    {
        comand |= ARG_REG;
        val     = RAX;
    }
    else if (stricmp(arg, "rbx") == 0)
    {
        comand |= ARG_REG;
        val     = RBX;
    }
    else if (stricmp(arg, "rcx") == 0)
    {
        comand |= ARG_REG;
        val     = RCX;
    }
    else if (stricmp(arg, "rdx") == 0)
    {
        comand |= ARG_REG;
        val     = RDX;
    }
    else
    {
        CHECK(1, "Wrong register name\n", -1);
    }
    
    (*comands)[(*comand_index)++] = comand;
    (*comands)[(*comand_index)++] = val;

    return 0;
}

int Compilation(int** comands, int* number_comand, int number_lines, const char** text)
{
    CHECK(number_comand == nullptr, "number_comand = nullptr\n", -1);
    CHECK(comands       == nullptr, "comands = nullptr\n",       -1);

    *comands      = (int*)calloc(number_lines * 3 + 1, sizeof(int));

    CHECK(*comands == nullptr, "Error during allocation memory for comands array\n", -1);

    int comand_index = 0;
    for(int line = 0; line < number_lines; line++)
    {
        char cmd[MAX_COMAND_LENGHT] = "";

        int number_few_char = 0;
        sscanf(text[line], "%s%n", cmd, &number_few_char);

        if (stricmp(cmd, "push") == 0)
        {
            const char* args = text[line] + number_few_char; 
            CHECK(ParseArgs(args, comands, &comand_index) != 0, "Wrong push arg", -1);
        }
        else if(stricmp(cmd, "pop") == 0)
        {
            //....
            //....
        }
        else if (stricmp(cmd, "add") == 0)
        {
            (*comands)[comand_index++] = CMD_ADD;
        }
        else if (stricmp(cmd, "div") == 0)
        {
            (*comands)[comand_index++] = CMD_DIV;
        }
        else if (stricmp(cmd, "sub") == 0)
        {
            (*comands)[comand_index++] = CMD_SUB;
        }
        else if (stricmp(cmd, "mul") == 0)
        {
            (*comands)[comand_index++] = CMD_MUL;
        }
        else if (stricmp(cmd, "out") == 0)
        {
            (*comands)[comand_index++] = CMD_OUT;
        }
        else if (stricmp(cmd, "hlt") == 0)
        {
            (*comands)[comand_index++] = CMD_HLT;
            break;
        }
        else
        {
            fprintf(stderr, "Wrong comand in line %d\n", line + 1);
            LogPrintf("Wrong comand in line %d\n", line + 1);
            return -1;
        }
    }

    *number_comand = comand_index;
    
    return 0;
}

int PutProgramToFile(Header* header, int* comands)
{
    CHECK(header == nullptr, "Header == nullptr\n", -1);

    FILE* executable_file = fopen(EXECUTABLE, "wb");

    CHECK(executable_file == nullptr, "Error during open file for write binary program to\n", -1);

    fwrite(header, sizeof(*header), 1, executable_file);
    fwrite(comands, sizeof(int), header->comands_number, executable_file);

    fclose(executable_file);
}

int GetProgramAndCompile(const char* program_file)
{
    CHECK(OpenLogFile("AssLogs.txt") != 0, "Error while logs open\n", -1);

    int          number_lines   = 0;
    const char **text           = nullptr;
    char*        original_text  = nullptr; 
    
    CHECK(GetProgramText(program_file, &number_lines, &text, &original_text) != 0, "Error during read text program\n", -1);

    int* comands        = nullptr;
    int  number_comands = 0;

    Compilation(&comands, &number_comands, number_lines, text);
    
    Header header = {};
    InitHeader(&header, number_comands);
    
    PutProgramToFile(&header, comands);

    free(comands);
    CloseLogFile();
}

int main()
{
    GetProgramAndCompile("Program.txt");
}