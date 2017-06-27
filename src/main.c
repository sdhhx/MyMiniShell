#include "apue.h"
#include "command.h"
#include "myShell.h"

/*
 * @author: hehaoxing
 * @date: 2017-06-14
 */

int
main(int argc, char* argv[]){
	char prompt[MAX_LEN];
	char line[MAX_LEN];
	int num;
	shell_init();

	//exit
	if(setjmp(jmpbuffer) != 0){
		freeCommandline();
		exit(0);
	}

	while(1){
		prompt_display(prompt);
		printf("%s", prompt);
		fflush(stdout);
		fgets(line, MAX_LEN, stdin);
		if((num = parseCmdline(line)) >= 0){
			/*Debug:
			printf("Command Count:%d\n", num);
		   printCommand(commandline);
		    */
		   execute_job();
		}else{
			fprintf(stderr, "Command Error.\n");
		}
	}
	return 0;
}
