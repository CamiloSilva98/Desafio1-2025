#include "analyzer.h"
#include "crypto.h"
#include "descompresion.h"
#include <cstdlib>
char* identify_and_recover(unsigned char* datosEncriptados,
                           size_t tamano, const char* pista,
                           size_t longitudPista,
                           int& metodoEncontrado,
                           unsigned char& claveEncontrada,
                           int& rotacionEncontrada)
{// Búsqueda exhaustiva según el preinforme: 2 métodos × 7 rotaciones × 256 claves
    for (int clave = 0x0; clave <= 0xff; clave++)
    {
        //cout<<"\nclave: "<<clave;
        for (int rotacion = 1; rotacion <= 7; rotacion++)
        {// Crear una copia para no modificar los datos originales
            //cout<<"\nrotacion: "<<rotacion;
            unsigned char* datosCopia = new unsigned char[tamano];
            for (size_t j = 0; j < tamano; j++)
            {
                datosCopia[j] = datosEncriptados[j];
            }
            // Desencriptar datos (XOR primero, luego rotación según el preinforme)
            desencriptar(datosCopia, tamano, clave, rotacion);
            // Probar RLE
            size_t tamanoSalidaRle = 0;
            char* salidaRle = descomprimirRle(datosCopia, tamano, tamanoSalidaRle);
            if (salidaRle && tamanoSalidaRle > 0)
            {
                int pos = buscar_subarray(salidaRle, tamanoSalidaRle, pista, longitudPista);
                if (pos >= 0)
                {
                    metodoEncontrado = 0; // RLE
                    claveEncontrada = clave;
                    rotacionEncontrada = rotacion;
                    delete[] datosCopia;
                    return salidaRle; // Retornar mensaje recuperado
                }
                free(salidaRle);
            }
            // Probar LZ78 si RLE no funcionó
            size_t tamanoSalidaLz = 0;
            char* salidaLz = descomprimirLz78(datosCopia, tamano, tamanoSalidaLz);
            if (salidaLz && tamanoSalidaLz > 0)
            {
                int posL = buscar_subarray(salidaLz, tamanoSalidaLz, pista, longitudPista);
                if (posL >= 0)
                {
                    metodoEncontrado = 1; // LZ78
                    claveEncontrada = clave;
                    rotacionEncontrada = rotacion;
                    delete[] datosCopia;
                    return salidaLz;
                }
                free(salidaLz);
            }

            delete[] datosCopia;
        }
    }

    return nullptr; // No encontrado
}

