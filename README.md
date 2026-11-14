# Planificador de rutas y logística: agua con cisternas

Proyecto de Paradigmas de Programación para el abastecimiento de agua en Comas y Carabayllo. C++ gestiona los datos, Haskell realiza cálculos y Prolog responde consultas. Los módulos se comunicarán mediante CSV.

## Estado actual

- C++: menú inicial con salida y funciones de registro en memoria de puntos, conexiones, cisternas y pedidos (4.1.3 de Carlos).
- Haskell: tipos de conexiones, unidades y pedidos (4.2.2 de Carlos), más un punto de entrada mínimo.
- Prolog: lectura de los cuatro CSV base (4.3.1 de Carlos) y regla de atencion por disponibilidad y capacidad (4.3.5). Se puede consultar con codigos o proporcionar listas de datos.
- La captura de registros desde el menú, la carga y el guardado CSV en C++, los cálculos, la gestión de estados y los adicionales siguen pendientes. Los registros C++ de esta etapa se prueban llamando a las funciones; todavía no se ingresan por consola ni se conservan al cerrar el programa.

## Archivos

| Ruta | Contenido |
| --- | --- |
| `cpp/main.cpp` | Punto de entrada y menú inicial. |
| `cpp/tipos.h` | Estructuras y enumeraciones compartidas. |
| `cpp/carlos.h`, `cpp/carlos.cpp` | Declaraciones e implementación del registro en memoria. |
| `cpp/nicolas.cpp`, `cpp/valentino.cpp`, `cpp/diego.cpp` | Archivos reservados para los demás integrantes. |
| `haskell/Main.hs` | Punto de entrada de Haskell. |
| `haskell/Carlos.hs` | Tipos y funciones de acceso a sus campos. |
| `haskell/Nicolas.hs`, `haskell/Valentino.hs`, `haskell/Diego.hs` | Archivos de los demás integrantes. |
| `prolog/main.pl` y archivos por integrante | Punto de carga y predicados; `carlos.pl` lee los CSV y define `puede_atender/2` y `puede_atender/4`. |
| `data/examples/` | Ocho CSV de prueba versionados. |
| `data/runtime/` | Datos locales de ejecución, ignorados por Git. |
| `tests/registro_carlos.cpp` | Pruebas de las funciones de registro. |
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

El menú actual permite salir con `0`.

## Probar los registros de Carlos

Con GCC:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic tests/registro_carlos.cpp cpp/carlos.cpp -o build/pruebas_registro.exe
.\build\pruebas_registro.exe
```

Alternativa MSVC:

```powershell
cl /nologo /std:c++17 /EHsc /W4 tests/registro_carlos.cpp cpp/carlos.cpp /Fo:build\ /Fe:build\pruebas_registro.exe
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

Las funciones reciben textos y números ya preparados. No incluyen lectura de consola, análisis del texto numérico ni revisión de los caracteres de cada campo CSV. Esas comprobaciones deben conectarse antes de usar el registro desde una entrada externa.

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
