default: example

example.o: example.cpp
	g++ -Wall -std=c++17 -I../../../include -I/opt/homebrew/include -c ../../../lib/json.hpp example.cpp

example: example.o
	g++ -L../../../cmake-build-debug -L/opt/homebrew/lib example.o -o example -lvault -lcurl -lpqxx

.PHONY: macos
macos:
	install_name_tool -change @rpath/libvault.0.dylib ../../../cmake-build-debug/libvault.0.dylib example


.PHONY: clean
clean:
	rm -f example.o example

vault:
	docker run -p 8200:8200 vault

postgres:
	docker run -e POSTGRES_USER=postgres -e POSTGRES_PASSWORD=postgres -p 5432:5432 postgres
