// Prueba manual del submenu de Diego con pocos datos.
#include <iostream>
#include "../cpp/tipos.h"
#include "../cpp/diego.h"

int main() {
    Punto puntos[3] = {
        {"P01", "Planta_Chillon", TipoPunto::planta, "carabayllo"},
        {"P03", "AAHH_Collique", TipoPunto::asentamiento, "comas"},
        {"P07", "Mercado_Tungasuca", TipoPunto::mercado, "carabayllo"}};
    Conexion conexiones[2] = {
        {"P01", "P07", 3.5, 12, EstadoConexion::habilitada},
        {"P07", "P03", 5.5, 20, EstadoConexion::habilitada}};
    Unidad unidades[2] = {
        {"U01", "cisterna", 10000, 3.5, EstadoUnidad::disponible},
        {"U02", "cisterna", 7000, 3.0, EstadoUnidad::ocupada}};
    Pedido pedidos[2] = {
        {"D001", "P01", "P03", 6000, Prioridad::alta, EstadoPedido::pendiente, ""},
        {"D003", "P01", "P07", 7000, Prioridad::alta, EstadoPedido::asignado, "U02"}};
    DimensionCisterna alturas[10];
    RestriccionAltura restricciones[10];
    int cantidadAlturas = 0;
    int cantidadRestricciones = 0;

    bool correcto = menuConsultasDiego(puntos, 3, conexiones, 2, unidades, 2, pedidos, 2,
                                       alturas, &cantidadAlturas, 10,
                                       restricciones, &cantidadRestricciones, 10);
    if (correcto) return 0;
    return 1;
}