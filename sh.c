#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef enum { false, true } bool;

char **tokenize(char *, int *, char);
void free_memory(char **, int, char *);
void create_shutdown_file();
char **get_variable_and_value(char *);
void create_shutdown_file();

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
			exit(1);
		}
		else if (strcmp(local_argv[0], "export") == 0)
		{
			char ** variable_value = get_variable_and_value(local_argv[1]);
			if (setenv(variable_value[0], variable_value[1], 1) == -1)
			{
				printf("PUTO EL QUE LO LEA\n");
			}
		}
		else if (strcmp(local_argv[0], "echo") == 0)
		{
			char * temp = &(local_argv[1][1]);
			char * value = getenv(temp);
			if (value == NULL)
				for(i = 1; i < local_argc; i++)
					printf("%s\n", local_argv[i]);
			else
				printf("%s\n", getenv(temp));
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

/* Free alocated memory assigned to pointers */
void free_memory(char ** argv, int argc, char * command)
{
	int i;
	for (i = 0; i < argc; i++)
		free(argv[i]);
	free(argv);

	free(command);
}

/* Creates a file to comunicate to init program and close all terminals */
void create_shutdown_file()
{
	FILE * file;

	file = fopen("shutdown.txt", "w");

	fputs("yes", file);

	fclose(file);
}

/* Returns a list of strings containing at 0 the variable name and in 1 the variable value */
char **get_variable_and_value(char *string)
{
	char **variable_and_value = NULL;

	if (string[0] == '=')
		return variable_and_value;

	int length = strlen(string), i, last_i, j = 0;
	variable_and_value = (char **)malloc(sizeof(char *) * 2);
	bool not_found_equal = true;
	variable_and_value[0] = (char *)malloc(sizeof(char));
	variable_and_value[1] = (char *)malloc(sizeof(char));
	for(i = 0; i < length; i++)
	{
		if (string[i] != '=' && not_found_equal)
		{
			last_i = i + 1;
			variable_and_value[0][i] = string[i];
			variable_and_value[0] = (char *)realloc(variable_and_value[0], sizeof(char) * (i + 2));
		}
		else if (string[i] == '=' && not_found_equal)
		{
			not_found_equal = false;
		}
		else if (!not_found_equal)
		{
			variable_and_value[1][j++] = string[i];
			variable_and_value[1] = (char *)realloc(variable_and_value[1], sizeof(char) * (j + 1));
		}
	}
	variable_and_value[0][last_i] = '\0';
	variable_and_value[1][j] = '\0';
	return variable_and_value;
}