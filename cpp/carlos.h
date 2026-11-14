// Incluir tipos.h antes de este archivo, una sola vez por .cpp.
// Cada registro devuelve "" si salio bien, o un mensaje si hubo un error.
// cantidad apunta al contador del arreglo y capacidad es su tamano real.

std::string registrarPunto(Punto nuevo, Punto puntos[], int* cantidad, int capacidad);

std::string registrarConexion(Conexion nueva, Conexion conexiones[], int* cantidad,
                             int capacidad, Punto puntos[], int cantidadPuntos);

std::string registrarUnidad(Unidad nueva, Unidad unidades[], int* cantidad, int capacidad);

std::string registrarPedido(Pedido nuevo, Pedido pedidos[], int* cantidad,
                           int capacidad, Punto puntos[], int cantidadPuntos);

// FA - Autor: Carlos Doig. Este detalle solo se guarda en memoria, no en otro CSV.
// Conservamos las entregadas para reconocer un pedido que ya fue procesado.
struct ReservaAgua {
    std::string codigo_pedido;
    std::string punto_planta;
    int litros;
    bool entregada;
};

std::string consultarAguaLibre(StockAgua stock, int* litrosLibres);

// Reconstruye las reservas de pedidos asignados solo si su suma coincide con el CSV.
// El arreglo de salida debe estar vacio y tener capacidad suficiente.
std::string prepararReservasIniciales(StockAgua stock, Pedido pedidos[], int cantidadPedidos,
                                     ReservaAgua reservas[], int* cantidad, int capacidad);

// Se llama despues de validar la asignacion, pero antes de cambiar el pedido.
std::string reservarAgua(Pedido pedido, StockAgua* stock, ReservaAgua reservas[],
                        int* cantidad, int capacidad);

// Se llama antes de marcar entregado el pedido y liberar su cisterna.
std::string registrarEntregaAgua(Pedido pedido, StockAgua* stock,
                                ReservaAgua reservas[], int cantidad);

// El llamador indica el tope de trabajo; no se inventa un limite en el CSV.
std::string reponerAgua(StockAgua* stock, int litrosRecibidos, int topeStock);

// 4.1.1 - Carlos Doig. Devuelve 0 al salir o 1 si la lectura de entrada falla.
int ejecutarMenu();
