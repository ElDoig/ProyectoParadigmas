#include <iostream>
#include "../cpp/tipos.h"
#include "../cpp/carlos.h"

void comprobarStock(bool condicion, std::string caso, int* pruebas, int* fallos) {
    *pruebas = *pruebas + 1;
    if (!condicion) {
        *fallos = *fallos + 1;
        std::cout << "FALLO: " << caso << "\n";
    }
}

bool mismoStock(StockAgua a, StockAgua b) {
    return a.punto_planta == b.punto_planta && a.litros_stock == b.litros_stock &&
           a.litros_reservados == b.litros_reservados;
}

bool mismaReserva(ReservaAgua a, ReservaAgua b) {
    return a.codigo_pedido == b.codigo_pedido && a.punto_planta == b.punto_planta &&
           a.litros == b.litros && a.entregada == b.entregada;
}

int main() {
    int pruebas = 0;
    int fallos = 0;
    int libres = -1;
    StockAgua stock = {"P01", 12000, 4000};
    ReservaAgua reservas[6];
    int cantidad = 0;
    // Los 4000 litros iniciales pertenecen a este pedido de prueba ya asignado.
    Pedido anteriores[1] = {
        {"D000", "P01", "P02", 4000, Prioridad::alta, EstadoPedido::asignado, "U02"}
    };
    comprobarStock(consultarAguaLibre(stock, &libres) == "" && libres == 8000,
                   "12000 menos 4000 son 8000 libres", &pruebas, &fallos);
    comprobarStock(prepararReservasIniciales(stock, anteriores, 1, reservas, &cantidad, 6) == "",
                   "Preparar reservas desde un estado inicial coherente", &pruebas, &fallos);
    comprobarStock(cantidad == 1 && reservas[0].litros == 4000 && !reservas[0].entregada,
                   "Conservar la reserva inicial de su pedido", &pruebas, &fallos);

    Pedido nuevo = {"D001", "P01", "P03", 9000, Prioridad::alta, EstadoPedido::pendiente, ""};
    StockAgua antes = stock;
    ReservaAgua reservaAnterior = reservas[0];
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) ==
                   "No hay suficientes litros libres para ese pedido.",
                   "Rechazar reserva de 9000 con solo 8000 libres", &pruebas, &fallos);
    comprobarStock(mismoStock(stock, antes) && cantidad == 1 && mismaReserva(reservas[0], reservaAnterior),
                   "Un rechazo no modifica stock, contador ni reservas", &pruebas, &fallos);
    nuevo.litros = 6000;
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) == "",
                   "Reservar 6000 litros", &pruebas, &fallos);
    comprobarStock(stock.litros_stock == 12000 && stock.litros_reservados == 10000 && cantidad == 2,
                   "Reservar compromete agua sin descontarla del stock", &pruebas, &fallos);
    comprobarStock(nuevo.estado == EstadoPedido::pendiente && nuevo.unidad == "",
                   "Reservar no asigna el pedido ni una cisterna", &pruebas, &fallos);
    antes = stock;
    ReservaAgua reservaNueva = reservas[1];
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) != "" &&
                   mismoStock(stock, antes) && cantidad == 2 && mismaReserva(reservas[1], reservaNueva),
                   "Rechazar reserva duplicada sin modificar datos", &pruebas, &fallos);
    comprobarStock(registrarEntregaAgua(nuevo, &stock, reservas, cantidad) != "" && mismoStock(stock, antes),
                   "No entregar un pedido que todavia no fue asignado", &pruebas, &fallos);

    // Simulamos aqui los cambios que hara Valentino, no dentro de nuestras funciones.
    nuevo.estado = EstadoPedido::asignado;
    nuevo.unidad = "U01";
    Pedido alterado = nuevo;
    alterado.litros = 5000;
    comprobarStock(registrarEntregaAgua(alterado, &stock, reservas, cantidad) != "" &&
                   mismoStock(stock, antes) && mismaReserva(reservas[1], reservaNueva),
                   "Rechazar entrega si cambiaron los litros reservados", &pruebas, &fallos);
    alterado = nuevo;
    alterado.origen = "P99";
    comprobarStock(registrarEntregaAgua(alterado, &stock, reservas, cantidad) != "" && mismoStock(stock, antes),
                   "Rechazar entrega desde otra planta", &pruebas, &fallos);
    alterado = nuevo;
    alterado.codigo = "INEXISTENTE";
    comprobarStock(registrarEntregaAgua(alterado, &stock, reservas, cantidad) != "" && mismoStock(stock, antes),
                   "Rechazar entrega sin reserva", &pruebas, &fallos);
    comprobarStock(registrarEntregaAgua(nuevo, &stock, reservas, cantidad) == "",
                   "Entregar los 6000 litros reservados", &pruebas, &fallos);
    comprobarStock(stock.litros_stock == 6000 && stock.litros_reservados == 4000 && reservas[1].entregada,
                   "Descontar exactamente 6000 y liberar solo su reserva", &pruebas, &fallos);
    comprobarStock(mismaReserva(reservas[0], reservaAnterior) && cantidad == 2,
                   "Conservar la otra reserva y el historial", &pruebas, &fallos);
    comprobarStock(nuevo.estado == EstadoPedido::asignado && nuevo.unidad == "U01",
                   "La entrega de agua no cambia estados ni libera la cisterna", &pruebas, &fallos);
    antes = stock;
    reservaNueva = reservas[1];
    comprobarStock(registrarEntregaAgua(nuevo, &stock, reservas, cantidad) != "" &&
                   mismoStock(stock, antes) && mismaReserva(reservas[1], reservaNueva),
                   "Rechazar entrega duplicada sin volver a descontar", &pruebas, &fallos);
    nuevo.estado = EstadoPedido::pendiente;
    nuevo.unidad = "";
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) != "" && mismoStock(stock, antes),
                   "No volver a reservar un pedido del historial", &pruebas, &fallos);
    comprobarStock(consultarAguaLibre(stock, &libres) == "" && libres == 2000,
                   "El agua libre se conserva al entregar una reserva", &pruebas, &fallos);

    // 20000 es un tope solo para esta prueba; no se agrega al contrato CSV.
    comprobarStock(reponerAgua(&stock, 5000, 20000) == "" && stock.litros_stock == 11000 &&
                   stock.litros_reservados == 4000,
                   "Reponer 5000 sin cambiar reservas", &pruebas, &fallos);
    comprobarStock(consultarAguaLibre(stock, &libres) == "" && libres == 7000,
                   "La reposicion aumenta el agua libre", &pruebas, &fallos);
    antes = stock;
    comprobarStock(reponerAgua(&stock, 0, 20000) != "" && mismoStock(stock, antes),
                   "Rechazar reposicion cero", &pruebas, &fallos);
    comprobarStock(reponerAgua(&stock, -5000, 20000) != "" && mismoStock(stock, antes),
                   "Rechazar reposicion negativa", &pruebas, &fallos);
    comprobarStock(reponerAgua(&stock, 1000, 10000) != "" && mismoStock(stock, antes),
                   "Rechazar tope menor que el stock", &pruebas, &fallos);
    comprobarStock(reponerAgua(&stock, 10000, 20000) != "" && mismoStock(stock, antes),
                   "Rechazar reposicion mayor que el espacio indicado", &pruebas, &fallos);
    comprobarStock(reponerAgua(&stock, 9000, 20000) == "" && stock.litros_stock == 20000,
                   "Aceptar reposicion que llega exactamente al tope", &pruebas, &fallos);
    comprobarStock(reponerAgua(&stock, 1, 20000) != "" && stock.litros_stock == 20000,
                   "Rechazar otro litro cuando ya se alcanzo el tope", &pruebas, &fallos);
    StockAgua grande = {"P01", 2000000000, 0};
    antes = grande;
    comprobarStock(reponerAgua(&grande, 1000000000, 2100000000) != "" && mismoStock(grande, antes),
                   "Rechazar antes de una suma que desbordaria un int de 32 bits", &pruebas, &fallos);

    antes = stock;
    nuevo.codigo = "D002";
    nuevo.litros = 0;
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) != "" && mismoStock(stock, antes),
                   "Rechazar reserva cero", &pruebas, &fallos);
    nuevo.litros = -100;
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) != "" && mismoStock(stock, antes),
                   "Rechazar reserva negativa", &pruebas, &fallos);
    nuevo.litros = 100;
    nuevo.origen = "P99";
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) != "" && mismoStock(stock, antes),
                   "Rechazar reserva en otra planta", &pruebas, &fallos);
    nuevo.origen = "P01";
    nuevo.codigo = "";
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) != "" && mismoStock(stock, antes),
                   "Rechazar pedido sin codigo", &pruebas, &fallos);
    nuevo.codigo = "D002";
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 2) != "" && cantidad == 2 && mismoStock(stock, antes),
                   "Rechazar reserva sin espacio en el arreglo", &pruebas, &fallos);
    nuevo.estado = EstadoPedido::asignado;
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidad, 6) != "" && mismoStock(stock, antes),
                   "No reservar un pedido que ya fue asignado", &pruebas, &fallos);

    // Copia en memoria de la inconsistencia actual de los CSV; no se editan archivos.
    Pedido asignados[2] = {
        {"D003", "P01", "P07", 7000, Prioridad::alta, EstadoPedido::asignado, "U02"},
        {"D005", "P01", "P09", 5000, Prioridad::baja, EstadoPedido::asignado, "U04"}
    };
    StockAgua inconsistente = {"P01", 12000, 4000};
    StockAgua copiaInconsistente = inconsistente;
    ReservaAgua iniciales[2] = {{"SIN_CAMBIAR", "P01", 1, true}, {"SIN_CAMBIAR_2", "P01", 1, true}};
    ReservaAgua centinela = iniciales[0];
    int cantidadIniciales = 0;
    comprobarStock(prepararReservasIniciales(inconsistente, asignados, 2, iniciales, &cantidadIniciales, 2) != "",
                   "Detectar 4000 reservados frente a 12000 asignados", &pruebas, &fallos);
    comprobarStock(mismoStock(inconsistente, copiaInconsistente) && cantidadIniciales == 0 &&
                   mismaReserva(iniciales[0], centinela),
                   "No reparar cifras ni cargar parcialmente las reservas", &pruebas, &fallos);

    // Otro estado SOLO de prueba, donde stock y pedidos si coinciden.
    StockAgua coherente = {"P01", 12000, 12000};
    comprobarStock(prepararReservasIniciales(coherente, asignados, 2, iniciales, &cantidadIniciales, 1) != "" &&
                   cantidadIniciales == 0 && mismaReserva(iniciales[0], centinela),
                   "Preparacion rechazada por capacidad no escribe a medias", &pruebas, &fallos);
    comprobarStock(prepararReservasIniciales(coherente, asignados, 2, iniciales, &cantidadIniciales, 2) == "" &&
                   cantidadIniciales == 2,
                   "Preparar dos reservas coherentes", &pruebas, &fallos);
    comprobarStock(consultarAguaLibre(coherente, &libres) == "" && libres == 0,
                   "Todo el stock comprometido deja cero libre", &pruebas, &fallos);
    comprobarStock(prepararReservasIniciales(coherente, asignados, 2, iniciales, &cantidadIniciales, 2) != "" &&
                   cantidadIniciales == 2,
                   "No reinicializar un historial en uso", &pruebas, &fallos);
    comprobarStock(registrarEntregaAgua(asignados[1], &coherente, iniciales, cantidadIniciales) == "" &&
                   coherente.litros_stock == 7000 && coherente.litros_reservados == 7000,
                   "Entregar reservas en otro orden", &pruebas, &fallos);
    comprobarStock(registrarEntregaAgua(asignados[0], &coherente, iniciales, cantidadIniciales) == "" &&
                   coherente.litros_stock == 0 && coherente.litros_reservados == 0,
                   "Agotar stock sin hacerlo negativo", &pruebas, &fallos);
    comprobarStock(registrarEntregaAgua(asignados[0], &coherente, iniciales, cantidadIniciales) != "" &&
                   coherente.litros_stock == 0 && coherente.litros_reservados == 0,
                   "No entregar otra vez aunque el stock este agotado", &pruebas, &fallos);

    StockAgua invalido = {"P01", -1, 0};
    libres = 123;
    comprobarStock(consultarAguaLibre(invalido, &libres) != "" && libres == 123,
                   "Stock negativo no produce un resultado de consulta", &pruebas, &fallos);
    invalido.litros_stock = 10;
    invalido.litros_reservados = -1;
    comprobarStock(consultarAguaLibre(invalido, &libres) != "" && libres == 123,
                   "Rechazar litros reservados negativos", &pruebas, &fallos);
    invalido.litros_reservados = 11;
    antes = invalido;
    comprobarStock(reponerAgua(&invalido, 1, 20) != "" && mismoStock(invalido, antes),
                   "No operar si las reservas superan el stock", &pruebas, &fallos);
    invalido = {"", 10, 0};
    comprobarStock(consultarAguaLibre(invalido, &libres) != "",
                   "Rechazar stock sin planta", &pruebas, &fallos);
    comprobarStock(consultarAguaLibre(stock, nullptr) != "",
                   "Rechazar salida de consulta nula", &pruebas, &fallos);
    comprobarStock(reponerAgua(nullptr, 10, 20) != "",
                   "Rechazar reposicion sin stock", &pruebas, &fallos);
    comprobarStock(registrarEntregaAgua(nuevo, nullptr, reservas, cantidad) != "",
                   "Rechazar entrega sin stock", &pruebas, &fallos);
    nuevo.estado = EstadoPedido::pendiente;
    comprobarStock(reservarAgua(nuevo, &stock, reservas, nullptr, 6) != "",
                   "Rechazar reserva sin contador", &pruebas, &fallos);
    int cantidadMala = -1;
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidadMala, 6) != "" && cantidadMala == -1,
                   "Rechazar contador negativo", &pruebas, &fallos);
    cantidadMala = 7;
    comprobarStock(reservarAgua(nuevo, &stock, reservas, &cantidadMala, 6) != "" && cantidadMala == 7,
                   "Rechazar contador mayor que la capacidad", &pruebas, &fallos);
    ReservaAgua malas[2] = {{"D010", "P01", 4000, false}, {"D010", "P01", 1000, true}};
    StockAgua saldo = {"P01", 10000, 4000};
    antes = saldo;
    cantidadMala = 2;
    comprobarStock(reservarAgua(nuevo, &saldo, malas, &cantidadMala, 2) != "" && mismoStock(saldo, antes),
                   "Rechazar historial duplicado", &pruebas, &fallos);
    malas[1].codigo_pedido = "D011";
    malas[0].litros = 3000;
    comprobarStock(reservarAgua(nuevo, &saldo, malas, &cantidadMala, 2) != "" && mismoStock(saldo, antes),
                   "Rechazar suma de reservas menor que el total", &pruebas, &fallos);
    malas[0].litros = 5000;
    comprobarStock(reservarAgua(nuevo, &saldo, malas, &cantidadMala, 2) != "" && mismoStock(saldo, antes),
                   "Rechazar suma de reservas mayor que el total", &pruebas, &fallos);
    malas[0].litros = 4000;
    malas[0].punto_planta = "P99";
    comprobarStock(reservarAgua(nuevo, &saldo, malas, &cantidadMala, 2) != "" && mismoStock(saldo, antes),
                   "No mezclar historiales de distintas plantas", &pruebas, &fallos);
    malas[0].punto_planta = "P01";
    malas[0].litros = 0;
    comprobarStock(reservarAgua(nuevo, &saldo, malas, &cantidadMala, 2) != "" && mismoStock(saldo, antes),
                   "Rechazar reserva inicial sin litros", &pruebas, &fallos);
    cantidadMala = 0;
    comprobarStock(reservarAgua(nuevo, &saldo, malas, &cantidadMala, 2) != "" && mismoStock(saldo, antes),
                   "No usar stock reservado sin detalle de pedidos", &pruebas, &fallos);

    // Repetimos el ciclo con cantidades distintas y comprobamos las cuentas.
    for (int i = 1; i <= 20; i++) {
        StockAgua prueba = {"P01", 1000, 0};
        ReservaAgua detalle[1];
        int total = 0;
        Pedido pedido = {"CICLO", "P01", "P02", i * 10, Prioridad::alta, EstadoPedido::pendiente, ""};
        bool correcto = reservarAgua(pedido, &prueba, detalle, &total, 1) == "";
        correcto = correcto && prueba.litros_reservados == i * 10 && prueba.litros_stock == 1000;
        pedido.estado = EstadoPedido::asignado;
        pedido.unidad = "U01";
        correcto = correcto && registrarEntregaAgua(pedido, &prueba, detalle, total) == "";
        correcto = correcto && prueba.litros_stock == 1000 - i * 10 && prueba.litros_reservados == 0;
        correcto = correcto && prueba.litros_stock >= prueba.litros_reservados && prueba.litros_stock >= 0;
        comprobarStock(correcto, "Ciclo de reserva y entrega conserva los invariantes", &pruebas, &fallos);
    }

    std::cout << pruebas << " pruebas de stock, " << fallos << " fallos.\n";
    if (fallos != 0) return 1;
    return 0;
}
