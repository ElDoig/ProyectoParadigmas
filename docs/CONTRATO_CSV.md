# Contrato de archivos CSV

## Alcance y formato común

Los cuatro archivos base conservan los campos mínimos de la sección 3 del enunciado con los nombres de los datos de prueba actuales: `codigo`, `zona`, `capacidad_litros`, `costo_km`, `litros` y `unidad`. Esta revisión toma las cabeceras de `data/examples/` de la base `19eead9`, según lo solicitado por el equipo. Sustituye los nombres anteriores `id`, `distrito`, `costo_por_km`, `carga_litros` y `unidad_asignada`; no se admiten como cabeceras alternativas. Los cuatro CSV adicionales conservan los nombres y campos del Anexo A. Las estructuras están en `cpp/tipos.h` y los tipos Haskell en `haskell/Carlos.hs`; aún no hay parsers ni validadores en el código.

- Codificación UTF-8 sin BOM. Primera fila obligatoria con las cabeceras exactas y en el orden mostrado. Escritura con LF; los lectores también deben aceptar CRLF de Windows.
- Separador: coma. Decimal: punto, sin separadores de miles ni símbolos monetarios. Ejemplos: `10000`, `3.5`. Los textos de este perfil sencillo no contienen comas, comillas ni saltos de línea; no se usa escape de campos. Si se necesita CSV con campos entrecomillados, acordar y actualizar los tres lectores primero.
- Sin filas vacías, comentarios ni espacios al inicio/final de un campo. Todas las filas tienen tantas columnas como la cabecera, incluso si la última está vacía.
- Identificadores: texto sensible a mayúsculas, sin espacios; ejemplos `P01`, `U01`, `D001`. No convertirlos a números. En Prolog usar átomos como `'P01'`, no variables `P01`, ni cambiar los IDs a minúsculas sin una conversión acordada.
- `entero`: número en base 10 sin parte decimal. `decimal`: número decimal finito con punto opcional. Distancias en km, tiempos en minutos, volúmenes en litros, alturas en metros y dinero en soles peruanos (PEN).
- Vacío = campo de longitud cero entre comas o al final de la fila. No escribir `NULL`, `N/A`, `-` ni `0` para representar ausencia. Solo `unidad` de `pedidos.csv` admite vacío. Los demás campos son obligatorios cuando hay una fila.
- Las cabeceras, estados y enumeraciones usan exactamente las palabras en minúscula documentadas. Los nombres de lugares pueden tener tildes. Los ejemplos son sintéticos.

## Archivos obligatorios

### `puntos.csv`

Cabecera: `codigo,nombre,tipo,zona`

| Columna | Tipo | Regla |
| --- | --- | --- |
| codigo | texto | Clave única del punto; ejemplo `P01`. |
| nombre | texto | Nombre no vacío. |
| tipo | enumeración | `planta`, `colegio`, `asentamiento` o `mercado`. |
| zona | texto | Zona del punto; ejemplos actuales `comas` y `carabayllo`. |

Los receptores se distinguen por `colegio`, `asentamiento` o `mercado`; `destino` no es un tipo de punto en los datos actuales. `zona` es texto y no una enumeración limitada a los dos ejemplos.

### `conexiones.csv`

Cabecera: `origen,destino,distancia_km,tiempo_min,estado`

| Columna | Tipo | Regla / unidad |
| --- | --- | --- |
| origen | texto | Referencia a `puntos.codigo`. |
| destino | texto | Referencia a `puntos.codigo`, distinto de origen. |
| distancia_km | decimal | Mayor que cero, kilómetros. |
| tiempo_min | entero | Mayor que cero, minutos estimados por tramo. |
| estado | enumeración | `habilitada` o `cerrada`. |

Cada vía es de doble sentido y se registra una sola vez. La clave es el par **no ordenado** de puntos: `P01,P02` y `P02,P01` serían duplicados. Distancia, tiempo, estado y restricciones de altura aplican a ambos sentidos. Los lectores generan la interpretación inversa; C++ no debe guardar otra fila para ella. No hay vías paralelas en este contrato inicial. Los cálculos y reglas de rutas usan solo conexiones habilitadas.

### `unidades.csv`

Cabecera: `codigo,tipo,capacidad_litros,costo_km,estado`

| Columna | Tipo | Regla / unidad |
| --- | --- | --- |
| codigo | texto | Clave única de la unidad; ejemplo `U01`. |
| tipo | texto | `cisterna` en los datos del escenario. |
| capacidad_litros | entero | Mayor que cero, litros. |
| costo_km | decimal | Mayor que cero, PEN/km. |
| estado | enumeración | `disponible` u `ocupada`. |

### `pedidos.csv`

Cabecera: `codigo,origen,destino,litros,prioridad,estado,unidad`

| Columna | Tipo | Regla / unidad |
| --- | --- | --- |
| codigo | texto | Clave única del pedido; ejemplo `D001`. |
| origen | texto | Referencia a `puntos.codigo`; una planta en este escenario. |
| destino | texto | Referencia a `puntos.codigo`, distinto del origen. |
| litros | entero | Mayor que cero, litros solicitados. |
| prioridad | enumeración | `alta`, `media` o `baja`, en ese orden de atención. |
| estado | enumeración | `pendiente`, `asignado` o `entregado`. |
| unidad | texto opcional | Referencia a `unidades.codigo`; vacío si no tiene unidad. |

Un pendiente no tiene unidad. Un asignado debe referenciar una unidad ocupada con capacidad suficiente. Al entregar se libera la unidad; como convención inicial se conserva su ID en el pedido entregado para consultas históricas. Esa unidad puede estar disponible o atender otro pedido. No se añaden otros estados al enunciado. El plan de recarga es una simulación para pedidos que exceden la capacidad y no elude la validación de asignación base.

Ejemplo de ausencia: `D001,P01,P02,6000,alta,pendiente,` (incluye la coma final).

## Archivos adicionales del Anexo A

### `stock_agua.csv` — Carlos Doig

Cabecera: `punto_planta,litros_stock,litros_reservados`

| Columna | Tipo | Regla / unidad |
| --- | --- | --- |
| punto_planta | texto | Clave única, referencia a un punto de tipo `planta`. |
| litros_stock | entero | Mayor o igual a cero, litros existentes. |
| litros_reservados | entero | Entre cero y `litros_stock`, litros comprometidos. |

Agua libre = stock − reservas. No hay un archivo de reservas separado en la propuesta. C++ deberá reservar al asignar, descontar del stock y liberar la reserva al entregar, y registrar reposiciones. El ejemplo de stock conserva 12000 litros y 4000 reservados, pero los pedidos asignados actuales son `D003` (7000) y `D005` (5000): suman 12000. Los datos de stock deben coordinarse antes de probar la integración de reservas; no se corrigen como parte de la definición de tipos. No existe aún código para estas operaciones.

### `horarios_recepcion.csv` — Nicolás Kou

Cabecera: `punto_destino,apertura_min,cierre_min,descarga_min`

| Columna | Tipo | Regla / unidad |
| --- | --- | --- |
| punto_destino | texto | Clave única, referencia al punto receptor. |
| apertura_min | entero | Entre 0 y 1439, minutos desde medianoche. |
| cierre_min | entero | Mayor que apertura y menor o igual a 1440. |
| descarga_min | entero | Mayor que cero, duración en minutos. |

Una ventana por destino, dentro del mismo día; `540` representa 09:00 y `1440` el límite de fin del día. La espera cuenta si se llega antes de abrir. El fin de descarga debe ser menor o igual al cierre; no se modelan ventanas nocturnas. Falta acordar cómo informar destinos sin horario registrado.

### `dimensiones_cisternas.csv` — Diego Rodriguez

Cabecera: `unidad,altura_m`

| Columna | Tipo | Regla / unidad |
| --- | --- | --- |
| unidad | texto | Clave única, referencia a `unidades.codigo`. |
| altura_m | decimal | Mayor que cero, metros. |

Si falta una dimensión, la compatibilidad de esa unidad queda sin determinar hasta que el equipo acuerde su manejo. No equivale a altura cero.

### `restricciones_altura.csv` — Diego Rodriguez

Cabecera: `origen,destino,altura_max_m`

| Columna | Tipo | Regla / unidad |
| --- | --- | --- |
| origen | texto | Referencia al extremo de una conexión existente. |
| destino | texto | El otro extremo de la misma conexión. |
| altura_max_m | decimal | Mayor que cero, metros. |

Clave: par no ordenado de extremos, sin duplicar el sentido inverso. Una fila limita ambos sentidos; una conexión sin fila se considera sin límite de altura registrado, como indica la propuesta. La altura de la cisterna debe ser menor o igual al límite. No usar un campo vacío ni cero para indicar ausencia de restricción: se omite la fila completa.

El adicional de Valentino usa los CSV base sin agregar campos ni archivos. Su plan no persiste cambios ni altera stock, reservas o estados.

## Datos versionados y datos de ejecución

- `data/examples/`: ocho CSV versionados. Los base actuales incluyen 10 puntos, 14 conexiones (dos cerradas), 4 unidades y 9 pedidos con los tres estados, superando los mínimos de la sección 3 (8 puntos, 12 conexiones con al menos una cerrada, 3 unidades y 6 pedidos). Solo modificarlos de manera coordinada para mejorar las pruebas. La incoherencia de reservas descrita arriba queda pendiente.
- `data/runtime/`: los mismos nombres y cabeceras para las copias locales. Ignorado por Git salvo `.gitkeep`. El README incluye el comando de copia inicial.
- En el flujo futuro, solo C++ escribe o actualiza estos datos; Haskell y Prolog leen después de un guardado completo. No leer mientras C++ actualiza archivos relacionados. Ejecutar desde la raíz para resolver `data/runtime/` igual en los tres lenguajes.
- Los lectores comprobarán cabeceras, cantidad de columnas, claves, referencias y tipos. Errores y coherencia entre archivos deben acordarse antes de implementar la carga. No se supone que un archivo ausente equivale a cero registros.
- No hay CSV de resultados todavía. La forma de mostrar o exportar reportes se acordará al implementar; no se versionan archivos generados.

Los estados y campos mínimos provienen del enunciado; las convenciones de representación aquí descritas son acuerdos iniciales propuestos por el scaffold. Cualquier cambio debe coordinarse y reflejarse en ejemplos y lectores de los tres lenguajes, conservando las funcionalidades comprometidas.
