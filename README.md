# Planificador de rutas y logística: agua con cisternas

Proyecto de Paradigmas de Programación (grupo 1) para el abastecimiento de agua con camiones cisterna en Comas y Carabayllo. C++ registra y gestiona los datos, Haskell calcula rutas, costos y reportes, y Prolog responde consultas con reglas. Los tres módulos se comunican mediante archivos CSV: C++ los escribe y Haskell y Prolog los leen.

Integrantes: Carlos Doig, Nicolás Kou, Valentino Mantilla y Diego Rodríguez.

## Funcionalidades por integrante

### Funcionalidades base

| Módulo | Carlos Doig | Nicolás Kou | Valentino Mantilla | Diego Rodríguez |
| --- | --- | --- | --- | --- |
| C++ | 4.1.1 Menú y 4.1.3 Registro con validaciones | 4.1.2 Carga inicial de CSV y 4.1.5 Guardado de CSV | 4.1.4 Gestión de estados | 4.1.6 Consultas y 4.1.7 Reporte general |
| Haskell | 4.2.2 Tipos | 4.2.1 Lectura de CSV y 4.2.3 Vecinos | 4.2.4 Rutas posibles y 4.2.5 Costo de ruta | 4.2.6 Reporte de pedidos y 4.2.7 Resultado esperado |
| Prolog | 4.3.1 Lectura de CSV y 4.3.5 `puede_atender` | 4.3.2 `conectado` | 4.3.3 `alcanzable` y 4.3.6 `pedidos_inaccesibles` | 4.3.4 `ruta_evitando` |

### Funcionalidades adicionales

| Autor | Funcionalidad | Lenguajes | Dónde se prueba |
| --- | --- | --- | --- |
| Carlos Doig | Control de stock y reserva de agua en la planta | C++ | Opción 2 (asignar y entregar) y opción 4 (agua) |
| Nicolás Kou | Evaluación de entregas dentro del horario de recepción | C++, Haskell | C++ opción 5; Haskell opción 4 |
| Valentino Mantilla | Plan de recarga para un pedido de gran volumen | C++, Haskell | C++ opción 2 → 4; Haskell opción 5 |
| Diego Rodríguez | Compatibilidad de cisternas según restricciones de altura | C++, Prolog | C++ opción 3 → 6, 7 y 8; Prolog `ruta_compatible` y relacionadas |

En el código, cada parte está identificada con un comentario que indica el número de la funcionalidad o `FA` y su autor.

## Archivos

| Ruta | Contenido |
| --- | --- |
| `cpp/main.cpp` | Punto de entrada; llama al menú de Carlos. |
| `cpp/tipos.h` | Estructuras y enumeraciones compartidas. |
| `cpp/carlos.h`, `cpp/carlos.cpp` | Menú, registros, stock y reservas, y unión de los estados con el agua. |
| `cpp/nicolas.h`, `cpp/nicolas.cpp` | Carga inicial y guardado de los CSV. |
| `cpp/horarios.h`, `cpp/horarios.cpp` | Horarios de recepción y su submenú (adicional de Nicolás). |
| `cpp/valentino.h`, `cpp/valentino.cpp` | Gestión de estados y consulta del plan de recarga. |
| `cpp/diego.h`, `cpp/diego.cpp` | Consultas, reporte general y control de alturas. |
| `haskell/Main.hs` | Punto de entrada y menú del módulo Haskell. |
| `haskell/Carlos.hs` | Tipos y funciones de acceso a sus campos. |
| `haskell/Nicolas.hs` | Lectura de CSV, vecinos y evaluación de horarios. |
| `haskell/Valentino.hs` | Rutas posibles, costo de ruta y plan de recarga. |
| `haskell/Diego.hs` | Reporte de pedidos y resumen general. |
| `prolog/main.pl` | Punto de carga; deja disponibles las reglas de todos los integrantes. |
| `prolog/carlos.pl`, `prolog/nicolas.pl`, `prolog/valentino.pl`, `prolog/diego.pl` | Reglas de cada integrante. |
| `data/examples/` | Ocho CSV de prueba versionados. |
| `data/runtime/` | Datos de trabajo que escribe C++; ignorados por Git. |
| `tests/` | Pruebas automáticas de Carlos (C++ y Prolog). |

## Datos: `data/runtime` y `data/examples`

Los tres módulos siguen la misma regla para leer cada archivo: si existe en `data/runtime/`, usan ese; si no, usan la copia de `data/examples/`. C++ guarda siempre en `data/runtime/` después de cada operación que cambia datos. Así, Haskell y Prolog leen lo último que guardó C++, y un repositorio recién clonado funciona sin preparar nada.

Los datos de ejemplo tienen 10 puntos, 14 conexiones (2 cerradas), 4 cisternas y 9 pedidos, además de los archivos de las funcionalidades adicionales: `stock_agua.csv`, `horarios_recepcion.csv`, `dimensiones_cisternas.csv` y `restricciones_altura.csv`.

Para volver a los datos iniciales, borrar los CSV de trabajo (se conserva `.gitkeep`):

```powershell
Remove-Item data\runtime\*.csv -ErrorAction SilentlyContinue
```

## Preparar Windows

Se necesitan un compilador C++17, GHC y SWI-Prolog. No hay dependencias externas del proyecto.

- C++: con [MSYS2 UCRT64](https://www.msys2.org/), instalar GCC mediante `pacman -S mingw-w64-ucrt-x86_64-gcc` y añadir `C:\msys64\ucrt64\bin` al PATH. Como alternativa, usar las herramientas C++ de Visual Studio.
- Haskell: instalar GHC con la [guía de GHCup para Windows](https://www.haskell.org/ghcup/install/).
- Prolog: instalar [SWI-Prolog](https://www.swi-prolog.org/download/stable) y marcar la opción para añadirlo al PATH.

Abrir una terminal nueva después de configurar el PATH y comprobar con `g++ --version`, `ghc --version` y `swipl --version`. **Todos los comandos siguientes se ejecutan desde la raíz del repositorio.**

## C++

Compilar con GCC:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
g++ -std=c++17 -Wall -Wextra -pedantic cpp/main.cpp cpp/carlos.cpp cpp/nicolas.cpp cpp/horarios.cpp cpp/valentino.cpp cpp/diego.cpp -o build/planificador.exe
.\build\planificador.exe
```

Si el ejecutable se abre fuera de una terminal con `C:\msys64\ucrt64\bin` en el PATH y aparece un error de DLL, añadir `-static` al comando.

Con MSVC, desde Developer PowerShell for Visual Studio:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
cl /nologo /std:c++17 /EHsc /W4 cpp/main.cpp cpp/carlos.cpp cpp/nicolas.cpp cpp/horarios.cpp cpp/valentino.cpp cpp/diego.cpp /Fo:build\ /Fe:build\planificador.exe
.\build\planificador.exe
```

Al iniciar, el programa muestra lo que cargó, por ejemplo: `Datos cargados: 10 puntos, 14 conexiones, 4 cisternas y 9 pedidos.` y `Stock cargado para P01.`

| Opción | Función |
| --- | --- |
| 1 | Registrar puntos, conexiones, cisternas y pedidos, con validaciones. |
| 2 | Cerrar o habilitar conexiones, asignar y entregar pedidos con control de agua, y consultar si un pedido necesita plan de recarga. |
| 3 | Consultar puntos, conexiones y pedidos, ver el reporte general, registrar alturas y límites de altura, y consultar si una cisterna pasa por una vía. |
| 4 | Consultar el agua libre de una planta, reponer agua o iniciar el stock de una planta sin stock. |
| 5 | Registrar y consultar horarios de recepción. |
| 0 | Salir. En los submenús, volver al menú principal. |

La entrada usa `cin >>`: cada campo se escribe sin espacios (por ejemplo `Planta_Chillon`) y los decimales con punto. Los errores de validación se muestran y el menú continúa; una operación rechazada no modifica los datos. Si una lectura numérica falla o se termina la entrada, el programa termina con código 1 sin guardar.

La asignación reserva el agua del pedido en su planta y ocupa la cisterna; la entrega descuenta el stock, libera la reserva y deja disponible la cisterna. Al iniciar, el stock se carga de `stock_agua.csv` y se reconstruyen las reservas de los pedidos asignados; si no coinciden con `litros_reservados`, el programa avisa y esa planta queda sin iniciar.

Ejemplos con los datos iniciales (opción 2):

| Entrada | Resultado |
| --- | --- |
| Asignar `D002 U01` | Pedido asignado, agua reservada y cisterna ocupada. |
| Asignar `D004 U03` | Rechazado: 18000 litros superan los 15000 de U03. |
| Entregar `D003` | Pedido entregado, agua descontada y cisterna U02 libre. |
| Consultar recarga `D004 U02` | 18000 L contra 7000 L: requiere recargas (el plan se calcula en Haskell). |

## Haskell

```powershell
runghc -ihaskell haskell/Main.hs
```

También se puede compilar una vez y ejecutar sin volver a compilar:

```powershell
ghc -ihaskell -outputdir build haskell/Main.hs -o build/planificador_hs.exe
.\build\planificador_hs.exe
```

Al compilar aparecen dos avisos sobre `head` y `tail` en `Nicolas.hs`; no afectan el funcionamiento.

| Opción | Función | Ejemplo con los datos iniciales |
| --- | --- | --- |
| 1 | Vecinos de un punto (4.2.3) | `P03` → P06, P07, P04, P05 |
| 2 | Rutas posibles y su costo (4.2.4 y 4.2.5) | `P01` a `P05` → 12 rutas; la más corta, 11.50 km |
| 3 | Reporte de pedidos (4.2.6) | Pedidos por estado, carga por cisterna y pendientes por prioridad |
| 4 | Entregas dentro del horario (adicional de Nicolás) | `D001` saliendo a las `07:00` → llegada, espera y fin de descarga por ruta |
| 5 | Plan de recarga (adicional de Valentino) | `D004` con `U02` → 3 viajes (7000, 7000 y 4000 L), 25.00 km, S/ 75.00 |
| 6 | Resumen general (4.2.7) | Ruta más corta y costo de cada pedido pendiente |
| 0 | Salir | |

## Prolog

```powershell
swipl -q -s prolog/main.pl
```

Los códigos van entre comillas simples porque empiezan con mayúscula; sin comillas, Prolog los tomaría como variables. Cada consulta termina en punto. Si una consulta muestra una respuesta y espera, `;` muestra la siguiente y Enter termina. Para salir: `halt.`

Ejemplos con los datos iniciales:

```prolog
?- conectado('P01', 'P06', D).
% 4.3.2 (Nicolás). D = 4.0

?- alcanzable('P01', 'P06').
% 4.3.3 (Valentino). true

?- alcanzable('P01', 'P10').
% false: la única vía a P10 está cerrada.

?- ruta_evitando('P01', 'P09', 'P07', Ruta).
% 4.3.4 (Diego). Ruta = ['P01', 'P08', 'P09']

?- puede_atender('U01', 'D001').
% 4.3.5 (Carlos). true

?- puede_atender('U02', 'D001').
% false: U02 está ocupada.

?- pedidos_inaccesibles(P).
% 4.3.6 (Valentino). P = ['D006']

?- ruta_compatible('U01', 'P01', 'P03', Ruta).
% Adicional de Diego. Ruta = ['P01', 'P06', 'P03']

?- ruta_compatible('U03', 'P01', 'P03', Ruta).
% false: U03 (3.8 m) no pasa por ninguna vía hacia P03.

?- cisternas_compatibles(['P01', 'P06', 'P03'], Unidades).
% Unidades = ['U01', 'U02', 'U04']

?- restriccion_que_impide('U03', ['P01', 'P06', 'P03'], A, B, Max).
% A = 'P06', B = 'P03', Max = 3.6
```

Cada regla tiene una versión que lee los CSV en cada consulta (por ejemplo `alcanzable/2`) y otra que recibe las listas ya leídas (`alcanzable/3`). No se usa una base de hechos dinámica: las listas contienen términos `punto`, `conexion`, `unidad` y `pedido`, que las reglas recorren con `member`.

## Prueba de integración

Comprueba que un cambio hecho en C++ lo leen Haskell y Prolog. Partir de los datos iniciales.

1. En C++, opción 2 → 1: cerrar `P01 P08 cerrada` y luego `P07 P09 cerrada`. Salir con 0 para guardar.
2. En Prolog: `pedidos_inaccesibles(P).` ahora da `P = ['D004', 'D006']`, y `alcanzable('P01', 'P08').` da `false`, porque P08 quedó aislado.
3. En Haskell, opción 5 con `D004` y `U02`: "No hay ruta habilitada entre la planta y el destino".

## Pruebas automáticas de Carlos

Estas pruebas usan datos aislados en memoria y no leen ni modifican los CSV. Crear antes la carpeta `build/`.

Registros:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic tests/registro_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp cpp/nicolas.cpp cpp/horarios.cpp -o build/pruebas_registro.exe
.\build\pruebas_registro.exe
```

Stock y reservas:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic tests/stock_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp cpp/nicolas.cpp cpp/horarios.cpp -o build/pruebas_stock.exe
.\build\pruebas_stock.exe
```

Estados y agua:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic tests/estados_agua_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp cpp/nicolas.cpp cpp/horarios.cpp -o build/pruebas_estados_agua.exe
.\build\pruebas_estados_agua.exe
```

Con MSVC, reemplazar `g++ ... -o build/X.exe` por `cl /nologo /std:c++17 /EHsc /W4 ... /Fo:build\ /Fe:build\X.exe`.

Prolog:

```powershell
swipl -q -s tests/atencion_carlos.pl -g pruebas_atencion -t halt
swipl -q -s tests/lectura_carlos.pl -g pruebas_lectura -t halt
```

Los archivos de `tests/datos_menu/` son recorridos del menú creados cuando C++ todavía empezaba sin datos. Ahora el programa carga los CSV al iniciar y guarda en `data/runtime/`, así que sus resultados ya no coinciden con los descritos originalmente.
