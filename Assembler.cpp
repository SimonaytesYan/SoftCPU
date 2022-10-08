#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef int Elem;
#define LOGS_TO_FILE

#include "Assembler.h"
#include "Libs/Logging/Logging.h"
#include "Libs/FileWork/FileWork.h"
#include "Libs/ComandSystem/ComandSystem.h"

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

int CheckSquereBracket(const char* args, int program_line, bool* ram)
{
    int  opened = 0;
    int  closed = 0; 
    int  i = 0;
    
    while(args[i] != '\0')
    {
        if (args[i] == '[')
        {
            opened++;
            *ram = true;
            CHECK_SYNTAX(opened > 1, "More than one opened breaket in line\n", -1, program_line);
        }
        else if (args[i] == ']')
        {
            closed++;
            CHECK_SYNTAX(closed > 1, "More than one closed breaket in line\n", -1, program_line);
            CHECK_SYNTAX(opened == 0, "Closed breaket before opened\n",        -1, program_line);
        }
        else if (closed == 1)
        {
            CHECK_SYNTAX(args[i] != ' ' && args[i] != '\t', "Any symbols after closed bracket\n", -1, program_line);
        }

        i++;
    }
    CHECK_SYNTAX(opened - closed != 0, "A square bracket is not closed\n", -1, program_line);

    return 0;
}

int ParseArgs(const char* args, int* comands, int comand_index, int* comand, int* arg1, int* arg2, int line)
{
    CHECK(arg1    == nullptr, "arg1 = nullptr",    -1);
    CHECK(arg2    == nullptr, "arg2 = nullptr",    -1);
    CHECK(args    == nullptr, "args = nullptr",    -1);
    CHECK(comands == nullptr, "comands = nullptr", -1);
    CHECK(comand  == nullptr, "arg1 = nullptr",    -1);

    *comand = 0;
    bool ram = false;
    if (CheckSquereBracket(args, line, &ram) != 0)
        return -1;
    if (ram)
        *comand |= ARG_MEM;
    
    char separators[] = " \t+[]";
    char* arg_1 = strtok((char*)args, separators);
    char* arg_2 = strtok(nullptr, separators);

    CHECK_SYNTAX((arg_1 == nullptr), "Wrong push args. arg_1 = nullptr\n", -1, line);
    
    printf("arg1 = <%s>\n", arg_1);
    if (0 <= arg_1[0] - '0' && arg_1[0] - '0' <= 9)
    {
        *comand |= ARG_IMMED;
        *arg1    = atoi(arg_1);
    }
    else if (stricmp(arg_1, "rax") == 0)
    {
        *comand |= ARG_REG;
        *arg1    = RAX;
    }
    else if (stricmp(arg_1, "rbx") == 0)
    {
        *comand |= ARG_REG;
        *arg1    = RBX;
    }
    else if (stricmp(arg_1, "rcx") == 0)
    {
        *comand |= ARG_REG;
        *arg1    = RCX;
    }
    else if (stricmp(arg_1, "rdx") == 0)
    {
        *comand |= ARG_REG;
        *arg1    = RDX;
    }
    else
    {
        CHECK_SYNTAX(1, "Wrong register name\n", -1, line);
    }

    if (arg_2 != nullptr)
    {
        if (0 <= arg_2[0] - '0' && arg_2[0] - '0' <= 9)
        {
            CHECK_SYNTAX((*comand & ARG_IMMED) != 0, "Wrong args\n", -1, line);

            *comand |= ARG_IMMED;
            *arg2    = atoi(arg_2);

            int temp = *arg1;
            *arg1 = *arg2;
            *arg2 = temp;
        }
        else 
        {
            CHECK_SYNTAX((*comand & ARG_REG) != 0, "Wrong  args\n", -1, line);

            if (stricmp(arg_2, "rax") == 0)
            {
                *comand |= ARG_REG;
                *arg2    = RAX;
            }
            else if (stricmp(arg_2, "rbx") == 0)
            {
                *comand |= ARG_REG;
                *arg2    = RBX;
            }
            else if (stricmp(arg_2, "rcx") == 0)
            {
                *comand |= ARG_REG;
                *arg2    = RCX;
            }
            else if (stricmp(arg_2, "rdx") == 0)
            {
                *comand |= ARG_REG;
                *arg2    = RDX;
            }
            else
            {
                CHECK_SYNTAX(1, "Wrong register name\n", -1, line);
            }
        }
    }

    return 0;
}

int GetArgsForPop(const char* args, int* comands, int comand_index, int* comand, int* arg1, int* arg2, int line)
{
    if(ParseArgs(args, comands, comand_index, comand, arg1, arg2, line) != 0)
        return -1;
    
    CHECK_SYNTAX(((*comand & ARG_MEM) == 0) && ((*comand & ARG_IMMED) != 0), "Wrong pop args", -1, line);        

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
            int arg1   = -1;
            int arg2   = -1;
            int comand = 0;
            CHECK_SYNTAX(ParseArgs(args, *comands, comand_index, &comand, &arg1, &arg2, line + 1) != 0, "Wrong push arg", -1, line + 1);

            (*comands)[comand_index++] = comand | CMD_PUSH;
            if (arg1 != -1)
                (*comands)[comand_index++] = arg1;
            if (arg2 != -1)
                (*comands)[comand_index++] = arg2;
        }
        else if(stricmp(cmd, "pop") == 0)
        { 
            int arg1   = -1;
            int arg2   = -1;
            int comand = 0;
            const char* args = text[line] + number_few_char; 
            CHECK_SYNTAX(GetArgsForPop(args, *comands, comand_index, &comand, &arg1, &arg2, line + 1) != 0, "Wrong pop arg", -1, line + 1);  
            (*comands)[comand_index++] = comand | CMD_POP;
            if (arg1 != -1)
                (*comands)[comand_index++] = arg1;
            if (arg2 != -1)
                (*comands)[comand_index++] = arg2;        
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