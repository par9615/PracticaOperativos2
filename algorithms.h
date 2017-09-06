#ifndef _ALGORITHMS_H
#define _ALGORITHMS_H value

/* Special data types */
typedef enum { false, true } bool;

/* Returns an array of pointers with every pointer, pointing to a word
   and updates the value of argc, using a separator*/
extern char **create_argv_argc(char * string, int * argc, char separator);

/* Returns an array of characters reading only a string before endline */
extern char *get_line();

#endif