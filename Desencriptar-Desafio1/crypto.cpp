#include "crypto.h"

unsigned char aplicarXor(unsigned char byte, unsigned char K)
{
    return byte ^ K;
}
unsigned char rotarNDerecha(unsigned char x, int n)
{
    n = n % 8;
    return ((x >> n) | (x << (8 - n))) & 0xFF;
}
void desencriptar(unsigned char* buffer, size_t tamaño, unsigned char K, int n)
{
    for (size_t i = 0; i < tamaño; i++)
    {
        buffer[i] = aplicarXor(buffer[i], K);
        buffer[i] = rotarNDerecha(buffer[i], n);
    }
}
