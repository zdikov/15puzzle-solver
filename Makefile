all: build
	g++ src/*.cpp -std=c++17 -Wall -Werror -Wextra -o build/solver -Ofast
windows: build
	x86_64-w64-mingw32-g++ src/*.cpp -std=c++17 -static -o build/solver.exe -Ofast
debug: build
	g++ src/*.cpp -std=c++17 -Wall -Werror -Wextra -o build/solver-debug -O0 -g
build:
	mkdir build
clean:
	rm -rf build
