#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "io_utils.h"
#include "analyzer.h"
using namespace std;

int main()
{
    int numCasos;
    cout << "Cuantos archivos se evaluaran?: ";
    cin >> numCasos;

    for (int i = 1; i <= numCasos; i++)
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

            int metodoEncontrado = -1;      // 0=RLE, 1=LZ78
            unsigned char claveEncontrada = 0;
            int rotacionEncontrada = 0;

            cout << "Iniciando busqueda exhaustiva..." << endl;

            // Llamar a la función del analyzer
            char* mensajeRecuperado = identify_and_recover(
                datosEncriptados,
                tamano,
                pista,
                longitudPista,
                metodoEncontrado,
                claveEncontrada,
                rotacionEncontrada
                );

            if (mensajeRecuperado)
            {
                // ¡ÉXITO! Se encontró la solución
                cout << "EXITO! Pista encontrada" << endl;
                cout << "Metodo: " << (metodoEncontrado == 0 ? "RLE" : "LZ78") << endl;
                cout << "Clave: 0x" <<hex << (int)claveEncontrada << dec << endl;
                cout << "Rotacion: " << rotacionEncontrada << " bits" << endl;

                // El tamaño se puede obtener con strlen para texto o calcularlo
                size_t tamanoRecuperado = strlen(mensajeRecuperado);
                cout << "Tamanio recuperado: " << tamanoRecuperado << " bytes" << endl;

                // Crear nombre del archivo de salida
                sprintf(nombreSalida, "Archivos\\Recuperado_%s_%d.txt",(metodoEncontrado == 0 ? "RLE" : "LZ78"), i);
                if (guardarMensajeRecuperado(nombreSalida, mensajeRecuperado, tamanoRecuperado))
                {
                    cout << "Archivo guardado: " << nombreSalida << endl;
                }
                else
                {
                    cout << "Error al guardar el archivo" << endl;
                }
                // Mostrar parte del mensaje recuperado
                cout << "Mensaje recuperado: ";
                for (size_t j = 0; j < min(100, (int)tamanoRecuperado); j++)
                {
                    cout << mensajeRecuperado[j];
                }
                if (tamanoRecuperado > 100) cout << "...";
                cout << endl;

                // Liberar memoria del mensaje recuperado
                free(mensajeRecuperado);
            }
            else
            {
                cout << "ADVERTENCIA: La pista no fue encontrada en ninguna combinacion para el caso " << i << endl;
            }
        }
        else
        {
            cout << "Error leyendo archivos del caso " << i << endl;
        }
        // Liberar memoria de los archivos leídos
        if (datosEncriptados)
        {
            liberarBuffer(datosEncriptados);
        }
        if (pista)
        {
            liberarBuffer(pista);
        }

        cout << "\n================================\n" << endl;
    }

    return 0;
}
