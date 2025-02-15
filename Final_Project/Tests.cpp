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
    char trueChar[] = "true";
    char falseChar[] = "false";
    char yesChar[] = "yes";
    char noChar[] = "no";
    char onChar[] = "on";
    char offChar[] = "off";

    if(findSubString(inputChar, trueChar) != -1) {
        substring = (char *) malloc(4);
        LOG(TRACE) << "Contains true: " << inputChar;
        int startIndex = findSubString(inputChar, trueChar);
        getSubString(substring, inputChar, startIndex, startIndex + 3);
        LOG(TRACE) << "true substring: " << substring;
        std::string subStr(substring);
        ASSERT_TRUE(toBool(subStr, false));
        free(substring);
    }

    if(findSubString(inputChar, falseChar) != -1) {
        substring = (char *) malloc(5);
        LOG(TRACE) << "Contains false: " << inputChar;
        int startIndex = findSubString(inputChar, falseChar);
        getSubString(substring, inputChar, startIndex, startIndex + 4);
        LOG(TRACE) << "false substring: " << substring;
        std::string subStr(substring);
        ASSERT_FALSE(toBool(subStr, true));
        free(substring);
    }

    if(findSubString(inputChar, yesChar) != -1) {
        substring = (char *) malloc(3);
        LOG(TRACE) << "Contains yes: " << inputChar;
        int startIndex = findSubString(inputChar, yesChar);
        getSubString(substring, inputChar, startIndex, startIndex + 2);
        LOG(TRACE) << "yes substring: " << substring;
        std::string subStr(substring);
        ASSERT_TRUE(toBool(subStr, false));
        free(substring);
    }

    if(findSubString(inputChar, noChar) != -1) {
        substring = (char *) malloc(2);
        LOG(TRACE) << "Contains no: " << inputChar;
        int startIndex = findSubString(inputChar, noChar);
        getSubString(substring, inputChar, startIndex, startIndex + 1);
        LOG(TRACE) << "no substring: " << substring;
        std::string subStr(substring);
        ASSERT_FALSE(toBool(subStr, true));
        free(substring);
    }

    if(findSubString(inputChar, onChar) != -1) {
        substring = (char *) malloc(2);
        LOG(TRACE) << "Contains on: " << inputChar;
        int startIndex = findSubString(inputChar, onChar);
        getSubString(substring, inputChar, startIndex, startIndex + 1);
        LOG(TRACE) << "on substring: " << substring;
        std::string subStr(substring);
        ASSERT_TRUE(toBool(subStr, false));
        free(substring);
    }

    if(findSubString(inputChar, offChar) != -1) {
        substring = (char *) malloc(3);
        LOG(TRACE) << "Contains off: " << inputChar;
        int startIndex = findSubString(inputChar, offChar);
        getSubString(substring, inputChar, startIndex, startIndex + 2);
        LOG(TRACE) << "off substring: " << substring;
        std::string subStr(substring);
        ASSERT_FALSE(toBool(subStr, true));
        free(substring);
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
