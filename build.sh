cd build
find . ! -name '.gitignore' -type f -exec rm -f {} + # delete all files from ./build/ other than .gitignore
em++ ../cpp/main.cpp ../cpp/node.cpp --bind -s WASM=1 -std=c++17 -s -o main.js || exit 1
mv main.js ../web/gen/
mv main.wasm ../web/gen/