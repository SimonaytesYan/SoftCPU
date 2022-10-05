#include <stdlib.h>

int main()
{
    system("g++ -c Libs/FileWork/FileWork.cpp");

    system("g++ Assembler.cpp FileWork.o -o Comp.exe");
    system("Comp.exe");
    system("g++ Processor.cpp -o Start.exe");
    system("Start.exe a.sy");
}