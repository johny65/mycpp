all: hello.so
	mv hello.so /usr/lib/i386-linux-gnu/geany/hello.so

hello.o: hello.c
	gcc -c hello.c -fPIC `pkg-config --cflags geany`

hello.so: hello.o
	gcc hello.o -o hello.so -shared `pkg-config --libs geany`
	
clear:
	rm /usr/lib/i386-linux-gnu/geany/hello.so
