# Verificación de Sprint 0

Comprobaciones realizadas en Windows durante la preparación del commit inicial. No equivalen a pruebas de las funcionalidades pendientes.

| Comprobación | Resultado real |
| --- | --- |
| Estado inicial | Repositorio vacío, rama `main`, sin commits ni archivos de trabajo previos. |
| C++ | Compilados los cinco `.cpp` con MSVC 19.12.25835 para x64, C++17 y `/W4`; código de salida 0, sin advertencias. |
| Menú C++ | Probadas entrada `0`, entrada inválida seguida de `0`, y fin de entrada. En los tres casos aparece el menú y el proceso termina con código 0. |
| Haskell | No se encontraron `ghc` ni `runghc` en el PATH ni en las ubicaciones habituales revisadas. No se ejecutó ni se compiló. |
| Prolog | No se encontró `swipl` en el PATH ni en las ubicaciones habituales revisadas. No se probó la carga en un intérprete. |
| CSV | Leídos los ocho ejemplos con el módulo estándar `csv` de Python. Verificadas cabeceras, cantidad de columnas, ausencia de BOM, claves y referencias. |
| Datos base | 8 puntos, 12 conexiones con una cerrada y sin duplicados inversos, 3 unidades y 6 pedidos con los tres estados. |
| Coherencia de ejemplos | Valores positivos, horarios dentro del día, alturas, estados/asignaciones y 4000 litros reservados coherentes con el pedido asignado. |
| Documentación | Revisados enlaces locales y presencia de los 20 números de funcionalidades base en los TODO. |

El compilador encontrado pertenece al SDK ScopeCppSDK de una instalación de Visual Studio y no estaba en el PATH. Para esta comprobación se configuraron `INCLUDE`, `LIB` y `PATH` solo dentro del proceso de compilación, con sus directorios locales de VC y Windows SDK. No se modificó la configuración del equipo. El README ofrece GCC y Developer PowerShell para instalaciones habituales; el comando GCC no se ejecutó en este entorno.

Los chequeos de CSV y del menú se ejecutaron como comprobaciones puntuales con Python del entorno de trabajo. Python no es una dependencia del proyecto. Los resultados de compilación están en `build/`, ignorados por Git. No se incorporan scripts de pruebas de algoritmos que aún no existen.

Para repetir las comprobaciones de los puntos de entrada, usar los comandos del README. En un equipo con GHC y SWI-Prolog, falta ejecutar Haskell y comprobar la carga de Prolog antes de dar ambos módulos por verificados.
