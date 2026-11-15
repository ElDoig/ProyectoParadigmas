# Planificador de rutas y logística: agua con cisternas

Proyecto de Paradigmas de Programación para el abastecimiento de agua en Comas y Carabayllo. C++ gestiona los datos, Haskell realiza cálculos y Prolog responde consultas. Los módulos se comunicarán mediante CSV.

## Estado actual

- C++: menu de Carlos (4.1.1) con registro por consola de puntos, conexiones, cisternas y pedidos (4.1.3), inicio de stock por planta, consulta de agua libre y reposiciones. La opcion 2 integra los estados de Valentino con las reservas y entregas de agua de Carlos.
- C++: opcion 3 conectada al submenu de Diego: consultas de puntos, conexiones y pedidos, reporte general, registro de alturas y limites de vias, y consulta de paso por altura.
- Haskell: tipos de conexiones, unidades y pedidos (4.2.2 de Carlos), más un punto de entrada mínimo.
- Prolog: lectura de los cuatro CSV base (4.3.1 de Carlos) y regla de atencion por disponibilidad y capacidad (4.3.5). Se puede consultar con codigos o proporcionar listas de datos.
- La carga y el guardado CSV en C++, los calculos de Haskell y el adicional de Nicolas siguen pendientes de integracion en esta rama. La sesion C++ empieza vacia y sus datos no se conservan al salir. La consulta C++ de recargas solo indica si el pedido supera la capacidad; no calcula viajes, distancia ni costo.

## Archivos

| Ruta | Contenido |
| --- | --- |
| `cpp/main.cpp` | Punto de entrada; llama al menu de Carlos. |
| `cpp/tipos.h` | Estructuras y enumeraciones compartidas. |
| `cpp/carlos.h`, `cpp/carlos.cpp` | Menu, registros y adicional de stock y reservas. |
| `cpp/diego.h`, `cpp/diego.cpp` | Consultas, reporte general y control de alturas de Diego. |
| `cpp/valentino.h`, `cpp/valentino.cpp` | Funciones de estados y consulta de recargas de Valentino. El menu de Carlos coordina los estados con el agua. |
| `cpp/nicolas.cpp` | Archivo reservado para Nicolas. |
| `haskell/Main.hs` | Punto de entrada de Haskell. |
| `haskell/Carlos.hs` | Tipos y funciones de acceso a sus campos. |
| `haskell/Nicolas.hs`, `haskell/Valentino.hs`, `haskell/Diego.hs` | Archivos de los demás integrantes. |
| `prolog/main.pl` y archivos por integrante | Punto de carga y predicados; `carlos.pl` lee los CSV y define `puede_atender/2` y `puede_atender/4`. |
| `data/examples/` | Ocho CSV de prueba versionados. |
| `data/runtime/` | Datos locales de ejecución, ignorados por Git. |
| `tests/registro_carlos.cpp` | Pruebas de las funciones de registro. |
| `tests/stock_carlos.cpp` | Pruebas de stock, reservas, entregas y reposiciones. |
| `tests/estados_agua_carlos.cpp` | Pruebas de asignacion y entrega coordinadas con el agua. |
| `tests/datos_menu/recorrido.txt` | Entradas para repetir un recorrido del menu sin modificar archivos CSV. |
| `tests/datos_menu/consultas_diego.txt` | Recorrido de registros, consultas, reporte y alturas desde la opcion 3. |
| `tests/datos_menu/estados_agua.txt` | Recorrido de asignacion, reserva, entrega y reposicion desde el menu. |
| `tests/atencion_carlos.pl` | Datos aislados y pruebas de la regla de atencion. |
| `tests/lectura_carlos.pl`, `tests/datos_csv/` | Pruebas de lectura y archivos pequenos para comprobar cabeceras. |

## Preparar Windows

Se necesitan un compilador C++17, GHC y SWI-Prolog. No hay dependencias externas del proyecto.

- C++: con [MSYS2 UCRT64](https://www.msys2.org/), instalar GCC mediante `pacman -S mingw-w64-ucrt-x86_64-gcc`. Para usarlo desde PowerShell, añadir `C:\msys64\ucrt64\bin` al PATH si se usó la instalación predeterminada. Como alternativa, usar las herramientas C++ de Visual Studio.
- Haskell: instalar GHC siguiendo la [guía de GHCup para Windows](https://www.haskell.org/ghcup/install/).
- Prolog: instalar [SWI-Prolog](https://www.swi-prolog.org/download/stable) y habilitar su acceso desde el PATH.

Abrir una terminal nueva después de configurar el PATH. Todos los comandos siguientes se ejecutan desde la raíz del repositorio en PowerShell.

## Compilar y ejecutar C++

Con GCC:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
g++ -std=c++17 -Wall -Wextra -pedantic cpp/main.cpp cpp/carlos.cpp cpp/nicolas.cpp cpp/valentino.cpp cpp/diego.cpp -o build/planificador.exe
.\build\planificador.exe
```

Con MSVC, desde Developer PowerShell for Visual Studio:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
cl /nologo /std:c++17 /EHsc /W4 cpp/main.cpp cpp/carlos.cpp cpp/nicolas.cpp cpp/valentino.cpp cpp/diego.cpp /Fo:build\ /Fe:build\planificador.exe
.\build\planificador.exe
```

## Usar el menu de Carlos

| Opcion principal | Funcion |
| --- | --- |
| 1 | Registrar puntos, conexiones, cisternas y pedidos mediante un submenu. |
| 2 | Cerrar o habilitar conexiones, asignar y entregar pedidos con control de agua, y consultar necesidad de recargas. |
| 3 | Submenu de Diego: consultas, reporte general, alturas de cisternas y limites de vias. |
| 4 | Iniciar stock de una planta registrada, consultar agua libre y reponer agua. |
| 0 | Salir. En los submenus, volver al menu principal. |

La entrada usa `cin >>`: escribir cada campo sin espacios, por ejemplo `Planta_Chillon`, y los decimales con punto. El programa indica el orden de los campos. Las cisternas nuevas empiezan disponibles y los pedidos pendientes sin unidad; los errores de validacion se muestran y permiten seguir en el menu.

Si una extraccion numerica falla o se termina la entrada, el programa avisa y termina con codigo 1, sin guardar. La salida con `0` devuelve codigo 0. La recuperacion de entradas numericas mal formadas queda pendiente; no se ha añadido un analizador de texto.

Hay espacio de trabajo para 100 puntos, conexiones, cisternas, pedidos, alturas y restricciones, y control de agua de 10 plantas con hasta 100 reservas por planta. Son capacidades de los arreglos de esta sesion, no nuevos limites de los CSV. El control de stock solo se puede iniciar una vez por planta y empieza sin reservas porque esta sesion aun no carga pedidos asignados. La integracion futura debe cargar el stock y preparar sus reservas, sin reemplazarlos con un inicio nuevo.

Para asignar un pedido desde la opcion 2, primero inicia el stock de su planta de origen en la opcion 4. La asignacion reserva agua y ocupa la cisterna; la entrega descuenta el stock, libera la reserva y deja disponible la cisterna. Una operacion rechazada conserva los datos. Las reservas y entregas no se ejecutan por separado. El submenu de Diego comparte los registros y estados; sus alturas y restricciones se conservan al volver al menu principal, hasta salir del programa.

Para probar la asignacion y entrega completas despues de compilar:

```powershell
Get-Content -LiteralPath tests/datos_menu/estados_agua.txt | .\build\planificador.exe
```

Se inicia un stock de 12000 litros y se asigna un pedido de 6000: quedan 6000 reservados y 6000 libres, sin reducir el stock. Al entregar quedan 6000 de stock y cero reservados; al reponer 5000 quedan 11000 libres. La opcion 3 permite ver el pedido asignado y luego entregado, con la cisterna disponible al final.

Para probar la opcion 3, registra primero los puntos, una conexion, una cisterna y un pedido desde la opcion 1. Luego entra a 3: las opciones 1 a 5 consultan esos datos y muestran el reporte; 6 registra la altura de una cisterna, 7 el limite de una conexion y 8 compara ambas alturas. Usa 0 para volver al menu principal.

Tambien puedes repetir ese recorrido automaticamente:

```powershell
Get-Content -LiteralPath tests/datos_menu/consultas_diego.txt | .\build\planificador.exe
```

Debe mostrar 2 puntos, 1 conexion, 1 cisterna y 1 pedido pendiente de 6000 litros. La cisterna de 3.5 metros no pasa por el limite de 3.2 metros; al volver a entrar al submenu, la misma restriccion se conserva y se aplica en el sentido inverso. La integracion se comprobo con 6 escenarios de consola, incluidos duplicados, entrada invalida y el recorrido anterior de agua.

Para repetir un ejemplo completo despues de compilar:

```powershell
Get-Content -LiteralPath tests/datos_menu/recorrido.txt | .\build\planificador.exe
```

Registra dos puntos, una conexion, una cisterna y un pedido; inicia 12000 litros para la planta y repone 5000 con un tope de prueba de 20000. Debe mostrar 17000 litros libres al final y salir normalmente. No lee ni modifica los CSV. Se comprobaron 10 escenarios de consola con el ejecutable MSVC, incluidos errores y limites de los arreglos.

## Probar los registros de Carlos

Con GCC:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic tests/registro_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp -o build/pruebas_registro.exe
.\build\pruebas_registro.exe
```

Alternativa MSVC:

```powershell
cl /nologo /std:c++17 /EHsc /W4 tests/registro_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp /Fo:build\ /Fe:build\pruebas_registro.exe
.\build\pruebas_registro.exe
```

Crear primero `build/` con el comando anterior. Resultado esperado: `51 pruebas, 0 fallos.` Las pruebas usan datos aislados en memoria y no modifican los CSV.

Para llamar a las funciones desde otro `.cpp`, incluir `tipos.h` y luego `carlos.h`, una sola vez cada uno. Por ejemplo:

```cpp
#include "tipos.h"
#include "carlos.h"

// Dentro de una funcion:
Punto puntos[10];
int cantidadPuntos = 0;
Punto planta = {"P01", "Planta Chillon", TipoPunto::planta, "carabayllo"};
std::string error = registrarPunto(planta, puntos, &cantidadPuntos, 10);
```

Si `error == ""`, se agregó el registro y aumentó el contador. Si hay un error, devuelve su explicación sin modificar los datos. El llamador debe proporcionar la capacidad real del arreglo. Las unidades nuevas se registran disponibles y los pedidos pendientes sin unidad; asignar o entregar corresponde a la gestión de estados. La carga de registros históricos desde CSV es una operación distinta.

Las funciones de registro reciben textos y numeros ya preparados. El menu captura esos datos y llama a las mismas funciones probadas. La validacion completa de los caracteres de cada campo CSV y del formato del texto numerico sigue pendiente; `cin >>` realiza la extraccion basica autorizada para esta etapa.

## Stock y reservas de Carlos

`StockAgua` conserva los tres campos de `stock_agua.csv`. `ReservaAgua` identifica el pedido, la planta y los litros; su marca `entregada` permite rechazar una segunda entrega. Este detalle se mantiene en un arreglo en memoria, sin agregar otro CSV. Se utiliza un arreglo separado por planta y las entregadas se conservan durante la sesion.

| Funcion | Uso |
| --- | --- |
| `consultarAguaLibre(stock, &libres)` | Devuelve stock menos reservas; valida primero el saldo. |
| `prepararReservasIniciales(stock, pedidos, cantidadPedidos, reservas, &cantidad, capacidad)` | Prepara las reservas de pedidos asignados al iniciar; rechaza diferencias con el total del stock sin corregir cifras. El arreglo debe estar vacio. |
| `reservarAgua(pedido, &stock, reservas, &cantidad, capacidad)` | Reserva un pedido pendiente sin unidad, si hay litros libres y espacio; impide duplicados. |
| `registrarEntregaAgua(pedido, &stock, reservas, cantidad)` | Descuenta el volumen reservado de un pedido asignado y libera su reserva una sola vez. |
| `reponerAgua(&stock, litrosRecibidos, topeStock)` | Agrega litros positivos sin superar el tope indicado por el llamador. |

El resultado sigue siendo `""` en caso de exito o un mensaje de error. Antes de una reserva o entrega, la suma de las reservas activas debe coincidir con `litros_reservados`. Si hay un rechazo, no se modifican el saldo, las reservas ni su contador. El llamador debe indicar las cantidades y capacidades reales de sus arreglos.

La opcion 2 llama a `asignarPedidoConAgua` y `entregarPedidoConAgua`, declaradas en `carlos.h`. Ambas reutilizan las validaciones y cambios de estado de Valentino. Primero preparan los estados en copias del pedido y la cisterna; despues operan sobre el agua y, solo si sale bien, aplican las copias. Asi un rechazo no deja estados cambiados sin su reserva o descuento. Las funciones originales de Valentino se conservan; su submenu independiente no se usa en el flujo integrado porque no recibe el stock. Nicolas conectara despues la carga y el guardado de todos los datos relacionados; no guardar cambios intermedios.

El contrato no fija capacidad maxima para la planta. `topeStock` es un limite de trabajo recibido como parametro, no una nueva columna ni una cifra impuesta a los datos. Comprobar `litrosRecibidos <= topeStock - stockActual` antes de sumar evita el desbordamiento usando operaciones basicas. El tope debe ser representable como `int` y no menor que el stock actual.

Con GCC, despues de crear `build/`:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic tests/stock_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp -o build/pruebas_stock.exe
.\build\pruebas_stock.exe
```

Alternativa MSVC:

```powershell
cl /nologo /std:c++17 /EHsc /W4 tests/stock_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp /Fo:build\ /Fe:build\pruebas_stock.exe
.\build\pruebas_stock.exe
```

Resultado obtenido con MSVC: `80 pruebas de stock, 0 fallos.` Se usan estados aislados en memoria; no se escriben archivos.

Para comprobar la integracion entre estados y agua con GCC:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic tests/estados_agua_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp -o build/pruebas_estados_agua.exe
.\build\pruebas_estados_agua.exe
```

Alternativa MSVC:

```powershell
cl /nologo /std:c++17 /EHsc /W4 tests/estados_agua_carlos.cpp cpp/carlos.cpp cpp/diego.cpp cpp/valentino.cpp /Fo:build\ /Fe:build\pruebas_estados_agua.exe
.\build\pruebas_estados_agua.exe
```

Resultado obtenido con MSVC: `26 pruebas de estados y agua, 0 fallos.` Comprueban rechazos sin cambios, capacidad y agua insuficientes, reservas, entregas, duplicados, reposicion y reutilizacion de la cisterna. Tambien se comprobaron 11 recorridos del menu, incluidos el de Diego, la falta de stock, dos plantas independientes, cambios de conexiones, consulta de recargas y fin de entrada. El programa y las tres pruebas C++ compilaron con `/W4 /WX`, sin advertencias.

**Dato pendiente de reconciliar:** el CSV actual tiene 12000 litros de stock y 4000 reservados, pero los pedidos asignados D003 y D005 suman 12000. La preparacion inicial rechaza esta diferencia. Si las asignaciones reflejan la realidad, el total reservado tendria que coincidir con sus 12000 litros; si los 4000 son correctos, deben revisarse los pedidos. El equipo debe confirmar la fuente correcta: no se modificaron los CSV automaticamente.

## Ejecutar Haskell

```powershell
runghc -ihaskell haskell/Main.hs
```

Debe mostrar el mensaje de inicio. No se ha podido compilar ni ejecutar Haskell en el entorno de trabajo actual porque no se encontró GHC.

## Cargar Prolog

```powershell
swipl -q -s prolog/main.pl
```

Para salir del intérprete, escribir `halt.`. Para cargar y salir directamente:

```powershell
swipl -q -s prolog/main.pl -g halt
```

Preparar primero los CSV en `data/runtime/` con el comando de la ultima seccion. Luego se pueden ejecutar estas consultas:

```prolog
?- leer_datos(Puntos, Conexiones, Unidades, Pedidos).
% Devuelve cuatro listas de registros, sin las cabeceras.

?- puede_atender('U01', 'D001').
% Esperado con las copias de los ejemplos: true.

?- puede_atender('U02', 'D001').
% Esperado: false, porque U02 esta ocupada.

?- puede_atender('U01', 'D004').
% Esperado: false, porque D004 pide 18000 litros y U01 tiene capacidad de 10000.
```

Los codigos en mayusculas se escriben entre comillas simples para que Prolog los trate como atomos y no como variables. La consulta de dos argumentos lee los archivos en cada llamada y reutiliza `puede_atender(CodigoUnidad, CodigoPedido, Unidades, Pedidos)`. Esta regla exige disponibilidad y capacidad suficiente, incluida la igualdad; no asigna pedidos ni modifica datos.

Los terminos `unidad` y `pedido` conservan el orden de los campos CSV, indicado en los comentarios de `carlos.pl`. Para probar con los datos aislados del archivo de pruebas, abrir:

```powershell
swipl -q -s tests/atencion_carlos.pl
```

Luego ejecutar en el interprete:

```prolog
?- pruebas_atencion.
% Esperado: true.

?- datos_atencion(Unidades, Pedidos), puede_atender(grande, normal, Unidades, Pedidos).
% Esperado: true; capacidad suficiente.

?- datos_atencion(Unidades, Pedidos), puede_atender(justa, normal, Unidades, Pedidos).
% Esperado: true; capacidad exactamente igual.

?- datos_atencion(Unidades, Pedidos), puede_atender(pequena, normal, Unidades, Pedidos).
% Esperado: false; capacidad insuficiente.

?- datos_atencion(Unidades, Pedidos), puede_atender(ocupada, normal, Unidades, Pedidos).
% Esperado: false; unidad ocupada.
```

Si el interprete pide elegir otra solucion, pulsar Enter para terminar esa consulta. La prueba completa tambien comprueba pedidos demasiado grandes, codigos inexistentes y listas vacias. Para ejecutarla sin interaccion:

```powershell
swipl -q -s tests/atencion_carlos.pl -g pruebas_atencion -t halt
```

Para probar la lectura de los cuatro archivos de ejemplo, los codigos, los numeros, los campos vacios y las cabeceras:

```powershell
swipl -q -s tests/lectura_carlos.pl -g pruebas_lectura -t halt
```

La lectura usa [library(csv), incluida en SWI-Prolog](https://www.swi-prolog.org/pldoc/man?section=csv), autorizada por el equipo. Comprueba las cabeceras y la cantidad de columnas; convierte los numeros y conserva los codigos. Una cabecera incorrecta hace fallar la lectura. Un archivo ausente o una fila con columnas incorrectas produce un error del lector. La validacion completa de registros corresponde al registro en C++; estos lectores no repiten esas validaciones ni reparan datos.

No se usa una base de hechos dinamica: las listas contienen terminos `punto`, `conexion`, `unidad` y `pedido`, que las reglas pueden consultar con `member`. No se ha podido ejecutar Prolog en el entorno de trabajo porque no se encontro SWI-Prolog: los resultados anteriores son los esperados, no resultados medidos.

## CSV de prueba y ejecución

Los cuatro archivos base son `puntos.csv`, `conexiones.csv`, `unidades.csv` y `pedidos.csv`. Los adicionales son `stock_agua.csv`, `horarios_recepcion.csv`, `dimensiones_cisternas.csv` y `restricciones_altura.csv`. Sus cabeceras actuales definen los nombres que utilizará el código. Los datos base incluyen 10 puntos, 14 conexiones, 4 cisternas y 9 pedidos.

Para crear las copias locales iniciales:

```powershell
Copy-Item -LiteralPath (Get-ChildItem -LiteralPath data/examples -Filter *.csv).FullName -Destination data/runtime
```

El comando reemplaza las copias locales existentes. No usarlo si se necesitan conservar esos datos. En el flujo integrado, C++ escribira en `data/runtime/` y Haskell y Prolog leeran despues de cada guardado completo. Prolog ya puede leer esas copias; la persistencia C++ y la lectura Haskell siguen pendientes.
