#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
	int pid, status;

	//Datos del input de consola
	char user[50];
	char pass[30];

	//Datos del input de archivo
	char user_f[50];
	char pass_f[30];

	//Banderas para verificar user y pass
	int user_v = 0, pass_v = 0;

	printf("Nombre de usuario >> ");
	while(scanf("%s", user) != EOF)
	{

		printf("Contraseña >> ");
		scanf("%s", pass);

		user_v = pass_v = 0;

		FILE *file;

		if((file = fopen("passwd.txt", "r")) == NULL)
			printf("Error al verificar contraseña\n");
		
		else
		{
			while(fscanf(file, "%[^:]", user_f) != EOF)
			{
				fgetc(file);
				fscanf(file, "%[^\n]", pass_f);
				fgetc(file);

				if(strcmp(user_f, user) == 0)
				{
					if(strcmp(pass_f, pass) == 0)
						user_v = pass_v = 1;
					
					else
						user_v = 1;
				}
			}				
		}

		if(user_v && pass_v)
		{
			printf("Ingreso exitoso\n");

			pid = fork();
			if(pid == 0)
				execlp("sh", "sh", NULL); //se cambia por el sh.c

			wait(&status);
		}

		else if(user_v && !pass_v)
			printf("Contraseña errónea\n");

		else
			printf("Usuario inexistente\n");


		printf("Nombre de usuario >> ");
	}
	return 0;
}