pdfboo: main.o
	g++ -o $@ $< -Lpodofo/lib -lpodofo -lfreetype -lfontconfig -ljpeg -lpthread

main.o: main.cpp
	g++ -c -o $@ $< -Ipodofo/include
