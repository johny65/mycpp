all: jjutils.so
	#mv jjutils.so /usr/lib/i386-linux-gnu/geany/jjutils.so
	mv jjutils.so /usr/lib/x86_64-linux-gnu/geany/jjutils.so

jjutils.o: jjutils.c
	gcc -c jjutils.c -fPIC `pkg-config --cflags geany`

jjutils.so: jjutils.o
	gcc jjutils.o -o jjutils.so -shared `pkg-config --libs geany`
	
clear:
	rm /usr/lib/i386-linux-gnu/geany/jjutils.so
