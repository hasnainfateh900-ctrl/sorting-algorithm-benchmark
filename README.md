# Sorting Algorithm Benchmark

A C++ command-line program for comparing the measured runtime of six sorting algorithms across different input sizes.

## Algorithms

- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quicksort
- Shell Sort

## What it demonstrates

- Implementing and comparing classic algorithms
- Generating test datasets and measuring execution time
- Repeating runs and calculating average runtime
- Relating observed behavior to time-complexity analysis

## Build

Requires a C++17-compatible compiler.

### Windows (MinGW-w64)

```powershell
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o sorting_benchmark.exe
.\sorting_benchmark.exe
```

### macOS / Linux

```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o sorting_benchmark
./sorting_benchmark
```

Use the program's menu to generate datasets before running the benchmark options. Dataset files are generated locally and are intentionally excluded from version control because they can be large.

## Benchmarking notes

For fair comparisons, build with optimizations enabled (`-O2`), run on the same machine, and avoid other CPU-heavy work during measurements. Run the same test several times and report averages. Do not compare timings across different machines as if they were directly equivalent.

## Before presenting results

- Run all menu options on your own machine.
- Save verified output in a small `results/` folder or a CSV file.
- Include machine specifications, compiler version, and build flags.
- Explain any unexpectedly slow runs, especially for quadratic-time algorithms on large inputs.

## Limitations / next improvements

- Add automated correctness tests for every algorithm.
- Separate dataset generation, algorithm implementations, and the CLI into separate files.
- Export results to CSV and add a script to plot runtime versus input size.
- Guard against invalid menu input and failed file writes.
