prog: main.o spanningtree.o 
	gcc -o prog main.o spanningtree.o 

main.o: main.c
	gcc -c main.c

spanningtree.o: spanningtree.c
	gcc -c spanningtree.c
	
