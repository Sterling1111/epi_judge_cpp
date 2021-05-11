#include <vector>

#include "test_framework/generic_test.h"
using std::vector;
// Given n, return all primes up to and including n.
vector<int> GeneratePrimes(int n) {
    if(n < 2) return {};
    vector<int> primes{2};
    vector<uint8_t> sieve((n-1)/2, 1);
    for(long i = 0; i < sieve.size(); i++) {
        if(sieve[i]) {
            int p = (i << 1) + 3;
            primes.push_back(p);
            for(long j = ((i*i) << 1) + 6*i + 3; j < sieve.size(); j+=p)
                sieve[j] = 0;
        }
    }
    return primes;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "prime_sieve.cc", "prime_sieve.tsv",
                         &GeneratePrimes, DefaultComparator{}, param_names);
}
