#include "test_framework/generic_test.h"


//reverses the bits of an unsigned 64-bit number by starting from the LSB of num
//and appending it to the right of result. We then shift num 1 to the right to eliminate
//the digit we processed and decrement LSBpos. Once we run out of 1 digits to process on
//num we shift over the digits of result to the left so that the first digit we added to
//result sits on the very left of the number.
uint64_t reverseBits1(uint64_t num) {
    uint64_t result{};
    uint8_t LSBpos{sizeof(uint64_t) * 8};
    while(num) {
        result <<= 0x1;
        result |= (num & 0x1);
        num >>= 0x1;
        LSBpos--;
    }
    return result << LSBpos;
}

constexpr auto size = std::numeric_limits<uint16_t>::max() + 1;

//Computes the reverse of a uint16-bit number at compile time with constexpr. Recursion must be
//used instead of loops in constexpr. This is the recursive implementation of reverseBits1.
constexpr uint16_t reverse(uint16_t num, uint16_t result = 0, uint8_t LSBpos = sizeof(uint16_t) * 8) {
    return num ? reverse(num >> 0x1, uint16_t(result << 0x1 | (num & 0x1)), --LSBpos) : result << LSBpos;
}

//returns an std::array of the reverse of the bits of every uint16_t bit number at compile time
//with constexpr.
constexpr auto precomputeReverse() {
    std::array<uint16_t, size> precomputedReverse{};
    for(uint32_t i = 0; i < size; i++)
        const_cast<uint16_t&>(static_cast<const std::array<uint16_t, size>&>(precomputedReverse)[i]) = reverse(
                static_cast<uint16_t>(i));
    return precomputedReverse;
}

constexpr auto precomputedReverse = precomputeReverse();

//computes the reverse of the bits of a 64-bit number by using a table of the reverse of
//all 16-bit number calculated at compile time. We cast to uint64_t after we dereference
//precomputedReverse because the array contains 16 bit numbers but we are shifting to the
//left by more than that which would eliminate all digits in the number.
uint64_t reverseBits2(uint64_t num) {
    return uint64_t(precomputedReverse[num >> 48]) |
           uint64_t(precomputedReverse[(num >> 32) & 0xFFFF]) << 16 |
           uint64_t(precomputedReverse[(num >> 16) & 0xFFFF]) << 32 |
           uint64_t(precomputedReverse[num & 0xFFFF]) << 48;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"x"};
    GenericTestMain(args, "reverse_bits.cc", "reverse_bits.tsv",
                    &reverseBits1, DefaultComparator{}, param_names);
    GenericTestMain(args, "reverse_bits.cc", "reverse_bits.tsv",
                    &reverseBits2, DefaultComparator{}, param_names);
    return 0;
}
