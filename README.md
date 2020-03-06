# Congested clique routing
Implementation and visualization of the [Optimal Deterministic Routing algorithm on the Congested Clique](https://people.mpi-inf.mpg.de/~clenzen/pubs/L13optimal.pdf)

## Getting started

[Install Emscripten](https://emscripten.org/docs/getting_started/downloads.html) and activate necessary environment variables in your terminal

Then run the build script

```console
./build.sh
```

Finally start local server with e.g.

```console
npx serve -d ./web/dist/
```

## Building C++ sources

Navigate to `cpp` directory and compile the sources using `make`

```console
cd ./cpp
make
```

Executing the compiled output will run several tests on both hardcoded and randomly generated inputs (see `test.cpp`).

## Developing web part

After navigating to `web` directory, install required dependencies

```console
npm install
```

To enable webpack to automatically rebuild on every change to source code run

```console
npm run watch
```

Finally, start-up a local server with e.g.

```console
npx serve -d ./dist
```

## Algorithm

An alternative description of the algorithm, which might be more useful for the purpose of implementation can be found [here](https://github.com/AleksTeresh/congested-clique-routing/blob/master/algorithm.Rmd)
