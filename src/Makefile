all:
	make list_test.exe

clean:
	del list.o
	del liblist.a

graph_c.exe: graph.c list.o map.o
	gcc -g graph.c -o graph_c.exe list.o map.o

graph.exe: graph.cpp
	g++ -g graph.cpp -o graph.exe
	# g++ -g -fsanitize=address graph.cpp -o graph.exe -lasan

list.o: list.c list.h
	gcc -g -c list.c

map.o: map.c map.h list.h list.o
	gcc -g -c map.c

libcollection.a: list.o map.o
	ar rcs libcollection.a list.o map.o

list_test.exe: list_test.c libcollection.a
	gcc -g list_test.c -o list_test.exe -L. -lcollection

map_test.exe: map_test.c libcollection.a
	gcc -g map_test.c -o map_test.exe -L. -lcollection