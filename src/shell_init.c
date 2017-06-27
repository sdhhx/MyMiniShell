#include "apue.h"
#include "myShell.h"
#include "command.h"

void
shell_init(){
	char *prompt_token = malloc(sizeof(char) * 4);
	memset(prompt_token, 0, 4);
	if(!getenv("PROMPT_TOKEN"))
		putenv("PROMPT_TOKEN=$");

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	initCommandline();
	umask(0);
}
