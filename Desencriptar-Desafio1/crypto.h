#ifndef CRYPTO_H
#define CRYPTO_H
#include <cstddef>

unsigned char aplicarXor(unsigned char byte, unsigned char K);
void desencriptar(unsigned char* buffer, size_t tamaño, unsigned char K);

#endif // CRYPTO_H
