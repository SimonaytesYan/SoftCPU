
all: file_work
	g++ Main.cpp FileWork.o -o Start.exe
	.\Start.exe

file_work:
	g++ -c Libs\FileWork\FileWork.cpp