#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define oops(m,x) { perror(m) ; exit(x);}

main(int ac, char **av)
{
	int thepipe[2];
	int pid;
	
	if ( pipe( thepipe ) == -1){
		oops("Cannot get a pipe", 1);
	}

	if ( (pid = fork() ) == -1 ){
		oops("Cannot fork", 2);
	}

	if ( pid > 0 ) {
		int child_pipe[2];
		int child_pid;
		
		close(thepipe[1]);
		
		if (dup2(thepipe[0], 0) == -1){
			oops("could not redirect stdin", 3);
		}
		
		close(thepipe[0]);

		if (pipe(child_pipe) == -1){
			oops("Cannot get a pipe",4);
		}

		if ( (child_pid = fork() ) == -1 ){
			oops("Cannot fork",5);
		}

		if ( child_pid > 0 ) {

			close(child_pipe[1]);

			if (dup2(child_pipe[0], 0 ) == -1){
				oops("could not redirect stdout", 6);
			}

			close(child_pipe[0]);
			
			execlp(av[3], av[3], NULL);
		}

		if ( child_pid == 0 ) {
			
			close(child_pipe[0]);
			
			if ( dup2(child_pipe[1], 1) == -1 ){
				oops("could not redirect stdin", 7);
			}
			
			close(child_pipe[1]);

			execlp( av[2], av[2], NULL);
		}
		
	}

	if ( pid == 0 ) {
		close(thepipe[0]);

		if ( dup2(thepipe[1], 1) == -1 ){
			oops("could not redirect stdout", 8);
		}

		close(thepipe[1]);
		
		execlp(av[1], av[1], NULL);
	}
}
