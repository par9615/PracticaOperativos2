#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

#define NPROC 6

typedef enum{false, true} bool;

/*Return true if shutdown_file exists. If exists if means that user entered shutdown command*/
bool shutdown_file()
{
	FILE * file;
	file = fopen("shutdown.txt", "r");

	if(file)
	{
		fclose(file);
		return true;
	}

	return false;
}

int get_pid_index(int children[], int pid)
{
	for(int i = 0; i < NPROC; i++)
	{
		if(children[i] == pid)
			return i;
	}

	return -1;
}

int main()
{
	int pid, wpid, index;
	int status;
	int children[NPROC];

	//Child processes are created
	for(int i = 0; i < NPROC; i++)
	{
		pid = fork();

		if(pid == 0)		
			execlp("xterm", "xterm", "-e" , "./getty", NULL);
		else
			children[i] = pid;

	}

	/*pid = fork();
	if(pid == 0)
		execlp("ps", "ps", NULL);

	wait(&status);*/
	
	//Checks forever when a child process is closed
	while(true)
	{
		
		pid = wait(&status);

		//Shutdown 
		if(shutdown_file())
		{
			remove("shutdown.txt");

			for(int i = 0; i < NPROC; i++)
				kill(children[i], SIGKILL);

			return 0;
		}
		
		//Xterm closed
		else
		{
			index = get_pid_index(children, pid);
			
			//New process is created and saved in children array
			pid = fork();
			children[index] = pid;

			if(pid == 0)
			{
				execlp("xterm", "xterm", "-e" , "./getty", NULL);
			}	
		}		

	}

	

	return 0;
}

