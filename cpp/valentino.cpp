#include "tipos.h"
#include "valentino.h"

// 4.1.4 - Valentino Mantilla: gestion de estados (paradigma imperativo).
// Las funciones no leen ni escriben en consola: el submenu "Gestionar estados"
// de carlos.cpp las llama y une la asignacion y la entrega con el control de agua.

// Buscamos el pedido por su codigo. Devuelve su posicion o -1.
int buscarPedidoEstados(std::string codigo, Pedido pedidos[], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        if (pedidos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// Buscamos la unidad por su codigo. Devuelve su posicion o -1.
int buscarUnidadEstados(std::string codigo, Unidad unidades[], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        if (unidades[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

// Clausula de guarda comun: un arreglo nullptr con cantidad > 0, o una
// cantidad negativa, harian que el for lea memoria fuera de limites.
bool arreglosCorrectos(Pedido pedidos[], int cantidadPedidos,
                       Unidad unidades[], int cantidadUnidades) {
    if (cantidadPedidos < 0 || (cantidadPedidos > 0 && pedidos == nullptr)) return false;
    if (cantidadUnidades < 0 || (cantidadUnidades > 0 && unidades == nullptr)) return false;
    return true;
}

// ---------------------------------------------------------------------
// Cerrar o habilitar una conexion
// ---------------------------------------------------------------------
std::string cambiarEstadoConexion(std::string origen, std::string destino,
                                  EstadoConexion nuevoEstado,
                                  Conexion conexiones[], int cantidadConexiones) {
    if (cantidadConexiones < 0 || (cantidadConexiones > 0 && conexiones == nullptr)) {
        return "El arreglo de conexiones o su cantidad es incorrecto.";
    }
    if (nuevoEstado != EstadoConexion::habilitada && nuevoEstado != EstadoConexion::cerrada) {
        return "El estado de la conexion no es valido.";
    }
    for (int i = 0; i < cantidadConexiones; i++) {
        // La via es de doble sentido: P01-P02 es la misma que P02-P01.
        if ((conexiones[i].origen == origen && conexiones[i].destino == destino) ||
            (conexiones[i].origen == destino && conexiones[i].destino == origen)) {
            if (conexiones[i].estado == nuevoEstado) {
                if (nuevoEstado == EstadoConexion::cerrada) return "La conexion ya esta cerrada.";
                return "La conexion ya esta habilitada.";
            }
            conexiones[i].estado = nuevoEstado;
            return "";
        }
    }
    return "No existe una conexion entre esos puntos.";
}

// ---------------------------------------------------------------------
// Asignar un pedido pendiente a una unidad disponible
// ---------------------------------------------------------------------
std::string validarAsignacion(std::string codigoPedido, std::string codigoUnidad,
                              Pedido pedidos[], int cantidadPedidos,
                              Unidad unidades[], int cantidadUnidades,
                              int* posPedido, int* posUnidad) {
    // Clausulas de guarda (precondiciones) antes de tocar el estado.
    if (posPedido == nullptr || posUnidad == nullptr) {
        return "Falta donde guardar las posiciones encontradas.";
    }
    if (!arreglosCorrectos(pedidos, cantidadPedidos, unidades, cantidadUnidades)) {
        return "Los arreglos de pedidos o unidades son incorrectos.";
    }
    int p = buscarPedidoEstados(codigoPedido, pedidos, cantidadPedidos);
    if (p == -1) return "No existe un pedido con ese codigo.";
    if (pedidos[p].estado != EstadoPedido::pendiente) {
        return "Solo se puede asignar un pedido pendiente.";
    }
    int u = buscarUnidadEstados(codigoUnidad, unidades, cantidadUnidades);
    if (u == -1) return "No existe una unidad con ese codigo.";
    if (unidades[u].estado != EstadoUnidad::disponible) {
        return "La unidad no esta disponible.";
    }
    // Regla base: la cisterna debe poder llevar toda la carga en un viaje.
    if (unidades[u].capacidad_litros < pedidos[p].litros) {
        return "La capacidad de la unidad es menor que los litros del pedido.";
    }
    // Paso por direccion: escribimos en las variables de quien llamo.
    *posPedido = p;
    *posUnidad = u;
    return "";
}

std::string asignarPedido(std::string codigoPedido, std::string codigoUnidad,
                          Pedido pedidos[], int cantidadPedidos,
                          Unidad unidades[], int cantidadUnidades) {
    int p = -1;
    int u = -1;
    std::string error = validarAsignacion(codigoPedido, codigoUnidad, pedidos, cantidadPedidos,
                                          unidades, cantidadUnidades, &p, &u);
    if (error != "") return error;

    // Mutacion del estado: recien cambiamos los datos despues de validar todo.
    pedidos[p].estado = EstadoPedido::asignado;
    pedidos[p].unidad = unidades[u].codigo;
    unidades[u].estado = EstadoUnidad::ocupada;
    return "";
}

// ---------------------------------------------------------------------
// Marcar un pedido como entregado y liberar su unidad
// ---------------------------------------------------------------------
std::string validarEntrega(std::string codigoPedido,
                           Pedido pedidos[], int cantidadPedidos,
                           Unidad unidades[], int cantidadUnidades,
                           int* posPedido, int* posUnidad) {
    if (posPedido == nullptr || posUnidad == nullptr) {
        return "Falta donde guardar las posiciones encontradas.";
    }
    if (!arreglosCorrectos(pedidos, cantidadPedidos, unidades, cantidadUnidades)) {
        return "Los arreglos de pedidos o unidades son incorrectos.";
    }
    int p = buscarPedidoEstados(codigoPedido, pedidos, cantidadPedidos);
    if (p == -1) return "No existe un pedido con ese codigo.";
    if (pedidos[p].estado != EstadoPedido::asignado || pedidos[p].unidad == "") {
        return "Solo se puede entregar un pedido asignado a una unidad.";
    }
    int u = buscarUnidadEstados(pedidos[p].unidad, unidades, cantidadUnidades);
    if (u == -1) return "La unidad asignada al pedido no esta registrada.";
    *posPedido = p;
    *posUnidad = u;
    return "";
}

std::string entregarPedido(std::string codigoPedido,
                           Pedido pedidos[], int cantidadPedidos,
                           Unidad unidades[], int cantidadUnidades) {
    int p = -1;
    int u = -1;
    std::string error = validarEntrega(codigoPedido, pedidos, cantidadPedidos,
                                       unidades, cantidadUnidades, &p, &u);
    if (error != "") return error;

    // El pedido conserva el codigo de su unidad como historial (igual que en el CSV).
    pedidos[p].estado = EstadoPedido::entregado;
    unidades[u].estado = EstadoUnidad::disponible;
    return "";
}

// ---------------------------------------------------------------------
// FA - Valentino Mantilla: consulta para el plan de recarga.
// No agrega CSV ni cambia estados: solo revisa y devuelve los datos.
// ---------------------------------------------------------------------
std::string consultarPlanRecarga(std::string codigoPedido, std::string codigoUnidad,
                                 Pedido pedidos[], int cantidadPedidos,
                                 Unidad unidades[], int cantidadUnidades,
                                 int* litrosPedido, int* capacidadUnidad) {
    if (litrosPedido == nullptr || capacidadUnidad == nullptr) {
        return "Falta donde guardar los datos consultados.";
    }
    if (!arreglosCorrectos(pedidos, cantidadPedidos, unidades, cantidadUnidades)) {
        return "Los arreglos de pedidos o unidades son incorrectos.";
    }
    int p = buscarPedidoEstados(codigoPedido, pedidos, cantidadPedidos);
    if (p == -1) return "No existe un pedido con ese codigo.";
    if (pedidos[p].estado != EstadoPedido::pendiente) {
        return "El plan de recarga solo se simula para pedidos pendientes.";
    }
    int u = buscarUnidadEstados(codigoUnidad, unidades, cantidadUnidades);
    if (u == -1) return "No existe una unidad con ese codigo.";
    if (unidades[u].capacidad_litros <= 0) {
        return "La unidad tiene una capacidad incorrecta.";
    }
    if (pedidos[p].litros <= unidades[u].capacidad_litros) {
        return "El pedido cabe en un solo viaje; no necesita plan de recarga.";
    }
    *litrosPedido = pedidos[p].litros;
    *capacidadUnidad = unidades[u].capacidad_litros;
    return "";
}
