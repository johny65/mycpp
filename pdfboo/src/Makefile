MACROS = DEBUG

all: pdfboo

pdfboo: main.o pdfdocument.o utils.o
	g++ -o ../bin/$@ $^ -L../podofo/lib -lpodofo -lfreetype -lfontconfig -ljpeg -lpthread

main.o: main.cpp pdfdocument.h utils.h
	g++ -c $< -o $@ -I../podofo/include -D$(MACROS)

pdfdocument.o: pdfdocument.cpp pdfdocument.h utils.h debug.h
	g++ -c $< -o $@ -I../podofo/include -D$(MACROS)

utils.o: utils.cpp utils.h
	g++ -c $< -o $@ -I../podofo/include -D$(MACROS)