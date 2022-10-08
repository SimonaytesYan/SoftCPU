
all: logging assembler_exe processor_exe file_work
	.\Exe\Comp.exe
	.\Exe\Start.exe a.sy

assembler_exe:
	g++ Assembler.cpp Logging.o FileWork.o -o Exe\Comp.exe

processor_exe:
	g++ Processor.cpp Logging.o -o Exe\Start.exe

interpret_exe: file_work
	g++ Interpreter.cpp.cpp FileWork.o -o Exe\Start.exe
	.\Start.exe

logging:
	g++ -c Libs\Logging\Logging.cpp 

processor_o:
	g++ -c  Processor.cpp

disassembler_exe: processor_o
	g++ Disassembler.cpp Logging.o Processor.o -o Exe\Disass.exe
	.\Disass.exe a.sy

file_work:
	g++ -c Libs\FileWork\FileWork.cpp