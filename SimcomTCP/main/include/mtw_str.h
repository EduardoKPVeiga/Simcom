#ifndef STR_H_
#define STR_H_

#include <string>
#include <cstring>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)         \
    ((byte) & 0x80 ? '1' : '0'),     \
        ((byte) & 0x40 ? '1' : '0'), \
        ((byte) & 0x20 ? '1' : '0'), \
        ((byte) & 0x10 ? '1' : '0'), \
        ((byte) & 0x08 ? '1' : '0'), \
        ((byte) & 0x04 ? '1' : '0'), \
        ((byte) & 0x02 ? '1' : '0'), \
        ((byte) & 0x01 ? '1' : '0')

using namespace std;

namespace mtw_str
{
#define SIZE(x) ((sizeof(x)) - 1)

    bool valid_char(char c);

    char *reverse(char *str);

    char *decimal_to_char_array(uint16_t decimal);

    string to_hex_string(char *char_array, int char_array_size);

    void remove_char(char *a, size_t *size, char c);

    int StrContainsSubstr(char *str, char *sub_str, int size_str, int size_sub_str);

    int StrContainsChar(char *str, char a, int size_str);
}

#endif