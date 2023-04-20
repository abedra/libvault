default: example

example.o: example.cpp
	g++ -Wall -Wno-unused-private-field -std=c++17 -I../../../include -c -I/opt/homebrew/opt/openssl@1.1/include ../../../lib/json.hpp example.cpp

example: example.o
	g++ -L../../../cmake-build-debug -L/opt/homebrew/opt/openssl@1.1/lib example.o -o example -lvault -lcurl -lssl -lcrypto

.PHONY: macos
macos:
	install_name_tool -change @rpath/libvault.0.dylib ../../../cmake-build-debug/libvault.0.dylib example

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
