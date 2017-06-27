#include "apue.h"
#include "myShell.h"

int is_childdir(const char* parent, char *child);

int
prompt_display(char* prompt){
	char username[MAX_LEN];
	char homedir[MAX_LEN];
	char currentdir[MAX_LEN];
	char hostname[MAX_LEN];
	char display_dir[MAX_LEN];
	char* prompt_token = getenv("PROMPT_TOKEN");

	struct passwd *user;
	user = getpwuid(getuid());
	strcpy(username, user -> pw_name);
	strcpy(homedir, user -> pw_dir);
	if(gethostname(hostname, MAX_LEN))
		strcpy(hostname, "unknown");
	getcwd(currentdir, MAX_LEN);

	int dir_flag;
	dir_flag = is_childdir(homedir,currentdir);
	if(dir_flag < 0){
		snprintf(display_dir, MAX_LEN, "%s", currentdir);
	}
	else{
		snprintf(display_dir, MAX_LEN, "~%s", currentdir + dir_flag);
	}
	snprintf(prompt, MAX_LEN, "%s@%s:%s%s", username, hostname, display_dir, prompt_token);
	return 0;
}

void
prompt_change(char* prompt_token){
	char str[32];
	snprintf(str, 32, "PROMPT_TOKEN=%s", prompt_token);
	putenv(str);
}

int
is_childdir(const char* parent, char *child){
	if (parent == NULL && child == NULL){
		return -1;
	}
	int parent_len = strlen(parent);
	int child_len = strlen(child);
	if((parent_len > child_len) || (parent_len == 0 || child_len == 0))
		return -1;
	int i;
	for(i = 0; i < parent_len; i++){
		if(parent[i] != child[i])
			return -1;
	}
	return parent_len;
}
