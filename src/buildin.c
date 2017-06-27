#include "apue.h"
#include "myShell.h"
#include "command.h"

int do_chdir(int argc, char* param[]);
int do_exit(int argc, char* param[]);
int do_chprompt(int argc, char* param[]);

int
is_exit(char* command, int argc, char* param[]){
	int status;
	if(strcmp(command, "exit") == 0){
		//zombie process
		while(waitpid(-1, &status, WNOHANG) > 0);
		do_exit(argc, param);
		return 1;
	}
	return 0;
}

int
buildin(char* command, int argc, char* param[]){
	if(strcmp(command, "cd") == 0){
		do_chdir(argc, param);
		return 1;
	}else if(strcmp(command, "chprompt") == 0){
		do_chprompt(argc, param);
		return 1;
	}
	return 0;
}

//Change prompt
int
do_chprompt(int argc, char* param[]){
	//printf("prompt\n");
	if(argc != 2){
		printf("Input Error\n");
	}else{
		prompt_change(param[1]);
	}
	return 1;
}

//Change dir
int
do_chdir(int argc, char* param[]){
	//printf("chdir\n");
	int fd;
	if(argc != 2){
		printf("Input Error\n");
	}else{
		fd = open(param[1], O_RDONLY);
		fchdir(fd);
		close(fd);
	}
	return 1;
}

//Jump to exit
int
do_exit(int argc, char* param[]){
	printf("exit\n");
	longjmp(jmpbuffer, 1);
}

