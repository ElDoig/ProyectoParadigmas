#include "tipos.h"
#include "carlos.h"

// 4.1.3 - Carlos Doig: registros en memoria con validaciones.
// El arreglo y su contador los recibe cada funcion; no hay datos globales.

// Buscamos el punto para revisar referencias sin repetir el recorrido.
int buscarPuntoRegistro(std::string codigo, Punto puntos[], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        if (puntos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

std::string registrarPunto(Punto nuevo, Punto puntos[], int* cantidad, int capacidad) {
    if (puntos == nullptr || cantidad == nullptr) {
        return "Falta el arreglo de puntos o su contador.";
    }
    if (capacidad < 0 || *cantidad < 0 || *cantidad > capacidad) {
        return "El contador o la capacidad de puntos es incorrecto.";
    }
    if (*cantidad == capacidad) {
        return "No hay espacio para otro punto.";
    }
    if (nuevo.codigo == "" || nuevo.nombre == "" || nuevo.zona == "") {
        return "El codigo, nombre y zona del punto son obligatorios.";
    }
    if (nuevo.tipo != TipoPunto::planta && nuevo.tipo != TipoPunto::colegio &&
        nuevo.tipo != TipoPunto::asentamiento && nuevo.tipo != TipoPunto::mercado) {
        return "El tipo de punto no es valido.";
    }
    if (buscarPuntoRegistro(nuevo.codigo, puntos, *cantidad) != -1) {
        return "Ya existe un punto con ese codigo.";
    }

    // Solo agregamos el dato despues de pasar todas las comprobaciones.
    puntos[*cantidad] = nuevo;
    *cantidad = *cantidad + 1;
    return "";
}

std::string registrarConexion(Conexion nueva, Conexion conexiones[], int* cantidad,
                             int capacidad, Punto puntos[], int cantidadPuntos) {
    if (conexiones == nullptr || cantidad == nullptr) {
        return "Falta el arreglo de conexiones o su contador.";
    }
    if (capacidad < 0 || *cantidad < 0 || *cantidad > capacidad) {
        return "El contador o la capacidad de conexiones es incorrecto.";
    }
    if (*cantidad == capacidad) {
        return "No hay espacio para otra conexion.";
    }
    if (cantidadPuntos < 0 || (cantidadPuntos > 0 && puntos == nullptr)) {
        return "Los puntos recibidos son incorrectos.";
    }
    if (nueva.origen == "" || nueva.destino == "") {
        return "El origen y destino de la conexion son obligatorios.";
    }
    if (nueva.origen == nueva.destino) {
        return "La conexion debe unir dos puntos distintos.";
    }
    if (buscarPuntoRegistro(nueva.origen, puntos, cantidadPuntos) == -1 ||
        buscarPuntoRegistro(nueva.destino, puntos, cantidadPuntos) == -1) {
        return "El origen y destino deben ser puntos registrados.";
    }
    if (!(nueva.distancia_km > 0) || nueva.tiempo_min <= 0) {
        return "La distancia y el tiempo deben ser mayores que cero.";
    }
    if (nueva.estado != EstadoConexion::habilitada && nueva.estado != EstadoConexion::cerrada) {
        return "El estado de la conexion no es valido.";
    }
    for (int i = 0; i < *cantidad; i++) {
        // La via es de doble sentido, asi que el orden no cambia la conexion.
        if ((conexiones[i].origen == nueva.origen && conexiones[i].destino == nueva.destino) ||
            (conexiones[i].origen == nueva.destino && conexiones[i].destino == nueva.origen)) {
            return "Ya existe una conexion entre esos puntos.";
        }
    }

    conexiones[*cantidad] = nueva;
    *cantidad = *cantidad + 1;
    return "";
}

std::string registrarUnidad(Unidad nueva, Unidad unidades[], int* cantidad, int capacidad) {
    if (unidades == nullptr || cantidad == nullptr) {
        return "Falta el arreglo de unidades o su contador.";
    }
    if (capacidad < 0 || *cantidad < 0 || *cantidad > capacidad) {
        return "El contador o la capacidad de unidades es incorrecto.";
    }
    if (*cantidad == capacidad) {
        return "No hay espacio para otra unidad.";
    }
    if (nueva.codigo == "") {
        return "El codigo de la unidad es obligatorio.";
    }
    if (nueva.tipo != "cisterna") {
        return "El tipo de unidad debe ser cisterna.";
    }
    if (nueva.capacidad_litros <= 0 || !(nueva.costo_km > 0)) {
        return "La capacidad y el costo por kilometro deben ser mayores que cero.";
    }
    // Una unidad nueva empieza libre; asignarla corresponde a la gestion de estados.
    if (nueva.estado != EstadoUnidad::disponible) {
        return "Una cisterna nueva debe registrarse como disponible.";
    }
    for (int i = 0; i < *cantidad; i++) {
        if (unidades[i].codigo == nueva.codigo) {
            return "Ya existe una unidad con ese codigo.";
        }
    }

    unidades[*cantidad] = nueva;
    *cantidad = *cantidad + 1;
    return "";
}

std::string registrarPedido(Pedido nuevo, Pedido pedidos[], int* cantidad,
                           int capacidad, Punto puntos[], int cantidadPuntos) {
    if (pedidos == nullptr || cantidad == nullptr) {
        return "Falta el arreglo de pedidos o su contador.";
    }
    if (capacidad < 0 || *cantidad < 0 || *cantidad > capacidad) {
        return "El contador o la capacidad de pedidos es incorrecto.";
    }
    if (*cantidad == capacidad) {
        return "No hay espacio para otro pedido.";
    }
    if (cantidadPuntos < 0 || (cantidadPuntos > 0 && puntos == nullptr)) {
        return "Los puntos recibidos son incorrectos.";
    }
    if (nuevo.codigo == "" || nuevo.origen == "" || nuevo.destino == "") {
        return "El codigo, origen y destino del pedido son obligatorios.";
    }
    if (nuevo.origen == nuevo.destino) {
        return "El origen y destino del pedido deben ser distintos.";
    }
    int origen = buscarPuntoRegistro(nuevo.origen, puntos, cantidadPuntos);
    int destino = buscarPuntoRegistro(nuevo.destino, puntos, cantidadPuntos);
    if (origen == -1 || destino == -1) {
        return "El origen y destino deben ser puntos registrados.";
    }
    if (puntos[origen].tipo != TipoPunto::planta) {
        return "El pedido debe salir de una planta de abastecimiento.";
    }
    if (nuevo.litros <= 0) {
        return "Los litros del pedido deben ser mayores que cero.";
    }
    if (nuevo.prioridad != Prioridad::alta && nuevo.prioridad != Prioridad::media &&
        nuevo.prioridad != Prioridad::baja) {
        return "La prioridad del pedido no es valida.";
    }
    // Primero registramos la solicitud. Su asignacion se hace despues.
    if (nuevo.estado != EstadoPedido::pendiente || nuevo.unidad != "") {
        return "Un pedido nuevo debe estar pendiente y sin unidad asignada.";
    }
    for (int i = 0; i < *cantidad; i++) {
        if (pedidos[i].codigo == nuevo.codigo) {
            return "Ya existe un pedido con ese codigo.";
        }
    }

    pedidos[*cantidad] = nuevo;
    *cantidad = *cantidad + 1;
    return "";
}

// TODO 4.1.1 (Carlos Doig): conectar estos registros al menu cuando se acuerde la entrada.
// TODO adicional (Carlos Doig): control de stock y reserva de agua en stock_agua.csv.
