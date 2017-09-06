#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

#define NPROC 6

int main()
{
	int pid;
	int status;

	for(int i = 0; i < NPROC; i++)
	{
		pid = fork();

		if(pid == 0)		
			execlp("xterm", "xterm", "-e" , "./getty", NULL);
	}
	

	for(int i = 0; i < NPROC; i++)
	{
		wait(&status);
		printf("juas --> %d\n", status>>8);
	}

	

	return 0;
}

