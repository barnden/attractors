# attractors
## Quadratic Strange Attractor
### Compiling on Windows
Inside WSL2 install emscripten sdk, on Windows install minify and terser globally

Run `make all`

Serve `dist` directory using `python3 -m http.server`

Goto `localhost:8000`

## Symmetric Icon Attractor
### Using on Windows
Compile `module.cpp` using `x86_64-w65-mingw32-gcc -std=c++20 module.cpp -o test.dll`

Run `windows.py`
