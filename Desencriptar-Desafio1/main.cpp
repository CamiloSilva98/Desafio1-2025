#include <iostream>
#include "io_utils.h"
#include "crypto.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>
using namespace std;
int main()
{
    int numCasos;
    cout << "Cuantos archivos se evaluaran?: ";
    cin >> numCasos;
    for (int i=1;i<=numCasos;i++)
    {
        char nombreEncriptado[30];
        char nombrePista[30];
        sprintf(nombreEncriptado, "Archivos\\Encriptado%d.txt", i);
        sprintf(nombrePista, "Archivos\\pista%d.txt", i);
        cout << "Procesando caso " << i << "..." << endl;
        size_t tamañoEncriptado, longitudPista;
        unsigned char* datosEncriptados = leerArchivoEncriptado(nombreEncriptado, tamañoEncriptado);
        char* pista = leerFragmentoConocido(nombrePista, longitudPista);

        if (datosEncriptados && pista)
        {
            cout << "Archivos leidos correctamente" << endl;
            cout << "Peso encriptado: " << tamañoEncriptado << " bytes" << endl;
            cout << "Pista: " << pista << endl;
        }
        else
        {
            cout << "Error leyendo archivos del caso " << i << endl;
        }
        int k = 0x40;
        int n = 3;
        desencriptar(datosEncriptados, tamañoEncriptado, k, n);
        liberarBuffer(datosEncriptados);
        liberarBuffer(pista);
    }
    return 0;
}


// ------ DESCOMPRESIÓN RLE -----
// Formato asumido: pares (count, symbol) en bytes consecutivos.
// Ejemplo: [3]['A'][2]['B'] → "AAABB"

char* descomprimirRle(const unsigned char* datos, size_t tamaño, size_t& salidaSize) {
    size_t capacidad = tamaño * 16;
    char* salida = (char*)malloc(capacidad);
    if (!salida) return nullptr;

    size_t posOut = 0;
    for (size_t i = 0; i + 1 < tamaño; i += 2) {
        unsigned char count = datos[i];
        char simbolo = (char)datos[i + 1];

        // Expandir el símbolo count veces
        for (int j = 0; j < count; j++) {
            if (posOut >= capacidad) {
                capacidad *= 2;
                salida = (char*)realloc(salida, capacidad);
                if (!salida) return nullptr;
            }
            salida[posOut++] = simbolo;
        }
    }

    salidaSize = posOut;
    salida = (char*)realloc(salida, salidaSize);
    return salida;
}


// ----- DESCOMPRESIÓN LZ78 -----
// Formato asumido: cada entrada = [uint16_t index][char symbol]
// Ejemplo clásico: (0,'A')(0,'B')(2,'A')(2,'B')...

struct NodoLZ78 {
    int parent;
    char symbol;
};

char* descomprimirLz78(const unsigned char* datos, size_t tamaño, size_t& salidaSize) {
    size_t capacidadDict = 1024;
    size_t usadosDict = 0;
    NodoLZ78* dict = (NodoLZ78*)malloc(capacidadDict * sizeof(NodoLZ78));
    if (!dict) return nullptr;

    size_t capacidadOut = tamaño * 16;
    char* salida = (char*)malloc(capacidadOut);
    if (!salida) {
        free(dict);
        return nullptr;
    }

    size_t posOut = 0;
    size_t pos = 0;

    while (pos + 3 <= tamaño) {
        uint16_t indice = (datos[pos] << 8) | datos[pos + 1];
        char simbolo = (char)datos[pos + 2];
        pos += 3;

        // Reconstruir palabra desde el diccionario
        char temp[4096];
        int tempLen = 0;

        if (indice > 0 && indice <= usadosDict) {
            int p = indice - 1;
            while (p >= 0 && tempLen < 4096) {
                temp[tempLen++] = dict[p].symbol;
                p = dict[p].parent;
            }
            for (int i = tempLen - 1; i >= 0; i--) {
                if (posOut >= capacidadOut) {
                    capacidadOut *= 2;
                    salida = (char*)realloc(salida, capacidadOut);
                    if (!salida) {
                        free(dict);
                        return nullptr;
                    }
                }
                salida[posOut++] = temp[i];
            }
        }

        // Añadir símbolo final
        if (posOut >= capacidadOut) {
            capacidadOut *= 2;
            salida = (char*)realloc(salida, capacidadOut);
            if (!salida) {
                free(dict);
                return nullptr;
            }
        }
        salida[posOut++] = simbolo;

        // Guardar en diccionario
        if (usadosDict >= capacidadDict) {
            capacidadDict *= 2;
            dict = (NodoLZ78*)realloc(dict, capacidadDict * sizeof(NodoLZ78));
            if (!dict) return nullptr;
        }
        dict[usadosDict].parent = indice - 1;
        dict[usadosDict].symbol = simbolo;
        usadosDict++;
    }

    salidaSize = posOut;
    salida = (char*)realloc(salida, salidaSize);

    free(dict);
    return salida;
}
