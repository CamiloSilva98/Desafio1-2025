#ifndef CRYPTO_H
#define CRYPTO_H
#include <cstddef>

unsigned char aplicarXor(unsigned char byte, unsigned char K);
unsigned char rotarNDerecha(unsigned char x, int n);
void desencriptar(unsigned char* buffer, size_t tamano, unsigned char K, int n);

#endif // CRYPTO_H
