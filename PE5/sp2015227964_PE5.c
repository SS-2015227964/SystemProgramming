#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

void main(){

        void f(int);
        int i;

        signal(SIGINT, f);

        while(1){
                printf("HELLO\n");
                printf("HELLO <Press The Ctrl + C>\n");
                sleep(3);
        }
}

void f(int signum){

        char a;

        printf("\nInterrupted! OK to quit (y/n)?");
	scanf("%c",&a);

        if (a == 'y'){
                exit(0);
        }else if( a == 'n'){
		getc(stdin);
                return;
        }
}
                        