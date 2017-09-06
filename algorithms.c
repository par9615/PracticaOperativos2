#include "algorithms.h"

/* Returns an array of pointers with every pointer, pointing to a word
   and updates the value of argc, using a separator*/
extern char **create_argv_argc(char * string, int * argc, char separator)
{
	char ** argv = (char**)malloc(sizeof(char *));
	int i, j, words = 0;
	int lenght = strlen(string);

	for (i = 0; i < lenght; i++)
	{
		if (string[i] == separator){
			continue;
		}
		else
		{
			argv = (char **)realloc(argv, sizeof(char *) * (words + 1));
			argv[words] = (char *)malloc(sizeof(char));
			j = 0;
			for ( ; i < lenght && string[i] != separator; i++)
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

/* Returns an array of characters reading only a string before endline */
extern char * get_line() {
	int i = 0;
	command = (char *)malloc(sizeof(char));

	while((c = getchar()) != '\n')
	{
		command[i++] = c; // Adds the value of c to the array
		command = realloc(command, i + 1); // Adds space for the next character
	}
	command[i] = '\0'; // character end string

	return command;
}
