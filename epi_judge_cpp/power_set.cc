#include <vector>

#include "test_framework/generic_test.h"
using std::vector;

vector<vector<int>> GeneratePowerSet(const vector<int>& input_set) {
    vector<vector<int>> result{};
    unsigned long size{1UL << input_set.size()};    //there are 2^n elements in the P(input_set)
    for(unsigned long i = 0; i < size; i++) {
        unsigned long x{i};
        vector<int> subset{};
        //we care about the bit representation of x. Every 1 represents the position of the array to extract.
        //Start from the LSB = position 0 and move right to the MSB = 63
        while(x) {
            //x & ~(x - 1) gets us a number with all 0 except position of input_set we want which is 1
            //log2 gets us the position of the input_set to add
            subset.push_back(input_set.at(log2(x & ~(x - 1))));
            //we already added that elem in input_set associated with that 1 so remove it.
            x &= (x - 1);
        }
        result.push_back(subset);
    }
    return result;
}

void GeneratePowerSet2Recursive(const vector<int>& input_set, int index, vector<vector<int>>& powerSet, vector<int>& subset) {
    powerSet.push_back(subset);

    for(int i{index}; i < input_set.size(); i++) {
        subset.push_back(input_set.at(index));
        //Generate all subsets of input_set of which start with subset increasing order.
        GeneratePowerSet2Recursive(input_set, ++index, powerSet, subset);
        subset.pop_back();
    }
}

void GenerateSubsetsOfSizeNRecursive(const vector<int>& input_set, int index, int subsetSize, vector<vector<int>>& subsets, vector<int>& subset) {
    if(subset.size() == subsetSize) {
        subsets.push_back(subset);
        return;
    }
    for(int i{index}; i < input_set.size(); i++) {
        subset.push_back(input_set.at(index));
        //Generate all subsets of input_set of size subsetSize which start with subset increasing order.
        GenerateSubsetsOfSizeNRecursive(input_set, ++index, subsetSize, subsets, subset);
        subset.pop_back();
    }
}

vector<vector<int>> GenerateSubsetsOfSizeN(const vector<int>& input_set, int size) {
    vector<vector<int>> powerSet{};
    vector<int> subset{};
    //Generate all subsets of input_set of which start with subset increasing order.
    GenerateSubsetsOfSizeNRecursive(input_set, 0, size, powerSet, subset);
    return powerSet;
}

vector<vector<int>> GeneratePowerSet3(const vector<int>& input_set) {
    vector<vector<int>> powerSet{};
    vector<int> subset{};
    for(int i = 0; i <= input_set.size(); i++) {
        vector<vector<int>> subsets = GenerateSubsetsOfSizeN(input_set, i);
        for(const auto& elem : subsets)
            powerSet.push_back(elem);
    }
    return powerSet;
}


vector<vector<int>> GeneratePowerSet2(const vector<int>& input_set) {
    vector<vector<int>> powerSet{};
    vector<int> subset{};
    //Generate all subsets of input_set of which start with subset increasing order.
    GeneratePowerSet2Recursive(input_set, 0, powerSet, subset);
    return powerSet;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input_set"};
  return GenericTestMain(args, "power_set.cc", "power_set.tsv",
                         &GeneratePowerSet3, UnorderedComparator{}, param_names);
}
