#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef enum { false, true } bool;

typedef struct node
{
	char * key;
	char * value;
	struct node *left;
	struct node *right;
} Node;

Node *create_node(char * key, char * value);
void insert(Node *parent, char * key, char * value);
char *get_value_it(Node *parent, char * key);
char *get_value_re(Node * parent, char * key);
void change_if_variable(char **array_string);
char **get_variable_and_value(char *string);
char **tokenize(char * string, int * size, char separator);
void free_memory(char ** argv, int argc, char * command);
void create_shutdown_file();
void print_tree(Node * node);

/* Global env variables */
Node * tree;

int main()
{
	// Tree
	tree = (Node *)malloc(sizeof(Node));
	tree->key = "PATH";
	tree->value = "\\bin";
	tree->left = NULL;
	tree->right = NULL;

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

		if (strcmp(local_argv[0], "export") == 0)
		{
			char ** array_string = get_variable_and_value(local_argv[1]);
			if (array_string == NULL)
				printf("export: '%s:' not a valid identifier\n", local_argv[1]);
			else
				insert(tree, array_string[0], array_string[1]);
		}
		else if (strcmp(local_argv[0], "echo") == 0)
		{
			for(i = 1; local_argv[i] != NULL; i++)
			{
				printf("%s ", local_argv[i]);
			}
			printf("\n");
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

/* Creates a new struct of node */
Node *create_node(char * key, char * value)
{
	Node * node = (Node *)malloc(sizeof(Node));
	node->key = key;
	node->value = value;
	node->left = NULL;
	node->right = NULL;

	return node;
}

/* Inserts a node with a key and value inside the tree */
void insert(Node *parent, char * key, char * value)
{
	if (parent == NULL)
	{
		parent = create_node(key, value);
		return;
	}

	int movement = strcmp(parent->key, key);
	if (movement == 0)
	{
		parent->value = value;
		return;
	}
	else if (movement < 0 && parent->left == NULL)
	{
		parent->left = create_node(key, value);
		return;
	}
	else if (movement > 0 && parent->right == NULL)
	{
		parent->right = create_node(key, value);
		return;
	}

	else if(movement > 0 && parent->right != NULL)
		insert(parent->right, key, value);

	else if (movement < 0 && parent->left != NULL)
		insert(parent->left, key, value);
}

/* Get value with key inside tree iterative */
char *get_value_it(Node *parent, char * key)
{
	Node * actual = parent;
	while(actual != NULL)
	{
		int movement = strcmp(actual->key, key);
		if (movement == 0)
			return actual->value;
		else if (movement < 0)
			actual = actual->left;
		else
			actual = actual->right;
	}
	return NULL;
}

/* Get value with key inside tree recursively */
char *get_value_re(Node * parent, char * key)
{
	if (parent == NULL)
		return NULL;

	int movement = strcmp(parent->key, key);
	if (movement == 0)
		return parent->value;
	else if (movement < 0)
		get_value_re(parent->left, key);
	else
		get_value_re(parent->right, key);
}

/* Changes every registered env variable for its value */
void change_if_variable(char **array_string)
{
	int i = 0;
	char * cut_string;
	while(array_string[i] != NULL)
	{
		if (array_string[i][0] == '$')
		{
			int length = strlen(array_string[i]), j;
			cut_string = (char *)malloc(sizeof(char) * (length));

			for (j = 0 ; j <= length; j++) // Copies \0 to the new string
				cut_string[j] = array_string[i][j+1];

			char * variable_value = get_value_re(tree, cut_string);
			if (variable_value != NULL)
			{
				free(array_string[i]);
				array_string[i] = (char *)malloc(sizeof(char) * strlen(variable_value));
				for (j = 0; j <= length; j++)
					array_string[i][j] = variable_value[j];
			}
		}
		i++;
	}
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
	change_if_variable(array_pointers);
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

/* Prints env binary tree */
void print_tree(Node * node)
{
	if (node == NULL)
		return;
	printf("key: %s, value:%s\n", node->key, node->value);
	print_tree(node->left);
	print_tree(node->right);
}