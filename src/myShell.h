#ifndef _MYSHELL_H
#define _MYSHELL_H
#include <pwd.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LEN 1024
#define MAX_COMMAND 16
#define MAX_PARAM 16

void shell_init();
int prompt_display(char *);
void prompt_change(char *prompt);

#endif
