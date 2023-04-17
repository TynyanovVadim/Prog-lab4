#include <stdio.h>
#include <math.h>

#include "strings.h"

//29

size_t slen(const char *string) 
{
    size_t size = 0;
    for (size = 0; string[size] != '\0'; size++);
    return size;
}

char *stok(char *string, const char delim)
{
    static char *last;
    char *tok;
    
    if ((!string && !last)) {
        return NULL;
    } else if (string) {
        tok = string;
        last = string;
    } else {
        tok = last;
    }

    while (1) {
        if (*last == delim) {
            *last++ = '\0';
            return tok;
        } else if (*last == '\0') {
            last = NULL;
            return tok;
        }
        last++;
    }
    
    return NULL;
}

size_t sspn(const char *string1, const char *string2)
{
    size_t i, j;
    for(i = 0; string1[i] != '\0'; i++) {
        int is_include = 0;
        for (j = 0; string2[j] != '\0'; j++) {
            if (string1[i] == string2[j]) {
                is_include = 1;
                break ;
            }
        }
        if (!is_include) {
            return i;
        }
    }
    return i;
}

size_t scspn( const char * string1, const char * string2)
{
    size_t i, j;
    for(i = 0; string1[i] != '\0'; i++) {
        int is_include = 0;
        for (j = 0; string2[j] != '\0'; j++) {
            if (string1[i] == string2[j]) {
                is_include = 1;
                break ;
            }
        }
        if (is_include) {
            return i;
        }
    }
    return i;
}

int scmp(const char *str1, const char *str2)
{
    char ch1;
    char ch2;

    while (1) {
        ch1 = *str1++;
        ch2 = *str2++;

        if (ch1 != ch2) {
            return ch1 < ch2 ? -1: 1;
        }
        if (!ch1) {
            break;
        }
    }
    return 0;
}

char *scpy(char *to, char *from)
{   
    char *s1 = to;
    char *s2 = from;
    while ((*s1++ = *s2++) != '\0');
    return to;
}

int stoi(char *str)
{
    int res = 0;
    int exp = slen(str) - 1;
    for (int i = 0; i < slen(str); i++) {
        res += (str[i] - '0') * pow(10, exp--);
    }
    return res;
}

size_t scount(char *str, char c)
{
    size_t count = 0;
    while (*str != '\0') {
        if (*str == c) {
            count++;
        }
        str++;
    }
    return count;
}
