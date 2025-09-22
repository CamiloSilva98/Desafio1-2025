#include <iostream>
#include "io_utils.h"
using namespace std;
int main()
{
    int numCasos;
    cout << "Cuantos archivos se evaluaran?: ";
    cin >> numCasos;
    for (int i=1;i<=numCasos;i++)
    {
        char nombreEncriptado[20];
        char nombrePista[20];
        sprintf(nombreEncriptado, "Encriptado%d.txt", i);
        sprintf(nombrePista, "pista%d.txt", i);
        cout << "Procesando caso " << i << "..." << endl;
    }
    return 0;
}
