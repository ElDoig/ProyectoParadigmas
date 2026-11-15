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


// Adicional - Diego Rodriguez. Campos de dimensiones_cisternas.csv y restricciones_altura.csv.
struct DimensionCisterna {
    std::string unidad;
    double altura_m;
};

struct RestriccionAltura {
    std::string origen;
    std::string destino;
    double altura_max_m;
};

// Registra la altura de una cisterna existente; una sola altura por cisterna.
std::string registrarAltura(DimensionCisterna nueva, DimensionCisterna alturas[], int* cantidad,
                            int capacidad, Unidad unidades[], int cantidadUnidades);

// Registra el limite de altura de una via existente; un solo limite por via.
std::string registrarRestriccionAltura(RestriccionAltura nueva, RestriccionAltura restricciones[],
                                       int* cantidad, int capacidad,
                                       Conexion conexiones[], int cantidadConexiones);

// Indica en puedePasar si la cisterna cabe por la via; una via sin limite no la restringe.
std::string consultarPasoAltura(std::string unidad, std::string origen, std::string destino,
                                DimensionCisterna alturas[], int cantidadAlturas,
                                RestriccionAltura restricciones[], int cantidadRestricciones,
                                bool* puedePasar);