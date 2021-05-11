#include <string>

#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::string;

string IntToString(int x) {
    long long num{x};       //std::numeric_limits<int>::max() == 2147483647. So -2147483648 == -2147483648. Just cast to take care of this problem.
    if(!num) return "0";
    bool negative{num < 0};
    if(negative) num = -num;
    string result{};
    while(num) {
      result.push_back('0' + num % 10);
      num /= 10;
    }
    if(negative) result.push_back('-');
    reverse(result.begin(), result.end());
    return result;
}
int StringToInt(const string& s) {
    bool negative{s[0] == '-'};
    int result{};

    for(int i = negative; i < s.size(); i++) {
      if(isdigit(s[i]))
          result = result * 10 + s[i] - '0';
    }
    return negative ? -result : result;
}

void Wrapper(int x, const string& s) {
  if (stoi(IntToString(x)) != x) {
    throw TestFailure("Int to string conversion failed");
  }

  if (StringToInt(s) != x) {
    throw TestFailure("String to int conversion failed");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "s"};
  return GenericTestMain(args, "string_integer_interconversion.cc",
                         "string_integer_interconversion.tsv", &Wrapper,
                         DefaultComparator{}, param_names);
}
