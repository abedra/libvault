default: example

example.o: example.cpp
	g++ -c example.cpp

example: example.o
	g++ example.o -o example -lvault -lcurl

.PHONY: clean
clean:
	rm example.o example
