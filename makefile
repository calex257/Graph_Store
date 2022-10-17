all: build

build: graph.c queue.c main.c adj_list.c algos.c stack.c gen_queue.c
	gcc -Wall -g -std=c99 -o tema3 $^

clean: build
	rm tema3