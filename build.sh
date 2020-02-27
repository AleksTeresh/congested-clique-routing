cd build
find . ! -name '.gitignore' -type f -exec rm -f {} + # delete all files from ./build/ other than .gitignore
em++ ../cpp/main.cpp ../cpp/node.cpp --bind -s WASM=1 -s MODULARIZE=1 -s 'EXPORT_NAME="InitRuntime"' -s ENVIRONMENT=web -std=c++17 -s -o main.js || exit 1
mkdir -p ../web/dist/
mv main.js ../web/dist/
mv main.wasm ../web/dist/

cd ../web
npm run build
