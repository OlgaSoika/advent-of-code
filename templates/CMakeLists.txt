# Advent of Code <YEAR> – Day <DAY>

**Problem:** https://adventofcode.com/<YEAR>/day/<DAY>

How to build
- Single-file (g++):
```bash
g++ -std=c++20 -O2 -o aoc_solution solution.cpp
g++ -std=c++20 -O2 -o aoc_solution_part2 solution_part2.cpp
```
- With CMake (no cd):
```bash
cmake -S . -B build
cmake --build build
```

How to run
- Single-file build:
```bash
./aoc_solution < input.txt
./aoc_solution_part2 < input.txt
```
- CMake build (binaries in build/):
```bash
./build/aoc_solution < input.txt
./build/aoc_solution_part2 < input.txt
```
