# Tipos compartidos de Carlos

Esta parte implementa las estructuras C++ base y la funcionalidad Haskell 4.2.2. La fuente de los campos es `CONTRATO_CSV.md`. No agrega columnas, lectores CSV, validaciones, gestión de estados ni stock.

## Uso en C++17

Desde cualquier archivo de `cpp/`, incluir `#include "tipos.h"`. Los cuatro registros están en el namespace `logistica`: `Punto`, `Conexion`, `Unidad` y `Pedido`. `Unidad` representa una cisterna; no crear otra estructura con los mismos campos.

```cpp
#include "tipos.h"

logistica::Conexion conexion{
    "P01", "P06", 4.0, 15, logistica::EstadoConexion::habilitada
};
logistica::Pedido pedido{
    "D001", "P01", "P03", 6000,
    logistica::Prioridad::alta, logistica::EstadoPedido::pendiente,
    std::nullopt
};
```

Los miembros tienen exactamente los nombres y orden de las cabeceras de los CSV de prueba y del contrato actualizado. `Punto.tipo` usa `TipoPunto::{planta,colegio,asentamiento,mercado}`; cada estado y la prioridad tienen su propio `enum class` para no mezclar dominios. `Unidad.tipo` sigue siendo texto, como indica el contrato.

## Uso en Haskell

Importar `Carlos (Conexion(..), Unidad(..), Pedido(..), EstadoConexion(..), EstadoUnidad(..), Prioridad(..), EstadoPedido(..))` o seleccionar solo los nombres necesarios. Se exportan constructores y selectores. `Main.hs` puede conservar su `import Carlos ()`; los módulos de compañeros incorporarán los tipos cuando los necesiten.

```haskell
import Carlos

conexion :: Conexion
conexion = Conexion "P01" "P06" 4.0 15 Habilitada

pedido :: Pedido
pedido = Pedido
    { pedidoCodigo = "D001"
    , pedidoOrigen = "P01"
    , pedidoDestino = "P03"
    , pedidoLitros = 6000
    , pedidoPrioridad = Alta
    , pedidoEstado = Pendiente
    , pedidoUnidad = Nothing
    }
```

`Eq` permite comparar registros y estados; `Show` permite inspeccionarlos. Solo `Prioridad` deriva `Ord`: el orden ascendente es `Alta`, `Media`, `Baja`, para el reporte posterior. No se define un orden artificial de registros ni estados.

## Correspondencia de campos

| Registro | Campos CSV / miembros C++ | Selectores Haskell en el mismo orden |
| --- | --- | --- |
| Punto | `codigo`, `nombre`, `tipo`, `zona` | No requerido en 4.2.2; solo se define en C++. |
| Conexion | `origen`, `destino`, `distancia_km`, `tiempo_min`, `estado` | `conexionOrigen`, `conexionDestino`, `conexionDistanciaKm`, `conexionTiempoMin`, `conexionEstado` |
| Unidad | `codigo`, `tipo`, `capacidad_litros`, `costo_km`, `estado` | `unidadCodigo`, `unidadTipo`, `unidadCapacidadLitros`, `unidadCostoKm`, `unidadEstado` |
| Pedido | `codigo`, `origen`, `destino`, `litros`, `prioridad`, `estado`, `unidad` | `pedidoCodigo`, `pedidoOrigen`, `pedidoDestino`, `pedidoLitros`, `pedidoPrioridad`, `pedidoEstado`, `pedidoUnidad` |

Los prefijos Haskell evitan colisiones de selectores sin extensiones ni dependencias externas. En ambos lenguajes los identificadores son texto y conservan mayúsculas y ceros.

Los enteros se representan con `std::int64_t` / `Int64` (64 bits con signo); los lectores deben detectar desbordamientos antes de construir el registro. Los decimales usan `double` / `Double`: son aproximaciones binarias, por lo que falta acordar redondeo al calcular y mostrar costos. Esto no cambia el formato decimal del CSV. Los tipos no verifican automáticamente positividad, finitud, referencias ni coherencia entre archivos.

## Coordinación con Nicolás y Valentino

- Nicolás: usar estos registros en la carga/guardado C++ y en el lector Haskell. Convertir explícitamente textos CSV a enumeraciones. `show Habilitada` produce `Habilitada`, no `habilitada`; no usar `Show` ni los valores numéricos de un enum C++ como serialización CSV.
- Campo `unidad` de pedidos: vacío se convierte a `std::nullopt` / `Nothing`; un ID a `std::optional<std::string>{"U01"}` / `Just "U01"`. Una cadena vacía dentro de `Just` o `optional` no representa ausencia. Al escribir vacío, conservar la coma final.
- Valentino: asignar con `EstadoPedido::asignado` / `Asignado` y una unidad identificada, y ocupar la unidad. Al entregar, conservar el ID histórico en el pedido y liberar la unidad. Estas operaciones siguen pendientes en su módulo; el stock se coordinará después con Carlos.
- Las conexiones representan una sola fila por vía de doble sentido; los tipos no duplican el sentido inverso ni implementan rutas. El plan de recarga posterior conserva las mismas estructuras.

## Revisión del contrato y datos pendientes

Al actualizar `Carlos` desde `origin/main` (base `19eead9`), se detectaron diferencias entre los CSV de prueba y el contrato anterior. Por indicación del equipo, se adoptaron las cabeceras de esos datos y se actualizó `CONTRATO_CSV.md` con los siguientes cambios:

| Archivo | Nombres anteriores | Nombres adoptados |
| --- | --- | --- |
| `puntos.csv` | `id,nombre,tipo,distrito`; tipo `planta` o `destino` | `codigo,nombre,tipo,zona`; tipos `planta`, `colegio`, `asentamiento`, `mercado`. |
| `unidades.csv` | `id,tipo,capacidad_litros,costo_por_km,estado` | `codigo,tipo,capacidad_litros,costo_km,estado` |
| `pedidos.csv` | `id,origen,destino,carga_litros,prioridad,estado,unidad_asignada` | `codigo,origen,destino,litros,prioridad,estado,unidad` |

Los lectores futuros deben usar únicamente los nombres adoptados. No había lectores ni funciones implementados que hubiera que migrar en `cpp/` o `haskell/`. No se cambió ningún CSV.

La integración del stock sí tiene un dato pendiente: `stock_agua.csv` conserva 4000 litros reservados, pero los pedidos asignados `D003` y `D005` suman 12000. Carlos y Valentino deben coordinar su actualización antes de probar reservas. Los campos, tipos y estados de los registros base sí representan los datos actuales.

## Verificación realizada

- C++: compilación completa de los cinco `.cpp` con MSVC 19.12, C++17, `/W4 /WX`; sin advertencias. Menú probado con `0`, salida correcta.
- Encabezado: compilación y enlace de dos consumidores independientes, inclusión repetida de `tipos.h`, construcción de los cuatro registros, estados, ausencia/presencia de unidad, conservación de la unidad entregada y volumen superior al límite de un entero de 32 bits. Prueba terminada con código 0.
- Correspondencia: comprobadas las cuatro cabeceras contra el contrato y los miembros C++, y los valores de tipos de punto, estados y prioridades contra los datos de prueba.
- Haskell: revisión estática de los siete tipos exportados, los 17 selectores sin duplicados y los imports. Solo se usa `Data.Int` del paquete estándar `base`. No se encontró GHC disponible: la compilación y la ejecución siguen pendientes; una revisión estática no las sustituye.
- Los programas y fuentes usados para las comprobaciones puntuales están en `build/carlos-base/`, ignorado por Git. No se requieren nuevas dependencias del proyecto.

En un equipo con GHC, desde la raíz se puede verificar la integración con `ghc -Wall -Werror -fno-code -ihaskell haskell/Main.hs` y ejecutar con `runghc -ihaskell haskell/Main.hs`. Las funciones lectoras de Nicolás deben importar los tipos de `Carlos` al implementarse.
