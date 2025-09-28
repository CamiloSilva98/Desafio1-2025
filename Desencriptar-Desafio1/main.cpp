#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "io_utils.h"
#include "crypto.h"
#include "descompresion.h"
using namespace std;
int main()
{
    int numCasos;
    cout << "Cuantos archivos se evaluaran?: ";
    cin >> numCasos;

    for (int i=1;i<=numCasos;i++)
    {
        char nombreEncriptado[50];
        char nombrePista[50];
        char nombreSalida[50];
        sprintf(nombreEncriptado, "Archivos\\Encriptado%d.txt", i);
        sprintf(nombrePista, "Archivos\\pista%d.txt", i);
        cout << "Procesando caso " << i << "..." << endl;
        size_t tamano = 0;
        size_t longitudPista = 0;
        unsigned char* datosEncriptados = leerArchivoEncriptado(nombreEncriptado, tamano);
        char* pista = leerFragmentoConocido(nombrePista, longitudPista);

        if (datosEncriptados && pista && tamano > 0 && longitudPista > 0)
        {
            cout << "Archivos leidos correctamente" << endl;
            cout << "Peso encriptado: " << tamano << " bytes" << endl;
            cout << "Pista: " << pista << endl;

            // Parámetros de desencriptación
            unsigned char clave = 0x40;
            int rotacion = 3;

            // Desencriptar datos
            desencriptar(datosEncriptados, tamano, clave, rotacion);

            bool encontrado = false;

            // Búsqueda exhaustiva según el preinforme: 2 métodos × 7 rotaciones × 256 claves
            for (unsigned char clave = 0; clave <= 255 && !encontrado; clave++)
            {
                for (int rotacion = 1; rotacion <= 7 && !encontrado; rotacion++)
                {
                    // Crear una copia para no modificar los datos originales
                    unsigned char* datosCopia = new unsigned char[tamano];
                    for (size_t j = 0; j < tamano; j++) {
                        datosCopia[j] = datosEncriptados[j];
                    }

                    // Desencriptar datos (XOR primero, luego rotación según el preinforme)
                    desencriptar(datosCopia, tamano, clave, rotacion);

                    // Probar RLE
                    size_t tamanoSalidaRle = 0;
                    char* salidaRle = descomprimirRle(datosCopia, tamano, tamanoSalidaRle);

                    if (salidaRle && tamanoSalidaRle > 0) {
                        int pos = buscar_subarray(salidaRle, tamanoSalidaRle, pista, longitudPista);
                        if (pos >= 0) {
                            sprintf(nombreSalida, "Archivos\\Recuperado_RLE_%d.txt", i);
                            if (guardarArchivoBuffer(salidaRle, tamanoSalidaRle, nombreSalida)) {
                                cout << "¡EXITO! Pista encontrada con RLE, clave=" << (int)clave
                                     << ", rotacion=" << rotacion << endl;
                                cout << "Archivo guardado: " << nombreSalida << endl;
                            }
                            encontrado = true;
                        }
                        free(salidaRle);
                    }

                    // Probar LZ78 si RLE no funcionó
                    if (!encontrado) {
                        size_t tamanoSalidaLz = 0;
                        char* salidaLz = descomprimirLz78(datosCopia, tamano, tamanoSalidaLz);

                        if (salidaLz && tamanoSalidaLz > 0) {
                            int posL = buscar_subarray(salidaLz, tamanoSalidaLz, pista, longitudPista);
                            if (posL >= 0) {
                                sprintf(nombreSalida, "Archivos\\Recuperado_LZ78_%d.txt", i);
                                if (guardarArchivoBuffer(salidaLz, tamanoSalidaLz, nombreSalida)) {
                                    cout << "¡EXITO! Pista encontrada con LZ78, clave=" << (int)clave
                                         << ", rotacion=" << rotacion << endl;
                                    cout << "Archivo guardado: " << nombreSalida << endl;
                                }
                                encontrado = true;
                            }
                            free(salidaLz);
                        }
                    }

                    delete[] datosCopia;
                }
            }

            if (!encontrado) {
                cout << "ADVERTENCIA: La pista no fue encontrada en ninguna combinación para el caso " << i << endl;
            }
        }
        else
        {
            cout << "Error leyendo archivos del caso " << i << endl;
        }

        // Siempre liberar memoria al final
        if (datosEncriptados) {
            liberarBuffer(datosEncriptados);
        }
        if (pista) {
            liberarBuffer(pista);
        }
    }
    return 0;
}
