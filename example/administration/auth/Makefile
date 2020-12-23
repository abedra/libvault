default: example

example.o: ../../shared/shared.h example.cpp
	g++ -std=c++17 -c example.cpp

example: example.o
	g++ example.o -o example -lvault -lcurl

.PHONY: clean
clean:
	rm -f example.o example

vault:
	docker run -p 8200:8200 vault