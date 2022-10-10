#include <stdlib.h>

int main()
{
    
    system("g++ -c Libs/FileWork/FileWork.cpp");

    system("md Exe");
    system("g++ Assembler/Assembler.cpp FileWork.o -o Exe/Comp.exe");
    system("start Exe/Comp.exe");
    system("g++ Processor/Processor.cpp -o Exe/Start.exe");
    system("start Exe/Start.exe a.sy");
}