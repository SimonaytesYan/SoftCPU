#include <stdio.h>
#include <stdlib.h>

#include "Errors\Errors.h"

typedef int Elem;
const int    OUTPUT_TYPE = 0;   //!This constant is used to print stack elements to logs in right format
                                //!0 - int
                                //!1 - char
                                //!2 - float
                                //!3 - double
                                //!4 - long long

#define LOGS_TO_FILE
size_t PrintElem(Elem value, FILE *fp)
{
    char format[5] = "%";
    switch (OUTPUT_TYPE)
    {
        case 0:
            format[1] = 'd';
        break;
        case 1:
            format[1] = 'c';
        break;
        case 2:
            format[1] = 'f';
        break;
        case 3:
            format[1] = 'l';
            format[2] = 'g';
        break;
        case 4:
            format[1] = 'l';
            format[2] = 'l';
            format[3] = 'u';
        break;
    }
        
    #ifdef LOGS_TO_CONSOLE
        printf(format, value);
    #endif
    #ifdef LOGS_TO_FILE
        if (fp == nullptr)
            return ERROR_LOGS_OPEN;
        fprintf(fp, format, value);
    #endif

    return NO_ERROR; 
}

#include "Libs/Stack.h"
#include "Libs/FileWork/FileWork.h"

const char program[] = "Program.txt";

enum COMANDS{
    CMD_END  = 0,
    CMD_PUSH = 1,
    CMD_ADD  = 2,
    CMD_SAB  = 3,
    CMD_MUL  = 4,
    CMD_DIV  = 5,
    CMD_OUT  = 6,
    CMD_IN   = 7
};

const int MAX_COMAND_LENGHT = 100;

int main()
{
    Stack stk = {};
    StackCtor(&stk, 0);

    FILE* logs         = fopen(LOGS, "w");
    FILE* program_file = fopen(program, "r");

    if (logs == nullptr)
    {
        printf("Error while logs open\n");
        return 0;
    }
    if (program_file == nullptr)
    {
        printf("Error while program file open\n");
        return 0;
    }  
    printf("Normal open files\n");

    int   file_size = get_text_size(program);
    char* all_text = (char*) calloc(file_size, sizeof(char));

    if (all_text == nullptr)
    {
        LogPrintf(logs, "Error calloc memory to all text\n");
        return 0;
    }

    int lines_number  = gettext(all_text, file_size, program_file);
    printf("Took program\n");
    const char **text          = (const char**)calloc(lines_number, sizeof(char*)); 

    getlines_from_text(text, all_text);
    printf("Splited by lines\n");

    printf("Line number = %d\n", lines_number);
    for(int line = 0; line < lines_number; line++)
    {
        char cmd[MAX_COMAND_LENGHT] = "";

        int number_few_char = 0;
        sscanf(text[line], "%s%n", cmd, &number_few_char);
        printf("[%d] %s\n", line, text[line]);

        printf("cmd = <%s>\n", cmd);
        if (stricmp(cmd, "push") == 0)
        {
            printf("Do push\n");
            int val = 0;
            sscanf(text[line] + number_few_char, "%d", &val);
            StackPush(&stk, val);
        }
        else if (stricmp(cmd, "add") == 0)
        {
            printf("Do add\n");
            StackPush(&stk, StackPop(&stk) + StackPop(&stk)); 
        }
        else if (stricmp(cmd, "div") == 0)
        {
            printf("Do div\n");
            Elem a1 = StackPop(&stk);
            Elem a2 = StackPop(&stk);
            StackPush(&stk, a2 / a1); 
        }
        else if (stricmp(cmd, "sub") == 0)
        {
            printf("Do sub\n");
            Elem a1 = StackPop(&stk);
            Elem a2 = StackPop(&stk);
            StackPush(&stk, a2 - a1);
        }
        else if (stricmp(cmd, "mul") == 0)
        {
            printf("Do mul\n");
            StackPush(&stk, StackPop(&stk) * StackPop(&stk)); 
        }
        else if (stricmp(cmd, "out") == 0)
        {
            printf("Do out\n");
            printf("%d\n", StackPop(&stk));
        }
        else if (stricmp(cmd, "hlt") == 0)
        {
            printf("Do hlt\n");
            break;
        }
        else
        {
            printf("Wrong comand\n");
            fprintf(logs, "Wrong comand in line %d", line);
            break;
        }

        fclose(logs);
        DUMP_STACK(stk);
        logs = fopen(LOGS, "a");
    }
    
    fclose(logs);
}