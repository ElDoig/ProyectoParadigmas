// Prueba de las consultas y el reporte de Diego con los datos de data/examples.
#include <iostream>
#include "../cpp/tipos.h"
#include "../cpp/diego.h"

int main() {
    Punto puntos[10] = {
        {"P01", "Planta_Chillon", TipoPunto::planta, "carabayllo"},
        {"P02", "IE_Comas_Norte", TipoPunto::colegio, "comas"},
        {"P03", "AAHH_Collique", TipoPunto::asentamiento, "comas"},
        {"P04", "Mercado_Collique", TipoPunto::mercado, "comas"},
        {"P05", "AAHH_La_Libertad", TipoPunto::asentamiento, "comas"},
        {"P06", "AAHH_El_Progreso", TipoPunto::asentamiento, "carabayllo"},
        {"P07", "Mercado_Tungasuca", TipoPunto::mercado, "carabayllo"},
        {"P08", "IE_Carabayllo", TipoPunto::colegio, "carabayllo"},
        {"P09", "AAHH_Raul_Porras", TipoPunto::asentamiento, "carabayllo"},
        {"P10", "AAHH_Lomas_de_Carabayllo", TipoPunto::asentamiento, "carabayllo"}};
    Conexion conexiones[14] = {
        {"P01", "P06", 4.0, 15, EstadoConexion::habilitada}, {"P01", "P07", 3.5, 12, EstadoConexion::habilitada},
        {"P01", "P08", 5.0, 18, EstadoConexion::habilitada}, {"P06", "P07", 2.0, 8, EstadoConexion::habilitada},
        {"P07", "P09", 3.0, 11, EstadoConexion::habilitada}, {"P08", "P09", 2.5, 10, EstadoConexion::habilitada},
        {"P06", "P03", 6.0, 22, EstadoConexion::habilitada}, {"P07", "P03", 5.5, 20, EstadoConexion::habilitada},
        {"P03", "P04", 1.5, 6, EstadoConexion::habilitada},  {"P03", "P05", 2.5, 9, EstadoConexion::habilitada},
        {"P04", "P02", 2.0, 8, EstadoConexion::habilitada},  {"P05", "P02", 3.0, 11, EstadoConexion::habilitada},
        {"P04", "P05", 1.8, 7, EstadoConexion::cerrada},     {"P09", "P10", 4.5, 17, EstadoConexion::cerrada}};
    Unidad unidades[4] = {
        {"U01", "cisterna", 10000, 3.5, EstadoUnidad::disponible},
        {"U02", "cisterna", 7000, 3.0, EstadoUnidad::ocupada},
        {"U03", "cisterna", 15000, 4.5, EstadoUnidad::disponible},
        {"U04", "cisterna", 5000, 2.5, EstadoUnidad::ocupada}};
    Pedido pedidos[9] = {
        {"D001", "P01", "P03", 6000, Prioridad::alta, EstadoPedido::pendiente, ""},
        {"D002", "P01", "P05", 4000, Prioridad::media, EstadoPedido::pendiente, ""},
        {"D003", "P01", "P07", 7000, Prioridad::alta, EstadoPedido::asignado, "U02"},
        {"D004", "P01", "P08", 18000, Prioridad::alta, EstadoPedido::pendiente, ""},
        {"D005", "P01", "P09", 5000, Prioridad::baja, EstadoPedido::asignado, "U04"},
        {"D006", "P01", "P10", 3000, Prioridad::alta, EstadoPedido::pendiente, ""},
        {"D007", "P01", "P02", 8000, Prioridad::media, EstadoPedido::entregado, "U01"},
        {"D008", "P01", "P06", 9000, Prioridad::baja, EstadoPedido::entregado, "U03"},
        {"D009", "P01", "P04", 2500, Prioridad::baja, EstadoPedido::entregado, "U01"}};

    std::cout << "-- Punto P03\n" << consultarPunto("P03", puntos, 10);
    std::cout << "-- Punto P99: " << consultarPunto("P99", puntos, 10) << "\n";
    std::cout << "-- Conexiones de P03\n" << consultarConexionesDePunto("P03", conexiones, 14);
    std::cout << "-- Pedido D004\n" << consultarPedido("D004", pedidos, 9);
    std::cout << "-- Pedidos asignados\n" << consultarPedidosPorEstado(EstadoPedido::asignado, pedidos, 9);
    std::cout << "--\n";
    mostrarReporteGeneral(puntos, 10, conexiones, 14, unidades, 4, pedidos, 9);
    return 0;
}