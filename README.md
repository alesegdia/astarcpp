# astarcpp

Simple A\* implementation in C++ using templates.

## Dependencies

* CMake (building)
* SDL2 and cairo (graphic test)

## Build and run tests

```
cd /path/to/astarcpp/
mkdir build && cd build
cmake .. -DASTARCPP_ENABLE_TESTS=1
make -j8
# tests are here -> /path/to/astarcpp/bin/
```

## Tests

* **tilemap-cairo-test**: press space to step the algorithm
* **basic-search-test**: counts processed nodes (to improve, add more stats)
