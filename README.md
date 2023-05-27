# Kleprlačka

> a matrix calculator with infix expression evaluation

# Usage
The calculator accepts a JSON config file as an optional command line argument. An example
config file can be found in `examples/config.json`. If no configuration file is 
specified, default values are used.

The calculator supports following operations, with a 3x3 matrix used as an example:
```
----------
A = [[1, 0, 0], [0, 1, 0], [0, 0, 1]] // declaring a matrix inline
----------
SCAN B
[1, 2, 3]
[4, 5, 6]
[7, 8, 9]
// an empty line signals the end of the matrix
----------
C = A + B // matrix addition
----------
D = A - B // matrix subtraction
----------
E = A * B // matrix multiplication
----------
F = INV A // matrix inverse calculation
----------
G = DET A // determinant calculation
----------
H = RANK A // rank calculation
----------
I = TRANSPOSE A // matrix transposition
----------
J = UNITE A B // matrix unification, stacks A on top of B 
----------
K = GAUSS A // gaussian elimination to row echelon form
----------
L = CUT A 2 2 1 1 // extracts a 2x2 sub-matrix from A, starting at offset 1x1
                  //       <rows>x<columns>                          <row> x<column>
----------
PRINT A // prints the matrix to standard output
----------
IMPORT example.json // imports matrices from a json file
----------
EXPORT example.json // exports currently stored variables to a json file
----------
```

If the result is not assigned to a variable, it gets printed to standard output instead:
```
>>> [[1, 1], [1, 1]] + [[2, 2], [2, 2]]
// [3, 3]
// [3, 3]
```

Exit the app with:

```
Ctrl+C
```

# Building from sources

> The calculator can be build from sources using:

```
make compile
```

# Starting the application

> To start the calculator, simply type:

```
make run
```

# Documentation

> documentation for the sources can be generated using:

```
make doc
```
