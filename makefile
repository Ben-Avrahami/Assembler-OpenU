all: Assembler
Assembler: main.c functions.c MacroDeployment.c FirstPass.c SecondPass.c CreateEntryFile.c CreateObjectFile.c  CreateExternalsFile.c Project.h
	gcc main.c functions.c MacroDeployment.c FirstPass.c SecondPass.c CreateEntryFile.c CreateObjectFile.c  CreateExternalsFile.c  -g -ansi -pedantic -Wall -lm -o Assembler

