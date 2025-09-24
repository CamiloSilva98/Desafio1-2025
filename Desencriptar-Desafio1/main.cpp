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


// ------ DESCOMPRESIÓN RLE ------
// Formato asumido: pares (count, symbol) en bytes consecutivos.
// Ejemplo: [3]['A'][2]['B'] → "AAABB"

char* descomprimirRle(const unsigned char* datos, size_t tamaño, size_t& salidaSize) {

    size_t capacidad = tamaño * 16;  // Estimación inicial: salida no puede ser mayor a count * cantidad de pares
    char* salida = (char*)malloc(capacidad);   // Reservamos un buffer dinámico grande y luego lo ajustamos
    if (!salida) return nullptr;

    size_t posOut = 0;
    for (size_t i = 0; i + 1 < tamaño; i += 2) {
        unsigned char count = datos[i];
        char simbolo = (char)datos[i + 1];

        if (posOut + count >= capacidad) {  // Asegurar capacidad
            capacidad *= 2;
            salida = (char*)realloc(salida, capacidad);
            if (!salida) return nullptr;
        }

        for (int j = 0; j < count; j++) { // Repetir el símbolo count veces
            salida[posOut++] = simbolo;
        }
    }

    salidaSize = posOut;
    salida = (char*)realloc(salida, salidaSize);     // Ajustar tamaño de memoria
    return salida;
}


