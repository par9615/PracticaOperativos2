#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	int pid;
	int status;

	for(int i = 0; i < 6; i++)
	{
		pid = fork();

		if(pid == 0)
			execlp("xterm", "xterm", "-e" , "./getty", NULL);
	}

	wait(&status);

	return 0;
}

