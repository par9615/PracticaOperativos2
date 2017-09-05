#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "vector.h"

typedef enum { false, true } bool;

// Borra espacios al principio (causan ruido)
char *delete_first_spaces(char * string)
{
	int lenght = strlen(string), i, j = 0;
	bool not_found_word = true;
	char *new_string = (char *)malloc(sizeof(char));

	if (string[0] == '')
	{
		for (i = 1; i < lenght; i++)
		{
			if (string[i] == ' ' && not_found_word)
				continue;
			else {
				if (not_found_word)
					not_found_word = false;
				new_string[j++] = string[i];
				new_string = (char *)realloc(new_string, sizeof(char) * (j + 1));
			}
		}
		new_string[j] = '\0';
		free(string);
		return new_string;
	}
	free(new_string);
	return string;
}

// Regresa un arreglo de enteros del inicio de cada palabra
int *words_identifiers(char * string, int * count_strings)
{
	int *index;
	int i, length = strlen(string);
	bool not_found_word = true;
	char *new_string = malloc(sizeof(char));

	for (i = 0; i < lenght - 1; i++)
	{
		if (new_string[i] == ' ' && new_string[i+1] != ' ')
		{
			(*count_strings)++;
			index = (int *)realloc(index, sizeof(int) * (*count_strings));
			index[count_strings] = i + 1;
		}
	}
	return index;

}

int main()
{
	// Datos de input de consola
	char *command = (char *)malloc(sizeof(char));

	// Variables de apoyo para leer todo el comando
	char c;
	int i = 0;
	int count_strings = 0;

	printf("sh >> ");
	while((c = getchar()) != '\n' && c != EOF)
	{
		command[i++] = c; // Agrega letra del comando
		command = realloc(command, i + 1); // Agrega espacio para caracter
	}
	command[i] = '\0'; // caracter nulo termino de string

	int *words_indexes = words_identifiers(&command, &count_strings);

	// Crear condicionales para ver que accion ejecutar


	printf("Entered string: %s\n", command);

	return 0;
}