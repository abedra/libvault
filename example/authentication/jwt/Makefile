default: example

example.o: example.cpp
	g++ -std=c++17 -c ../../../lib/json.hpp example.cpp

example: example.o
	g++ example.o -o example -lvault -lcurl -lssl -lcrypto

.PHONY: clean
clean:
	rm -f example.o example

.PHONY: vault
vault:
	docker run -p 8200:8200 vault

.PHONY: keypair
keypair:
	openssl genrsa -out private.pem 2048
	openssl rsa -pubout -in private.pem -out public.pem