#include "descompresion.h"
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

char* descomprimirRle(const unsigned char* datos, size_t tamano, size_t& tamanoSalida) {
    if (!datos || tamano < 2) {
        tamanoSalida = 0;
        return nullptr;
    }

    size_t capacidad = (tamano > 0) ? (tamano * 8) : 1024;
    char* bufferSalida = static_cast<char*>(std::malloc(capacidad));
    if (!bufferSalida) {
        tamanoSalida = 0;
        return nullptr;
    }

    size_t posSalida = 0;
    for (size_t i = 0; i + 1 < tamano; i += 2) {
        unsigned char cantidad = datos[i];
        char simbolo = static_cast<char>(datos[i + 1]);

        if (posSalida + static_cast<size_t>(cantidad) >= capacidad) {
            while (posSalida + static_cast<size_t>(cantidad) >= capacidad) capacidad *= 2;
            char* tmp = static_cast<char*>(std::realloc(bufferSalida, capacidad));
            if (!tmp) { std::free(bufferSalida); tamanoSalida = 0; return nullptr; }
            bufferSalida = tmp;
        }

        for (unsigned int j = 0; j < cantidad; ++j) {
            bufferSalida[posSalida++] = simbolo;
        }
    }

    if (posSalida == 0) {
        std::free(bufferSalida);
        tamanoSalida = 0;
        return nullptr;
    }

    char* bufferFinal = static_cast<char*>(std::realloc(bufferSalida, posSalida));
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
    EntradaLz78* diccionario = static_cast<EntradaLz78*>(std::malloc(capacidadDicc * sizeof(EntradaLz78)));
    if (!diccionario) {
        tamanoSalida = 0;
        return nullptr;
    }
    size_t capacidadSalida = (tamano > 0) ? (tamano * 8) : 4096;
    char* bufferSalida = static_cast<char*>(std::malloc(capacidadSalida));
    if (!bufferSalida) {
        std::free(diccionario);
        tamanoSalida = 0;
        return nullptr;
    }
    size_t posLectura = 0;
    size_t posSalida = 0;

    const int MAX_TEMP = 8192;
    char temp[MAX_TEMP];

    while (posLectura + 3 <= tamano) {
        uint16_t indice = (static_cast<uint16_t>(datos[posLectura]) << 8) | static_cast<uint16_t>(datos[posLectura + 1]);
        char simbolo = static_cast<char>(datos[posLectura + 2]);
        posLectura += 3;

        int longitudTemp = 0;
        if (indice > 0 && indice <= usadosDicc) {
            int nodo = static_cast<int>(indice) - 1;
            // Validar que el nodo inicial está en rango válido
            if (nodo >= 0 && nodo < static_cast<int>(usadosDicc)) {
                while (nodo >= 0 && nodo < static_cast<int>(usadosDicc) && longitudTemp < MAX_TEMP - 1) {
                    temp[longitudTemp] = diccionario[nodo].simbolo;
                    longitudTemp++;
                    nodo = diccionario[nodo].padre;
                }
            }
            // Escribir los símbolos en orden inverso
            for (int t = longitudTemp - 1; t >= 0; --t) {
                if (posSalida + 1 >= capacidadSalida) {
                    capacidadSalida *= 2;
                    char* tmp = static_cast<char*>(realloc(bufferSalida, capacidadSalida));
                    if (!tmp) {
                        free(diccionario);
                        free(bufferSalida);
                        tamanoSalida = 0;
                        return nullptr;
                    }
                    bufferSalida = tmp;
                }
                bufferSalida[posSalida++] = temp[t];
            }
        }

        if (posSalida + 1 >= capacidadSalida) {
            capacidadSalida *= 2;
            char* tmp = static_cast<char*>(std::realloc(bufferSalida, capacidadSalida));
            if (!tmp) {
                std::free(diccionario);
                std::free(bufferSalida);
                tamanoSalida = 0;
                return nullptr;
            }
            bufferSalida = tmp;
        }
        bufferSalida[posSalida++] = simbolo;

        if (usadosDicc >= capacidadDicc) {
            capacidadDicc *= 2;
            EntradaLz78* tmpd = static_cast<EntradaLz78*>(std::realloc(diccionario, capacidadDicc * sizeof(EntradaLz78)));
            if (!tmpd) {
                std::free(diccionario);
                std::free(bufferSalida);
                tamanoSalida = 0;
                return nullptr;
            }
            diccionario = tmpd;
        }
        diccionario[usadosDicc].padre = static_cast<int>(indice) - 1;
        diccionario[usadosDicc].simbolo = simbolo;
        usadosDicc++;
    }
    if (posSalida == 0) {
        std::free(diccionario);
        std::free(bufferSalida);
        tamanoSalida = 0;
        return nullptr;
    }
    char* bufferFinal = static_cast<char*>(std::realloc(bufferSalida, posSalida));
    if (bufferFinal) {
        bufferSalida = bufferFinal;
    }
    tamanoSalida = posSalida;

    std::free(diccionario);
    return bufferSalida;
}

int buscar_subarray(const char* haystack, size_t haylen, const char* needle, size_t needlen) {
    if (!haystack || !needle || needlen == 0 || haylen < needlen) {
        return -1;
    }

    for (size_t i = 0; i <= haylen - needlen; i++) {
        size_t j = 0;
        while (j < needlen && haystack[i + j] == needle[j]) {
            j++;
        }
        if (j == needlen) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool guardarArchivoBuffer(const char* datos, size_t size, const char* nombreSalida) {
    std::ofstream archivo(nombreSalida, std::ios::binary);
    if (!archivo.is_open()) {
        std::cout << "Error: No se pudo crear " << nombreSalida << std::endl;
        return false;
    }
    archivo.write(datos, static_cast<std::streamsize>(size));
    archivo.close();
    return true;
}
