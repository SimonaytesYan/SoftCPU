
all: create_folders logging assembler_exe processor_exe file_work
	.\Exe\Comp.exe
	.\Exe\Start.exe a.sy

create_folders:
	mkdir -p Exe
	mkdir -p Obj

assembler_exe: create_folders logging file_work comand_system
	g++ Assembler\Assembler.cpp Obj\ComandSystem.o Obj\Logging.o Obj\FileWork.o -o Exe\Comp.exe

processor_o: create_folders
	g++ -c  Processor\Processor.cpp -o Obj\Processor.o

processor_exe: create_folders logging comand_system
	g++ Processor\Processor.cpp Obj\Logging.o Obj\ComandSystem.o -o Exe\Start.exe

interpret_exe: create_folders file_work
	g++ Interpreter.cpp.cpp Obj\FileWork.o -o Exe\Start.exe
	Exe\.\Start.exe

disassembler_exe: processor_o create_folders comand_system logging
	g++ Disassembler.cpp Obj\Processor.o Obj\ComandSystem.o Obj\Logging.o -o Exe\Disass.exe
	Exe\.\Disass.exe a.sy

file_work: create_folders
	g++ -c Libs\FileWork\FileWork.cpp -o Obj\FileWork.o

logging: create_folders
	g++ -c Libs\Logging\Logging.cpp -o Obj\Logging.o

comand_system: create_folders
	g++ -c Libs\ComandSystem\ComandSystem.cpp -o Obj\ComandSystem.o