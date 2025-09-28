#ifndef ANALYZER_H
#define ANALYZER_H

#include <cstddef>

char* identify_and_recover(unsigned char* datosEncriptados,
                           size_t tamano, const char* pista,
                           size_t longitudPista,
                           int& metodoEncontrado,
                           unsigned char& claveEncontrada,
                           int& rotacionEncontrada);

#endif // ANALYZER_H
