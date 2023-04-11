#include "string_utility.h"

#include <stdio.h>

void puterr(string_t s)
{
    fputs(s, stderr);
    fputs(ENDL, stderr);
}
