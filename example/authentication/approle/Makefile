default: example

example.o: example.cpp
	g++ -Wall -std=c++17 -I../../../include -c ../../../lib/json.hpp example.cpp

example: example.o
	g++ -L../../../cmake-build-debug example.o -o example -lvault -lcurl

.PHONY: macos
macos:
	install_name_tool -change @rpath/libvault.0.dylib ../../../cmake-build-debug/libvault.0.dylib example

.PHONY: clean
clean:
	rm -f example.o example

vault:
	docker run -p 8200:8200 vault
