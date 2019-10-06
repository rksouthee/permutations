#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <algorithm>
#include <iterator>
#include <utility>

template <typename I>
using DistanceType = typename std::iterator_traits<I>::difference_type;

template <typename I>
using ValueType = typename std::iterator_traits<I>::value_type;

template <typename I, typename P, typename N>
// requires ForwardIterator(I) && UnaryPredicate(P) && Integer(N)
std::pair<I, I> partition_stable_position_n_nonempty(I f, N n, P p)
{
    // precondition: n > 0
    if (n == N(1)) return std::make_pair(f + p(f), f + 1);
    auto h = n / N(2);
    auto x = partition_stable_position_n_nonempty(f, h, p);
    auto y = partition_stable_position_n_nonempty(x.second, n - h, p);
    return std::make_pair(std::rotate(x.first, x.second, y.first), y.second);
}

template <typename I, typename P, typename N>
// requires ForwardIterator() && UnaryPredicate(P) && Integer(N)
std::pair<I, I> partition_stable_position_n(I f, N n, P p)
{
    if (n == N(0)) return std::make_pair(f, f);
    return partition_stable_position_n_nonempty(f, n, p);
}

template <typename I, typename P>
// requires ForwardIterator(I) && UnaryPredicate(P)
I partition_stable_position(I f, I l, P p)
{
    return partition_stable_position_n(f, l - f, p).first;
}

template <typename I, typename P>
I find_if_not_backward(I f, I l, P p)
{
    while (true) {
        if (l == f) return f;
        --l;
        if (!p(*f)) return ++l;
    }
}

template <typename I, typename P>
// requires BidirectionalIterator(I) && UnaryPredicate(P)
I remove_if_unstable(I f, I l, P p)
{
    while (true) {
        f = std::find_if(f, l, p);
        if (f == l) return f;
        l = find_if_not_backward(f, l, p);
        if (l == f) return l;
        *f++ = std::move(*--l);
    }
}

template <typename N>
N largest_power_of_three_minus_one(N n)
{
    // precondition: n > 1
    N x{2};
    n = (n - 2) / 3;
    while (x <= n) x = x * 3 + 2;
    return x;
}

template <typename I>
bool odd(const I& x) { return (x & 0x1) != 0; }

template <typename I>
I twice(const I& x) { return x << I(1); }

template <typename I>
I half_nonnegative(const I& x) { return x >> I(1); }

template <typename I, typename F>
void rotate_cycle_from(I i, F from)
{
    using T = ValueType<I>;
    T tmp = std::move(*i);
    I start = i;
    for (I j = from(i); j != start; j = from(j)) {
        *i = std::move(*j);
        i = j;
    }
    *i = std::move(tmp);
}

template <typename I, typename F>
void rotate_cycles(I f, DistanceType<I> n, F from)
{
    // n == 3^k - 1
    using N = DistanceType<I>;
    N i(0);
    do {
        rotate_cycle_from(f + i, from);
        i = i * N(3) + N(2);
    } while (i != n);
}

template <typename I>
// requires RandomAccessIterator(I)
struct inshuffle_rotator {
    using N = DistanceType<I>;
    I f;
    N half_n;

    inshuffle_rotator(I f, N n) :
        f(f), half_n(half_nonnegative(n)) { }

    I operator()(const I& x) const
    {
        N d = x - f;
        if (odd(d)) d = half_nonnegative(d);
        else        d = half_nonnegative(d) + half_n;
        return f + d;
    }
};

/*
 * Peiyush Jain, Microsoft Corporation.
 * A Simple In-Place Algorithm for In-Shuffle.
 * July 2004
 * https://arxiv.org/abs/0805.1598v1
 */
template <typename I>
// requires RandomAccessIterator(I)
I inshuffle_n(I f, DistanceType<I> n)
{
    using N = DistanceType<I>;
    if (n < N(2)) return f + n;
    N m = largest_power_of_three_minus_one(n);
    while (true) {
        N half_n = half_nonnegative(n);
        N half_m = half_nonnegative(m);
        I l = std::rotate(f + half_m, f + half_n, f + half_n + half_m);
        rotate_cycles(f, m, inshuffle_rotator<I>(f, m));
        n = n - m;
        if (n < N(2)) return l + n;
        f = l;
        while (m > n) m = m / N(3);
    }
}

template <typename I>
// requires RandomAccessIterator(I)
void inshuffle(I f, I l)
{
    inshuffle_n(f, l - f);
}

template <typename I>
struct unweave_rotator {
    using N = DistanceType<I>;
    I f;
    N half_n;

    unweave_rotator(I f, N n) :
        f(f), half_n(half_nonnegative(n)) { }

    I operator()(const I& x) const
    {
        N d = x - f;
        if (d < half_n) d = twice(d) + N(1);
        else            d = twice(d - half_n);
        return f + d;
    }
};

template <typename I>
std::pair<I, I> unweave_n(I f, DistanceType<I> n)
{
    using N = DistanceType<I>;
    if (n < N(2)) return std::make_pair(f, f + n);
    N t = largest_power_of_three_minus_one(n);
    rotate_cycles(f, t, unweave_rotator<I>(f, t));
    I m = f + t;
    f = f + half_nonnegative(t);
    n = n - t;
    while (n > N(1)) {
        while (t > n) t = t / N(3);
        rotate_cycles(m, t, unweave_rotator<I>(m, t));
        f = std::rotate(f, m, m + half_nonnegative(t));
        m = m + t;
        n = n - t;
    }
    return std::make_pair(f, m);
}

template <typename I>
// requires RandomAccessIterator(I)
I unweave(I f, I l)
{
    return unweave_n(f, l - f).first;
}

#endif
