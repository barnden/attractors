CXX=g++-10
CXXFLAGS=-std=c++20 -O3

AR=ar
ARFLAGS=-sq

EMCXX=em++
EMCXXFLAGS=-std=c++2a -O3 -s WASM=1 -s STANDALONE_WASM\
 -s WASM_BIGINT -s EXPORTED_FUNCTIONS="[_init,_next,_x,_y,_color,_malloc,_free]" -o attractor.js

OPT=wasm-opt
OPTFLAGS=-O3

all: dirs wasm js

dirs:
	mkdir -p ./build/cpp ./build/js ./build/emcc ./build/opt
	mkdir -p ./dist/assets/js ./dist/assets/wasm

wasm:
	${EMCXX} ${EMCXXFLAGS} ./src/cpp/attractor.cpp -o ./build/emcc/attractor.js
	${OPT} ${OPTFLAGS} ./build/emcc/attractor.wasm -o ./build/opt/attractor.wasm
	cp ./build/opt/attractor.wasm ./dist/assets/js
	cp ./build/opt/attractor.js ./dist/assets/js

js:
	cmd.exe /C ".\build.bat"