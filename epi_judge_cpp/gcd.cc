#include "test_framework/generic_test.h"
#include <algorithm>

bool odd(long long x) { return x & 0x1; }

long long half(long long x) { return x >> 1; }

//3 theorems to know relatively easy to prove which make sense of algorithm.
//Theorem 1: x even y even ==> gcd(a, b) = 2 * gcd(a/2, b/2)
//Theorem 2: x even y odd ==> gcd(x, y) = gcd(x/2, y)
//Theorem 3: x > y ==> gcd(x, y) = gcd(y, x-y)
long long Gcd(long long x, long long y) {
    if(!(x and y)) return std::max(x, y);
    if(!odd(x) && !odd(y)) return 2 * Gcd(half(x), half(y));
    if(!odd(x)) return Gcd(half(x), y);
    if(!odd(y)) return Gcd(x, half(y));
    return x < y ? Gcd(x, y - x) : Gcd(y, x - y);
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "gcd.cc", "gcd.tsv", &Gcd, DefaultComparator{},
                         param_names);
}
