#ifndef PROYECTO_PARADIGMAS_TIPOS_H
#define PROYECTO_PARADIGMAS_TIPOS_H

#include <cstdint>
#include <optional>
#include <string>

// Carlos Doig: estructuras compartidas de los cuatro CSV base.
// Campos en el orden de docs/CONTRATO_CSV.md; sin lectura ni validaciones.
namespace logistica {

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
    std::int64_t tiempo_min;
    EstadoConexion estado;
};

// Una unidad representa una cisterna; no se duplica como otro struct.
struct Unidad {
    std::string codigo;
    std::string tipo; // "cisterna" en el escenario; el contrato lo define texto.
    std::int64_t capacidad_litros;
    double costo_km;
    EstadoUnidad estado;
};

struct Pedido {
    std::string codigo;
    std::string origen;
    std::string destino;
    std::int64_t litros;
    Prioridad prioridad;
    EstadoPedido estado;
    // std::nullopt representa el campo CSV vacio, no una cadena vacia.
    std::optional<std::string> unidad;
};

} // namespace logistica

#endif // PROYECTO_PARADIGMAS_TIPOS_H
