#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef enum { false, true } bool;

/* Returns an array of pointers with every pointer, pointing to a word
   and updates the value of size using a separator*/
char **tokenize(char * string, int * size, char separator)
{
	char ** array_pointers = (char**)malloc(sizeof(char *));
	int i, j, words = 0;
	int lenght = strlen(string);

	for (i = 0; i < lenght; i++)
	{
		if (string[i] == separator){
			continue;
		}
		else
		{
			array_pointers = (char **)realloc(array_pointers, sizeof(char *) * (words + 1));
			array_pointers[words] = (char *)malloc(sizeof(char));
			j = 0;
			for ( ; i < lenght && string[i] != separator; i++)
			{
				array_pointers[words][j++] = string[i];
				array_pointers[words] = (char *)realloc(array_pointers[words], sizeof(char) * (j + 1));
			}
			array_pointers[words++][j] = '\0';
		}
	}
	array_pointers[words] = NULL;
	*size = words;

	return array_pointers;
}

void free_memory(char ** argv, int argc, char * command)
{
	int i;
	for (i = 0; i < argc; i++)
		free(argv[i]);
	free(argv);

	free(command);
}

void create_shutdown_file()
{
	FILE * file;

	file = fopen("shutdown.txt", "w");

	fputs("yes", file);

	fclose(file);
}

int main()
{
	// Argv and argc
	char ** local_argv;
	int local_argc = 0;

	// array to read command string
	char * command;

	char c;
	int i;

	
	while(c != EOF)
	{
		i = 0;
		printf("sh >> ");

		command = (char *)malloc(sizeof(char));

		while((c = getchar()) != '\n')
		{
			command[i++] = c; // Adds the value of c to the array
			command = realloc(command, i + 1); // Adds space for the next character
		}
		command[i] = '\0'; // character end string
		
		// Creation of argv and update of argc
		local_argv = tokenize(command, &local_argc, ' ');
		if (strcmp(local_argv[0], "exit") == 0)
		{
			exit(0);
		}
		else if (strcmp(local_argv[0], "shutdown") == 0)
		{
			create_shutdown_file();
			exit(0);
		}
		else
		{
			int pid = fork(), status, status_child;
			if (pid == 0) {
				status_child = execvp(local_argv[0], local_argv);
				if (status_child == -1)
					printf("%s: command not found\n", local_argv[0]);
			}
			wait(&status);
		}

		free_memory(local_argv, local_argc, command);
	}

	return 0;
}