# Practical Sorting of Encrypted Numbers in Approximate Homomorphic Encryption
<center>
<img src="imgs/console_noback.png" alt="Console presentation image" width=92% >
</center>

This repository contains the source code for the work-in-progress paper *Practical Sorting of Encrypted Numbers in Approximate Homomorphic Encryption*. With this code it is possibly to sort a vector of encrypted values in reasonable time.

The idea is to use a [Sorting Network](https://en.wikipedia.org/wiki/Sorting_network), which is an *oblivious* algorithm whose behavior does not depend on the input. The network is implemented using the [OpenFHE](https://eprint.iacr.org/2022/915) implementation of the [RNS-CKKS scheme](https://eprint.iacr.org/2020/1118)

##
