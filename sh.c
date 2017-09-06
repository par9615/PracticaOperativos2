#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef enum { false, true } bool;

/* Returns an array of pointers with every pointer, pointing to a word
   and updates the value of argc*/
char **create_argv_argc(char * string, int * argc)
{
	char ** argv = (char**)malloc(sizeof(char *));
	int i, j, words = 0;
	int lenght = strlen(string);

	for (i = 0; i < lenght; i++)
	{
		if (string[i] == ' '){
			continue;
		}
		else
		{
			argv = (char **)realloc(argv, sizeof(char *) * (words + 1));
			argv[words] = (char *)malloc(sizeof(char));
			j = 0;
			for ( ; i < lenght && string[i] != ' '; i++)
			{
				argv[words][j++] = string[i];
				argv[words] = (char *)realloc(argv[words], sizeof(char) * (j + 1));
			}
			argv[words++][j] = '\0';
		}
	}
	argv[words] = NULL;
	*argc = words;

	return argv;
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

	fputs(file, "yes");

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
		local_argv = create_argv_argc(command, &local_argc);

		if (strcmp(local_argv[0], "export") == 0)
		{
			printf("export action\n");
		}
		else if (strcmp(local_argv[0], "echo") == 0)
		{
			printf("echo action\n");
		}
		else if (strcmp(local_argv[0], "exit") == 0)
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
			printf("%s: command not found\n", local_argv[0]);
		}

		free_memory(local_argv, local_argc, command);
	}

	return 0;
}