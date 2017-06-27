#include "apue.h"
#include "myShell.h"
#include "command.h"

static void sig_int(int signo){
	printf("Interupted.\n");
	exit(1);
}

static void sig_quit(int signo){
	printf("Quit.\n");
	exit(2);
}

int
execute_job(){
	int status;
	//zombie process
	while(waitpid(-1, &status, WNOHANG) > 0);
	//exec many process
	if(commandline->count == 0){
		return 0;
	}
	//waiting for expanding to job execute (pip)
	execute_command(0);
	return 0;
}

int
execute_command(int cmdno){
	int pid;
	int status;
	//Is exit?
	if(!is_exit(commandline->cmd[cmdno].command, commandline->cmd[cmdno].argc, commandline->cmd[cmdno].argv));

	if((pid = fork())!= 0){//parent
		//is background
		if(commandline->background == 0){
			waitpid(pid, &status, 0);
		}
		sleep(1);
		return 0;
	}else{//child
		int fd1 = -1, fd2 = -1;

		signal(SIGINT, sig_int);
		signal(SIGQUIT, sig_quit);

		if(commandline->background == 1){
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
		}
		//Is bulid in command?
		//Bulid in command:cd and chprompt
		if(buildin(commandline->cmd[cmdno].command, commandline->cmd[cmdno].argc, commandline->cmd[cmdno].argv) != 0){
			return 0;
		}

		if(commandline->cmd[cmdno].isInRedir){
			//printf("read.\n");
			fd1 = open(commandline->cmd[cmdno].infile, O_RDWR | O_CREAT, 0666);
			dup2(fd1, STDIN_FILENO);
			close(fd1);
		}
		if(commandline->cmd[cmdno].isOutRedir){
			//printf("write.\n");
			if(commandline->cmd[cmdno].isOutRedir == 1){
				fd2 = open(commandline->cmd[cmdno].outfile, O_TRUNC | O_WRONLY | O_CREAT, 0666);
			}else if(commandline->cmd[0].isOutRedir == 2){
				fd2 = open(commandline->cmd[cmdno].outfile, O_WRONLY | O_APPEND | O_CREAT, 0666);
			}
			dup2(fd2, STDOUT_FILENO);
			close(fd2);
		}
		close(STDERR_FILENO);
		/*
		 * Warnning: redirect symbol cannot be ignored in params.
		 * Using argv[] and default environ
		 */
		execvp(commandline->cmd[cmdno].command, commandline->cmd[cmdno].argv);
		//fflush(NULL);
		exit(0);
	}
}
