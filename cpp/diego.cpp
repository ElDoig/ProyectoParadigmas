#include <iostream>
#include "tipos.h"
#include "diego.h"

std::string textoTipoPunto(TipoPunto tipo) {
    if (tipo == TipoPunto::planta) return "planta";
    if (tipo == TipoPunto::colegio) return "colegio";
    if (tipo == TipoPunto::asentamiento) return "asentamiento";
    return "mercado";
}

std::string textoEstadoConexion(EstadoConexion estado) {
    if (estado == EstadoConexion::habilitada) return "habilitada";
    return "cerrada";
}

std::string textoEstadoUnidad(EstadoUnidad estado) {
    if (estado == EstadoUnidad::disponible) return "disponible";
    return "ocupada";
}

std::string textoPrioridad(Prioridad prioridad) {
    if (prioridad == Prioridad::alta) return "alta";
    if (prioridad == Prioridad::media) return "media";
    return "baja";
}

std::string textoEstadoPedido(EstadoPedido estado) {
    if (estado == EstadoPedido::pendiente) return "pendiente";
    if (estado == EstadoPedido::asignado) return "asignado";
    return "entregado";
}

// 4.1.6 - muestra los datos de un punto buscado por codigo.
std::string consultarPunto(std::string codigo, Punto puntos[], int cantidadPuntos) {
    for (int i = 0; i < cantidadPuntos; i++) {
        if (puntos[i].codigo == codigo) {
            std::cout << puntos[i].codigo << " | " << puntos[i].nombre << " | "
                      << textoTipoPunto(puntos[i].tipo) << " | " << puntos[i].zona << "\n";
            return "";
        }
    }
    return "No existe un punto con ese codigo.";
}

// Muestra las vias que llegan o salen del punto (son de doble sentido).
std::string consultarConexionesDePunto(std::string codigo, Conexion conexiones[],
                                       int cantidadConexiones) {
    int encontradas = 0;
    for (int i = 0; i < cantidadConexiones; i++) {
        std::string otro = "";
        if (conexiones[i].origen == codigo) otro = conexiones[i].destino;
        else if (conexiones[i].destino == codigo) otro = conexiones[i].origen;
        if (otro != "") {
            std::cout << codigo << " - " << otro << " | " << conexiones[i].distancia_km << " km | "
                      << conexiones[i].tiempo_min << " min | "
                      << textoEstadoConexion(conexiones[i].estado) << "\n";
            encontradas++;
        }
    }
    if (encontradas == 0) return "Ese punto no tiene conexiones registradas.";
    return "";
}

// Muestra los datos de un pedido buscado por codigo.
std::string consultarPedido(std::string codigo, Pedido pedidos[], int cantidadPedidos) {
    for (int i = 0; i < cantidadPedidos; i++) {
        if (pedidos[i].codigo == codigo) {
            std::string unidad = pedidos[i].unidad;
            if (unidad == "") unidad = "sin unidad";
            std::cout << pedidos[i].codigo << " | " << pedidos[i].origen << " -> "
                      << pedidos[i].destino << " | " << pedidos[i].litros << " litros | "
                      << textoPrioridad(pedidos[i].prioridad) << " | "
                      << textoEstadoPedido(pedidos[i].estado) << " | " << unidad << "\n";
            return "";
        }
    }
    return "No existe un pedido con ese codigo.";
}

// Muestra todos los pedidos que tienen el estado indicado.
std::string consultarPedidosPorEstado(EstadoPedido estado, Pedido pedidos[], int cantidadPedidos) {
    int encontrados = 0;
    for (int i = 0; i < cantidadPedidos; i++) {
        if (pedidos[i].estado == estado) {
            consultarPedido(pedidos[i].codigo, pedidos, cantidadPedidos);
            encontrados++;
        }
    }
    if (encontrados == 0) return "No hay pedidos con ese estado.";
    return "";
}

// 4.1.7- cuenta puntos, conexiones, unidades y pedidos por categoria.
void mostrarReporteGeneral(Punto puntos[], int cantidadPuntos,
                           Conexion conexiones[], int cantidadConexiones,
                           Unidad unidades[], int cantidadUnidades,
                           Pedido pedidos[], int cantidadPedidos) {
    int plantas = 0, colegios = 0, asentamientos = 0, mercados = 0;
    for (int i = 0; i < cantidadPuntos; i++) {
        if (puntos[i].tipo == TipoPunto::planta) plantas++;
        else if (puntos[i].tipo == TipoPunto::colegio) colegios++;
        else if (puntos[i].tipo == TipoPunto::asentamiento) asentamientos++;
        else mercados++;
    }

    int habilitadas = 0;
    double kmHabilitados = 0;
    for (int i = 0; i < cantidadConexiones; i++) {
        if (conexiones[i].estado == EstadoConexion::habilitada) {
            habilitadas++;
            kmHabilitados += conexiones[i].distancia_km;
        }
    }

    int disponibles = 0, capacidadDisponible = 0;
    for (int i = 0; i < cantidadUnidades; i++) {
        if (unidades[i].estado == EstadoUnidad::disponible) {
            disponibles++;
            capacidadDisponible += unidades[i].capacidad_litros;
        }
    }

    int pendientes = 0, asignados = 0, entregados = 0, litrosPendientes = 0;
    for (int i = 0; i < cantidadPedidos; i++) {
        if (pedidos[i].estado == EstadoPedido::pendiente) {
            pendientes++;
            litrosPendientes += pedidos[i].litros;
        } else if (pedidos[i].estado == EstadoPedido::asignado) asignados++;
        else entregados++;
    }

    std::cout << "REPORTE GENERAL\n"
              << "Puntos: " << cantidadPuntos << " (plantas " << plantas << ", colegios " << colegios
              << ", asentamientos " << asentamientos << ", mercados " << mercados << ")\n"
              << "Conexiones: " << cantidadConexiones << " (habilitadas " << habilitadas
              << ", cerradas " << cantidadConexiones - habilitadas << ", " << kmHabilitados
              << " km habilitados)\n"
              << "Cisternas: " << cantidadUnidades << " (disponibles " << disponibles
              << ", ocupadas " << cantidadUnidades - disponibles << ", " << capacidadDisponible
              << " litros disponibles)\n"
              << "Pedidos: " << cantidadPedidos << " (pendientes " << pendientes << ", asignados "
              << asignados << ", entregados " << entregados << ", " << litrosPendientes
              << " litros pendientes)\n";
}