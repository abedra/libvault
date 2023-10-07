default: example

example.o: example.cpp
	g++ -Wall -Wno-deprecated-declarations -std=c++17 -I../../../include -c ../../../lib/json.hpp example.cpp

example: example.o
	g++ -L../../../cmake-build-debug example.o -o example -lvault -lcurl -lssl -lcrypto

.PHONY: clean
clean:
	rm -f example.o example

.PHONY: vault
vault:
	docker run -p 8200:8200 hashicorp/vault:latest

.PHONY: keypair
keypair:
	openssl genrsa -out private.pem 2048
	openssl rsa -pubout -in private.pem -out public.pem
