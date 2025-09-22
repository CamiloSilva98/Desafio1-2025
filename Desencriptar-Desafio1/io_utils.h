#ifndef IO_UTILS_H
#define IO_UTILS_H
#include <cstddef>

unsigned char* leerArchivoEncriptado(const char* nombreArchivo, size_t& tamaño);
char* leerFragmentoConocido(const char* nombreArchivo, size_t& longitud);
bool guardarMensajeRecuperado(const char* nombreArchivo, const char* mensaje, size_t longitud);
void liberarBuffer(void* buffer);

#endif // IO_UTILS_H
