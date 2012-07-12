N = ej

$(N): $(N).o
	g++ -Wall -o $@ $< -pthread -lX11 -lfftw3

$(N).o: $(N).cpp pdi.h
	g++ -Wall -c $< -o $@ -O0
