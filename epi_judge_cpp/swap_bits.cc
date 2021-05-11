#include "test_framework/generic_test.h"
uint64_t swapBits(uint64_t num, uint8_t i, uint8_t j) {
    //check that the values at the indices are not the same
    if(((num >> i) & 0x1) != ((num >> j) & 0x1)) {
        //create an unsigned long number where the bits at positions i and j are 1 and all
        //other positions are 0. Now we XOR the original number with this new number we created.
        //1 XOR 1 = 0 and 0 XOR 1 = 1 so the bits will flip. If the bits were both the same then
        //you would have 0 XOR 1 = 1 which is not a swap or 1 XOR 1 = 0 which is also not a swap.
        num ^= (1UL << i) | (1UL << j);
    }
    return num;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "i", "j"};
  return GenericTestMain(args, "swap_bits.cc", "swap_bits.tsv", &swapBits,
                         DefaultComparator{}, param_names);
}
