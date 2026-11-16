// 4.1.2 y 4.1.5 - Nicolas Kou Acosta: carga inicial y guardado de los CSV.
// Incluir tipos.h antes de este archivo, una sola vez por .cpp.
// Cada funcion devuelve "" si salio bien, o un mensaje con el error.
// Los programas se ejecutan desde la raiz del repositorio.

// Este archivo usa std::string porque carlos.cpp lo incluye y ahi no hay
// using namespace std. En los .cpp propios si se usa using namespace std.

// data/runtime guarda los datos de trabajo; data/examples los versionados.
std::string rutaRuntime(std::string nombre);
std::string rutaEjemplo(std::string nombre);

// Convierten texto a numero SIN lanzar excepciones: devuelven false si el
// texto no es un numero o si el valor no cabe en el tipo.
bool textoAEntero(std::string texto, int* valor);
bool textoADecimal(std::string texto, double* valor);

// 4.1.2. Lectura de un CSV sobre un arreglo vacio.
std::string cargarPuntos(std::string archivo, Punto puntos[], int* cantidad, int capacidad);
std::string cargarConexiones(std::string archivo, Conexion conexiones[], int* cantidad, int capacidad);
std::string cargarUnidades(std::string archivo, Unidad unidades[], int* cantidad, int capacidad);
std::string cargarPedidos(std::string archivo, Pedido pedidos[], int* cantidad, int capacidad);
std::string cargarStockAgua(std::string archivo, StockAgua stocks[], int* cantidad, int capacidad);

// 4.1.5. Escritura completa del CSV con su cabecera.
std::string guardarPuntos(std::string archivo, Punto puntos[], int cantidad);
std::string guardarConexiones(std::string archivo, Conexion conexiones[], int cantidad);
std::string guardarUnidades(std::string archivo, Unidad unidades[], int cantidad);
std::string guardarPedidos(std::string archivo, Pedido pedidos[], int cantidad);
std::string guardarStockAgua(std::string archivo, StockAgua stocks[], int cantidad);

// Carga los cuatro CSV base al iniciar el programa. Lee data/runtime y,
// si un archivo no existe ahi, toma la copia de data/examples.
std::string cargarDatosIniciales(Punto puntos[], int* cantidadPuntos, int capacidadPuntos,
                                 Conexion conexiones[], int* cantidadConexiones, int capacidadConexiones,
                                 Unidad unidades[], int* cantidadUnidades, int capacidadUnidades,
                                 Pedido pedidos[], int* cantidadPedidos, int capacidadPedidos);

// Guarda los cuatro CSV base en data/runtime despues de un cambio correcto.
std::string guardarDatos(Punto puntos[], int cantidadPuntos,
                         Conexion conexiones[], int cantidadConexiones,
                         Unidad unidades[], int cantidadUnidades,
                         Pedido pedidos[], int cantidadPedidos);
