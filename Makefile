create : main.c othello.c
	gcc -o main main.c othello.c matrix.c -g -lm -fsanitize=address
	./main