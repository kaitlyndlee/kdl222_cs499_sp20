#include "str_util.h"
#include "string.h"
#include <string>
#include <algorithm>

const char NULL_CHAR = '\0';
const int MAX_STR_LEN = 200;

//Get the equivalent character in different case
char lowercase(char c) {
    if(c >= 'A' && c <= 'Z')
        return 'a' + (c - 'A');
    else
        return c;
}

char uppercase(char c) {
    if(c >= 'a' && c <= 'z')
        return 'A' + (c - 'a');
    else
        return c;
}

//Convert a standard string's case
void toLowercase(std::string& str) {
    for(int i = 0, cnt = str.size(); i < cnt; ++i)
        str[i] = lowercase(str[i]);
}

void toUppercase(std::string& str) {
    for(int i = 0, cnt = str.size(); i < cnt; ++i)
        str[i] = uppercase(str[i]);
}

//Convert special characters to escape sequences in a string
std::string escape(const std::string& text) {
    std::string result;
    result.reserve(text.size());

    size_t pos = 0;
    size_t found = text.find_first_of("\n\r\t\\");
    while(found != std::string::npos) {
        if(found - pos > 0)
            result.append(text, pos, found - pos);

        switch(text[found]) {
            case '\n':
                result += "\\n";
            break;
            case '\r':
                result += "\\r";
            break;
            case '\t':
                result += "\\t";
            break;
            case '\\':
                result += "\\\\";
            break;
        }

        pos = found + 1;
        if(pos >= text.size())
            break;
        found = text.find_first_of("\n\r\t\\", pos);
    }

    if(pos < text.size())
        result.append(text, pos, text.size() - pos);

    return result;
}

//Convert literal escape sequences in a string
std::string unescape(const std::string& text) {
    std::string result;
    result.reserve(text.size());

    size_t pos = 0;
    size_t found = text.find('\\');
    while(found != std::string::npos) {
        if(found - pos > 0)
            result.append(text, pos, found - pos);

        if(found >= text.size() - 1)
            break;

        switch(text[found + 1]) {
            case 'n':
                result += '\n';
            break;
            case 'r':
                result += '\r';
            break;
            case 't':
                result += '\t';
            break;
            case '\\':
                result += '\\';
            break;
        }

        pos = found + 2;
        if(pos >= text.size())
            break;
        found = text.find('\\', pos);
    }

    if(pos < text.size())
        result.append(text, pos, text.size() - pos);

    return result;
}

//Compares c-style strings a and b, ignoring case
//Returns 0 if the strings are equal, the the difference of the first unequal character otherwise
char strcmp_nocase(const char* a, const char* b) {
    while(lowercase(*a) == lowercase(*b)) { //TODO: Unicode
        if(*a == 0)
            return 0;
        ++a; ++b;
    }

    return *a-*b;
}

//Compares standard strings a and b, ignoring case
bool streq_nocase(const std::string& a, const std::string& b) {
    return strcmp_nocase(a.c_str(), b.c_str()) == 0;
}


//Replace characters in a string
void replaceChar(std::string& str, char replace, char with) {
    for(int i = 0, cnt = str.size(); i < cnt; ++i)
        if(str[i] == replace)
            str[i] = with;
}


//Trim whitespace from both ends of the string
std::string trim(const std::string& input) {
    size_t left = input.find_first_not_of("\t\r\n ");
    size_t right = input.find_last_not_of("\t\r\n ");

    if(left == std::string::npos || right == std::string::npos)
        return "";
    return input.substr(left, right - left + 1);
}

std::string trim(const std::string& input, const char* trimChars) {
    size_t left = input.find_first_not_of(trimChars);
    size_t right = input.find_last_not_of(trimChars);

    if(left == std::string::npos || right == std::string::npos)
        return "";
    return input.substr(left, right - left + 1);
}

//Read boolean from string
bool toBool(const std::string& str, bool def) {
    static const std::string strYes("yes"), strNo("no"), strTrue("true"), strFalse("false"), strOn("on"), strOff("off");

    if(streq_nocase(str, strYes) || streq_nocase(str, strTrue) || streq_nocase(str, strOn))
        return true;
    else if(streq_nocase(str, strFalse) || streq_nocase(str, strNo) || streq_nocase(str, strOff))
        return false;
    else if(str.find_first_not_of("0123456789.+-eE") == std::string::npos)
        return toNumber<int>(str) != 0;
    else
        return def;
}

size_t length_of_buffer(const void *buf) {
    size_t length = 0;
    const unsigned char *buf_c = (const unsigned char *) buf;
    int i = 0 ;
    while(buf_c[i]) {
        length++;
        i++;
    }
    return length;
}

int is_binary(const void *buf) {
    size_t suspicious_bytes = 0;
    const unsigned char *buf_c = (const unsigned char *) buf;
    size_t buf_len = length_of_buffer(buf_c);
    size_t total_bytes = buf_len > 512 ? 512 : buf_len;
    
    size_t i;

    if (buf_len == 0) {
        /* Is an empty file binary? Is it text? */
        return 0;
    }

    if (buf_len >= 3 && buf_c[0] == 0xEF && buf_c[1] == 0xBB && buf_c[2] == 0xBF) {
        /* UTF-8 BOM. This isn't binary. */
        return 0;
    }

    if (buf_len >= 5 && strncmp((const char *) buf, "%PDF-", 5) == 0) {
        /* PDF. This is binary. */
        return 1;
    }

    for (i = 0; i < total_bytes; i++) {
        if (buf_c[i] == '\0') {
            /* NULL char. It's binary */
            return 1;
        } else if ((buf_c[i] < 7 || buf_c[i] > 14) && (buf_c[i] < 32 || buf_c[i] > 127)) {
            /* UTF-8 detection */
            if (buf_c[i] > 193 && buf_c[i] < 224 && i + 1 < total_bytes) {
                i++;
                if (buf_c[i] > 127 && buf_c[i] < 192) {
                    continue;
                }
            } else if (buf_c[i] > 223 && buf_c[i] < 240 && i + 2 < total_bytes) {
                i++;
                if (buf_c[i] > 127 && buf_c[i] < 192 && buf_c[i + 1] > 127 && buf_c[i + 1] < 192) {
                    i++;
                    continue;
                }
            }
            suspicious_bytes++;
            /* Disk IO is so slow that it's worthwhile to do this calculation after every suspicious byte. */
            /* This is true even on a 1.6Ghz Atom with an Intel 320 SSD. */
            /* Read at least 32 bytes before making a decision */
            if (i >= 32 && (suspicious_bytes * 100) / total_bytes > 10) {
                return 1;
            }
        }
    }
    if ((suspicious_bytes * 100) / total_bytes > 10) {
        return 1;
    }

    return 0;
}

void copyString(char *destination, char *source) {
    int index = 0;
     
    while( (index < MAX_STR_LEN) && (source[index] != NULL_CHAR) )
    {
        destination[index] = source[index];
        index++;
        
        // assign null character to next destination element
        destination[index] = NULL_CHAR;
    }
}

void concatenateString(char *destination, char *source) 
{
    // set destination index to end of destination string
    int destIndex = length_of_buffer(destination);

    int sourceIndex = 0;
    
    while( (sourceIndex < MAX_STR_LEN) && (source[sourceIndex] != NULL_CHAR) ) 
    {
        destination[destIndex] = source[sourceIndex];
        sourceIndex++;
        destIndex++;
        
        // assign null character to next destination element
        destination[destIndex] = NULL_CHAR;
    }
}

void getSubString(char *destStr, char *sourceStr, 
                                          int startIndex, int endIndex) 
{
    int sourceStrLen = length_of_buffer(sourceStr);
    int destIndex = 0;
    int sourceIndex = startIndex;
    char *tempSourceStr;
    
    if( (startIndex >= 0) && (startIndex <= endIndex) 
                                                && (endIndex < sourceStrLen) )
    {
        tempSourceStr = (char *) malloc(sourceStrLen + 1);
        copyString(tempSourceStr, sourceStr);
        
        while(sourceIndex < endIndex) 
        {
            destStr[destIndex] = tempSourceStr[sourceIndex];
            
            destIndex++;
            sourceIndex++;
            
            // assign null character to next destination element
            destStr[destIndex] = NULL_CHAR;
        }
        
        free(tempSourceStr);
    }
    else 
    {
        destStr[0] = NULL_CHAR;
    }
}

int findSubString(char *testStr, char *searchSubStr) 
{
    int testStrLen = length_of_buffer(testStr);
    int masterIndex = 0;    // location of substring start point
    int subStrIndex;
    int internalIndex;
    
    while(masterIndex < testStrLen) 
    {
        internalIndex = masterIndex;
        subStrIndex = 0;
        
        // while the characters in testStr match those in searchSubStr
        while( (internalIndex <= testStrLen) &&
                        (testStr[internalIndex] == searchSubStr[subStrIndex]) )
        {
            internalIndex++;
            subStrIndex++;
            
            // if the substring is found
            if(searchSubStr[subStrIndex] == NULL_CHAR) 
            {
                return masterIndex;
            }
        }
        
        masterIndex++;
    }
    return -1;
}
