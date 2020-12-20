default: example

example.o: example.cpp
	g++ -std=c++17 -c ../../../../lib/json.hpp example.cpp

example: example.o
	g++ example.o -o example -lvault -lcurl -lpqxx

.PHONY: clean
clean:
	rm -f example.o example

vault:
	docker run -p 8200:8200 vault

postgres:
	docker run -e POSTGRES_USER=postgres -e POSTGRES_PASSWORD=postgres -p 5432:5432 postgres