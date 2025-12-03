#include "utils.h"
#include <stdarg.h>
#include <stdlib.h>

short c_to_s(const char character) { return (short)(character - '0'); }

size_t min_size_t(const size_t elem_count, ...) {
    va_list args;
    va_start(args, elem_count);

    size_t min_value = (elem_count != 0) ? va_arg(args, size_t) : 0;
    for (size_t i = 1; i < elem_count; i++) {
        const size_t number = va_arg(args, size_t);
        min_value = (number < min_value) ? number : min_value;
    }

    va_end(args);
    return min_value;
}
