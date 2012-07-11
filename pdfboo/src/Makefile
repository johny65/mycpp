all: pdfboo

pdfboo: main.o pdfdocument.o
	g++ -o ../bin/$@ main.o pdfdocument.o -L../podofo/lib -lpodofo -lfreetype -lfontconfig -ljpeg -lpthread

main.o: main.cpp
	g++ -c $< -o $@ -I../podofo/include

pdfdocument.o: pdfdocument.cpp pdfdocument.h
	g++ -c $< -o $@ -I../podofo/include