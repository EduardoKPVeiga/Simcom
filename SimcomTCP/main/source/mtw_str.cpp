#include "mtw_str.h"

using namespace mtw_str;

bool mtw_str::valid_char(char c)
{
    if ((c < 33 || c > 125) && c != '\0' && c != '\r' && c != '\n')
        return 0;
    return 1;
}

char *mtw_str::reverse(char *str)
{
    char *p1, *p2;

    if (!str || !*str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

char *mtw_str::decimal_to_char_array(uint16_t decimal)
{
    if (decimal == 0)
        return "0";
    uint8_t digit = 0;
    string decimal_s = "";
    while (decimal != 0)
    {
        digit = decimal - ((uint16_t)(decimal / 10) * 10);
        decimal_s += (char)(digit + '0');
        decimal = (uint16_t)(decimal / 10);
    }
    char *decimal_array = reverse((char *)decimal_s.c_str());
    return decimal_array;
}

string mtw_str::to_hex_string(char *char_array, int char_array_size)
{
    string str = "";
    char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 0; i < char_array_size; ++i)
    {
        char const byte = char_array[i];

        str += hex_chars[(byte & 0xF0) >> 4];
        str += hex_chars[(byte & 0x0F) >> 0];
    }
    str += '\0';
    return str;
}

void mtw_str::remove_char(char *a, size_t *size, char c)
{
    char *aux = new char[*size];
    for (int i = 0; i < *size; i++)
        aux[i] = 0;
    char *aux_p = &(aux[0]);
    uint16_t qtd_c = 0;
    for (int i = 0; i < *size; i++)
    {
        if (a[i] != c)
        {
            *aux_p = a[i];
            aux_p++;
        }
        else
            qtd_c++;
    }
    *size -= qtd_c;
    for (int i = 0; i < *size; i++)
        a[i] = aux[i];
    delete[] aux;
}

int mtw_str::StrContainsSubstr(char *str, char *sub_str, int size_str, int size_sub_str)
{
    if (size_sub_str > size_str)
        return -1;

    int begin_position = -1;
    int i = 0, j = 0;
    for (; i < size_str; i++)
    {
        if (str[i] == sub_str[j])
        {
            if (j == 0)
                begin_position = i; // Marca início da correspondência

            j++;

            if (j == size_sub_str)
                return begin_position;
        }
        else
        {
            i -= j; // Retorna para a posição inicial da busca
            j = 0;
        }
    }
    return -1;
}

int mtw_str::StrContainsChar(char *str, char a, int size_str)
{
    for (int i = 0; i < size_str; i++)
    {
        if (str[i] == a)
            return i;
    }
    return -1;
}