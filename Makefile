
all: file_work
	g++ Assembler.cpp FileWork.o -o Comp.exe
	.\Comp.exe
	g++ Processor.cpp -o Start.exe
	.\Start.exe a.sy

interpret: file_work
	g++ Interpreter.cpp.cpp FileWork.o -o Start.exe
	.\Start.exe

disassembler:
	g++ Disassembler.cpp -o Disass.exe
	.\Disass.exe a.sy

file_work:
	g++ -c Libs\FileWork\FileWork.cpp