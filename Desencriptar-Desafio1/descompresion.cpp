#include "descompresion.h"
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

char* descomprimirRle(const unsigned char* datos, size_t tamano, size_t& tamanoSalida) {
    if (!datos || tamano < 2) {
        tamanoSalida = 0;
        return nullptr;
    }

    size_t capacidad = (tamano > 0) ? (tamano * 8) : 1024;
    char* bufferSalida = (char*)malloc(capacidad);
    if (!bufferSalida) {
        tamanoSalida = 0;
        return nullptr;
    }

    size_t posSalida = 0;
    for (size_t i = 0; i + 1 < tamano; i += 2) {
        unsigned char cantidad = datos[i];
        char simbolo = (char)datos[i + 1];

        if (posSalida + (size_t)cantidad >= capacidad) {
            while (posSalida + (size_t)cantidad >= capacidad) capacidad *= 2;
            char* tmp = (char*)realloc(bufferSalida, capacidad);
            if (!tmp) { free(bufferSalida); tamanoSalida = 0; return nullptr; }
            bufferSalida = tmp;
        }

        for (unsigned int j = 0; j < cantidad; ++j) {
            bufferSalida[posSalida++] = simbolo;
        }
    }

    if (posSalida == 0) {
        free(bufferSalida);
        tamanoSalida = 0;
        return nullptr;
    }

    char* bufferFinal = (char*)realloc(bufferSalida, posSalida);
    if (bufferFinal) bufferSalida = bufferFinal;
    tamanoSalida = posSalida;
    return bufferSalida;
}


struct EntradaLz78 {
    int padre;
    char simbolo;
};
char* descomprimirLz78(const unsigned char* datos, size_t tamano, size_t& tamanoSalida) {
    if (!datos || tamano < 3) {
        tamanoSalida = 0;
        return nullptr;
    }
    size_t capacidadDicc = 1024;
    size_t usadosDicc = 0;
    EntradaLz78* diccionario = (EntradaLz78*)malloc(capacidadDicc * sizeof(EntradaLz78));
    if (!diccionario) {
        tamanoSalida = 0;
        return nullptr;
    }
    size_t capacidadSalida = (tamano > 0) ? (tamano * 8) : 4096;
    char* bufferSalida = (char*)malloc(capacidadSalida);
    if (!bufferSalida) {
        free(diccionario);
        tamanoSalida = 0;
        return nullptr;
    }
    size_t posLectura = 0;
    size_t posSalida = 0;

    const int MAX_TEMP = 8192;
    char temp[MAX_TEMP];

    while (posLectura + 3 <= tamano) {
        uint16_t indice = ((uint16_t)datos[posLectura] << 8) | (uint16_t)datos[posLectura + 1];
        char simbolo = (char)datos[posLectura + 2];
        posLectura += 3;

        int longitudTemp = 0;
        if (indice > 0) {
            int nodo = (int)indice - 1;
            while (nodo >= 0 && longitudTemp < MAX_TEMP) {
                temp[longitudTemp++] = diccionario[nodo].simbolo;
                nodo = diccionario[nodo].padre;
            }
            for (int t = longitudTemp - 1; t >= 0; --t) {
                if (posSalida + 1 >= capacidadSalida) {
                    capacidadSalida *= 2;
                    char* tmp = (char*)realloc(bufferSalida, capacidadSalida);
                    if (!tmp) { free(diccionario); free(bufferSalida); tamanoSalida = 0; return nullptr; }
                    bufferSalida = tmp;
                }
                bufferSalida[posSalida++] = temp[t];
            }
        }
        if (posSalida + 1 >= capacidadSalida) {
            capacidadSalida *= 2;
            char* tmp = (char*)realloc(bufferSalida, capacidadSalida);
            if (!tmp) { free(diccionario); free(bufferSalida); tamanoSalida = 0; return nullptr; }
            bufferSalida = tmp;
        }
        bufferSalida[posSalida++] = simbolo;
        if (usadosDicc >= capacidadDicc) {
            capacidadDicc *= 2;
            EntradaLz78* tmpd = (EntradaLz78*)realloc(diccionario, capacidadDicc * sizeof(EntradaLz78));
            if (!tmpd) { free(diccionario); free(bufferSalida); tamanoSalida = 0; return nullptr; }
            diccionario = tmpd;
        }
        diccionario[usadosDicc].padre = (int)indice - 1;
        diccionario[usadosDicc].simbolo = simbolo;
        usadosDicc++;
    }
    if (posSalida == 0) {
        free(diccionario);
        free(bufferSalida);
        tamanoSalida = 0;
        return nullptr;
    }
    char* bufferFinal = (char*)realloc(bufferSalida, posSalida);
    if (bufferFinal) bufferSalida = bufferFinal;
    tamanoSalida = posSalida;

    free(diccionario);
    return bufferSalida;
}
