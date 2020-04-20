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

int stringReplaced(const std::string inputStr, std::string compareString, std::string replace, std::string with, int start, int end) {
    std::string copyStr = inputStr;
    size_t pos = 0;
    while ((pos = copyStr.find(replace, pos)) != std::string::npos) {
         copyStr.replace(pos, with.length(), with);
         pos += with.length();
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

    // Replace
    inputStr = originalStr;
    int substrStart = 0;
    int substrEnd = 1;
    if(stringSize > 2) {
        substrStart = DeepState_IntInRange(0, stringSize - 2);
        substrEnd = DeepState_IntInRange(substrStart + 1, stringSize - 1);
    }

    LOG(TRACE) << "Starting Replace, start and end:  " << substrStart << " " << substrEnd;
    char* newStringChar = DeepState_CStr_C(substrEnd - substrStart, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
    std::string newStr(newStringChar);
    LOG(TRACE) << "Got replace string:  " << newStr;
    std::string substr = inputStr.substr(substrStart, substrEnd - substrStart);
    std::string output = replace(inputStr, substr, newStr);
    LOG(TRACE) << "Replaced string:  " << inputStr;

    ASSERT_TRUE(stringReplaced(originalStr, inputStr, substr, newStr, substrStart, substrEnd));

    // Check if the string is binary, but do not assert
    int returnVal = is_binary(inputStr);
    LOG(TRACE) << "Is binary: " << returnVal;

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
            LOG(TRACE) << "Started Replace: " << inputStr;
            int substrStart = 0;
            int substrEnd = 1;
            if(stringSize > 2) {
                substrStart = DeepState_IntInRange(0, stringSize - 2);
                substrEnd = DeepState_IntInRange(substrStart + 1, stringSize - 1);
            }

            char* newStringChar = DeepState_CStr_C(substrEnd - substrStart, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
            std::string newStr(newStringChar);
            std::string substr = inputStr.substr(substrStart, substrEnd - substrStart);
            inputStr = replace(inputStr, substr, newStr);

            ASSERT_TRUE(stringReplaced(originalStrLong, inputStr, substr, newStr, substrStart, substrEnd));
            originalStrLong = inputStr;
            LOG(TRACE) << "Replaced substring: "<< substr << " with " <<  newStr << " string:" << inputStr;
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
