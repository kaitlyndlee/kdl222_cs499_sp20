#include <deepstate/DeepState.hpp>
#include <string.h>

// #include <string>
using namespace deepstate;

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

TEST(Fuzzingwins, StrTest) {
  char* inputString = DeepState_CStr_C(128, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_()/?*&%$#@![]{}");
  LOG(TRACE) << "String: " << inputString;
  ASSERT_FALSE(is_binary(inputString));
}

