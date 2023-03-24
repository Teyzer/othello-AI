create : main.c othello.c
	gcc -o main main.c othello.c -g
	./main