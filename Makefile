all: create_folders logging assembler_exe processor_exe file_work
	.\Exe\Comp.exe Program.txt
	.\Exe\Start.exe a.sy

create_folders:

assembler_exe: assembler_o create_folders logging file_work comand_system
	g++ Assembler\main.cpp Obj\Assembler.o Obj\ComandSystem.o Obj\Logging.o Obj\FileWork.o -o Exe\Comp.exe

assembler_o: create_folders logging file_work comand_system
	g++ -c Assembler\Assembler.cpp -o Obj\Assembler.o 

processor_exe: processor_o create_folders logging comand_system
	g++ Processor\main.cpp Obj\Processor.o Obj\Logging.o Obj\ComandSystem.o -o Exe\Start.exe

processor_o:
	g++ -c  Processor\Processor.cpp -o Obj\Processor.o

interpret_exe: create_folders file_work
	g++ Interpreter.cpp.cpp Obj\FileWork.o -o Exe\Start.exe
	Exe\.\Start.exe

disassembler_exe: processor_o create_folders comand_system logging
	g++ Disassembler\Disassembler.cpp Obj\Processor.o Obj\ComandSystem.o Obj\Logging.o -o Exe\Disass.exe
	Exe\.\Disass.exe a.sy

file_work: create_folders
	g++ -c Libs\FileWork\FileWork.cpp -o Obj\FileWork.o

logging: create_folders
	g++ -c Libs\Logging\Logging.cpp -o Obj\Logging.o

comand_system: create_folders
	g++ -c Libs\ComandSystem\ComandSystem.cpp -o Obj\ComandSystem.o