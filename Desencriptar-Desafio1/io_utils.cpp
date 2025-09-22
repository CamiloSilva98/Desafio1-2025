#include "io_utils.h"
#include <iostream>
#include <fstream>
using namespace std;
unsigned char* leerArchivoEncriptado(const char* nombreArchivo, size_t& tamaño)
{
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open())
    {
        cout << "Error: No se pudo abrir " << nombreArchivo << endl;
        return nullptr;
    }
    archivo.seekg(0, ios::end);
    tamaño = archivo.tellg();
    archivo.seekg(0, ios::beg);
    unsigned char* buffer = new unsigned char[tamaño];
    archivo.read(reinterpret_cast<char*>(buffer), tamaño);
    archivo.close();
    return buffer;
}
char* leerFragmentoConocido(const char* nombreArchivo, size_t& longitud)
{
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open())
    {
        cout << "Error: No se pudo abrir " << nombreArchivo << endl;
        return nullptr;
    }
    archivo.seekg(0, ios::end);
    longitud = archivo.tellg();
    archivo.seekg(0, ios::beg);
    char* fragmento = new char[longitud + 1];
    archivo.read(fragmento, longitud);
    fragmento[longitud] = '\0';
    archivo.close();
    return fragmento;
}
bool guardarMensajeRecuperado(const char* nombreArchivo, const char* mensaje, size_t longitud)
{
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open())
    {
        cout << "Error: No se pudo crear " << nombreArchivo << endl;
        return false;
    }
    archivo.write(mensaje, longitud);
    archivo.close();
    return true;
}
void liberarBuffer(void* buffer)
{
    delete[] static_cast<unsigned char*>(buffer);
}
