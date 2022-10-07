
all: assembler processor file_work
	.\Exe\Comp.exe
	.\Exe\Start.exe a.sy

assembler:
	g++ Assembler.cpp FileWork.o -o Exe\Comp.exe

processor:
	g++ Processor.cpp -o Exe\Start.exe

interpret: file_work
	g++ Interpreter.cpp.cpp FileWork.o -o Exe\Start.exe
	.\Start.exe

disassembler:
	g++ Disassembler.cpp -o Exe\Disass.exe
	.\Disass.exe a.sy

file_work:
	g++ -c Libs\FileWork\FileWork.cpp