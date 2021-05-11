#include "test_framework/generic_test.h"

//start from the LSB and move right. When two bits are encountered that are not the same
//swap them.
unsigned long long ClosestIntSameBitCount1(unsigned long long x) {
    for(int i = 0; i < 63; i++) {
      if(((x >> i) & 0x1) xor ((x >> (i + 1)) & 0x1)) {
          return x xor (0b11UL << i);
      }
    }
    return 0;
}

//the subtraction hunts for the first 1. When it finds it that bit becomes 0 and all the bits
//to the right of it become 1. We not the result which makes the 0 bit 1 and all those prapogated
//1 bits 0. Then all other digits of x become their inverse. We and with the original x which
//produces a number that is all 0 except for first set bit in x.
unsigned long long firstSetBit(unsigned long long x) {
    return x & ~(x - 1);
}

//the addition hunts for the first 0. When it finds it that bit becomes 1 and all the bits
//to the right of it become 0. We not x which takes that bit that was 0 before the addition
//and makes it a 1. We then and the two numbers together which produces a number that is 0
//in all bits except the first bit which is set to 0 which will be a 1.
unsigned long long firstUnsetBit(unsigned long long x) {
    return ~x & (x + 1);
}

//finds the first bit which is not set and the first bit which is set. return the larger of the two
//where the set and unset positions are denoted as an unsigned long number with 1 in the critical bit
//position and 0 elsewhere. we know that the previous bit must be opposite so swap them and then return.
unsigned long long ClosestIntSameBitCount2(unsigned long long x) {
    unsigned long cb2 {std::max(x & ~(x - 1), ~x & (x + 1))};
    return x xor (cb2 | cb2 >> 1);
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"x"};
    return GenericTestMain(args, "closest_int_same_weight.cc",
                         "closest_int_same_weight.tsv", &ClosestIntSameBitCount2,
                         DefaultComparator{}, param_names);
}
