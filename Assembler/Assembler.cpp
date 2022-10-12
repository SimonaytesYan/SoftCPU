#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int Elem;
#define LOGS_TO_FILE

#include "Assembler.h"
#include "../Libs/Logging/Logging.h"
#include "../Libs/FileWork/FileWork.h"
#include "../Libs/ComandSystem/ComandSystem.h"

#define DEF_CMD(name, num, ...) \
    CMD_##name = num,

enum COMANDS
{
    #include "..\Cmd.h"
};

#undef DEF_CMD

const char EXECUTABLE[]  = "a.sy";

const int MAX_COMAND_LEN = 100;

void DumpLabels(Label* labels)
{
    if (labels == nullptr)
        return;
        
    for(int i = 0; i < MAX_LABELS/10; i++)
    {
        printf("label[%d]\n", i);
        printf("{\n");
        printf("\tcmd_to = %d\n", labels[i].cmd_to);
        printf("\tname   = <%s>\n", labels[i].name);
        printf("}\n");
    }
}

int GetProgramText(const char* program, int* number_lines, const char*** text, char** original_text)
{
    CHECK(number_lines  == nullptr, "&number_lines = nullptr\n", -1);
    CHECK(text          == nullptr, "&text = nullptr\n",         -1);
    CHECK(original_text == nullptr, "original_text = nullptr\n", -1);

    FILE* program_file = fopen(program, "r");
    CHECK(program_file == nullptr, "Error while program text open\n", -1);

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
    CHECK(args == nullptr, "args = nullptr", -1);
    CHECK(ram  == nullptr, "ram = nullptr", -1);

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
            CHECK_SYNTAX(args[i] != ' ' && args[i] != '\t', "Any symbols after closed bracket\n", -1, program_line);

        i++;
    }
    CHECK_SYNTAX(opened - closed != 0, "A square bracket is not closed\n", -1, program_line);

    return 0;
}


//!-----------------
//!@param [out] comand
//!@param [out] arg
//!
//!-----------------
int ArgToInt(char* char_arg, int* comand, int* arg, int line)
{
    CHECK(char_arg == nullptr, "char_arg = nullptr", -1);
    CHECK(comand   == nullptr, "comand = nullptr",   -1);
    CHECK(arg      == nullptr,     "arg = nullptr",   -1);

    if (0 <= char_arg[0] - '0' && char_arg[0] - '0' <= 9)
    {
        CHECK_SYNTAX((*comand & ARG_IMMED) != 0, "Wrong args\n", -1, line);

        *comand |= ARG_IMMED;
        *arg = atoi(char_arg);
    }
    else 
    {
        CHECK_SYNTAX((*comand & ARG_REG) != 0, "Wrong  args\n", -1, line);
        *comand |= ARG_REG;
        
        if (stricmp(char_arg, "rax") == 0)
            *arg = RAX;
        else if (stricmp(char_arg, "rbx") == 0)
            *arg = RBX;
        else if (stricmp(char_arg, "rcx") == 0)
            *arg = RCX;
        else if (stricmp(char_arg, "rdx") == 0)
            *arg = RDX;
        else
            CHECK_SYNTAX(1, "Wrong register name\n", -1, line);
    }

    return 0;
}

int ParseArgs(char* args, int* comands, int* comand, int* arg1, int* arg2, int line)
{
    CHECK(arg1    == nullptr, "arg1 = nullptr",    -1);
    CHECK(arg2    == nullptr, "arg2 = nullptr",    -1);
    CHECK(args    == nullptr, "args = nullptr",    -1);
    CHECK(comands == nullptr, "comands = nullptr", -1);
    CHECK(comand  == nullptr, "arg1 = nullptr",    -1);

    int  start_len = strlen(args);
    bool ram       = false;
    if (CheckSquereBracket(args, line, &ram) != 0)
        return -1;
    if (ram)
        *comand |= ARG_MEM;

    for(int i = 0; i < start_len; i++)
        if (args[i] == '[' || args[i] == ']' || args[i] == '+')
            args[i] = ' ';

    char arg_1[MAX_COMAND_LEN] = {};
    char arg_2[MAX_COMAND_LEN] = {};
    int  n = 0;
    sscanf(args, "%s%n", arg_1, &n);
    sscanf(args + n, "%s", arg_2);
    
    ArgToInt(arg_1, comand, arg1, line);
    if (strlen(arg_2) != 0)
    {
        if ((*comand & ARG_REG) != 0)
        {
            *arg2 = *arg1;
            ArgToInt(arg_2, comand, arg1, line);
        }
        else
            ArgToInt(arg_2, comand, arg2, line);
    }
    if (ram)
    {
        args[1]             = '[';
        args[start_len - 1] = ']';
    }

    if ((*comand & CMD_MASK) == CMD_POP)
        CHECK_SYNTAX(((*comand & ARG_MEM) == 0) && ((*comand & ARG_IMMED) != 0), "Wrong pop args", -1, line);

    return 0;
}

int PutArgsAndCmdInArray(const char* args, int* comands, int* comand_index, int comand_type, int line)
{
    CHECK(args     == nullptr, "args = nullptr",    -1);
    CHECK(comands  == nullptr, "comands = nullptr", -1);

    int arg1   = -1;
    int arg2   = -1;
    int comand = comand_type;
    if (ParseArgs((char*)args, comands, &comand, &arg1, &arg2, line + 1) != 0)
        return -1;

    comands[(*comand_index)++] = comand;
    if (arg1 != -1)
        comands[(*comand_index)++] = arg1;
    if (arg2 != -1)
        comands[(*comand_index)++] = arg2;

    return 0;
}

int FindLabel(Label* labels, char* name, int* index)
{
    for(int i = 0; i < MAX_LABELS; i++)
    {
        if (stricmp(name, labels[i].name) == 0)
        {
            *index = i; 
            return 0;
        }
    }

    return -1;
}

int AddLabel(char* arg, Label* labels, int cmd_index, int line)
{
    int length = strlen(arg);
    arg[length - 1] = '\0';
    length--;

    CHECK_SYNTAX(length > MAX_LABEL_LEN || length <= 0, "Label length is to big\n", -1, line);

    int i = 0;
    for(i = 0; i < MAX_LABELS; i++)
    {
        if (stricmp(arg, labels[i].name) == 0)
            return 0;
        if (strlen(labels[i].name) == 0)
            break;
    }
    CHECK_SYNTAX(i >= MAX_LABELS, "Number of labels bigger than allowed\n", -1, line);
    
    labels[i].cmd_to = cmd_index;
    strcpy(labels[i].name, arg);
}

int GetArgsForJmp(const char* text, int* arg, Label* labels, int line)
{
    char char_arg[MAX_LABEL_LEN] = "";
    sscanf(text, "%s", char_arg);
    
    int index = 0;
    CHECK_SYNTAX(FindLabel(labels, char_arg, &index) != 0, "Found undeclared label\n", -1, line);
    
    *arg = labels[index].cmd_to;

    return 0;
}

#define DEF_CMD(name, num, arg, ...)                                                            \
    if (stricmp(cmd, #name) == 0)                                                               \
    {                                                                                           \
        if (arg == 1)                                                                           \
        {                                                                                       \
            const char* args = text[line] + number_few_char;                                    \
            if (PutArgsAndCmdInArray(args, *comands, &comand_index, CMD_##name, line) != 0)     \
                return -1;                                                                      \
        }                                                                                       \
        else if (arg == 2)                                                                      \
        {                                                                                       \
            (*comands)[comand_index++] = num;                                                   \
                                                                                                \
            const char* args = text[line] + number_few_char;                                    \
            int         argum  = 0;                                                             \
                                                                                                \
            if (number == 2)                                                                    \
            {                                                                                   \
                if (GetArgsForJmp(args, &argum, labels, line + 1) != 0)                         \
                    return -1;                                                                  \
            }                                                                                   \
                                                                                                \
            (*comands)[comand_index++] = argum;                                                 \
        }                                                                                       \
        else                                                                                    \
            (*comands)[comand_index++] = num;                                                   \
    } else

int Compilation(int** comands, int* number_comand, Label* labels, int number_lines, const char** text, int number)
{
    CHECK(number_comand == nullptr, "number_comand = nullptr\n", -1);
    CHECK(comands       == nullptr, "comands = nullptr\n",       -1);

    *comands = (int*)calloc(number_lines * 3 + 1, sizeof(int));

    CHECK(*comands == nullptr, "Error during allocation memory for comands array\n", -1);

    int comand_index = 0;
    for(int line = 0; line < number_lines; line++)
    {
        char cmd[MAX_COMAND_LEN] = "";
        int number_few_char = 0;
        sscanf(text[line], "%s%n", cmd, &number_few_char);

        if (strlen(cmd) == 0)
            continue;

        #include "..\Cmd.h"
        /*else*/
        {
            if (cmd[strlen(cmd) - 1] == ':')
                AddLabel(cmd, labels, comand_index, line + 1);
            else
            {
                LogPrintf("Wrong comand in line %d\n", line + 1);
                return -1;
            }
        }
    }

    *number_comand = comand_index;

    return 0;
}

#undef DEF_CMD

int PutProgramToFile(Header* header, int* comands)
{
    CHECK(header == nullptr, "Header == nullptr\n", -1);

    FILE* executable_file = fopen(EXECUTABLE, "wb");

    CHECK(executable_file == nullptr, "Error during open file for write binary program to\n", -1);

    fwrite(header, sizeof(*header), 1, executable_file);
    fwrite(comands, sizeof(int), header->comands_number, executable_file);

    fclose(executable_file);
}

int GetProgramCompileAndPutInFile(const char* program_file)
{
    CHECK(OpenLogFile("Assembler/AssLogs.txt") != 0, "Error while logs open\n", -1);

    int          number_lines   = 0;
    const char** text           = nullptr;
    char*        original_text  = nullptr; 
    
    CHECK(GetProgramText(program_file, &number_lines, &text, &original_text) != 0, "Error during read text program\n", -1);

    int* comands        = nullptr;
    int  number_comands = 0;

    Label labels[MAX_LABELS] = {};

    Compilation(&comands, &number_comands, labels, number_lines, text, 1);

    comands        = nullptr;
    number_comands = 0;
    Compilation(&comands, &number_comands, labels, number_lines, text, 2);
    
    Header header = {};
    InitHeader(&header, number_comands);
    
    PutProgramToFile(&header, comands);

    free(comands);
    CloseLogFile();
}

int main()
{
    return GetProgramCompileAndPutInFile("Program.txt");
}