#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct pinfo{
	FILE *file;
	pid_t pid;
	struct pinfo *next;
}pinofo;

static pinfo *plist = NULL;

int main()
{
	FILE *fp;
	char buf[BUFSIZ];

	fp = popen2("ls", "r");
	while(fgets(buf,BUFSIZ, fp) != NULL)
		fputs(buf,stdout);
}

FILE *popen2(const char *command, const char *mode)
{
	int fd[2];
	pinfo *recent;
	pinfo *old;

	if(pipe(fd)){
		return NULL;
	}

	recent = (pinfo*) malloc(sizeof(pinfo));
	recent->pid = fork();

	switch(recent->pid){

	case -1:
		close(fd[0]);
		close(fd[1]);
		free(recent);
		return NULL;
		break;

	case 0:
		for(old = plist; old; old = old->next){
			close(fileno(old->file));
		}
		
		if(mode[0] == 'r'){
			dup2(fd[1], STDOUT_FILENO);
		}
		else{
			dup2(fd[0], STDIN_FILENO);
		}

		close(fd[0]);
		close(fd[1]);
		
		execl("/bin/sh", "sh", "-c", command, (char*) NULL);
		
		exit(1);
		
		break;

	default:
		if(mode[0] == 'r'){
			close(fd[0]);
			if(!(recent->file = fdopen(fd[0], mode))){
				close(fd[1]);
			}
		}else{
			close(fd[0]);
			if(!(recent->file = fdopen(fd[1], mode))){
				close(fd[1]);
			}
		}
		
		recent->next = plist;
		
		plist = recent;
		
		break;
	}
	return recent->file;
}