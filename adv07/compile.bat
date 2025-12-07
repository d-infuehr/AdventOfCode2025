echo run this command with activated emscripten
REM git clone https://github.com/emscripten-core/emsdk.git
REM cd emsdk
REM emsdk.bat install latest
REM emsdk.bat activate latest
REM cd..
emcc main.cpp -o main.html -s SINGLE_FILE=1 --embed-file input.txt