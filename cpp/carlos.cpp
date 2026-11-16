#include <iostream>
#include "tipos.h"
#include "carlos.h"
#include "diego.h"
#include "valentino.h"
#include "nicolas.h"
#include "horarios.h"

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

// FA - Autor: Carlos Doig. Control de stock y reservas, sin guardar archivos.
std::string validarStockAgua(StockAgua stock) {
    if (stock.punto_planta == "") {
        return "Falta el codigo de la planta.";
    }
    if (stock.litros_stock < 0 || stock.litros_reservados < 0) {
        return "El stock y los litros reservados no pueden ser negativos.";
    }
    if (stock.litros_reservados > stock.litros_stock) {
        return "Los litros reservados no pueden superar el stock.";
    }
    return "";
}

// Cada planta trabaja con su propio arreglo de reservas.
std::string validarReservasAgua(StockAgua stock, ReservaAgua reservas[], int cantidad) {
    std::string error = validarStockAgua(stock);
    if (error != "") return error;
    if (cantidad < 0 || (cantidad > 0 && reservas == nullptr)) {
        return "El arreglo de reservas o su cantidad es incorrecto.";
    }

    int suma = 0;
    for (int i = 0; i < cantidad; i++) {
        if (reservas[i].codigo_pedido == "" || reservas[i].punto_planta != stock.punto_planta ||
            reservas[i].litros <= 0) {
            return "Hay una reserva con datos incorrectos o de otra planta.";
        }
        for (int j = 0; j < i; j++) {
            if (reservas[j].codigo_pedido == reservas[i].codigo_pedido) {
                return "Hay reservas repetidas para el mismo pedido.";
            }
        }
        if (!reservas[i].entregada) {
            // Comprobamos antes de sumar: la suma nunca pasa del total reservado.
            if (reservas[i].litros > stock.litros_reservados - suma) {
                return "Las reservas por pedido no coinciden con el total del stock.";
            }
            suma = suma + reservas[i].litros;
        }
    }
    if (suma != stock.litros_reservados) {
        return "Las reservas por pedido no coinciden con el total del stock.";
    }
    return "";
}

std::string consultarAguaLibre(StockAgua stock, int* litrosLibres) {
    if (litrosLibres == nullptr) return "Falta donde guardar el agua libre.";
    std::string error = validarStockAgua(stock);
    if (error != "") return error;
    *litrosLibres = stock.litros_stock - stock.litros_reservados;
    return "";
}

std::string prepararReservasIniciales(StockAgua stock, Pedido pedidos[], int cantidadPedidos,
                                     ReservaAgua reservas[], int* cantidad, int capacidad) {
    std::string error = validarStockAgua(stock);
    if (error != "") return error;
    if (reservas == nullptr || cantidad == nullptr || capacidad < 0 || cantidadPedidos < 0 ||
        (cantidadPedidos > 0 && pedidos == nullptr)) {
        return "Los arreglos, cantidades o capacidad recibidos son incorrectos.";
    }
    if (*cantidad != 0) {
        return "La preparacion inicial requiere un arreglo de reservas vacio.";
    }

    int suma = 0;
    int necesarias = 0;
    for (int i = 0; i < cantidadPedidos; i++) {
        if (pedidos[i].origen == stock.punto_planta && pedidos[i].estado == EstadoPedido::asignado) {
            if (pedidos[i].codigo == "" || pedidos[i].litros <= 0 || pedidos[i].unidad == "") {
                return "Hay un pedido asignado con datos incorrectos.";
            }
            for (int j = 0; j < i; j++) {
                if (pedidos[j].codigo == pedidos[i].codigo) {
                    return "Hay pedidos repetidos al preparar las reservas.";
                }
            }
            if (necesarias == capacidad) return "No hay espacio para las reservas iniciales.";
            if (pedidos[i].litros > stock.litros_reservados - suma) {
                return "Los pedidos asignados no coinciden con los litros reservados del stock.";
            }
            suma = suma + pedidos[i].litros;
            necesarias = necesarias + 1;
        }
    }
    if (suma != stock.litros_reservados) {
        return "Los pedidos asignados no coinciden con los litros reservados del stock.";
    }

    // Recién copiamos despues de validar todo; un error no deja una carga a medias.
    int destino = 0;
    for (int i = 0; i < cantidadPedidos; i++) {
        if (pedidos[i].origen == stock.punto_planta && pedidos[i].estado == EstadoPedido::asignado) {
            ReservaAgua reserva = {pedidos[i].codigo, stock.punto_planta, pedidos[i].litros, false};
            reservas[destino] = reserva;
            destino = destino + 1;
        }
    }
    *cantidad = destino;
    return "";
}

std::string reservarAgua(Pedido pedido, StockAgua* stock, ReservaAgua reservas[],
                        int* cantidad, int capacidad) {
    if (stock == nullptr || reservas == nullptr || cantidad == nullptr) {
        return "Falta el stock, el arreglo de reservas o su contador.";
    }
    if (capacidad < 0 || *cantidad < 0 || *cantidad > capacidad) {
        return "El contador o la capacidad de reservas es incorrecto.";
    }
    std::string error = validarReservasAgua(*stock, reservas, *cantidad);
    if (error != "") return error;
    if (pedido.codigo == "" || pedido.origen != stock->punto_planta || pedido.litros <= 0) {
        return "El pedido debe tener codigo, litros positivos y salir de esta planta.";
    }
    if (pedido.estado != EstadoPedido::pendiente || pedido.unidad != "") {
        return "El pedido debe estar pendiente y sin unidad antes de reservar.";
    }
    for (int i = 0; i < *cantidad; i++) {
        if (reservas[i].codigo_pedido == pedido.codigo) {
            return "Ese pedido ya tiene una reserva o una entrega registrada.";
        }
    }
    if (*cantidad == capacidad) return "No hay espacio para otra reserva.";
    if (pedido.litros > stock->litros_stock - stock->litros_reservados) {
        return "No hay suficientes litros libres para ese pedido.";
    }

    ReservaAgua reserva = {pedido.codigo, stock->punto_planta, pedido.litros, false};
    reservas[*cantidad] = reserva;
    stock->litros_reservados = stock->litros_reservados + pedido.litros;
    *cantidad = *cantidad + 1;
    return "";
}

std::string registrarEntregaAgua(Pedido pedido, StockAgua* stock,
                                ReservaAgua reservas[], int cantidad) {
    if (stock == nullptr) return "Falta el stock de la planta.";
    std::string error = validarReservasAgua(*stock, reservas, cantidad);
    if (error != "") return error;
    if (pedido.codigo == "" || pedido.origen != stock->punto_planta || pedido.litros <= 0) {
        return "Los datos del pedido no corresponden a esta planta.";
    }
    if (pedido.estado != EstadoPedido::asignado || pedido.unidad == "") {
        return "El pedido debe estar asignado antes de registrar la entrega de agua.";
    }
    int posicion = -1;
    for (int i = 0; i < cantidad; i++) {
        if (reservas[i].codigo_pedido == pedido.codigo) posicion = i;
    }
    if (posicion == -1) return "No existe una reserva para ese pedido.";
    if (reservas[posicion].entregada) return "La entrega de ese pedido ya fue registrada.";
    if (reservas[posicion].litros != pedido.litros) {
        return "Los litros del pedido cambiaron respecto a su reserva.";
    }

    // La reserva ya validada alcanza: descontamos una sola vez y conservamos el historial.
    stock->litros_stock = stock->litros_stock - reservas[posicion].litros;
    stock->litros_reservados = stock->litros_reservados - reservas[posicion].litros;
    reservas[posicion].entregada = true;
    return "";
}

std::string reponerAgua(StockAgua* stock, int litrosRecibidos, int topeStock) {
    if (stock == nullptr) return "Falta el stock de la planta.";
    std::string error = validarStockAgua(*stock);
    if (error != "") return error;
    if (litrosRecibidos <= 0) return "Los litros recibidos deben ser mayores que cero.";
    if (topeStock < stock->litros_stock) return "El tope no puede ser menor que el stock actual.";
    // El tope lo indica quien llama. Restamos primero para no desbordar el int al sumar.
    if (litrosRecibidos > topeStock - stock->litros_stock) {
        return "La reposicion supera el tope de stock indicado.";
    }
    stock->litros_stock = stock->litros_stock + litrosRecibidos;
    return "";
}

// Preparamos el cambio en copias. Solo lo aplicamos si tambien se pudo reservar el agua.
std::string asignarPedidoConAgua(std::string codigoPedido, std::string codigoUnidad,
                                Pedido pedidos[], int cantidadPedidos,
                                Unidad unidades[], int cantidadUnidades,
                                StockAgua* stock, ReservaAgua reservas[],
                                int* cantidadReservas, int capacidadReservas) {
    int p = -1;
    int u = -1;
    std::string error = validarAsignacion(codigoPedido, codigoUnidad, pedidos, cantidadPedidos,
                                         unidades, cantidadUnidades, &p, &u);
    if (error != "") return error;

    Pedido preparado[1] = {pedidos[p]};
    Unidad preparada[1] = {unidades[u]};
    error = asignarPedido(codigoPedido, codigoUnidad, preparado, 1, preparada, 1);
    if (error != "") return error;
    error = reservarAgua(pedidos[p], stock, reservas, cantidadReservas, capacidadReservas);
    if (error != "") return error;

    pedidos[p] = preparado[0];
    unidades[u] = preparada[0];
    return "";
}

std::string entregarPedidoConAgua(std::string codigoPedido,
                                 Pedido pedidos[], int cantidadPedidos,
                                 Unidad unidades[], int cantidadUnidades,
                                 StockAgua* stock, ReservaAgua reservas[], int cantidadReservas) {
    int p = -1;
    int u = -1;
    std::string error = validarEntrega(codigoPedido, pedidos, cantidadPedidos,
                                      unidades, cantidadUnidades, &p, &u);
    if (error != "") return error;
    if (unidades[u].estado != EstadoUnidad::ocupada) {
        return "La unidad del pedido asignado debe estar ocupada.";
    }

    Pedido preparado[1] = {pedidos[p]};
    Unidad preparada[1] = {unidades[u]};
    error = entregarPedido(codigoPedido, preparado, 1, preparada, 1);
    if (error != "") return error;
    // El pedido original sigue asignado cuando descontamos su reserva.
    error = registrarEntregaAgua(pedidos[p], stock, reservas, cantidadReservas);
    if (error != "") return error;

    pedidos[p] = preparado[0];
    unidades[u] = preparada[0];
    return "";
}

// 4.1.1 - Carlos Doig. Guardamos la sesion en arreglos, sin cargar ni guardar CSV.
// Estos tamanos son espacio de trabajo del menu, no limites nuevos del contrato.
struct AguaPlantaMenu {
    StockAgua stock;
    ReservaAgua reservas[100];
    int cantidadReservas;
};

struct DatosMenu {
    Punto puntos[100];
    Conexion conexiones[100];
    Unidad unidades[100];
    Pedido pedidos[100];
    DimensionCisterna alturas[100];
    RestriccionAltura restricciones[100];
    AguaPlantaMenu agua[10];
    // FA - Nicolas Kou: horarios de recepcion de la sesion.
    HorarioRecepcion horarios[100];
    int cantidadPuntos;
    int cantidadConexiones;
    int cantidadUnidades;
    int cantidadPedidos;
    int cantidadAlturas;
    int cantidadRestricciones;
    int cantidadPlantasAgua;
    int cantidadHorarios;
};

void mostrarResultadoMenu(std::string error, std::string mensaje) {
    if (error == "") std::cout << mensaje << "\n";
    else std::cout << "No se realizo la operacion: " << error << "\n";
}

bool capturarPuntoMenu(DatosMenu* datos) {
    Punto nuevo;
    std::string tipo;
    std::cout << "Codigo nombre tipo zona (sin espacios).\n"
              << "Tipos: planta, colegio, asentamiento, mercado.\nDatos: ";
    if (!(std::cin >> nuevo.codigo >> nuevo.nombre >> tipo >> nuevo.zona)) return false;
    if (tipo == "planta") nuevo.tipo = TipoPunto::planta;
    else if (tipo == "colegio") nuevo.tipo = TipoPunto::colegio;
    else if (tipo == "asentamiento") nuevo.tipo = TipoPunto::asentamiento;
    else if (tipo == "mercado") nuevo.tipo = TipoPunto::mercado;
    else {
        std::cout << "Tipo de punto no valido. No se registro el punto.\n";
        return true;
    }
    mostrarResultadoMenu(registrarPunto(nuevo, datos->puntos, &datos->cantidadPuntos, 100),
                         "Punto registrado en memoria.");
    return true;
}

bool capturarConexionMenu(DatosMenu* datos) {
    Conexion nueva;
    std::string estado;
    std::cout << "Origen destino distancia_km tiempo_min estado.\n"
              << "Estados: habilitada, cerrada.\nDatos: ";
    if (!(std::cin >> nueva.origen >> nueva.destino >> nueva.distancia_km >> nueva.tiempo_min >> estado)) {
        return false;
    }
    if (estado == "habilitada") nueva.estado = EstadoConexion::habilitada;
    else if (estado == "cerrada") nueva.estado = EstadoConexion::cerrada;
    else {
        std::cout << "Estado no valido. No se registro la conexion.\n";
        return true;
    }
    mostrarResultadoMenu(registrarConexion(nueva, datos->conexiones, &datos->cantidadConexiones,
                                           100, datos->puntos, datos->cantidadPuntos),
                         "Conexion registrada en memoria.");
    return true;
}

bool capturarUnidadMenu(DatosMenu* datos) {
    Unidad nueva;
    nueva.tipo = "cisterna";
    nueva.estado = EstadoUnidad::disponible;
    std::cout << "Codigo capacidad_litros costo_km. La cisterna empieza disponible.\nDatos: ";
    if (!(std::cin >> nueva.codigo >> nueva.capacidad_litros >> nueva.costo_km)) return false;
    mostrarResultadoMenu(registrarUnidad(nueva, datos->unidades, &datos->cantidadUnidades, 100),
                         "Cisterna registrada en memoria.");
    return true;
}

bool capturarPedidoMenu(DatosMenu* datos) {
    Pedido nuevo;
    std::string prioridad;
    nuevo.estado = EstadoPedido::pendiente;
    nuevo.unidad = "";
    std::cout << "Codigo origen destino litros prioridad. El pedido empieza pendiente.\n"
              << "Prioridades: alta, media, baja.\nDatos: ";
    if (!(std::cin >> nuevo.codigo >> nuevo.origen >> nuevo.destino >> nuevo.litros >> prioridad)) return false;
    if (prioridad == "alta") nuevo.prioridad = Prioridad::alta;
    else if (prioridad == "media") nuevo.prioridad = Prioridad::media;
    else if (prioridad == "baja") nuevo.prioridad = Prioridad::baja;
    else {
        std::cout << "Prioridad no valida. No se registro el pedido.\n";
        return true;
    }
    mostrarResultadoMenu(registrarPedido(nuevo, datos->pedidos, &datos->cantidadPedidos,
                                         100, datos->puntos, datos->cantidadPuntos),
                         "Pedido registrado en memoria.");
    return true;
}

bool menuRegistrosCarlos(DatosMenu* datos) {
    std::string opcion;
    while (true) {
        std::cout << "\nREGISTRAR INFORMACION\n1. Punto\n2. Conexion\n3. Cisterna\n4. Pedido\n0. Volver\nOpcion: ";
        if (!(std::cin >> opcion)) return false;
        if (opcion == "0") return true;
        if (opcion == "1") {
            if (!capturarPuntoMenu(datos)) return false;
        } else if (opcion == "2") {
            if (!capturarConexionMenu(datos)) return false;
        } else if (opcion == "3") {
            if (!capturarUnidadMenu(datos)) return false;
        } else if (opcion == "4") {
            if (!capturarPedidoMenu(datos)) return false;
        } else std::cout << "Opcion no valida.\n";
    }
}

int buscarAguaPlantaMenu(std::string codigo, DatosMenu* datos) {
    for (int i = 0; i < datos->cantidadPlantasAgua; i++) {
        if (datos->agua[i].stock.punto_planta == codigo) return i;
    }
    return -1;
}

bool iniciarStockMenu(DatosMenu* datos) {
    StockAgua nuevo;
    nuevo.litros_reservados = 0;
    std::cout << "Codigo de planta y litros iniciales. Solo se inicia una vez por planta en esta sesion.\nDatos: ";
    if (!(std::cin >> nuevo.punto_planta >> nuevo.litros_stock)) return false;
    int punto = buscarPuntoRegistro(nuevo.punto_planta, datos->puntos, datos->cantidadPuntos);
    if (punto == -1 || datos->puntos[punto].tipo != TipoPunto::planta) {
        std::cout << "Primero registra ese punto como planta.\n";
        return true;
    }
    if (buscarAguaPlantaMenu(nuevo.punto_planta, datos) != -1) {
        std::cout << "La planta ya tiene stock. Usa reposicion para agregar agua.\n";
        return true;
    }
    if (datos->cantidadPlantasAgua == 10) {
        std::cout << "No hay espacio para otra planta en esta sesion.\n";
        return true;
    }
    int libres = 0;
    std::string error = consultarAguaLibre(nuevo, &libres);
    if (error == "") {
        int posicion = datos->cantidadPlantasAgua;
        datos->agua[posicion].stock = nuevo;
        datos->agua[posicion].cantidadReservas = 0;
        datos->cantidadPlantasAgua = datos->cantidadPlantasAgua + 1;
    }
    mostrarResultadoMenu(error, "Stock iniciado en memoria, sin reservas.");
    return true;
}

bool consultarStockMenu(DatosMenu* datos) {
    std::string codigo;
    std::cout << "Codigo de planta: ";
    if (!(std::cin >> codigo)) return false;
    int posicion = buscarAguaPlantaMenu(codigo, datos);
    if (posicion == -1) {
        std::cout << "Esa planta no tiene stock iniciado en esta sesion.\n";
        return true;
    }
    int libres = 0;
    std::string error = consultarAguaLibre(datos->agua[posicion].stock, &libres);
    if (error != "") mostrarResultadoMenu(error, "");
    else std::cout << "Stock: " << datos->agua[posicion].stock.litros_stock
                   << " | Reservados: " << datos->agua[posicion].stock.litros_reservados
                   << " | Libres: " << libres << " litros.\n";
    return true;
}

bool reponerStockMenu(DatosMenu* datos) {
    std::string codigo;
    int recibidos = 0;
    int tope = 0;
    std::cout << "Codigo de planta, litros recibidos y tope de trabajo.\nDatos: ";
    if (!(std::cin >> codigo >> recibidos >> tope)) return false;
    int posicion = buscarAguaPlantaMenu(codigo, datos);
    if (posicion == -1) {
        std::cout << "Esa planta no tiene stock iniciado en esta sesion.\n";
        return true;
    }
    mostrarResultadoMenu(reponerAgua(&datos->agua[posicion].stock, recibidos, tope),
                         "Reposicion registrada en memoria.");
    return true;
}

bool menuAguaCarlos(DatosMenu* datos) {
    std::string opcion;
    while (true) {
        std::cout << "\nCONTROL DE AGUA\n1. Iniciar stock de una planta\n2. Consultar agua libre\n"
                  << "3. Reponer agua\n0. Volver\n"
                  << "Las reservas y entregas se realizan desde la opcion 2 del menu principal.\nOpcion: ";
        if (!(std::cin >> opcion)) return false;
        if (opcion == "0") return true;
        if (opcion == "1") {
            if (!iniciarStockMenu(datos)) return false;
        } else if (opcion == "2") {
            if (!consultarStockMenu(datos)) return false;
        } else if (opcion == "3") {
            if (!reponerStockMenu(datos)) return false;
        } else std::cout << "Opcion no valida.\n";
    }
}

// El menu coordina ambas partes sin cambiar las funciones de Valentino.
bool gestionarPedidoConAguaMenu(DatosMenu* datos, bool esEntrega) {
    std::string pedido;
    std::string unidad;
    int p = -1;
    int u = -1;
    std::string error;
    if (esEntrega) {
        std::cout << "Codigo del pedido entregado: ";
        if (!(std::cin >> pedido)) return false;
        error = validarEntrega(pedido, datos->pedidos, datos->cantidadPedidos,
                               datos->unidades, datos->cantidadUnidades, &p, &u);
    } else {
        std::cout << "Codigo del pedido y codigo de la cisterna.\nDatos: ";
        if (!(std::cin >> pedido >> unidad)) return false;
        error = validarAsignacion(pedido, unidad, datos->pedidos, datos->cantidadPedidos,
                                  datos->unidades, datos->cantidadUnidades, &p, &u);
    }
    if (error != "") {
        mostrarResultadoMenu(error, "");
        return true;
    }
    int planta = buscarAguaPlantaMenu(datos->pedidos[p].origen, datos);
    if (planta == -1) {
        std::cout << "Primero inicia el stock de la planta de origen desde la opcion 4 del menu principal.\n";
        return true;
    }
    if (esEntrega) {
        error = entregarPedidoConAgua(pedido, datos->pedidos, datos->cantidadPedidos,
                                      datos->unidades, datos->cantidadUnidades,
                                      &datos->agua[planta].stock, datos->agua[planta].reservas,
                                      datos->agua[planta].cantidadReservas);
        mostrarResultadoMenu(error, "Pedido entregado, agua descontada y cisterna liberada.");
    } else {
        error = asignarPedidoConAgua(pedido, unidad, datos->pedidos, datos->cantidadPedidos,
                                     datos->unidades, datos->cantidadUnidades,
                                     &datos->agua[planta].stock, datos->agua[planta].reservas,
                                     &datos->agua[planta].cantidadReservas, 100);
        mostrarResultadoMenu(error, "Pedido asignado, agua reservada y cisterna ocupada.");
    }
    return true;
}

bool menuEstadosConAguaCarlos(DatosMenu* datos) {
    std::string opcion;
    while (true) {
        std::cout << "\nGESTIONAR ESTADOS\n1. Cerrar o habilitar conexion\n2. Asignar pedido a cisterna\n"
                  << "3. Marcar pedido como entregado\n4. Consultar necesidad de recargas\n0. Volver\nOpcion: ";
        if (!(std::cin >> opcion)) return false;
        if (opcion == "0") return true;
        if (opcion == "1") {
            std::string origen, destino, estado;
            std::cout << "Origen destino nuevo_estado (habilitada, cerrada).\nDatos: ";
            if (!(std::cin >> origen >> destino >> estado)) return false;
            if (estado != "habilitada" && estado != "cerrada") {
                std::cout << "Estado no valido. No se cambio la conexion.\n";
            } else {
                EstadoConexion nuevo = EstadoConexion::habilitada;
                if (estado == "cerrada") nuevo = EstadoConexion::cerrada;
                mostrarResultadoMenu(cambiarEstadoConexion(origen, destino, nuevo,
                                     datos->conexiones, datos->cantidadConexiones),
                                     "Estado de la conexion actualizado en memoria.");
            }
        } else if (opcion == "2" || opcion == "3") {
            if (!gestionarPedidoConAguaMenu(datos, opcion == "3")) return false;
        } else if (opcion == "4") {
            std::string pedido, unidad;
            int litros = 0;
            int capacidad = 0;
            std::cout << "Codigo del pedido y codigo de la cisterna.\nDatos: ";
            if (!(std::cin >> pedido >> unidad)) return false;
            std::string error = consultarPlanRecarga(pedido, unidad, datos->pedidos,
                                datos->cantidadPedidos, datos->unidades, datos->cantidadUnidades,
                                &litros, &capacidad);
            if (error != "") mostrarResultadoMenu(error, "");
            else std::cout << "Pedido: " << litros << " L. Cisterna: " << capacidad
                           << " L. Requiere recargas; viajes, distancia y costo se calculan en Haskell.\n";
        } else std::cout << "Opcion no valida.\n";
    }
}

// 4.1.2 - Nicolas Kou Acosta: carga inicial de los CSV al abrir el programa.
// El stock de una planta solo se carga si sus reservas cuadran con los pedidos
// asignados; si no cuadran se avisa y esa planta queda sin iniciar, para no
// corregir los datos por nuestra cuenta.
void cargarSesionNicolas(DatosMenu* datos) {
    std::string error = cargarDatosIniciales(
        datos->puntos, &datos->cantidadPuntos, 100,
        datos->conexiones, &datos->cantidadConexiones, 100,
        datos->unidades, &datos->cantidadUnidades, 100,
        datos->pedidos, &datos->cantidadPedidos, 100);
    if (error != "") {
        std::cout << "No se pudo cargar la informacion guardada: " << error << "\n"
                  << "La sesion empieza vacia.\n";
        datos->cantidadPuntos = 0;
        datos->cantidadConexiones = 0;
        datos->cantidadUnidades = 0;
        datos->cantidadPedidos = 0;
        return;
    }
    std::cout << "Datos cargados: " << datos->cantidadPuntos << " puntos, "
              << datos->cantidadConexiones << " conexiones, "
              << datos->cantidadUnidades << " cisternas y "
              << datos->cantidadPedidos << " pedidos.\n";

    error = cargarHorarios(rutaRuntime("horarios_recepcion.csv"), datos->horarios,
                           &datos->cantidadHorarios, 100);
    if (error != "") {
        error = cargarHorarios(rutaEjemplo("horarios_recepcion.csv"), datos->horarios,
                               &datos->cantidadHorarios, 100);
    }
    if (error != "") std::cout << "Aviso de horarios: " << error << "\n";
    else std::cout << "Horarios de recepcion cargados: " << datos->cantidadHorarios << ".\n";

    StockAgua stocks[10];
    int cantidadStocks = 0;
    error = cargarStockAgua(rutaRuntime("stock_agua.csv"), stocks, &cantidadStocks, 10);
    if (error != "") {
        error = cargarStockAgua(rutaEjemplo("stock_agua.csv"), stocks, &cantidadStocks, 10);
    }
    if (error != "") {
        std::cout << "Aviso de stock: " << error << "\n";
        return;
    }
    for (int i = 0; i < cantidadStocks; i++) {
        int posicion = datos->cantidadPlantasAgua;
        datos->agua[posicion].cantidadReservas = 0;
        std::string errorReservas = prepararReservasIniciales(
            stocks[i], datos->pedidos, datos->cantidadPedidos,
            datos->agua[posicion].reservas, &datos->agua[posicion].cantidadReservas, 100);
        if (errorReservas != "") {
            std::cout << "La planta " << stocks[i].punto_planta
                      << " no se inicio: " << errorReservas << "\n"
                      << "Revisa stock_agua.csv con el equipo o inicia el stock desde la opcion 4.\n";
            datos->agua[posicion].cantidadReservas = 0;
            continue;
        }
        datos->agua[posicion].stock = stocks[i];
        datos->cantidadPlantasAgua = datos->cantidadPlantasAgua + 1;
        std::cout << "Stock cargado para " << stocks[i].punto_planta << ".\n";
    }
}

// 4.1.5 - Nicolas Kou Acosta: guarda la informacion despues de cada cambio.
void guardarSesionNicolas(DatosMenu* datos) {
    std::string error = guardarDatos(datos->puntos, datos->cantidadPuntos,
                                     datos->conexiones, datos->cantidadConexiones,
                                     datos->unidades, datos->cantidadUnidades,
                                     datos->pedidos, datos->cantidadPedidos);
    if (error != "") {
        std::cout << "Aviso: no se pudo guardar. " << error << "\n";
        return;
    }
    if (datos->cantidadPlantasAgua > 0) {
        StockAgua stocks[10];
        for (int i = 0; i < datos->cantidadPlantasAgua; i++) {
            stocks[i] = datos->agua[i].stock;
        }
        error = guardarStockAgua(rutaRuntime("stock_agua.csv"), stocks, datos->cantidadPlantasAgua);
        if (error != "") std::cout << "Aviso: no se pudo guardar el stock. " << error << "\n";
    }
}

int ejecutarMenu() {
    DatosMenu datos;
    datos.cantidadPuntos = 0;
    datos.cantidadConexiones = 0;
    datos.cantidadUnidades = 0;
    datos.cantidadPedidos = 0;
    datos.cantidadAlturas = 0;
    datos.cantidadRestricciones = 0;
    datos.cantidadPlantasAgua = 0;
    datos.cantidadHorarios = 0;
    std::cout << "Planificador de rutas y logistica - agua con cisternas\n"
              << "Escribe nombres sin espacios, por ejemplo Planta_Chillon.\n";
    cargarSesionNicolas(&datos);

    std::string opcion;
    bool entradaCorrecta = true;
    while (entradaCorrecta) {
        std::cout << "\nMENU PRINCIPAL\n1. Registrar informacion\n2. Gestionar estados\n"
                  << "3. Consultas generales y reporte\n4. Control de agua\n"
                  << "5. Horarios de recepcion\n0. Salir\nOpcion: ";
        if (!(std::cin >> opcion)) entradaCorrecta = false;
        else if (opcion == "0") {
            std::cout << "Hasta luego.\n";
            return 0;
        } else if (opcion == "1") entradaCorrecta = menuRegistrosCarlos(&datos);
        else if (opcion == "4") entradaCorrecta = menuAguaCarlos(&datos);
        else if (opcion == "5") {
            // FA - Nicolas Kou: registro y consulta de horarios de recepcion.
            entradaCorrecta = menuHorariosNicolas(datos.horarios, &datos.cantidadHorarios, 100,
                                                  datos.puntos, datos.cantidadPuntos);
        }
        else if (opcion == "3") {
            // Diego consulta los mismos datos y actualiza sus contadores con los punteros.
            entradaCorrecta = menuConsultasDiego(
                datos.puntos, datos.cantidadPuntos, datos.conexiones, datos.cantidadConexiones,
                datos.unidades, datos.cantidadUnidades, datos.pedidos, datos.cantidadPedidos,
                datos.alturas, &datos.cantidadAlturas, 100,
                datos.restricciones, &datos.cantidadRestricciones, 100);
        } else if (opcion == "2") entradaCorrecta = menuEstadosConAguaCarlos(&datos);
        else std::cout << "Opcion no valida.\n";
        // 4.1.5 - Nicolas Kou: solo las opciones que cambian datos vuelven a guardar.
        if (entradaCorrecta && (opcion == "1" || opcion == "2" || opcion == "4")) {
            guardarSesionNicolas(&datos);
        }
    }
    // Sin clear/ignore ni un analizador propio: si cin falla, evitamos repetir el error en un bucle.
    std::cout << "Fin de entrada o dato numerico invalido. La sesion termina sin guardar.\n";
    return 1;
}
