#include <string>

#include "test_framework/generic_test.h"
using std::string;

//convert a string that represents a number in base b1 to a string that represents a number in b2
string ConvertBase(const string& num_as_string, int b1, int b2) {
    if(num_as_string.empty()) return "";
    bool negative{num_as_string[0] == '-'};
    string result{};
    int x{};
    for(int i = negative; i < num_as_string.size(); i++) {
        if(isdigit(num_as_string[i]))
            x = x * b1 + num_as_string[i] - '0';
        else
            x = x * b1 + num_as_string[i] - 'A' + 10;
    }
    if(!x) result = "0";
    while(x) {
        char digit = x % b2;
        result.push_back(digit > 9 ? 'A' + digit - 10 : '0' + digit);
        x /= b2;
    }
    if(negative) result.push_back('-');
    reverse(result.begin(), result.end());
    return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_as_string", "b1", "b2"};
  return GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                         &ConvertBase, DefaultComparator{}, param_names);
}
