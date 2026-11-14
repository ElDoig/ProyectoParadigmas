# Tipos compartidos de Carlos

Esta parte implementa las estructuras C++ base y la funcionalidad Haskell 4.2.2. La fuente de los campos es `CONTRATO_CSV.md`. No agrega columnas, lectores CSV, validaciones, gestión de estados ni stock.

## Uso en C++17

Desde cualquier archivo de `cpp/`, incluir `#include "tipos.h"` una sola vez, directamente en el `.cpp`. Los cuatro registros son `Punto`, `Conexion`, `Unidad` y `Pedido`. `Unidad` representa una cisterna; no crear otra estructura con los mismos campos.

Se utilizan `struct`, `enum class`, `int`, `double` y `std::string`. No hay un namespace propio ni guardas de preprocesador: el encabezado no debe incluirse desde otros encabezados ni dos veces en el mismo `.cpp`. Si la integración requiere inclusiones indirectas, consultar antes de añadir mecanismos no vistos en clase.

```cpp
#include "tipos.h"

Conexion conexion{
    "P01", "P06", 4.0, 15, EstadoConexion::habilitada
};
Pedido pedido{
    "D001", "P01", "P03", 6000,
    Prioridad::alta, EstadoPedido::pendiente, ""
};
```

Los miembros tienen exactamente los nombres y orden de las cabeceras de los CSV de prueba y del contrato actualizado. `Punto.tipo` usa `TipoPunto::{planta,colegio,asentamiento,mercado}`; cada estado y la prioridad tienen su propio `enum class` para no mezclar dominios. `Unidad.tipo` sigue siendo texto, como indica el contrato.

## Uso en Haskell

Los tres tipos usan constructores normales y las funciones de acceso usan patrones. `Carlos.hs` comienza con `module Carlos where`, la declaración mínima autorizada por Carlos para compartir los tipos entre archivos. Sin una lista de exportaciones, quedan disponibles todos los tipos, constructores y funciones definidos en el archivo. Los compañeros pueden usarlos con `import Carlos`. El punto de entrada existente sigue siendo compatible.

```haskell
import Carlos

conexion :: Conexion
conexion = Conexion "P01" "P06" 4.0 15 Habilitada

pedido :: Pedido
pedido = Pedido "D001" "P01" "P03" 6000 Alta Pendiente ""

-- Ejemplo de lectura de un campo con un patron:
litrosDeEjemplo :: Pedido -> Int
litrosDeEjemplo (Pedido _ _ _ litros _ _ _) = litros
```

`Eq` permite comparar registros y estados; `Show` permite inspeccionarlos. Solo `Prioridad` deriva `Ord`: el orden ascendente es `Alta`, `Media`, `Baja`, para el reporte posterior. No se define un orden artificial de registros ni estados.

## Correspondencia de campos

| Registro | Campos CSV / miembros C++ | Selectores Haskell en el mismo orden |
| --- | --- | --- |
| Punto | `codigo`, `nombre`, `tipo`, `zona` | No requerido en 4.2.2; solo se define en C++. |
| Conexion | `origen`, `destino`, `distancia_km`, `tiempo_min`, `estado` | `conexionOrigen`, `conexionDestino`, `conexionDistanciaKm`, `conexionTiempoMin`, `conexionEstado` |
| Unidad | `codigo`, `tipo`, `capacidad_litros`, `costo_km`, `estado` | `unidadCodigo`, `unidadTipo`, `unidadCapacidadLitros`, `unidadCostoKm`, `unidadEstado` |
| Pedido | `codigo`, `origen`, `destino`, `litros`, `prioridad`, `estado`, `unidad` | `pedidoCodigo`, `pedidoOrigen`, `pedidoDestino`, `pedidoLitros`, `pedidoPrioridad`, `pedidoEstado`, `pedidoUnidad` |

Los accesores Haskell son funciones normales escritas con pattern matching, no campos record generados automaticamente. Conservan los nombres usados en la tabla para facilitar el trabajo de los compañeros. En ambos lenguajes los identificadores son texto y conservan mayúsculas y ceros.

Los enteros se representan con `int` / `Int`, los tipos básicos vistos en los apuntes. No se garantiza que ambos tengan el mismo rango; los lectores deberán rechazar valores fuera del rango del tipo de destino. Los datos de prueba actuales caben en ambos. Los decimales usan `double` / `Double`: son aproximaciones binarias, por lo que falta acordar redondeo al calcular y mostrar costos. Esto no cambia el formato decimal del CSV. Los tipos no verifican automáticamente positividad, finitud, referencias ni coherencia entre archivos.

## Coordinación con Nicolás y Valentino

- Nicolás: usar estos registros en la carga/guardado C++ y en el lector Haskell. Convertir explícitamente textos CSV a enumeraciones. `show Habilitada` produce `Habilitada`, no `habilitada`; no usar `Show` ni los valores numéricos de un enum C++ como serialización CSV.
- Campo `unidad` de pedidos: la cadena `""` representa que no hay unidad asignada; otro texto, como `"U01"`, conserva el código. Es la misma representación de ausencia que usa el CSV. Al escribir vacío, conservar la coma final.
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

## Criterio de clase

La implementación se limita a lo respaldado por los apuntes de Paradigmas y por la confirmación del equipo sobre `std::string` y encabezados `.h`:

- Semana 04: `data`, constructores, `deriving`, `Eq`, `Show` y `Ord`.
- Semanas 04 y 05: pattern matching, firmas de funciones y comodín `_`.
- Resumen del examen 1: `Int`, `Double` y `String`.
- Semana 09: estructuras, tipos numéricos y `enum class`.
- Semana 11: ejemplo de inicialización de estructuras con llaves.
- Semanas 12 y 13: clases, constructores y herencia; no se necesitan para estas estructuras.
- Carlos confirmó que `import` sí se enseñó y autorizó `module Carlos where` con un comentario breve que explique su uso. Esta autorización se limita a la organización de los archivos.

Ante una funcionalidad que requiera recursos no vistos, consultar al equipo antes de incorporarlos. Las comprobaciones de la versión anterior con enteros de 64 bits y unidad opcional no certifican esta versión simplificada.

## Verificación de la versión simplificada

- C++: compilados los cinco `.cpp` con MSVC 19.12, C++17, `/W4 /WX`; sin advertencias. Menú probado con `0`, salida correcta.
- Compartición de estructuras: compilación y enlace de dos consumidores, incluyendo el encabezado una sola vez por `.cpp`. Construidos los cuatro tipos y probados los códigos, medidas, estados y unidad ausente (`""`) o presente. Resultado: código de salida 0.
- Los campos de las cuatro estructuras siguen coincidiendo con las cabeceras de los CSV de prueba.
- Haskell: revisión estática de los tres constructores y las 17 funciones de acceso; cantidad de argumentos y tipo del campo recuperado coherentes. No se encontró GHC disponible: no se ha compilado ni ejecutado esta versión.
- Eliminados del código de tipos C++ los recursos no vistos señalados en la revisión; el cuerpo de tipos Haskell ya no usa records ni tipos opcionales o enteros especializados. La declaración de módulo quedó simplificada a `module Carlos where`, según lo autorizado.
- Comprobaciones puntuales en `build/carlos-clases/`, ignorado por Git. Los datos CSV y archivos de compañeros se mantienen intactos.
