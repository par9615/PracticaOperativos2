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

int main()
{
	// array to read command string
	char *command = (char *)malloc(sizeof(char));

	// Argv and argc
	char ** local_argv;
	int local_argc = 0;

	char c;
	int i = 0;

	printf("sh >> ");
	while((c = getchar()) != '\n' && c != EOF)
	{
		command[i++] = c; // Adds the value of c to the array
		command = realloc(command, i + 1); // Adds space for the next character
	}
	command[i] = '\0'; // character end string

	// Creation of argv and update of argc
	local_argv = create_argv_argc(command, &local_argc);

	for (i = 0; i < local_argc; i++)
	{
		printf("argv[%d] = %s\n", i, local_argv[i]);
	}

	printf("Entered string: %s\n", command);

	return 0;
}