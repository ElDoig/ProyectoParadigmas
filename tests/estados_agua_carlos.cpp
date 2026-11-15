#include <iostream>
#include "../cpp/tipos.h"
#include "../cpp/carlos.h"

void comprobarIntegracion(bool condicion, std::string caso, int* pruebas, int* fallos) {
    *pruebas = *pruebas + 1;
    if (!condicion) {
        *fallos = *fallos + 1;
        std::cout << "FALLO: " << caso << "\n";
    }
}

int main() {
    int pruebas = 0;
    int fallos = 0;
    StockAgua stock = {"P01", 12000, 0};
    ReservaAgua reservas[4];
    int cantidad = 0;
    Pedido pedidos[2] = {
        {"D001", "P01", "P02", 6000, Prioridad::alta, EstadoPedido::pendiente, ""},
        {"D002", "P01", "P02", 9000, Prioridad::media, EstadoPedido::pendiente, ""}
    };
    Unidad unidades[2] = {
        {"U01", "cisterna", 10000, 3.5, EstadoUnidad::disponible},
        {"U02", "cisterna", 5000, 3.0, EstadoUnidad::disponible}
    };
    comprobarIntegracion(asignarPedidoConAgua("D001", "U01", pedidos, 2, unidades, 2,
                         nullptr, reservas, &cantidad, 4) != "",
                         "No asignar sin stock", &pruebas, &fallos);
    comprobarIntegracion(pedidos[0].estado == EstadoPedido::pendiente && pedidos[0].unidad == "" &&
                         unidades[0].estado == EstadoUnidad::disponible && cantidad == 0,
                         "Sin stock no cambian pedido ni cisterna", &pruebas, &fallos);
    comprobarIntegracion(asignarPedidoConAgua("D001", "U02", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 4) != "" && stock.litros_reservados == 0 &&
                         cantidad == 0 && unidades[1].estado == EstadoUnidad::disponible,
                         "Capacidad insuficiente no reserva agua", &pruebas, &fallos);
    comprobarIntegracion(asignarPedidoConAgua("D001", "U01", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 0) != "" && cantidad == 0 &&
                         stock.litros_reservados == 0 && pedidos[0].estado == EstadoPedido::pendiente,
                         "Sin espacio para reservas no se asigna", &pruebas, &fallos);

    StockAgua otraPlanta = {"P03", 12000, 0};
    comprobarIntegracion(asignarPedidoConAgua("D001", "U01", pedidos, 2, unidades, 2,
                         &otraPlanta, reservas, &cantidad, 4) != "" && cantidad == 0 &&
                         otraPlanta.litros_reservados == 0,
                         "No reservar en otra planta", &pruebas, &fallos);
    comprobarIntegracion(asignarPedidoConAgua("D001", "U01", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 4) == "",
                         "Asignar y reservar 6000 litros", &pruebas, &fallos);
    comprobarIntegracion(pedidos[0].estado == EstadoPedido::asignado && pedidos[0].unidad == "U01" &&
                         unidades[0].estado == EstadoUnidad::ocupada,
                         "Asignacion actualiza pedido y cisterna", &pruebas, &fallos);
    comprobarIntegracion(stock.litros_stock == 12000 && stock.litros_reservados == 6000 &&
                         cantidad == 1 && reservas[0].codigo_pedido == "D001" && !reservas[0].entregada,
                         "La asignacion reserva sin descontar stock", &pruebas, &fallos);
    comprobarIntegracion(asignarPedidoConAgua("D001", "U01", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 4) != "" && cantidad == 1 &&
                         stock.litros_reservados == 6000,
                         "No duplicar una asignacion ni su reserva", &pruebas, &fallos);
    comprobarIntegracion(asignarPedidoConAgua("D002", "U01", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 4) != "" &&
                         pedidos[1].estado == EstadoPedido::pendiente && cantidad == 1,
                         "No asignar otra vez una cisterna ocupada", &pruebas, &fallos);

    // Probamos agua insuficiente con otra cisterna que si tiene capacidad.
    unidades[1].capacidad_litros = 10000;
    comprobarIntegracion(asignarPedidoConAgua("D002", "U02", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 4) != "" &&
                         pedidos[1].estado == EstadoPedido::pendiente && pedidos[1].unidad == "" &&
                         unidades[1].estado == EstadoUnidad::disponible && cantidad == 1 &&
                         stock.litros_stock == 12000 && stock.litros_reservados == 6000,
                         "Agua insuficiente conserva todos los datos", &pruebas, &fallos);
    comprobarIntegracion(entregarPedidoConAgua("D002", pedidos, 2, unidades, 2,
                         &stock, reservas, cantidad) != "" && stock.litros_stock == 12000,
                         "No entregar un pendiente", &pruebas, &fallos);
    comprobarIntegracion(entregarPedidoConAgua("D001", pedidos, 2, unidades, 2,
                         &stock, reservas, 0) != "" && pedidos[0].estado == EstadoPedido::asignado &&
                         unidades[0].estado == EstadoUnidad::ocupada && stock.litros_stock == 12000,
                         "Una reserva incoherente no libera la cisterna", &pruebas, &fallos);
    pedidos[0].litros = 7000;
    comprobarIntegracion(entregarPedidoConAgua("D001", pedidos, 2, unidades, 2,
                         &stock, reservas, cantidad) != "" && !reservas[0].entregada &&
                         stock.litros_stock == 12000 && stock.litros_reservados == 6000,
                         "No entregar litros distintos a la reserva", &pruebas, &fallos);
    pedidos[0].litros = 6000;
    unidades[0].estado = EstadoUnidad::disponible;
    comprobarIntegracion(entregarPedidoConAgua("D001", pedidos, 2, unidades, 2,
                         &stock, reservas, cantidad) != "" && !reservas[0].entregada,
                         "No entregar con una cisterna que no esta ocupada", &pruebas, &fallos);
    unidades[0].estado = EstadoUnidad::ocupada;

    comprobarIntegracion(entregarPedidoConAgua("D001", pedidos, 2, unidades, 2,
                         &stock, reservas, cantidad) == "",
                         "Entregar el pedido reservado", &pruebas, &fallos);
    comprobarIntegracion(stock.litros_stock == 6000 && stock.litros_reservados == 0 &&
                         cantidad == 1 && reservas[0].entregada,
                         "Descontar exactamente 6000 y liberar la reserva", &pruebas, &fallos);
    comprobarIntegracion(pedidos[0].estado == EstadoPedido::entregado && pedidos[0].unidad == "U01" &&
                         unidades[0].estado == EstadoUnidad::disponible,
                         "Entrega conserva historial y libera cisterna", &pruebas, &fallos);
    comprobarIntegracion(entregarPedidoConAgua("D001", pedidos, 2, unidades, 2,
                         &stock, reservas, cantidad) != "" && stock.litros_stock == 6000 &&
                         stock.litros_reservados == 0 && unidades[0].estado == EstadoUnidad::disponible,
                         "Entrega duplicada no descuenta otra vez", &pruebas, &fallos);
    comprobarIntegracion(reponerAgua(&stock, 5000, 20000) == "" && stock.litros_stock == 11000,
                         "Reponer despues de entregar", &pruebas, &fallos);
    comprobarIntegracion(asignarPedidoConAgua("D002", "U01", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 4) == "" && cantidad == 2 &&
                         stock.litros_reservados == 9000,
                         "Reutilizar la cisterna liberada para otro pedido", &pruebas, &fallos);
    comprobarIntegracion(entregarPedidoConAgua("D001", pedidos, 2, unidades, 2,
                         &stock, reservas, cantidad) != "" && unidades[0].estado == EstadoUnidad::ocupada &&
                         stock.litros_stock == 11000 && stock.litros_reservados == 9000,
                         "Repetir entrega antigua no libera la nueva asignacion", &pruebas, &fallos);
    comprobarIntegracion(entregarPedidoConAgua("D002", pedidos, 2, unidades, 2,
                         &stock, reservas, cantidad) == "" && stock.litros_stock == 2000 &&
                         stock.litros_reservados == 0 && reservas[1].entregada,
                         "Entregar el segundo pedido con su propia reserva", &pruebas, &fallos);
    int libres = -1;
    comprobarIntegracion(consultarAguaLibre(stock, &libres) == "" && libres == 2000,
                         "Stock final coherente", &pruebas, &fallos);
    comprobarIntegracion(asignarPedidoConAgua("NO_EXISTE", "U01", pedidos, 2, unidades, 2,
                         &stock, reservas, &cantidad, 4) != "" && cantidad == 2,
                         "Rechazar pedido inexistente", &pruebas, &fallos);
    comprobarIntegracion(entregarPedidoConAgua("D001", nullptr, 2, unidades, 2,
                         &stock, reservas, cantidad) != "" && stock.litros_stock == 2000,
                         "Rechazar arreglos incorrectos sin acceder a ellos", &pruebas, &fallos);

    std::cout << pruebas << " pruebas de estados y agua, " << fallos << " fallos.\n";
    if (fallos > 0) return 1;
    return 0;
}
