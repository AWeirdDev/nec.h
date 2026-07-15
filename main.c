#include <stdio.h>

#define NEC_IMPLEMENTATION
#include "nec.h"

int main() {
    String s = nec_string_from_cstr("fuck you!");

    if (!nec_string_push_cstr(&s, " also me")) return 1;
    printf(NEC_SV_FMT "\n", NEC_SV_ARG(&s));

    return 0;
}
