#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <cassert>
#include <chrono>
#include "permutation.h"

void test_partition_stable_position()
{
    using I = typename std::vector<int>::iterator;
    std::vector<int> a(10);
    std::iota(a.begin(), a.end(), 0);
    auto pred = [&a] (const I& x) { return odd(x - a.begin()); };
    auto x = partition_stable_position_n(a.begin(), a.size(), pred);
    std::cout << "partition_stable_position_n\n";
    std::copy(a.begin(), x.first, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    std::copy(x.first, x.second, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}

void test_unweave()
{
    using I = typename std::vector<int>::iterator;
    std::vector<int> a(10);
    std::iota(a.begin(), a.end(), 0);
    auto x = unweave_n(a.begin(), a.size());
    std::cout << "\nunweave\n";
    std::copy(a.begin(), x.first, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    std::copy(x.first, x.second, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}

void test_inshuffle()
{
    using I = typename std::vector<int>::iterator;
    std::vector<int> a(10);
    std::iota(a.begin(), a.end(), 0);
    unweave_n(a.begin(), a.size());
    I l = inshuffle_n(a.begin(), a.size());
    std::cout << "\ninshuffle\n";
    std::copy(a.begin(), l, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}

void remove_if_example(bool stable)
{
    int a[] = { 1, 3, 2, 4, 7, 5, 6, 9, 8, 0 };
    int* last = std::end(a);
    const auto p = [] (int x) { return x % 2 == 0; };
    if (stable)
        last = std::remove_if(std::begin(a), last, p);
    else
        last = ::remove_if_unstable(std::begin(a), last, p);

    std::copy(a, last, std::ostream_iterator<int>(std::cout, " "));
}

int main()
{
    test_partition_stable_position();
    test_unweave();
    test_inshuffle();

    std::cout << "\nremove stable\n";
    remove_if_example(true);

    std::cout << "\nremove unstable\n";
    remove_if_example(false);

    std::cout << '\n';
}
