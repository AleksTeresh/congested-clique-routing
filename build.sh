rm -rf build/
mkdir build
cd build
em++ ../cpp/main.cpp ../cpp/node.cpp --bind -s WASM=1 -std=c++17 -s -o main.js || exit 1
mv main.js ../web/gen/
mv main.wasm ../web/gen/