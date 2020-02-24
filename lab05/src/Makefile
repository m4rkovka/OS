all: dyn2 dyn1

dyn2: main2.c
	gcc -o dyn2 main2.c -ldl -g

dyn1: main1.c deque.c
	gcc -fPIC -c deque.c
	gcc -shared -o libdeque_dyn.so deque.o
	gcc -o dyn1 main1.c -L . -l deque_dyn 