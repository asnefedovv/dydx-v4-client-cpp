C++ client for dYdX (v4 API).

## Development

Install dependencies, on Ubuntu the commands is:

```
sudo apt install libprotobuf-dev protobuf-compiler
```

The project uses CMake. To build the library and examples run

```
mkdir build
cd build
cmake ..
make
```

Note that running CMake will take a while because it downloads all the other dependenices.

## Getting Started

Sample code is located in `examples` folder

## Running examples

Build the corresponding target in CMake and run the binary, e.g.

```
cd build
make dydx_v4_indexer_rest_client_example
./examples/indexer_rest_client/dydx_v4_indexer_rest_client_example
```

## Running tests

```
cd build
make test
```
