# Planificador de rutas y logística: abastecimiento de agua con cisternas

Proyecto del curso Paradigmas de Programación. Modela una red simplificada de Comas y Carabayllo para distribuir agua desde una planta a colegios, mercados y asentamientos humanos. Los lugares, distancias y tiempos de los CSV son datos de prueba, no rutas verificadas para operar un servicio real.

## Estado: base inicial y tipos compartidos de Carlos

Esta entrega prepara la estructura para trabajar entre cuatro personas. C++ muestra un menú y permite salir; Haskell imprime un mensaje de inicio; Prolog tiene un punto de carga. Carlos ya definió las estructuras compartidas C++ en `cpp/tipos.h` y los tipos Haskell de 4.2.2 en `haskell/Carlos.hs`. Su uso y las decisiones de integración están en [TIPOS_COMPARTIDOS.md](docs/TIPOS_COMPARTIDOS.md). Los demás trabajos siguen señalados con TODO.

Quedan pendientes todos los registros, lecturas y escrituras CSV, validaciones de negocio, consultas, algoritmos, reportes y adicionales. El menú completo 4.1.1 también está pendiente. Tener CSV de ejemplo no significa que los programas ya los procesen.

## Papel de cada lenguaje

| Lenguaje | Responsabilidad futura |
| --- | --- |
| C++ | Registrar y validar información, gestionar estados y escribir los CSV. Es el único escritor de datos operativos. |
| Haskell | Leer CSV, definir tipos y calcular rutas, distancias, tiempos, costos y reportes con funciones puras. |
| Prolog | Leer CSV y construir hechos y reglas de conectividad, atención de pedidos y compatibilidad por altura. |

No se necesitan frameworks, bases de datos, Docker ni paquetes externos para los puntos de entrada.

## Estructura y archivos

```text
ProyectoParadigmas/
  cpp/             tipos.h, main.cpp, carlos.cpp, nicolas.cpp, valentino.cpp, diego.cpp
  haskell/         Main.hs, Carlos.hs, Nicolas.hs, Valentino.hs, Diego.hs
  prolog/          main.pl, carlos.pl, nicolas.pl, valentino.pl, diego.pl
  data/
    examples/      ocho CSV de prueba versionados
    runtime/       .gitkeep; copias locales y datos operativos ignorados por Git
  docs/
    ARQUITECTURA.md
    CONTRATO_CSV.md
    TIPOS_COMPARTIDOS.md
    VERIFICACION.md
  .gitattributes
  .gitignore
  README.md
```

Los tres `main` son los puntos de entrada e integración. Los otros cuatro archivos de cada lenguaje reservan el trabajo de cada integrante; no hace falta crear una carpeta por funcionalidad. En Haskell y Prolog, el punto de entrada ya carga los módulos; `Carlos.hs` exporta los tipos compartidos. Los archivos C++ de integrantes se incluyen en la compilación y pueden incluir `tipos.h` para usar las mismas estructuras.

`data/examples/` contiene `puntos.csv`, `conexiones.csv`, `unidades.csv`, `pedidos.csv`, `stock_agua.csv`, `horarios_recepcion.csv`, `dimensiones_cisternas.csv` y `restricciones_altura.csv`. Sus columnas están en [el contrato CSV](docs/CONTRATO_CSV.md). `.gitkeep` conserva la carpeta vacía; `.gitignore` excluye datos locales y compilados; `.gitattributes` normaliza saltos de línea para colaborar desde Windows.

[ARQUITECTURA.md](docs/ARQUITECTURA.md) describe los acuerdos de integración y pendientes. [VERIFICACION.md](docs/VERIFICACION.md) registra qué se comprobó realmente en Sprint 0.

## Preparar Windows

1. Instala [GitHub Desktop](https://desktop.github.com/) e inicia sesión con tu cuenta. En **File > Clone repository**, selecciona `ElDoig/ProyectoParadigmas` y una carpeta local.
2. Para C++, puedes instalar [MSYS2](https://www.msys2.org/). Abre **MSYS2 UCRT64** y ejecuta `pacman -S mingw-w64-ucrt-x86_64-gcc`. Para usarlo desde PowerShell, añade `C:\msys64\ucrt64\bin` al PATH de usuario si instalaste en la ruta predeterminada. También puedes usar MSVC si ya tienes las herramientas C++ de Visual Studio.
3. Para Haskell, sigue la [instalación oficial de GHCup para Windows](https://www.haskell.org/ghcup/install/) e instala GHC. Para este scaffold basta GHC con `runghc`; no se usa Stack ni un proyecto Cabal.
4. Instala la versión estable de [SWI-Prolog para Windows](https://www.swi-prolog.org/download/stable). Habilita su opción de PATH; si no está disponible, añade la carpeta `bin` de la instalación al PATH de usuario.
5. Abre una terminal nueva después de cambiar el PATH. Usa tu editor preferido, por ejemplo VS Code.

En GitHub Desktop, **Repository > Open in ...** abre la terminal configurada. Los comandos siguientes son para **PowerShell**, desde la carpeta raíz del repositorio. Si abre otra terminal, abre PowerShell en esa carpeta.

Comprueba las herramientas:

```powershell
git --version
g++ --version
runghc --version
swipl --version
```

Si un comando no se reconoce, revisa su instalación y el PATH antes de seguir. No es necesario instalar herramientas para lenguajes distintos de los que vayas a probar en ese momento.

## Compilar y ejecutar

### C++ (C++17)

Con GCC en PowerShell:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
g++ -std=c++17 -Wall -Wextra -pedantic cpp/main.cpp cpp/carlos.cpp cpp/nicolas.cpp cpp/valentino.cpp cpp/diego.cpp -o build/planificador.exe
.\build\planificador.exe
```

Debe aparecer el menú inicial. Escribe `0` y pulsa Enter para salir. Cualquier otra entrada vuelve a mostrar el menú.

Alternativa: desde **Developer PowerShell for Visual Studio**, con las herramientas C++ instaladas:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
cl /nologo /std:c++17 /EHsc /W4 cpp/main.cpp cpp/carlos.cpp cpp/nicolas.cpp cpp/valentino.cpp cpp/diego.cpp /Fo:build\ /Fe:build\planificador.exe
.\build\planificador.exe
```

### Haskell

```powershell
runghc -ihaskell haskell/Main.hs
```

Debe imprimir `Modulo Haskell iniciado. Calculos pendientes de implementar.` y terminar. La opción `-ihaskell` permite encontrar los cuatro módulos.

### Prolog

```powershell
swipl -q -s prolog/main.pl
```

Debe aparecer el intérprete sin errores de carga. Para salir, escribe `halt.` (con punto). Todavía no existen consultas de negocio. Para comprobar la carga y salir en una sola ejecución:

```powershell
swipl -q -s prolog/main.pl -g halt
```

## Reparto oficial: dónde programar

Cada integrante trabaja principalmente en su archivo de cada lenguaje. Conservamos la distribución del Anexo A:

| Integrante / archivos | C++ | Haskell | Prolog | Adicional |
| --- | --- | --- | --- | --- |
| Carlos Doig: `carlos.cpp`, `Carlos.hs`, `carlos.pl` | 4.1.1 menú; 4.1.3 registro con validaciones | 4.2.2 tipos | 4.3.1 lectura CSV; 4.3.5 atención de pedidos | Stock y reservas en C++. |
| Nicolás Kou: `nicolas.cpp`, `Nicolas.hs`, `nicolas.pl` | 4.1.2 carga inicial; 4.1.5 guardado CSV | 4.2.1 lectura CSV; 4.2.3 vecinos | 4.3.2 conexión directa | Registro de horarios en C++; evaluación de llegada, espera y descarga en Haskell. |
| Valentino Mantilla: `valentino.cpp`, `Valentino.hs`, `valentino.pl` | 4.1.4 gestión de estados | 4.2.4 rutas; 4.2.5 distancia y tiempo de ruta | 4.3.3 alcanzabilidad; 4.3.6 pedidos inaccesibles | Consulta en C++ y simulación del plan de recarga en Haskell. |
| Diego Rodriguez: `diego.cpp`, `Diego.hs`, `diego.pl` | 4.1.6 consultas; 4.1.7 reporte general | 4.2.6 reporte de pedidos; 4.2.7 resumen de resultados | 4.3.4 ruta evitando un punto | Registro de alturas en C++; compatibilidad de rutas en Prolog. |

Los comentarios TODO identifican número y responsable. El menú completo pertenece a Carlos; la salida de Haskell 4.2.7, a Diego. Coordinen sus cambios en los puntos de entrada con quienes integren otras funciones. No agreguen nuevos `main` para las funcionalidades individuales.

## Archivos compartidos y comunicación CSV

Los tres `main`, documentos, ejemplos y configuración Git requieren coordinación. También serán compartidos los tipos Haskell de Carlos y los futuros tipos/interfaces C++ y hechos Prolog. Antes de cambiar parámetros, columnas o predicados, acuerden el cambio y actualicen código consumidor y documentación en el mismo PR.

Para preparar datos locales de prueba, ejecuta una vez:

```powershell
Copy-Item -LiteralPath (Get-ChildItem -LiteralPath data/examples -Filter *.csv).FullName -Destination data/runtime
```

Este comando reemplaza las copias locales si ya existen: úsalo para iniciar o restablecer una prueba, conservando antes cualquier dato que necesites. No modifica los ejemplos versionados.

El flujo futuro será: C++ escribe en `data/runtime/`; al terminar el guardado, Haskell y Prolog leen esos mismos archivos. Ejecuten los programas desde la raíz. En Sprint 0 la copia es manual y ninguno de los programas abre aún los CSV. No editen ejemplos para registrar operaciones del día a día. Los lectores deberán recargar después de cambios; no se implementa sincronización automática.

## Trabajar con GitHub Desktop y Pull Requests

1. En **Current branch**, cambia a `main` y pulsa **Fetch origin**. Si aparece **Pull origin**, úsalo para traer lo integrado. Hazlo con tus cambios guardados en un commit y sin trabajo pendiente en la rama.
2. En **Current branch > New branch**, crea tu rama desde `main`: `feature/carlos-registro`, `feature/nicolas-carga`, `feature/valentino-estados` o `feature/diego-consultas`, según corresponda. Son ejemplos: cada persona crea su rama cuando comienza una tarea. `main` debe mantenerse estable.
3. Programa en tus archivos. Revisa la pestaña **Changes**: no deben aparecer ejecutables ni datos de `runtime`. Compila o ejecuta lo que cambiaste.
4. Escribe un resumen claro y pulsa **Commit to ...**. Un commit guarda cambios localmente. Luego **Publish branch** o **Push origin** sube tu rama a GitHub.
5. Para traer cambios de `main` a tu rama, guarda primero tu trabajo, actualiza `main`, vuelve a tu rama y usa **Branch > Merge into current branch...**, seleccionando `main`. Si hay conflictos, revisa los bloques con la otra persona; no aceptes una versión entera sin entender qué se pierde. Verifica y crea el commit de resolución.
6. Usa **Create Pull Request** (o **Preview Pull Request**, según la versión). En GitHub confirma que la base sea `main` y la rama de comparación sea la tuya. Explica qué funciona, qué archivos compartidos cambian y qué verificaste. Solicita revisión a otro integrante.
7. Atiende observaciones mediante nuevos commits en la misma rama. Cuando esté revisado y funcione integrado, usen **Merge pull request**. Actualicen `main` en Desktop. Pueden borrar la rama terminada y crear otra para la siguiente tarea.

Un Pull Request propone integrar cambios; hacer commit o subir una rama no los integra por sí solo. Eviten trabajar directamente en `main`, subir datos operativos y usar force push en ramas compartidas.

## Antes de implementar

Lean [ARQUITECTURA.md](docs/ARQUITECTURA.md) y [CONTRATO_CSV.md](docs/CONTRATO_CSV.md). Acuerden las firmas de funciones y estructuras comunes, la coordinación entre asignación y reservas, y el formato de errores y resultados. No cambien el reparto ni las funcionalidades adicionales registradas. Las decisiones de implementación pendientes están documentadas; Sprint 0 no las resuelve mediante código ficticio.
