# Practical Sorting of Encrypted Numbers in Approximate Homomorphic Encryption
<center>
<img src="imgs/console_noback.png" alt="Console presentation image" width=92% >
</center>

<img src="https://github.com/narger-ef/Practical-Sorting-of-Encrypted-Numbers/actions/workflows/cmake-multi-platform.yml/badge.svg" alt="CMake build result" width=30% >

---

This repository contains the source code for the work-in-progress paper *Practical Sorting of Encrypted Numbers in Approximate Homomorphic Encryption*. With this code it is possibly to sort a vector of encrypted values in "reasonable" time (e.g., few minutes for more than one thousand elements).

The idea is to use a [Sorting Network](https://en.wikipedia.org/wiki/Sorting_network), which is an *oblivious* algorithm whose behavior does not depend on the input. The network is implemented using the [OpenFHE](https://eprint.iacr.org/2022/915) implementation of the [RNS-CKKS scheme](https://eprint.iacr.org/2020/1118)


## Requirements
It is pretty straightforward to use this source code, first of all **be sure** that all requirements are satisfied.

- cmake
- OpenFHE (tested on [v1.1.4](https://github.com/openfheorg/openfhe-development/releases/tag/v1.1.4))

## How to use it

First of all clone the repository:

```
git clone https://github.com/narger-ef/Practical-Sorting-of-Encrypted-Numbers
```

Then, make a `build` folder and build the project with `cmake`
```
mkdir build
cmake -B "build" -S LowMemoryFHEResNet20
```
Before launching the program, go to the just created folder:

```
cd build
```

Now it is possible to launch the program! Notice that, before doing that, at least one argument is required:

### Required arguments:
One of the following two arguments is required in order to correctly give the input to the circuit.

- To use as input a random vector of values, use `--random n` where `n` is a power of two. For example:
```
./Sort --random 8
```

- To use as input a file, use `--file FILENAME` where `FILENAME` is the selected file. For example:
```
./Sort --file "../inputs/four_values.txt"
```

- Alternatively, you can provide a vector directly by enclosing it in square brackets, for example: `"[0.5, 0.12, 0.71, 0.42]"`. Notice that, in this case, the values $v_i$ in the vector $v$ must be $v_i \in [0, 1]$ and the length of the vector $|v|$ must be a power of two. For example:

```
./Sort "[0.5, 0.12, 0.71, 0.42]"
```

### Optional arguments:

It is also possible to change the behavior of the program by using some optional argument:

- `--toy_parameters`: with this argument the cryptosystem will not have the minimum requirement of $\lambda = 128$ security bits against classical computers. Suggested in case you want to play around with the algorithm. For example:
  
```
./Sort --random 8 --toy_parameters
```
- `--verbose`: when this argument is present, the program will give more information to the user about what is happening behind the scenes. For example:

```
./Sort --random 8 --toy_parameters --verbose
```
- `--poly_degree`: the degree of the Chebyshev polynomial approximating the ReLU function. Suggested values, from [here](https://github.com/openfheorg/openfhe-development/blob/main/src/pke/examples/FUNCTION_EVALUATION.md): 119, 247, 495

```
./Sort --random 8 --poly_degree 247 --verbose
```



## How it works
