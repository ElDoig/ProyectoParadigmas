#include <string>

// Carlos Doig: estructuras compartidas de los cuatro CSV base.
// Campos en el orden de docs/CONTRATO_CSV.md; sin lectura ni validaciones.
// Incluir este archivo una sola vez por .cpp; no incluirlo desde otro .h.

enum class TipoPunto { planta, colegio, asentamiento, mercado };
enum class EstadoConexion { habilitada, cerrada };
enum class EstadoUnidad { disponible, ocupada };
enum class Prioridad { alta, media, baja };
enum class EstadoPedido { pendiente, asignado, entregado };

struct Punto {
    std::string codigo;
    std::string nombre;
    TipoPunto tipo;
    std::string zona;
};

struct Conexion {
    std::string origen;
    std::string destino;
    double distancia_km;
    int tiempo_min;
    EstadoConexion estado;
};

// Una unidad representa una cisterna; no se duplica como otro struct.
struct Unidad {
    std::string codigo;
    std::string tipo; // "cisterna" en el escenario; el contrato lo define texto.
    int capacidad_litros;
    double costo_km;
    EstadoUnidad estado;
};

struct Pedido {
    std::string codigo;
    std::string origen;
    std::string destino;
    int litros;
    Prioridad prioridad;
    EstadoPedido estado;
    // "" representa el campo CSV vacio cuando no hay unidad asignada.
    std::string unidad;
};

// FA - Autor: Carlos Doig. Campos del CSV stock_agua.csv, sin cambiarlos.
struct StockAgua {
    std::string punto_planta;
    int litros_stock;
    int litros_reservados;
};
