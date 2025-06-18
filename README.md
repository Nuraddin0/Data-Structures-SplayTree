# Data Structures - Splay Tree

This repository contains an implementation of the Splay Tree data structure in C. Splay trees are a type of self-adjusting binary search tree with the additional property that recently accessed elements are quick to access again.

## Features

- Splay Tree structure with frequency-based and standard splaying
- Insert operation with automatic splaying
- Pre-order traversal output
- Frequency count for repeated elements
- Comparison of splay and modified splay performance
- Simple file I/O for batch operations

## Files

- `main.c` &mdash; Main implementation and demo program
- `splaytree.h` &mdash; (Recommended) Declarations for splay tree structure and functions
- `input.txt` &mdash; Input file with data to insert into the tree (comma-separated integers)
- `output.txt` &mdash; Output file with traversal and timing results

## How to Use

1. **Compile the code**  
   Use a C compiler such as gcc:
   ```sh
   gcc -o main main.c
   ```

2. **Prepare input**  
   Create an `input.txt` file in the same directory. List integers separated by commas, e.g.:
   ```
   10,5,20,10,15,25,
   ```

3. **Run the program**  
   ```sh
   ./main
   ```
   Output will be written to `output.txt`.

## Example

Given an `input.txt`:
```
10,5,20,10,15,25,
```

The `output.txt` will contain:
```
Pre-order demonstration of splay tree: (10,1) (5,0) (20,0) (15,0) (25,0) 
Comparison time of splay tree: 8 

Pre-order demonstration of mod-splay tree: (10,1) (5,0) (20,0) (15,0) (25,0) 
Comparison time of mod-splay tree: 7
```

## Structure Overview

- **StNode** — Node structure with data, frequency, parent, left, and right pointers
- **Insert** — Inserts a node and updates frequency if duplicate
- **Splay** — Brings the last accessed node to the root using rotations
- **preorder** — Prints a pre-order traversal to the output file

## Authors

- [Nuraddin0](https://github.com/Nuraddin0)
- [devran-hub](https://github.com/devran-hub)
