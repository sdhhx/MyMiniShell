#include "apue.h"
#include "command.h"
#include "myShell.h"

int
parseCmdline(char* line){
	//name is betweem cmd and ptr
	char *cmd = line;
	char *ptr = cmd;
	int rflag = 0;
	//Last key token
	char ltoken = ' ';

	clearCommandline();
	while(*ptr == ' ' || *ptr == '\t'){
		cmd++;
		ptr++;
	}
	//These token cannot be first character of the command
	if(*ptr == '>' || *ptr == '<' || *ptr == '>' || *ptr == '&'){
		  clearCommandline();
        return -1;
   //Empty command
   }else if(*ptr == '\n'){
        return 0;
    }

	int command_count, arg_count;
	while(*ptr != '\0'){
		command_count = commandline->count;
		arg_count = commandline->cmd[commandline->count].argc;
		switch(*ptr){
			case ' ': case '\t': case '\n':
				//Can't set & as filename
				if(*cmd != '&'){
					commandline->cmd[command_count].argv[arg_count] = malloc(sizeof(char) * (ptr - cmd + 1));
					memset(commandline->cmd[command_count].argv[arg_count], 0, ptr - cmd + 1);
					strncpy(commandline->cmd[command_count].argv[arg_count], cmd, ptr - cmd);
					commandline->cmd[command_count].argv[arg_count][ptr - cmd] = '\0';
					commandline->cmd[command_count].argc++;
					if(rflag == 0){
						//set command name
						strncpy(commandline->cmd[command_count].command, cmd, ptr - cmd);
						commandline->cmd[command_count].command[ptr - cmd] = '\0';
						rflag = 1;
					}else if(rflag == 1){
						//set argv
						//removed;
					}else if(rflag == 2){
						if(ltoken == '>'){
							//set output file
							strncpy(commandline->cmd[command_count].outfile, cmd, ptr - cmd);
							commandline->cmd[command_count].outfile[ptr - cmd] = '\0';
						}else if(ltoken == '<'){
							//set input file
							strncpy(commandline->cmd[command_count].infile, cmd, ptr - cmd);
							commandline->cmd[command_count].infile[ptr - cmd] = '\0';
						}
					}
				}

				while(*ptr == ' ' || *ptr == '\t')
					ptr++;
				if (*ptr == '\n'){
					commandline->cmd[command_count].argv[commandline->cmd[command_count].argc] = NULL;
					commandline->count++;
				}else{
					cmd = ptr--;
				}

				break;
			case '<':
				commandline->cmd[command_count].isInRedir = 1;
				if(rflag < 2)
					rflag = 2;
				//read all space in the next
				ptr++;
				while(*ptr == ' ' || *ptr == '\t')
					ptr++;
				cmd = ptr--;
				ltoken = '<';
				commandline->cmd[command_count].argv[arg_count] = malloc(sizeof(char) * 2);
				memset(commandline->cmd[command_count].argv[arg_count], 0, 2);
				strncpy(commandline->cmd[command_count].argv[arg_count], "<\0", 2);
				commandline->cmd[command_count].argc++;
				break;
			case '>':
				if(rflag < 2)
					rflag = 2;
				//judge '>' and ">>"
				if(*(ptr + 1) == '>'){
					ptr++;
					commandline->cmd[command_count].isOutRedir = 2;
					commandline->cmd[command_count].argv[arg_count] = malloc(sizeof(char) * 3);
					memset(commandline->cmd[command_count].argv[arg_count], 0, 2);
					strncpy(commandline->cmd[command_count].argv[arg_count], ">>\0", 3);
				}else{
					commandline->cmd[command_count].isOutRedir = 1;
					commandline->cmd[command_count].argv[arg_count] = malloc(sizeof(char) * 2);
					memset(commandline->cmd[command_count].argv[arg_count], 0, 2);
					strncpy(commandline->cmd[command_count].argv[arg_count], ">\0", 2);
				}
				//read all space in the next
				ptr++;
				while(*ptr == ' ' || *ptr == '\t')
					ptr++;
				cmd = ptr--;
				ltoken = '>';

				commandline->cmd[command_count].argc++;
				break;
			case '|':
				rflag = 0;
				//read all space in the next
				ptr++;
				while(*ptr == ' ' || *ptr == '\t')
					ptr++;
               //These token cannot be first character of the command
               if (*ptr == '>' || *ptr == '<' || *ptr == '>' || *ptr == '&'){
                	 clearCommandline();
                   return -1;
                }
				cmd = ptr--;
				//must be null
				commandline->cmd[command_count].argv[commandline->cmd[command_count].argc] = NULL;
				//init next command
				commandline->count++;
				commandline->cmd[commandline->count].argc = 0;
				commandline->cmd[commandline->count].isInRedir = 0;
				commandline->cmd[commandline->count].isOutRedir = 0;
				break;
			case '&':
				commandline->background = 1;
				break;
			default:
				break;
		}
		ptr++;
	}
	return commandline->count;
}

void
printCommand(Commandline *commandline){
	int i;
	int num = commandline->count;
	printf("IsBackground:%d\n", commandline->background);
	for(i = 0; i < num; i++){
		printf("Command:%s\n",commandline->cmd[i].command);
		int j, count = commandline->cmd[i].argc;
		for(j = 0; j < count; j++){
			printf("Argv:%s\n", commandline->cmd[i].argv[j]);
		}
		if(commandline -> cmd[i].isInRedir)
			printf("Input:%s\n", commandline->cmd[i].infile);
		if(commandline -> cmd[i].isOutRedir)
			printf("Output:%s\n", commandline->cmd[i].outfile);
	}
}

void
initCommandline(){
	int i, j;
	commandline = malloc(sizeof(Commandline));
	commandline->count = 0;
	commandline->background = 0;
	for(i = 0; i < MAX_COMMAND; i++){
		commandline->cmd[i].argc = 0;
		commandline->cmd[i].isInRedir = 0;
		commandline->cmd[i].isOutRedir = 0;
		commandline->cmd[i].infd = STDIN_FILENO;
		commandline->cmd[i].outfd = STDOUT_FILENO;
		for(j = 0; j < MAX_PARAM; j++){
			commandline->cmd[i].argv[j] = NULL;
		}
	}
}

void
clearCommandline(){
	int i, j;
	int count = commandline->count;
	int argv_count;
	commandline->count = 0;
	commandline->background = 0;
	for(i = 0; i < count; i++){
		argv_count = commandline->cmd[i].argc;
		for(j = 0; j < argv_count; j++){
			free(commandline->cmd[i].argv[j]);
		}
		commandline->cmd[i].argc = 0;
		commandline->cmd[i].isInRedir = 0;
		commandline->cmd[i].isOutRedir = 0;
		commandline->cmd[i].infd = STDIN_FILENO;
		commandline->cmd[i].outfd = STDOUT_FILENO;
	}
}

void
freeCommandline(){
	clearCommandline();
	free(commandline);
}

