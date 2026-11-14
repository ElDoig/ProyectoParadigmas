#include <iostream>
#include "tipos.h"
#include "valentino.h"

// 4.1.4 - Valentino Mantilla: gestion de estados (paradigma imperativo).
// Sigue el estilo de carlos.cpp y lo visto en clase (Semanas 08 a 10):
//  - Arreglos de estructuras (Pedido pedidos[], Unidad unidades[]) con su
//    cantidad; los estados son enum class (sin numeros magicos).
//  - Clausulas de guarda: primero se validan las precondiciones y, si algo
//    falla, se devuelve un mensaje sin tocar el estado.
//  - Mutacion controlada: el estado (los arreglos) solo se modifica
//    despues de pasar todas las comprobaciones.
//  - Paso por direccion: los resultados extra salen por punteros (int*),
//    revisando antes que no sean nullptr.
//  - Solo variables automaticas (pila): no se usa new/delete.

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

// ---------------------------------------------------------------------
// Submenu de gestion de estados (opcion 2 del menu principal)
// Mismo estilo que los menus de carlos.cpp: palabras sin espacios con
// cin y false si la lectura falla.
// ---------------------------------------------------------------------
void mostrarResultadoEstados(std::string error, std::string mensaje) {
    if (error == "") std::cout << mensaje << "\n";
    else std::cout << "No se realizo la operacion: " << error << "\n";
}

bool cambiarConexionMenu(Conexion conexiones[], int cantidadConexiones) {
    std::string origen;
    std::string destino;
    std::string estado;
    std::cout << "Origen destino nuevo_estado.\nEstados: habilitada, cerrada.\nDatos: ";
    if (!(std::cin >> origen >> destino >> estado)) return false;
    EstadoConexion nuevo;
    if (estado == "habilitada") nuevo = EstadoConexion::habilitada;
    else if (estado == "cerrada") nuevo = EstadoConexion::cerrada;
    else {
        std::cout << "Estado no valido. No se cambio la conexion.\n";
        return true;
    }
    mostrarResultadoEstados(cambiarEstadoConexion(origen, destino, nuevo, conexiones, cantidadConexiones),
                            "Estado de la conexion actualizado en memoria.");
    return true;
}

bool asignarPedidoMenu(Pedido pedidos[], int cantidadPedidos,
                       Unidad unidades[], int cantidadUnidades) {
    std::string pedido;
    std::string unidad;
    std::cout << "Codigo del pedido y codigo de la unidad.\nDatos: ";
    if (!(std::cin >> pedido >> unidad)) return false;
    mostrarResultadoEstados(asignarPedido(pedido, unidad, pedidos, cantidadPedidos,
                                          unidades, cantidadUnidades),
                            "Pedido asignado y unidad marcada como ocupada.");
    return true;
}

bool entregarPedidoMenu(Pedido pedidos[], int cantidadPedidos,
                        Unidad unidades[], int cantidadUnidades) {
    std::string pedido;
    std::cout << "Codigo del pedido entregado: ";
    if (!(std::cin >> pedido)) return false;
    mostrarResultadoEstados(entregarPedido(pedido, pedidos, cantidadPedidos,
                                           unidades, cantidadUnidades),
                            "Pedido entregado y unidad liberada.");
    return true;
}

bool consultarPlanMenu(Pedido pedidos[], int cantidadPedidos,
                       Unidad unidades[], int cantidadUnidades) {
    std::string pedido;
    std::string unidad;
    std::cout << "Codigo del pedido y codigo de la cisterna.\nDatos: ";
    if (!(std::cin >> pedido >> unidad)) return false;
    int litros = 0;
    int capacidad = 0;
    std::string error = consultarPlanRecarga(pedido, unidad, pedidos, cantidadPedidos,
                                             unidades, cantidadUnidades, &litros, &capacidad);
    if (error != "") {
        mostrarResultadoEstados(error, "");
        return true;
    }
    std::cout << "Pedido " << pedido << ": " << litros << " L. Cisterna " << unidad
              << ": " << capacidad << " L.\n"
              << "El pedido supera la capacidad: requiere plan de recarga.\n"
              << "Los viajes, la distancia y el costo se calculan en el modulo Haskell (planRecarga).\n";
    return true;
}

bool menuEstadosValentino(Conexion conexiones[], int cantidadConexiones,
                          Pedido pedidos[], int cantidadPedidos,
                          Unidad unidades[], int cantidadUnidades) {
    std::string opcion;
    while (true) {
        std::cout << "\nGESTIONAR ESTADOS\n1. Cerrar o habilitar conexion\n2. Asignar pedido a cisterna\n"
                  << "3. Marcar pedido como entregado\n4. Consultar plan de recarga (adicional)\n"
                  << "0. Volver\nOpcion: ";
        if (!(std::cin >> opcion)) return false;
        if (opcion == "0") return true;
        if (opcion == "1") {
            if (!cambiarConexionMenu(conexiones, cantidadConexiones)) return false;
        } else if (opcion == "2") {
            if (!asignarPedidoMenu(pedidos, cantidadPedidos, unidades, cantidadUnidades)) return false;
        } else if (opcion == "3") {
            if (!entregarPedidoMenu(pedidos, cantidadPedidos, unidades, cantidadUnidades)) return false;
        } else if (opcion == "4") {
            if (!consultarPlanMenu(pedidos, cantidadPedidos, unidades, cantidadUnidades)) return false;
        } else std::cout << "Opcion no valida.\n";
    }
}
