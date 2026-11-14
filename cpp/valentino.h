// 4.1.4 - Valentino Mantilla: gestion de estados.

// Cierra o habilita una conexion (se busca en cualquier sentido).
std::string cambiarEstadoConexion(std::string origen, std::string destino,
                                  EstadoConexion nuevoEstado,
                                  Conexion conexiones[], int cantidadConexiones);

// Solo revisa si el pedido se puede asignar a la unidad; no cambia nada.
// Devuelve en posPedido y posUnidad donde estan dentro de sus arreglos.
// Sirve para llamar a reservarAgua (Carlos) entre la validacion y el cambio.
std::string validarAsignacion(std::string codigoPedido, std::string codigoUnidad,
                              Pedido pedidos[], int cantidadPedidos,
                              Unidad unidades[], int cantidadUnidades,
                              int* posPedido, int* posUnidad);

// Valida y asigna: pedido pendiente -> asignado, unidad disponible -> ocupada.
std::string asignarPedido(std::string codigoPedido, std::string codigoUnidad,
                          Pedido pedidos[], int cantidadPedidos,
                          Unidad unidades[], int cantidadUnidades);

// Solo revisa si el pedido se puede marcar como entregado; no cambia nada.
// Sirve para llamar a registrarEntregaAgua (Carlos) antes del cambio.
std::string validarEntrega(std::string codigoPedido,
                           Pedido pedidos[], int cantidadPedidos,
                           Unidad unidades[], int cantidadUnidades,
                           int* posPedido, int* posUnidad);

// Valida y entrega: pedido asignado -> entregado, su unidad -> disponible.
std::string entregarPedido(std::string codigoPedido,
                           Pedido pedidos[], int cantidadPedidos,
                           Unidad unidades[], int cantidadUnidades);

// FA - Valentino Mantilla: consulta para el plan de recarga.
// Revisa que el pedido este pendiente y supere la capacidad de la cisterna,
// y devuelve los litros del pedido y la capacidad. No cambia estados.
// El plan (viajes, distancia y costo) lo calcula el modulo Haskell.
std::string consultarPlanRecarga(std::string codigoPedido, std::string codigoUnidad,
                                 Pedido pedidos[], int cantidadPedidos,
                                 Unidad unidades[], int cantidadUnidades,
                                 int* litrosPedido, int* capacidadUnidad);

// Submenu de la opcion 2 del menu principal (Gestionar estados).
// Devuelve false si la lectura de entrada falla, como los menus de Carlos.
bool menuEstadosValentino(Conexion conexiones[], int cantidadConexiones,
                          Pedido pedidos[], int cantidadPedidos,
                          Unidad unidades[], int cantidadUnidades);
