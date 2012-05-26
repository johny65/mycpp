all: u1st
	cp $< /usr/local/bin/$<
	chmod +x /usr/local/bin/$<

u1st: u1st.o
	g++ $< -o $@

u1st.o: u1st.cpp
	g++ -c $< -o $@ -O2 -s

clean:
	rm u1st u1st.o

.PHONY: clean