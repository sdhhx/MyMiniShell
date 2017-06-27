#ifndef _READCOMMAND_H
#define _READCOMMAND_H
#include "myShell.h"

typedef struct{
	char command[MAX_LEN];
	int argc;
	char *argv[MAX_PARAM];
	//file
	int isInRedir;
	char infile[MAX_LEN];
	int isOutRedir;
	char outfile[MAX_LEN];
	//pip
	int infd;
	int outfd;
}Command;

typedef struct{
	Command cmd[MAX_COMMAND];
	int count;
	int background;
}Commandline;

Commandline *commandline;
jmp_buf jmpbuffer;

void initCommandline();
int parseCmdline(char* line);
void clearCommandline();
void printCommand(Commandline *line);
void freeCommandline();
int execute_command(int cmdno);
int execute_job();
int buildin(char* command, int argc, char* param[]);
int is_exit(char* command, int argc, char* param[]);

#endif
