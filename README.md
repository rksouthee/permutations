# Permutations

In [From Mathematics to Generic Programming](https://www.fm2gp.com) a
permutation is defined as a function of n objects onto itself. In this library
I hope to collect a variety of useful permutations.

## Remove Unstable

The C++ Standard Template Library provides the algorithm
[`std::remove_if`](https://en.cppreference.com/w/cpp/algorithm/remove). This
algorithm is stable, that is the relative order of the remaining elements is
the same as it was before. This is a useful property and allows the algorithm
to be implemented using forward iterators. However it could be more efficient
by dropping the requirement of stability, this now requires that the iterators
are bidirectional.

## In-Shuffle

I came across a problem where it was necessary to split an interleaving of left
and right audio channels into a sequence of left channels and a sequence of
right channels in-place.  That is I wanted to transform something like this:

```txt
L0 R0 L1 R1 L2 R2 L3 R3 L4 R4
```

into

```txt
L0 L1 L2 L3
R0 R1 R2 R3
```

and to reverse this permutation.

Fortunately I came across [A Simple In-Place Algorithm for
In-Shuffle](https://arxiv.org/abs/0805.1598) by Peiyush Jain. I implemented the
algorithms, this implementation is iterative as opposed to the recursive one
described by Jain.

## Future Work

I'd like to benchmark the algorithms. In the case of the in-shuffle, I would like to compare it
against an algorithm that gets use to extra memory.

It will be useful to count the number of operations performed by each algorithm.
