#include "crypto.h"

unsigned char aplicarXor(unsigned char byte, unsigned char K)
{
    return byte ^ K;
}
void desencriptar(unsigned char* buffer, size_t tamaño, unsigned char K)
{
    for (size_t i = 0; i < tamaño; i++)
    {
        buffer[i] = aplicarXor(buffer[i], K);
    }
}
