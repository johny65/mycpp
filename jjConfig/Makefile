all: jjConfig.a

jjConfig.a: jjConfig.o
	ar -rcs $@ $<

jjConfig.o: jjConfig.cpp jjConfig.h
	g++ -c $< -o $@ -static -O2 -s