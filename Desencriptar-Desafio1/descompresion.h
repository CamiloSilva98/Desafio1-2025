#ifndef DESCOMPRESION_H
#define DESCOMPRESION_H

#include <cstddef>

char* descomprimirRle(const unsigned char* datos, size_t tamano, size_t& tamanoSalida);
char* descomprimirLz78(const unsigned char* datos, size_t tamano, size_t& tamanoSalida);
int buscar_subarray(const char* haystack, size_t haylen, const char* needle, size_t needlen);

#endif // DESCOMPRESION_H
