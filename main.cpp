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

int main()
{
    test_partition_stable_position();
    test_unweave();
    test_inshuffle();
}
