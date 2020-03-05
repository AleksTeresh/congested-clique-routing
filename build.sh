#!/bin/sh

SRC_CPP_FILES="
  ../cpp/test.cpp
  ../cpp/node.cpp
  ../cpp/clique-router.cpp
  ../cpp/color.cpp
  ../cpp/emscripten-bindings.cpp"
EMSCRIPTEN_FLAGS="
  -s WASM=1
  -s MODULARIZE=1
  -s EXPORT_NAME=\"InitRuntime\"
  -s ENVIRONMENT=web
  -s -o main.js"

mkdir -p ./build
cd build
em++ $SRC_CPP_FILES --bind -std=c++17 $EMSCRIPTEN_FLAGS || exit 1
mkdir -p ../web/dist/
mv main.js ../web/dist/
mv main.wasm ../web/dist/

cd ../web
npm run build
