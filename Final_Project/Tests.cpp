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

TEST(StrUtil, StrManipulation) {
    size_t stringSize = DeepState_SizeInRange(2, 32);
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

    // Check if the string is binary, but do not assert
    int returnVal = is_binary(inputChar);
    LOG(TRACE) << "Is binary: " << returnVal;

    // See if true/false is a substring and assert that it is a bool
    char *substring;
    char *trueChar = "true";
    char *falseChar = "false";
    if(findSubString(inputChar, trueChar) != -1) {
        int startIndex = findSubString(inputChar, trueChar);
        getSubString(substring, inputChar, startIndex, startIndex + 4);
        std::string subStr(substring);
        ASSERT_TRUE(toBool(subStr, false));
    }

    if(findSubString(inputChar, falseChar) != -1) {
        int startIndex = findSubString(inputChar, falseChar);
        getSubString(substring, inputChar, startIndex, startIndex + 5);
        std::string subStr(substring);
        ASSERT_FALSE(toBool(subStr, true));
    }

    // OneOf
    stringSize = DeepState_SizeInRange(2, 32);
    LOG(TRACE) << "String Size: " << stringSize;
    inputChar = DeepState_CStr_C(stringSize, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
    std::string originalStrLong(inputChar);
    inputStr = originalStrLong;

    for (int n = 0; n < stringSize; n++) {
        LOG(TRACE) << "Number of Iterations: " << n;
        OneOf(
          [&] {
            LOG(TRACE) << "Started Lowercase: " << inputStr;
            toLowercase(inputStr);

            ASSERT_TRUE(isLower(originalStrLong, inputStr));
            originalStrLong = inputStr;
            LOG(TRACE) << "Our Lowercase: " << inputStr;
          },
          [&] {
            LOG(TRACE) << "Started Uppercase: " << inputStr;
            toUppercase(inputStr);

            ASSERT_TRUE(isUpper(originalStrLong, inputStr));
            originalStrLong = inputStr;
            LOG(TRACE) << "Our Uppercase: " << inputStr;
          },
        [&] {
            LOG(TRACE) << "Started Trim: " << inputStr;
            inputStr = trim(inputStr);
            originalStrLong = inputStr;
            LOG(TRACE) << "Trimmed whitespace";
          },
        [&] {
            LOG(TRACE) << "Started Char Trim: " << inputStr;
            char *charToTrim = DeepState_CStrUpToLen(1);
            inputStr = trim(inputStr, charToTrim);
            originalStrLong = inputStr;
            LOG(TRACE) << "Trimmed char: "<< charToTrim << " string: " << inputStr;
          },
        [&] {
            LOG(TRACE) << "Started Replace Char: " << inputStr;
            char newChar = DeepState_Char();
            char charToReplace = DeepState_Char();
            replaceChar(inputStr, charToReplace, newChar);
            originalStrLong = inputStr;
            LOG(TRACE) << "Replaced char: " << charToReplace << " with: " << newChar << " string: " << inputStr;
          }
        );
    }
}
