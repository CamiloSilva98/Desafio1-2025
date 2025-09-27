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
        liberarBuffer(datosEncriptados);
        liberarBuffer(pista);
    }
    return 0;
}
