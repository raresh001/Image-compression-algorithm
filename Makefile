.PHONY: build clean

build: quadtree

quadtree: quaternarTree.c matrix.c main.c queue.c
	gcc $^ -o quadtree

%.o: %.c
	gcc -c $< -o $@

clean:
	rm -f *.o quadtree