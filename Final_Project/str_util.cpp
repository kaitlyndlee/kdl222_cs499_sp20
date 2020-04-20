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

bool streq_nocase(const std::string& str, const std::string& substr, unsigned start, unsigned length) {
    if(length == 0)
        length = std::max(substr.size(), str.size());
    if(start + length > str.size())
        return false;
    if(length > substr.size())
        return false;

    const char* a = &str[start];
    const char* b = &substr[0];
    
    while(length > 0) {
        if(lowercase(*a) != lowercase(*b)) //TODO: Unicode
            return false;
        ++a; ++b;
        --length;
    }
    return true;
}

int strfind_nocase(const std::string& str, const std::string& substr, unsigned start) {
    unsigned length = substr.size();
    unsigned strsize = str.size();
    if(start + length > strsize)
        return -1;

    unsigned i = start;
    for(; i <= strsize - length; ++i) {
        unsigned j = 0;
        for(; j < length; ++j) {
            if(lowercase(str[i+j]) != lowercase(substr[j]))
                break;
        }

        if(j == length)
            return i;
    }

    return -1;
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

//Escape a case-sensitive string for use on a case-insensitive filesystem
std::string escapeCase(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    for(unsigned i = 0, cnt = text.size(); i < cnt; ++i) {
        char ch = text[i];
        char low = lowercase(ch);

        if(ch == '-') {
            result += "--";
        }
        else if(low != ch) {
            result += "-";
            result += low;
        }
        else {
            result += ch;
        }
    }
    return result;
}

//Unescape a case-insensitive filename to a case-sensitive string
std::string unescapeCase(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    for(unsigned i = 0, cnt = text.size(); i < cnt; ++i) {
        char ch = text[i];

        if(text[i] == '-') {
            if(i == cnt - 1)
                break;
            if(text[i+1] == '-')
                result += "-";
            else
                result += uppercase(text[i+1]);
            ++i;
        }
        else {
            result += ch;
        }
    }
    return result;
}

//Checks to make sure the string is a valid identifier
bool isIdentifier(const std::string& identifier, const char* extraChars) {
    if(identifier.empty())
        return false;

    for(auto c = identifier.begin(), end = identifier.end(); c != end; ++c) {
        char chr = *c;

        //Valid characters are 0-9, a-z, A-Z, and '_'
        //Numbers cannot be the first character

        if(chr >= 'a' && chr <= 'z')
            continue;

        if(chr >= 'A' && chr <= 'Z')
            continue;

        if(chr == '_')
            continue;

        if(chr >= '0' && chr <= '9') {
            if(c == identifier.begin())
                return false;
            else
                continue;
        }

        if(extraChars != nullptr) {
            const char* allow = extraChars;
            bool allowed = false;
            while(*allow != '\0') {
                if(*c == chr) {
                    allowed = true;
                    break;
                }
                ++allow;
            }
            if(allowed)
                continue;
        }

        return false;
    }

    return true;
}

void makeIdentifier(std::string& identifier, const char* extraChars) {
    std::string output;
    for(auto c = identifier.begin(), end = identifier.end(); c != end; ++c) {
        char chr = *c;

        //Valid characters are 0-9, a-z, A-Z, and '_'
        //Numbers cannot be the first character

        if(chr >= 'a' && chr <= 'z') {
            output.push_back(chr);
            continue;
        }

        if(chr >= 'A' && chr <= 'Z') {
            output.push_back(chr);
            continue;
        }

        if(chr == '_') {
            output.push_back(chr);
            continue;
        }

        if(chr >= '0' && chr <= '9') {
            if(c == identifier.begin()) {
            }
            else {
                output.push_back(chr);
            }
        }

        if(extraChars != nullptr) {
            const char* allow = extraChars;
            bool allowed = false;
            while(*allow != '\0') {
                if(*c == chr) {
                    allowed = true;
                    break;
                }
                ++allow;
            }
            if(allowed) {
                output.push_back(chr);
                continue;
            }
        }

        if(chr == ' ') {
            output.push_back('_');
            continue;
        }
    }
    identifier = output;
}

//Compares standard strings a and b, ignoring case
bool streq_nocase(const std::string& a, const std::string& b) {
    return strcmp_nocase(a.c_str(), b.c_str()) == 0;
}

//Split a line into Key: Value
bool splitKeyValue(const std::string& input, std::string& key, std::string& value, const char* split) {
    size_t index = input.find(split);
    if(index == std::string::npos)
        return false;
    key = input.substr(0, index);
    value = input.substr(index + 1, input.size() - index - 1);
    return true;
}

//Do some basic line parsing operations in addition to splitting
bool parseKeyValue(std::string& line, std::string& key, std::string& value) {
    line = trim(line);

    if(line.empty())
        return false;

    if(!splitKeyValue(line, key, value))
        return false;

    key = trim(key);
    value = trim(value);

    //Remove quotes
    if(value.size() >= 2 && value.front() == '\"' && value.back() == '\"')
        value = value.substr(1, value.size() - 2);

    return !key.empty();
}

bool readKeyValue(std::ifstream& file, std::string& key, std::string& value) {
    std::string line;

    while(file.is_open() && file.good()) {
        std::getline(file, line);

        if(parseKeyValue(line, key, value))
            return true;
    }
    return false;
}

//Generic string split
void split(const std::string& input, std::vector<std::string>& out, char delimit, bool doTrim, bool listEmpty) {
    size_t start = 0;
    size_t size = 0;

    for(unsigned i = 0, cnt = input.size(); i < cnt; ++i) {
        char chr = input[i];
        if(chr == delimit) {
            if(size > 0) {
                if(doTrim)
                    out.push_back(trim(input.substr(start, size)));
                else
                    out.push_back(input.substr(start, size));
            }
            else if(listEmpty) {
                out.push_back("");
            }
            start = i + 1;
            size = 0;
        }
        else {
            ++size;
        }
    }

    if(size > 0) {
        if(doTrim)
            out.push_back(trim(input.substr(start, size)));
        else
            out.push_back(input.substr(start, size));
    }
}

//Generic string split
void split(const std::string& input, std::vector<std::string>& out, const char* delimit, bool doTrim, bool listEmpty) {
    size_t start = 0;
    size_t size = 0;
    int delimLen = strlen(delimit);

    for(unsigned i = 0, cnt = input.size(); i < cnt; ++i) {
        if(i + delimLen <= cnt && strncmp(&input[i], delimit, delimLen) == 0) {
            if(size > 0) {
                if(doTrim)
                    out.push_back(trim(input.substr(start, size)));
                else
                    out.push_back(input.substr(start, size));
            }
            else if(listEmpty) {
                out.push_back("");
            }
            start = i + delimLen;
            i += delimLen - 1;
            size = 0;
        }
        else {
            ++size;
        }
    }

    if(size > 0) {
        if(doTrim)
            out.push_back(trim(input.substr(start, size)));
        else
            out.push_back(input.substr(start, size));
    }
}

//Splits a string of the format:
// front<delim_front>inner<delim_back>back
bool split(const std::string& input, std::string& front, char delim_front, std::string& inner, char delim_back, std::string* back) {
    auto dOne = input.find(delim_front);
    if(dOne == input.npos)
        return false;
    auto dTwo = input.find(delim_back, dOne+1);
    if(dTwo == input.npos)
        return false;

    front = input.substr(0, dOne);
    inner = input.substr(dOne+1,dTwo-(dOne+1));
    if(back)
        *back = input.substr(dTwo+1,input.size()-(dTwo+1));
    return true;
}

//Match a string for a set of expressions with wildcards
void compile_pattern(const char* pattern, std::vector<std::string>& out) {
    std::string filterStr = pattern;

    toLowercase(filterStr);
    if(filterStr.size() == 0)
        return;

    split(filterStr, out, '*');
    if(filterStr[filterStr.size()-1] == '*')
        out.push_back("");
}

bool match(const char* name, const char* pattern) {
    std::vector<std::string> filters;

    compile_pattern(pattern, filters);
    return match(name, filters);
}

bool match(const char* name, const std::vector<std::string>& parts) {
    for(auto part = parts.begin(); part != parts.end(); ++part) {
        if(part->empty())
            continue;
        const char* pStr = strstr(name, part->c_str());
        if(pStr == 0)
            return false;
        name = pStr + part->size();
    }

    if(parts.size() > 0 && parts[parts.size()-1].empty())
        return true;
    else
        return *name == 0;
}


//Replace characters in a string
void replaceChar(std::string& str, char replace, char with) {
    for(int i = 0, cnt = str.size(); i < cnt; ++i)
        if(str[i] == replace)
            str[i] = with;
}

std::string& replace(std::string& str, const std::string& replace, const std::string& with) {
    str = std::move(replaced(str, replace, with));
    return str;
}

std::string replaced(const std::string& input, const std::string& replace, const std::string& with) {
    std::string out;
    size_t at = 0, found = 0;
    while(at < input.size()) {
        found = input.find(replace, at);
        if(found == std::string::npos)
            break;

        if(found >= at) {
            if(found > at)
                out.append(input, at, found-at);
            out.append(with);
        }

        at = found+replace.size();
    }
    if(at < input.size())
        out.append(input, at, input.size() - at);
    return out;
}

std::string& paragraphize(std::string& input, const std::string& parSep, const std::string& lineSep, bool startsParagraph) {
    std::string out;
    size_t at = 0, found = 0;
    if(startsParagraph) {
        found = input.find_first_not_of(" \t");
        if(found < input.size() && input[found] == '\n') {
            size_t second = input.find_first_not_of(" \t", found+1);
            if(second >= input.size() || input[second] != '\n') {
                at = found+1;
            }
        }
    }
    while(at < input.size()) {
        found = input.find('\n', at);
        if(found == std::string::npos)
            break;

        out.append(input, at, found-at);
        if(found > 0 && input[found-1] == '\\') {
            //Nothing here
            found += 1;
        }
        else if(found > 1 && input[found-1] == ' ' && input[found-2] == ' ') {
            out.append(lineSep);
            found += 1;
        }
        else {
            unsigned amount = 1;
            found += 1;
            while(found < input.size() && input[found] == '\n') {
                amount += 1;
                found += 1;
            }
            if(amount == 1) {
                out += ' ';
            }
            else if(amount == 2) {
                out.append(parSep);
            }
            else {
                for(unsigned i = 1; i < amount; ++i)
                    out.append(lineSep);
            }
        }
        at = found;
    }
    if(at < input.size())
        out.append(input, at, input.size() - at);
    input = std::move(out);
    return input;
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


//Join strings in a vector
std::string join(std::vector<std::string>& list, const char* delimiter, bool delim_final) {
    int delim_len = strlen(delimiter);
    int strs = list.size();
    int len = delim_len * (strs - delim_final ? 1 : 0);
    for(int i = 0; i < strs; ++i)
        len += list[i].size();

    std::string contents;
    contents.reserve(len);
    for(int i = 0; i < strs; ++i) {
        contents += list[i];
        if(delim_final || i < strs - 1)
            contents += delimiter;
    }

    return contents;
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
        
        while(sourceIndex <= endIndex) 
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
