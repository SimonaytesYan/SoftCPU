#ifndef __ASSEMBLER_SYM__
#define __ASSEMBLER_SYM__

#include "..\Libs\ComandSystem\ComandSystem.h"

//!--------------
//!
//!@param [out] text        Array lines of program 
//!@param [out] numer_lines number lines in program
//!
//!---------------
int GetProgramText(const char* program, int* number_lines, const char*** text, char** original_text);

int CheckSquereBracket(const char* args, int program_line);

int ParseArgs(const char* args, int* comands, int* comand, int* arg1, int* arg2, int line);

int GetArgsForPop(const char* args, int* comands, int* comand, int* arg1, int* arg2, int line);

//!--------------
//!
//!@param [out] comands       Array for program in binary representation 
//!@param [out] numer_comands Number comands in "comands" array
//!
//!---------------
int Compilation(int** comands, int* number_comand, int number_lines, const char** text);

int PutProgramToFile(Header* header, int* comands);

int GetProgramAndCompile(const char* program_file);

#endif //__ASSEMBLER_SYM__