#ifndef __ASSEMBLER_SYM__
#define __ASSEMBLER_SYM__

#include "..\Libs\ComandSystem\ComandSystem.h"

const int MAX_LABELS    = 64;
const int MAX_LABEL_LEN = 20;

struct Label 
{
    char name[MAX_LABEL_LEN] = "";
    int  cmd_to              = -1;
};

void DumpLabels(Label* labels);

//!-----------------
//!@param [out] index
//!
//!-----------------
int FindLabel(Label* labels, char* name, int* index);

int GetArgsForJmp(const char* text, int* arg, Label* labels, int line);

int AddLabel(char* arg, Label* label, int cmd_index, int line);

//!--------------
//!
//!@param [out] text        Array lines of program 
//!@param [out] numer_lines number lines in program
//!
//!---------------
int GetProgramText(const char* program, int* number_lines, const char*** text, char** original_text);

int CheckSquereBracket(const char* args, int program_line);

int ParseArgs(char* args, int* comands, int* comand, int* arg1, int* arg2, int line);

//!--------------
//!
//!@param [out] comands       Array for program in binary representation 
//!@param [out] numer_comands Number comands in "comands" array
//!
//!---------------
int Compilation(int** comands, int* number_comand, Label* labels, int number_lines, const char** text, int number);

int PutProgramToFile(Header* header, int* comands);

int GetProgramCompileAndPutInFile(const char* program_file);

#endif //__ASSEMBLER_SYM__