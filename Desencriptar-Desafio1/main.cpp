#include <iostream>
#include "io_utils.h"
#include "crypto.h"
//#include <cstdlib>
//#include <cstring>
//#include <cstdint>

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

        bool encontrado = false;
        //  Intento con RLE
        size_t salidaSizeRle = 0;
        char* salidaRle = descomprimirRle(datosEncriptados, tamañoEncriptado, salidaSizeRle);

        if (salidaRle && salidaSizeRle > 0) {
            int pos = buscar_subarray(salidaRle, salidaSizeRle, pista, longitudPista);
            if (pos >= 0) {
                sprintf(nombreSalida, "Archivos\\Recuperado_RLE_%d.txt", i);
                if (guardarArchivoBuffer(salidaRle, salidaSizeRle, nombreSalida))
                    cout << "La pista fue encontrada en RLE y se guardó correctamente: " << nombreSalida << endl;
                else
                    cout << "La pista fue encontrada en RLE, pero hubo un fallo al guardar el archivo." << endl;

                encontrado = true;
            } else {
                cout << "La pista no fue encontrada en RLE." << endl;
            }
        } else {
            cout << "No se obtuvo salida (RLE)." << endl;
        }

        // LZ78 (solo si no se encontró en RLE)
        size_t salidaSizeLz = 0;
        char* salidaLz = nullptr;

        if (!encontrado) {
            salidaLz = descomprimirLz78(datosEncriptados, tamañoEncriptado, salidaSizeLz);

            if (salidaLz && salidaSizeLz > 0) {
                int posL = buscar_subarray(salidaLz, salidaSizeLz, pista, longitudPista);
                if (posL >= 0) {
                    sprintf(nombreSalida, "Archivos\\Recuperado_LZ78_%d.txt", i);
                    if (guardarArchivoBuffer(salidaLz, salidaSizeLz, nombreSalida))
                        cout << "La pista fue encontrada en LZ78 y se guardó correctamente: " << nombreSalida << endl;
                    else
                        cout << "La pista fue encontrada en LZ78, pero hubo fallo al guardar el archivo." << endl;

                    encontrado = true;
                } else {
                    cout << "La pista no fue encontrada en LZ78." << endl;
                }
            } else {
                cout << "No se obtuvo salida (LZ78)." << endl;
            }
            if (salidaLz) liberarBuffer(salidaLz);
        }

        if (salidaRle) liberarBuffer(salidaRle);
        liberarBuffer(datosEncriptados);
        liberarBuffer(pista);
    }
    return 0;
}
