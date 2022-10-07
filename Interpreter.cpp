#include <stdio.h>
#include <stdlib.h>

#include "Errors\Errors.h"

typedef int Elem;

#include "Libs/PrintElem.h"

#include "Libs/Stack.h"
#include "Libs/FileWork/FileWork.h"

const char program[] = "Program.txt";

const int MAX_COMAND_LENGHT = 100;

int main()
{
    Stack stk = {};
    StackCtor(&stk, 0);

    CHECK(OpenLogFile("InterprLogs.txt"), "Error during open logfile", -1);

    FILE* program_file = fopen(program, "r");

    if (program_file == nullptr)
    {
        printf("Error while program file open\n");
        return 0;
    }  
    LogPrintf("Normal open files\n");

    int   file_size = get_text_size(program);
    char* all_text = (char*) calloc(file_size, sizeof(char));

    if (all_text == nullptr)
    {
        LogPrintf("Error calloc memory to all text\n");
        return 0;
    }

    int lines_number  = gettext(all_text, file_size, program_file);
    LogPrintf("Took program\n");
    const char **text          = (const char**)calloc(lines_number, sizeof(char*)); 

    getlines_from_text(text, all_text);
    LogPrintf("Splited by lines\n");

    LogPrintf("Line number = %d\n\n", lines_number);
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
            StackPush(&stk, val);
        }
        else if (stricmp(cmd, "add") == 0)
        {
            LogPrintf("Do add\n");
            StackPush(&stk, StackPop(&stk) + StackPop(&stk)); 
        }
        else if (stricmp(cmd, "div") == 0)
        {
            LogPrintf("Do div\n");
            Elem a1 = StackPop(&stk);
            Elem a2 = StackPop(&stk);
            StackPush(&stk, a2 / a1); 
        }
        else if (stricmp(cmd, "sub") == 0)
        {
            LogPrintf("Do sub\n");
            Elem a1 = StackPop(&stk);
            Elem a2 = StackPop(&stk);
            StackPush(&stk, a2 - a1);
        }
        else if (stricmp(cmd, "mul") == 0)
        {
            LogPrintf("Do mul\n");
            StackPush(&stk, StackPop(&stk) * StackPop(&stk)); 
        }
        else if (stricmp(cmd, "out") == 0)
        {
            LogPrintf("Do out\n");
            printf("%d\n", StackPop(&stk));
        }
        else if (stricmp(cmd, "hlt") == 0)
        {
            LogPrintf("Do hlt\n");
            break;
        }
        else
        {
            printf("Wrong comand\n");
            LogPrintf("Wrong comand in line %d", line);
            break;
        }

        DUMP_STACK(stk);
    }
    
    CloseLogFile();
}