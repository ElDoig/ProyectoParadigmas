// Prueba del registro de alturas y limites de vias de Diego.
#include <iostream>
#include "../cpp/tipos.h"
#include "../cpp/diego.h"

// Muestra "ok" o el mensaje de error de cada operacion.
void mostrar(std::string prueba, std::string error) {
    if (error == "") std::cout << prueba << ": ok\n";
    else std::cout << prueba << ": " << error << "\n";
}

int main() {
    Unidad unidades[4] = {
        {"U01", "cisterna", 10000, 3.5, EstadoUnidad::disponible},
        {"U02", "cisterna", 7000, 3.0, EstadoUnidad::ocupada},
        {"U03", "cisterna", 15000, 4.5, EstadoUnidad::disponible},
        {"U04", "cisterna", 5000, 2.5, EstadoUnidad::ocupada}};
    Conexion conexiones[3] = {
        {"P06", "P07", 2.0, 8, EstadoConexion::habilitada},
        {"P07", "P03", 5.5, 20, EstadoConexion::habilitada},
        {"P01", "P07", 3.5, 12, EstadoConexion::habilitada}};
    DimensionCisterna alturas[10];
    RestriccionAltura restricciones[10];
    int cantidadAlturas = 0;
    int cantidadRestricciones = 0;

    mostrar("Altura U01 3.5", registrarAltura({"U01", 3.5}, alturas, &cantidadAlturas, 10, unidades, 4));
    mostrar("Altura U03 3.8", registrarAltura({"U03", 3.8}, alturas, &cantidadAlturas, 10, unidades, 4));
    mostrar("Altura U01 repetida", registrarAltura({"U01", 3.0}, alturas, &cantidadAlturas, 10, unidades, 4));
    mostrar("Altura U99", registrarAltura({"U99", 3.0}, alturas, &cantidadAlturas, 10, unidades, 4));
    mostrar("Altura U02 0", registrarAltura({"U02", 0}, alturas, &cantidadAlturas, 10, unidades, 4));

    mostrar("Limite P07-P03 3.4",
            registrarRestriccionAltura({"P07", "P03", 3.4}, restricciones, &cantidadRestricciones, 10, conexiones, 3));
    mostrar("Limite P03-P07 repetido al reves",
            registrarRestriccionAltura({"P03", "P07", 3.0}, restricciones, &cantidadRestricciones, 10, conexiones, 3));
    mostrar("Limite P01-P10 sin conexion",
            registrarRestriccionAltura({"P01", "P10", 3.0}, restricciones, &cantidadRestricciones, 10, conexiones, 3));

    bool puede = false;
    consultarPasoAltura("U01", "P03", "P07", alturas, cantidadAlturas, restricciones, cantidadRestricciones, &puede);
    std::cout << "U01 por P03-P07: " << (puede ? "puede pasar" : "no puede pasar") << "\n";
    consultarPasoAltura("U03", "P01", "P07", alturas, cantidadAlturas, restricciones, cantidadRestricciones, &puede);
    std::cout << "U03 por P01-P07: " << (puede ? "puede pasar" : "no puede pasar") << "\n";
    mostrar("Paso de U02", consultarPasoAltura("U02", "P01", "P07", alturas, cantidadAlturas,
                                               restricciones, cantidadRestricciones, &puede));
    return 0;
}