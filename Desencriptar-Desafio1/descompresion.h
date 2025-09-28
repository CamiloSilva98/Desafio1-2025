#ifndef DESCOMPRESION_H
#define DESCOMPRESION_H

#include <cstddef>

char* descomprimirRle(const unsigned char* datos, size_t tamano, size_t& tamanoSalida);
char* descomprimirLz78(const unsigned char* datos, size_t tamano, size_t& tamanoSalida);
int buscar_subarray(const char* haystack, size_t haylen, const char* needle, size_t needlen);
bool guardarArchivoBuffer(const char* datos, size_t size, const char* nombreSalida);

#endif // DESCOMPRESION_H
