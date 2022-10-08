
all: logging assembler_exe processor_exe file_work
	.\Exe\Comp.exe
	.\Exe\Start.exe a.sy

assembler_exe: logging file_work comand_system
	g++ Assembler.cpp ComandSystem.o Logging.o FileWork.o -o Exe\Comp.exe

processor_exe: logging comand_system
	g++ Processor.cpp Logging.o ComandSystem.o -o Exe\Start.exe

interpret_exe: file_work
	g++ Interpreter.cpp.cpp FileWork.o -o Exe\Start.exe
	.\Start.exe

processor_o:
	g++ -c  Processor.cpp

disassembler_exe: comand_system logging
	g++ Disassembler.cpp ComandSystem.o Logging.o Processor.o -o Exe\Disass.exe
	.\Disass.exe a.sy

file_work:
	g++ -c Libs\FileWork\FileWork.cpp

logging:
	g++ -c Libs\Logging\Logging.cpp 

comand_system:
	g++ -c Libs\ComandSystem\ComandSystem.cpp