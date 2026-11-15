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

// Adicional - Diego Rodriguez: true si la via A-B coincide en cualquier sentido.
bool mismaVia(std::string origen1, std::string destino1, std::string origen2, std::string destino2) {
    return (origen1 == origen2 && destino1 == destino2) || (origen1 == destino2 && destino1 == origen2);
}

std::string registrarAltura(DimensionCisterna nueva, DimensionCisterna alturas[], int* cantidad,
                            int capacidad, Unidad unidades[], int cantidadUnidades) {
    if (alturas == nullptr || cantidad == nullptr) return "Falta el arreglo de alturas o su contador.";
    if (capacidad < 0 || *cantidad < 0 || *cantidad > capacidad) {
        return "El contador o la capacidad de alturas es incorrecto.";
    }
    if (*cantidad == capacidad) return "No hay espacio para otra altura.";
    if (!(nueva.altura_m > 0)) return "La altura debe ser mayor que cero.";
    bool existe = false;
    for (int i = 0; i < cantidadUnidades; i++) {
        if (unidades[i].codigo == nueva.unidad) existe = true;
    }
    if (!existe) return "La cisterna debe estar registrada.";
    for (int i = 0; i < *cantidad; i++) {
        if (alturas[i].unidad == nueva.unidad) return "Esa cisterna ya tiene una altura registrada.";
    }

    alturas[*cantidad] = nueva;
    *cantidad = *cantidad + 1;
    return "";
}

std::string registrarRestriccionAltura(RestriccionAltura nueva, RestriccionAltura restricciones[],
                                       int* cantidad, int capacidad,
                                       Conexion conexiones[], int cantidadConexiones) {
    if (restricciones == nullptr || cantidad == nullptr) {
        return "Falta el arreglo de restricciones o su contador.";
    }
    if (capacidad < 0 || *cantidad < 0 || *cantidad > capacidad) {
        return "El contador o la capacidad de restricciones es incorrecto.";
    }
    if (*cantidad == capacidad) return "No hay espacio para otra restriccion.";
    if (!(nueva.altura_max_m > 0)) return "La altura maxima debe ser mayor que cero.";
    bool existe = false;
    for (int i = 0; i < cantidadConexiones; i++) {
        if (mismaVia(conexiones[i].origen, conexiones[i].destino, nueva.origen, nueva.destino)) {
            existe = true;
        }
    }
    if (!existe) return "La via debe ser una conexion registrada.";
    for (int i = 0; i < *cantidad; i++) {
        if (mismaVia(restricciones[i].origen, restricciones[i].destino, nueva.origen, nueva.destino)) {
            return "Esa via ya tiene un limite de altura.";
        }
    }

    restricciones[*cantidad] = nueva;
    *cantidad = *cantidad + 1;
    return "";
}

std::string consultarPasoAltura(std::string unidad, std::string origen, std::string destino,
                                DimensionCisterna alturas[], int cantidadAlturas,
                                RestriccionAltura restricciones[], int cantidadRestricciones,
                                bool* puedePasar) {
    if (puedePasar == nullptr) return "Falta la variable del resultado.";
    int posicion = -1;
    for (int i = 0; i < cantidadAlturas; i++) {
        if (alturas[i].unidad == unidad) posicion = i;
    }
    if (posicion == -1) return "Esa cisterna no tiene altura registrada.";

    *puedePasar = true;
    for (int i = 0; i < cantidadRestricciones; i++) {
        if (mismaVia(restricciones[i].origen, restricciones[i].destino, origen, destino) &&
            alturas[posicion].altura_m > restricciones[i].altura_max_m) {
            *puedePasar = false;
        }
    }
    return "";
}