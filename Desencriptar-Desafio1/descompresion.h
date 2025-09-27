#ifndef DESCOMPRESION_H
#define DESCOMPRESION_H

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

char* descomprimirRle(const unsigned char* datos, size_t tamaño, size_t& salidaSize);
char* descomprimirLz78(const unsigned char* datos, size_t tamaño, size_t& salidaSize);
int buscar_subarray(const char* haystack, size_t haylen, const char* needle, size_t needlen);
bool guardarArchivoBuffer(const char* datos, size_t size, const char* nombreSalida);

#ifdef __cplusplus
}
#endif

#endif // DESCOMPRESION_H
