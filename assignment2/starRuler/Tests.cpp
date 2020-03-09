#include <deepstate/DeepState.hpp>

#include "str_util.h"

#include <iostream>
#include <string>

using namespace deepstate;


int isUpper(std::string inputStr, std::string compareString) {
    std::string copyStr = inputStr;
    for (std::string::size_type i = 0; i < copyStr.length(); ++i) {
        copyStr[i] = std::toupper(copyStr[i]);
    }
    return copyStr == compareString;
}

int isLower(std::string inputStr, std::string compareString) {
    std::string copyStr = inputStr;
    for (std::string::size_type i = 0; i < copyStr.length(); ++i) {
        copyStr[i] = std::tolower(copyStr[i]);
    }
    return copyStr == compareString;
}

int stringReplaced(std::string inputStr, std::string compareString, std::string replace, int start, int end) {
    std::string copyStr = inputStr;
    size_t pos;
    while ((pos = copyStr.find(copyStr.substr(start, end - start))) != std::string::npos) {
        copyStr.replace(pos, end - start, replace);
    }
    return copyStr == compareString;
}

TEST(StrUtil, StrManipulation) {
    size_t stringSize = DeepState_SizeInRange(1, 30);
    char* inputChar = DeepState_CStr_C(stringSize, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
    std::string originalStr(inputChar);

    // To lower
    std::string inputStr = originalStr;
    toLowercase(inputStr);

    ASSERT_TRUE(isLower(originalStr, inputStr));

    // To upper
    inputStr = originalStr;
    toUppercase(inputStr);

    ASSERT_TRUE(isUpper(originalStr, inputStr));

    // Escape
    inputStr = originalStr;
    inputStr = escape(inputStr);
    inputStr = unescape(inputStr);

    ASSERT_TRUE(originalStr == inputStr);

    // Replace
    // inputStr = originalStr;
    // int substrStart = DeepState_IntInRange(1, stringSize - 2);
    // int substrEnd = DeepState_IntInRange(substrStart + 1, stringSize - 1);

    // char* newStringChar = DeepState_CStr_C(substrEnd - substrStart, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
    // std::string newStr(newStringChar);

    // std::string output = replace(inputStr, inputStr.substr(substrStart, substrEnd - substrStart), newStr);

    // ASSERT_TRUE(stringReplaced(originalStr, inputStr, newStr, substrStart, substrEnd));

    // // Split/Join
    // inputStr = originalStr;
    // std::vector<std::string> out = {};
    // char *charToSplitOn = DeepState_CStr_C(1, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
    // split(inputStr, out, charToSplitOn, false, true);
    // std::string  output = join(out, charToSplitOn, false);
    // ASSERT_TRUE(output == inputStr);

    // OneOf
    stringSize = DeepState_SizeInRange(1, 256);
    LOG(TRACE) << "String Size: " << stringSize;
    inputChar = DeepState_CStr_C(stringSize, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
    std::string originalStrLong(inputChar);
    inputStr = originalStrLong;

    for (int n = 0; n < stringSize; n++) {
        LOG(TRACE) << "Number of Iterations: " + n;
        OneOf(
          [&] {
            LOG(TRACE) << "Our Lowercase: " + inputStr;
            toLowercase(inputStr);

            ASSERT_TRUE(isLower(originalStrLong, inputStr));
            originalStrLong = inputStr;
          },
          [&] {
            LOG(TRACE) << "Our Uppercase: " + inputStr;
            toUppercase(inputStr);

            ASSERT_TRUE(isUpper(originalStrLong, inputStr));
            originalStrLong = inputStr;
          },
        [&] {
            LOG(TRACE) << "Trimmed whitespace";
            inputStr = trim(inputStr);
            originalStrLong = inputStr;
          },
        [&] {
            LOG(TRACE) << "Trimmed char " + inputStr;
            char *charToTrim = DeepState_CStrUpToLen(1);
            inputStr = trim(inputStr, charToTrim);
            originalStrLong = inputStr;
          },
        [&] {
            LOG(TRACE) << "Replaced substring " + inputStr;
            int substrStart = DeepState_IntInRange(1, stringSize - 2);
            int substrEnd = DeepState_IntInRange(substrStart + 1, stringSize - 1);
            char* newStringChar = DeepState_CStr_C(substrEnd - substrStart, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
            std::string newStr(newStringChar);
            inputStr = replace(inputStr, inputStr.substr(substrStart, substrEnd - substrStart), newStr);

            ASSERT_TRUE(stringReplaced(originalStrLong, inputStr, newStr, substrStart, substrEnd));
            originalStrLong = inputStr;
          },
        [&] {
            LOG(TRACE) << "Replaced char " + inputStr;
            char newChar = DeepState_Char();
            char charToReplace = DeepState_Char();
            replaceChar(inputStr, charToReplace, newChar);
            originalStrLong = inputStr;
          }
        );
    }
}
