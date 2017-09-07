ll :	init getty sh

init : init.o
		gcc -o init init.o 

init.o : init.c
		gcc -c init.c

getty : getty.o
		gcc -o getty getty.o 

getty.o : getty.c
		gcc -c getty.c

sh : sh.o
		gcc -o sh sh.o 

sh.o : sh.c
		gcc -c sh.c

clean :
		rm init.o getty.o sh.o init getty sh
