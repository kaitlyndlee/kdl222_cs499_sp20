#include <deepstate/DeepState.hpp>

// #include <string>
using namespace deepstate;

char lowercase(char c) {
    if(c >= 'A' && c <= 'Z')
        return 'a' + (c - 'A');
    else
        return c;
}

char strcmp_nocase(const char* a, const char* b) {
    while(lowercase(*a) == lowercase(*b)) {
        if(*a == 0)
            return 0;
        ++a; ++b;
    }

    return *a-*b;
}

TEST(Fuzzingwins, StrTest) {
  char* inputString = DeepState_CStr_C(128, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
  LOG(TRACE) << "String: " << inputString;
  ASSERT_NE(strcmp_nocase(inputString, "i like pizza"), 0);
}

