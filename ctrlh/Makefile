all: include.so
	mv $< ~/.config/geany/plugins

include.o: include.cpp
	g++ -c $< -fPIC `pkg-config --cflags geany`

include.so: include.o
	g++ $< -o $@ -shared `pkg-config --libs geany`
