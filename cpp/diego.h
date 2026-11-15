// Textos en minuscula de cada enumeracion, igual que en los CSV.
std::string textoTipoPunto(TipoPunto tipo);
std::string textoEstadoConexion(EstadoConexion estado);
std::string textoEstadoUnidad(EstadoUnidad estado);
std::string textoPrioridad(Prioridad prioridad);
std::string textoEstadoPedido(EstadoPedido estado);

// 4.1.6- Cada consulta devuelve "" si encontro datos o un mensaje.
std::string consultarPunto(std::string codigo, Punto puntos[], int cantidadPuntos);
std::string consultarConexionesDePunto(std::string codigo, Conexion conexiones[],
                                       int cantidadConexiones);
std::string consultarPedido(std::string codigo, Pedido pedidos[], int cantidadPedidos);
std::string consultarPedidosPorEstado(EstadoPedido estado, Pedido pedidos[], int cantidadPedidos);

// 4.1.7. Resumen de la red, las unidades y los pedidos.
void mostrarReporteGeneral(Punto puntos[], int cantidadPuntos,
                           Conexion conexiones[], int cantidadConexiones,
                           Unidad unidades[], int cantidadUnidades,
                           Pedido pedidos[], int cantidadPedidos);