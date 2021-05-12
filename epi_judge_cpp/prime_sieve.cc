#include <vector>
#include <cmath>
#include "test_framework/generic_test.h"
using std::vector;
// Given n, return all primes up to and including n.
vector<int> GeneratePrimes2(int n) {
    if(n < 2) return {};
    if(n == 2) return {2};
    vector<int> primes{2};
    int size{(n-1)/2};
    int sqrtVal{static_cast<int>(sqrt(n))};
    int sqrtIndex{std::max(0, (sqrtVal - 3)/2)};
    vector<short> sieve(size, 1);
    long i{0};
    for(; i <= sqrtIndex; i++) {
        if(sieve[i]) {
            int p{static_cast<int>((i << 1) + 3)};
            primes.push_back(p);
            for(long j = (i << 1) * (i+3) + 3; j < size; j+=p)
                sieve[j] = 0;
        }
    }
    for(; i < size; i++)
        if(sieve[i]) primes.push_back(static_cast<int>((i << 1) + 3));
    return primes;
}

vector<int> GeneratePrimes(int n) {
    if(n < 2) return {};
    int size{(n-1)/2};
    vector<int> primes{2};
    vector<short> sieve(size, 1);

    for(long i = 0; i < size; i++) {
        if(sieve[i]) {
            int p{static_cast<int>((i << 1) + 3)};
            primes.push_back(p);
            for(long j = ((i*i) << 1) + 6*i + 3; j < size; j+=p)
                sieve[j] = 0;
        }
    }
    return primes;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "prime_sieve.cc", "prime_sieve.tsv",
                         &GeneratePrimes2, DefaultComparator{}, param_names);
}
