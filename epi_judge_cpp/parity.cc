#include "test_framework/generic_test.h"

//Computes the parity of an unsigned 64-bit number by starting at the LSB
//and moving to the MSB switching the parity every time a 1 bit is encountered.
uint8_t parity1(uint64_t num) {
    uint8_t parity{0};
    while (num) {
        //(num & 0x1) returns LSB of num. XOR switches parity when 1 encountered.
        parity ^= (num & 0x1);
        //remove the LSB from num
        num >>= 0x1;
    }
    return parity;
}

//Computes the parity of an unsigned 64-bit number starting from the LSB and moving
//to the MSB but only considering digits which are 1. This saves time compared to parity1
// which considers every bit even though only a 1 can switch the parity.
uint8_t parity2(uint64_t num) {
    uint8_t parity{0};
    while (num) {
        //since while(num) computed to true there must be another 1 bit in num so switch parity.
        parity ^= 0x1;
        //we subtract 1 from num which will need to borrow. It looks from the right for the least
        //significant 1. This bit will become 0 but the borrow will cause a propagation of 1 bits
        //to the right of the bit we are considering. To prevent this we and the new result with
        //the old one. This will prevent all those ones from being used in the next subtraction.
        num &= (num - 1);
    }
    return parity;
}

//If every 1 in the bit representation of an integer has a match then the parity is 0 else 1.
//We successively eliminate those bits that we can confirm have a match by XORing the first half of
//the bits with the second half. The number of 1 bits in the new number represent the bits that
//we still need to match in order for the parity to be even. So we again divide the number in
//half to match up the bits and eliminate those that are matched. Proceed until there is only 1 bit
//left. If it is 1 then we could not find a match for a single bit. If it is 0 then all bits were matched.
uint8_t parity3(uint64_t num) {
    //The right shifts return the left most half bits. The XOR compares the bits on the left
    //of num to the bits on the right of num. Then we only consider the right half
    num ^= (num >> 32);
    num ^= (num >> 16);
    num ^= (num >> 8);
    num ^= (num >> 4);
    num ^= (num >> 2);
    num ^= (num >> 1);
    //we only the last bit of num so and num with 0x1. We also return a uint8_t so cast num
    return static_cast<uint8_t>(num & 0x1);
}

//creates a struct which contains an array which contains the parity of all numbers from
//0 to numeric_limits<uint16_t>::max(). All of this can be done in compile time with constexpr
struct constArray {
    static constexpr uint32_t size = 0xFFFF + 1;
    uint8_t precomputedParity[size];

    constexpr constArray() : precomputedParity() {
        for (uint32_t i = 0; i < size; i++)
            precomputedParity[i] = parity(static_cast<uint16_t>(i));
    };

    static constexpr uint8_t parity(uint16_t num) {
        num ^= (num >> 8);
        num ^= (num >> 4);
        num ^= (num >> 2);
        num ^= (num >> 1);
        return uint8_t(num & 0x1);
    }
};

//creates an instance of consArray at compile time with constexpr
constexpr constArray table{};

//calculates parity of a uint16_t at compile time with constexpr
constexpr auto parity(uint16_t num) {
    num ^= (num >> 8);
    num ^= (num >> 4);
    num ^= (num >> 2);
    num ^= (num >> 1);
    return uint8_t(num & 0x1);
}

constexpr auto size = std::numeric_limits<uint16_t>::max() + 1;

//creates a std::array<uint8+t, size> which contains the parity of all numbers from
//0 to numeric_limits<uint16_t>::max() at compile time with constexpr
constexpr auto precomputeParity() {
    std::array<uint8_t, size> precomputedParity{};
    for(uint32_t i = 0; i < size; i++)
        const_cast<uint8_t&>(static_cast<const std::array<uint8_t, size>&>(precomputedParity)[i]) =
                parity(static_cast<uint16_t>(i));
    return precomputedParity;
}

constexpr std::array<uint8_t, size> precomputedParity = precomputeParity();

//computes the parity of all of the 16-bit subsections of the 64-bit number. The results
//are then XORed together. Its obvious why this returns the correct parity of the entire number.
uint8_t parity4(uint64_t num) {
    return precomputedParity[num >> 48] ^                 //this computes parity of most significant 16 bits
           precomputedParity[(num >> 32) & 0xFFFF] ^     //this computes parity of 2nd most significant 16 bits
           precomputedParity[(num >> 16) & 0xFFFF] ^     //this computes parity of 3rd most significant 16 bits
           precomputedParity[num & 0xFFFF];              //this computes the parity of least significant 16 bits
}

//does the same thing as the above function but uses an array instead of std::array.
uint8_t parity5(uint64_t num) {
    //arr cannot change what it points to. We use this pointer because we don't want to
    //dereference table each time to access the array it contains.
    const uint8_t* arr = table.precomputedParity;
    return arr[num >> 48] ^
           arr[(num >> 32) & 0xFFFF] ^
           arr[(num >> 16) & 0xFFFF] ^
           arr[num & 0xFFFF];
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"x"};
    GenericTestMain(args, "parity.cc", "parity.tsv", &parity1,
                  DefaultComparator{}, param_names);
    GenericTestMain(args, "parity.cc", "parity.tsv", &parity2,
                  DefaultComparator{}, param_names);
    GenericTestMain(args, "parity.cc", "parity.tsv", &parity3,
                    DefaultComparator{}, param_names);
    GenericTestMain(args, "parity.cc", "parity.tsv", &parity4,
                    DefaultComparator{}, param_names);
    GenericTestMain(args, "parity.cc", "parity.tsv", &parity5,
                    DefaultComparator{}, param_names);
    return 0;
}
