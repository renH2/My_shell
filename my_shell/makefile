myshell:myshell.o environment.o parser.o builtin.o exec.o
	gcc -o myshell myshell.o environment.o parser.o builtin.o exec.o

myshell.o:myshell.c const.h environment.h
	gcc -c myshell.c 

builtin.o:builtin.c environment.h
	gcc -c builtin.c
   
exec.o:exec.c exec.h environment.h const.h
	gcc -c exec.c
	
environment.o:environment.c environment.h
	gcc -c environment.c

parser.o:parser.c 
	gcc -c parser.c
	
clean:
	rm *.o 
