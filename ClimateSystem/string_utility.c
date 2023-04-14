#include "string_utility.h"

#include <stdio.h>

// Процедура вывода строки в
// стандартный поток ошибок
void puterr(string_t s)
{
    fputs(s, stderr);
    fputs(ENDL, stderr);
}
