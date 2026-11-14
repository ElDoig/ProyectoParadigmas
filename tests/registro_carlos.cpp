#include <iostream>
#include "../cpp/tipos.h"
#include "../cpp/carlos.h"

// Casos de prueba de la 4.1.3, sin archivos CSV ni entrada por consola.
void comprobar(bool condicion, std::string caso, int* pruebas, int* fallos) {
    *pruebas = *pruebas + 1;
    if (!condicion) {
        *fallos = *fallos + 1;
        std::cout << "FALLO: " << caso << "\n";
    }
}

int main() {
    int pruebas = 0;
    int fallos = 0;
    Punto puntos[4];
    int cantidadPuntos = 0;
    Punto planta = {"P01", "Planta Chillon", TipoPunto::planta, "carabayllo"};
    Punto colegio = {"P02", "Colegio Comas", TipoPunto::colegio, "comas"};
    Punto asentamiento = {"P03", "AAHH Collique", TipoPunto::asentamiento, "comas"};
    Punto mercado = {"P04", "Mercado", TipoPunto::mercado, "comas"};

    comprobar(registrarPunto(planta, puntos, &cantidadPuntos, 4) == "",
              "Registrar planta", &pruebas, &fallos);
    comprobar(registrarPunto(planta, puntos, &cantidadPuntos, 4) != "" && cantidadPuntos == 1,
              "Rechazar codigo de punto repetido", &pruebas, &fallos);
    Punto incorrecto = colegio;
    incorrecto.codigo = "";
    comprobar(registrarPunto(incorrecto, puntos, &cantidadPuntos, 4) != "",
              "Rechazar codigo vacio", &pruebas, &fallos);
    incorrecto = colegio;
    incorrecto.nombre = "";
    comprobar(registrarPunto(incorrecto, puntos, &cantidadPuntos, 4) != "",
              "Rechazar nombre vacio", &pruebas, &fallos);
    incorrecto = colegio;
    incorrecto.zona = "";
    comprobar(registrarPunto(incorrecto, puntos, &cantidadPuntos, 4) != "" && cantidadPuntos == 1,
              "Rechazar zona vacia sin agregar registros", &pruebas, &fallos);
    comprobar(registrarPunto(colegio, puntos, &cantidadPuntos, 4) == "",
              "Registrar colegio", &pruebas, &fallos);
    comprobar(registrarPunto(asentamiento, puntos, &cantidadPuntos, 4) == "",
              "Registrar asentamiento", &pruebas, &fallos);
    comprobar(registrarPunto(mercado, puntos, &cantidadPuntos, 4) == "",
              "Registrar mercado", &pruebas, &fallos);
    comprobar(registrarPunto(colegio, puntos, &cantidadPuntos, 4) != "" && cantidadPuntos == 4,
              "No escribir fuera de un arreglo lleno", &pruebas, &fallos);
    comprobar(puntos[0].codigo == "P01" && puntos[0].nombre == "Planta Chillon",
              "Conservar los puntos anteriores", &pruebas, &fallos);
    comprobar(registrarPunto(colegio, puntos, nullptr, 4) != "",
              "Rechazar contador nulo", &pruebas, &fallos);
    int cantidadIncorrecta = 5;
    comprobar(registrarPunto(colegio, puntos, &cantidadIncorrecta, 4) != "" && cantidadIncorrecta == 5,
              "Rechazar contador mayor que el arreglo", &pruebas, &fallos);
    cantidadIncorrecta = -1;
    comprobar(registrarPunto(colegio, puntos, &cantidadIncorrecta, 4) != "",
              "Rechazar contador negativo", &pruebas, &fallos);

    Conexion conexiones[2];
    int cantidadConexiones = 0;
    Conexion via = {"P01", "P02", 5.0, 20, EstadoConexion::habilitada};
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) == "",
              "Registrar conexion habilitada", &pruebas, &fallos);
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "",
              "Rechazar tramo repetido", &pruebas, &fallos);
    via.origen = "P02";
    via.destino = "P01";
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "",
              "Rechazar el mismo tramo en sentido inverso", &pruebas, &fallos);
    via.origen = "P01";
    via.destino = "P99";
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "",
              "Rechazar punto inexistente en conexion", &pruebas, &fallos);
    via.destino = "P01";
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "",
              "Rechazar conexion de un punto consigo mismo", &pruebas, &fallos);
    via.destino = "P03";
    via.distancia_km = 0;
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "",
              "Rechazar distancia cero", &pruebas, &fallos);
    via.distancia_km = -1;
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "",
              "Rechazar distancia negativa", &pruebas, &fallos);
    via.distancia_km = 3.5;
    via.tiempo_min = 0;
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "",
              "Rechazar tiempo cero", &pruebas, &fallos);
    via.tiempo_min = -5;
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "" && cantidadConexiones == 1,
              "Rechazar tiempo negativo sin cambiar contador", &pruebas, &fallos);
    via.tiempo_min = 12;
    via.estado = EstadoConexion::cerrada;
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) == "",
              "Registrar una segunda conexion cerrada", &pruebas, &fallos);
    comprobar(registrarConexion(via, conexiones, &cantidadConexiones, 2, puntos, 4) != "" && cantidadConexiones == 2,
              "Rechazar conexion con arreglo lleno", &pruebas, &fallos);

    Unidad unidades[2];
    int cantidadUnidades = 0;
    Unidad cisterna = {"U01", "cisterna", 10000, 3.5, EstadoUnidad::disponible};
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) == "",
              "Registrar cisterna", &pruebas, &fallos);
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) != "",
              "Rechazar codigo de unidad repetido", &pruebas, &fallos);
    cisterna.codigo = "U02";
    cisterna.capacidad_litros = 0;
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) != "",
              "Rechazar capacidad cero", &pruebas, &fallos);
    cisterna.capacidad_litros = -100;
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) != "",
              "Rechazar capacidad negativa", &pruebas, &fallos);
    cisterna.capacidad_litros = 7000;
    cisterna.costo_km = 0;
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) != "",
              "Rechazar costo cero", &pruebas, &fallos);
    cisterna.costo_km = -1;
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) != "",
              "Rechazar costo negativo", &pruebas, &fallos);
    cisterna.costo_km = 3;
    cisterna.tipo = "bus";
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) != "",
              "Rechazar unidad de otro tipo", &pruebas, &fallos);
    cisterna.tipo = "cisterna";
    cisterna.estado = EstadoUnidad::ocupada;
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) != "" && cantidadUnidades == 1,
              "No asignar una cisterna durante el registro", &pruebas, &fallos);
    cisterna.estado = EstadoUnidad::disponible;
    comprobar(registrarUnidad(cisterna, unidades, &cantidadUnidades, 2) == "",
              "Registrar segunda cisterna", &pruebas, &fallos);

    Pedido pedidos[3];
    int cantidadPedidos = 0;
    Pedido solicitud = {"D001", "P01", "P02", 6000, Prioridad::alta, EstadoPedido::pendiente, ""};
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) == "",
              "Registrar pedido pendiente", &pruebas, &fallos);
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "Rechazar codigo de pedido repetido", &pruebas, &fallos);
    solicitud.codigo = "D002";
    solicitud.origen = "P99";
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "Rechazar origen inexistente", &pruebas, &fallos);
    solicitud.origen = "P02";
    solicitud.destino = "P03";
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "El origen del pedido debe ser una planta", &pruebas, &fallos);
    solicitud.origen = "P01";
    solicitud.destino = "P99";
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "Rechazar destino inexistente", &pruebas, &fallos);
    solicitud.destino = "P01";
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "Rechazar origen igual al destino", &pruebas, &fallos);
    solicitud.destino = "P03";
    solicitud.litros = 0;
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "Rechazar carga cero", &pruebas, &fallos);
    solicitud.litros = -5;
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "Rechazar carga negativa", &pruebas, &fallos);
    solicitud.litros = 18000;
    solicitud.estado = EstadoPedido::asignado;
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "",
              "No asignar pedidos durante el registro", &pruebas, &fallos);
    solicitud.estado = EstadoPedido::pendiente;
    solicitud.unidad = "U01";
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "" && cantidadPedidos == 1,
              "Rechazar unidad en un pedido pendiente", &pruebas, &fallos);
    solicitud.unidad = "";
    solicitud.prioridad = Prioridad::media;
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) == "",
              "Aceptar pedido grande para evaluarlo despues", &pruebas, &fallos);
    comprobar(pedidos[1].litros == 18000 && pedidos[1].unidad == "",
              "Conservar carga y ausencia de unidad", &pruebas, &fallos);
    solicitud.codigo = "D003";
    solicitud.prioridad = Prioridad::baja;
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) == "",
              "Registrar prioridad baja", &pruebas, &fallos);
    comprobar(registrarPedido(solicitud, pedidos, &cantidadPedidos, 3, puntos, 4) != "" && cantidadPedidos == 3,
              "Rechazar pedido con arreglo lleno", &pruebas, &fallos);

    cantidadIncorrecta = 0;
    comprobar(registrarPunto(planta, nullptr, &cantidadIncorrecta, 1) != "",
              "Rechazar arreglo nulo", &pruebas, &fallos);
    comprobar(registrarConexion(via, conexiones, &cantidadIncorrecta, 2, nullptr, 1) != "",
              "Rechazar referencias a arreglo de puntos nulo", &pruebas, &fallos);
    comprobar(registrarPedido(solicitud, pedidos, &cantidadIncorrecta, 3, nullptr, 0) != "",
              "No registrar pedidos sin puntos", &pruebas, &fallos);
    comprobar(cantidadIncorrecta == 0,
              "Los rechazos no cambian el contador", &pruebas, &fallos);

    std::cout << pruebas << " pruebas, " << fallos << " fallos.\n";
    if (fallos != 0) return 1;
    return 0;
}
