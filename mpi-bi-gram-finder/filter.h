//libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ASCII Codes
// 65 - 90  -> A - Z
// 97 - 122 -> a - z
// 127 -> space
// 305 -> ı
// 287 -> ğ
// 246 -> ö
// 351 -> ş
// 252 -> ü 
// (Turkish Characters not supported yet)
char EXTRA_CHARACTERS[] = {};

int get_extra_char_size()
{
    return strlen(EXTRA_CHARACTERS);
}

/*
    To check whether given char is an alphabetic character or not
    - a-z
*/
int check_char(char c) //checks char if valid to append new string
{
    if (97 <= c & c <= 122)
    {
        return 1;
    }
    // printf("- '%c' is not a valid char\n", c);
    return 0;
}

/*  index_to_asci
    Map Given Index to Character

    Case a) For the characters a-z (97-122);
        return index + 97
    
    Case b) For the extra characters EXTRA_CHARACTERS
        - index = 26 to 26 + EXTRA_CHARACTERS.length
        
        - index at EXTRA_CHARACTERS array = index - 26
        - character EXTRA_CHARACTERS[index]
        
        // a = 0
        // b = 1
        // .
        // .
        // z = 25
        // ş = 26
        // ğ = 27

*/
int index_to_asci(int index)
{
    // For Case a)
    if (0 <= index & index <= 25)
    {
        return index + 97;
    }

    // For Case b)
    return EXTRA_CHARACTERS[index - 26];
}

/*  asci_to_index
    Map Given Character to Index

    Case a) For the characters a-z (97-122);
        return c - 97

    Case b) For the characters A-Z (65-90);
        return c + 32 - 97

    Case c) For the extra characters EXTRA_CHARACTERS
        - p = 1 to EXTRA_CHARACTERS.length
        return (122 - 97) + p
        
        // a = 0
        // b = 1
        // .
        // .
        // z = 25
        // ş = 26
        // ğ = 27
 */
int asci_to_index(char c)
{
    int index;

    // For Case b)
    if (65 <= c & c <= 90)
    {
        //printf("- '%c' is upper case. Converting to Lower Case\n", c);
        c = c + 32;
    }

    if (check_char(c) == 1)
    {
        int result = (int)(c)-97;
        //printf("- '%c'\tascii: %d\tindex: %d\n", c, c, result);
        return result;
    }

    //printf("- Looking for '%c' if it is in excepted character.\n", c);
    // For Case c)
    size_t len = strlen(EXTRA_CHARACTERS);

    // find index of c in EXTRA_CHARACTERS
    for (int i = 0; i < len; i++)
    {
        char extra = EXTRA_CHARACTERS[i];
        if (extra == c)
        {
            return 25 + (i + 1); //(122 - 97) = 25
        }
    }

    // printf("- Index cannot found for '%c'\n", c);
    return -1;
}