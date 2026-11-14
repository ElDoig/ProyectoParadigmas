// Incluir tipos.h antes de este archivo, una sola vez por .cpp.
// Cada registro devuelve "" si salio bien, o un mensaje si hubo un error.
// cantidad apunta al contador del arreglo y capacidad es su tamano real.

std::string registrarPunto(Punto nuevo, Punto puntos[], int* cantidad, int capacidad);

std::string registrarConexion(Conexion nueva, Conexion conexiones[], int* cantidad,
                             int capacidad, Punto puntos[], int cantidadPuntos);

std::string registrarUnidad(Unidad nueva, Unidad unidades[], int* cantidad, int capacidad);

std::string registrarPedido(Pedido nuevo, Pedido pedidos[], int* cantidad,
                           int capacidad, Punto puntos[], int cantidadPuntos);
