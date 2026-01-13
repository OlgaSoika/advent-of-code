# Advent of Code 2025 – Day 8

**Problem:** https://adventofcode.com/2025/day/8

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
./aoc_solution --filename input.txt
./aoc_solution_part2 --filename input.txt
```
- CMake build (binaries in build/):
```bash
./build/aoc_solution --filename input.txt
./build/aoc_solution_part2 --filename input.txt
```
Input format
- The program will prompt for the number of cables to process
- Default value: 10 (for testing with input1.txt)
- Puzzle input: 1000 (for full input.txt)
- Press Enter to use default, or type a custom number
