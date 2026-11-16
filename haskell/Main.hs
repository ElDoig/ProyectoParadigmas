{-
  Main.hs - Punto de entrada del modulo Haskell.

  Lee los CSV de data/runtime/ (escritos por C++) y ofrece un menu con
  las funciones de cada integrante. Haskell solo lee: nunca escribe CSV.
  Si un archivo todavia no existe en data/runtime/, usa la copia de
  data/examples/, igual que la carga inicial de C++ (rutaDisponible).

  Ejecutar desde la raiz del repositorio:
      runghc -ihaskell haskell/Main.hs
-}

module Main (main) where

import System.Directory (doesFileExist)
import System.IO (hFlush, stdout)

import Carlos
import Nicolas
import Valentino
import Diego

carpetaRuntime :: String
carpetaRuntime = "data/runtime/"

carpetaEjemplos :: String
carpetaEjemplos = "data/examples/"

-- Igual que rutaDisponible en C++: data/runtime si el archivo ya existe ahi;
-- si no, la copia de data/examples; Nothing si no esta en ninguna de las dos.
rutaDisponible :: String -> IO (Maybe String)
rutaDisponible nombre = do
  enRuntime  <- doesFileExist (carpetaRuntime ++ nombre)
  enEjemplos <- doesFileExist (carpetaEjemplos ++ nombre)
  return (elegirRuta nombre enRuntime enEjemplos)

-- Parte pura de la eleccion, con guardas.
elegirRuta :: String -> Bool -> Bool -> Maybe String
elegirRuta nombre enRuntime enEjemplos
  | enRuntime  = Just (carpetaRuntime ++ nombre)
  | enEjemplos = Just (carpetaEjemplos ++ nombre)
  | otherwise  = Nothing

main :: IO ()
main = do
  rutaConexiones <- rutaDisponible "conexiones.csv"
  rutaUnidades   <- rutaDisponible "unidades.csv"
  rutaPedidos    <- rutaDisponible "pedidos.csv"
  rutaHorarios   <- rutaDisponible "horarios_recepcion.csv"
  case (rutaConexiones, rutaUnidades, rutaPedidos) of
    (Just c, Just u, Just p) -> iniciar c u p rutaHorarios
    _ -> do
      putStrLn "No se encontraron conexiones.csv, unidades.csv o pedidos.csv"
      putStrLn "ni en data/runtime/ ni en data/examples/."
      putStrLn "Ejecuta el programa desde la raiz del repositorio."

-- Los horarios son de la funcionalidad adicional: si no hay archivo, la
-- lista queda vacia y el resto del modulo funciona igual.
leerOpcional :: Maybe String -> IO String
leerOpcional Nothing      = return ""
leerOpcional (Just ruta)  = readFile ruta

iniciar :: String -> String -> String -> Maybe String -> IO ()
iniciar rutaConexiones rutaUnidades rutaPedidos rutaHorarios = do
  textoConexiones <- readFile rutaConexiones
  textoUnidades   <- readFile rutaUnidades
  textoPedidos    <- readFile rutaPedidos
  textoHorarios   <- leerOpcional rutaHorarios

  let conexiones = leerConexiones textoConexiones
  let unidades   = leerUnidades textoUnidades
  let pedidos    = leerPedidos textoPedidos
  let horarios   = leerHorarios textoHorarios

  putStrLn "Modulo Haskell: calculos de rutas, costos y reportes."
  putStrLn ("Pedidos leidos de: " ++ rutaPedidos)
  putStrLn ("Conexiones: " ++ show (length conexiones)
            ++ " | Cisternas: " ++ show (length unidades)
            ++ " | Pedidos: " ++ show (length pedidos)
            ++ " | Horarios: " ++ show (length horarios))
  menu conexiones unidades pedidos horarios

menu :: [Conexion] -> [Unidad] -> [Pedido] -> [Horario] -> IO ()
menu conexiones unidades pedidos horarios = do
  putStrLn ""
  putStrLn "MENU HASKELL"
  putStrLn "1. Vecinos de un punto (4.2.3)"
  putStrLn "2. Rutas posibles y costos (4.2.4 y 4.2.5)"
  putStrLn "3. Reporte de pedidos (4.2.6)"
  putStrLn "4. Evaluar entregas dentro del horario de recepcion (FA Nicolas Kou)"
  putStrLn "5. Plan de recarga de un pedido grande (FA Valentino Mantilla)"
  putStrLn "6. Resumen general: pedidos, rutas y costos (4.2.7)"
  putStrLn "0. Salir"
  putStr "Opcion: "
  hFlush stdout
  opcion <- getLine
  case limpiar opcion of
    "0" -> putStrLn "Fin del modulo Haskell."
    "1" -> do opcionVecinos conexiones
              menu conexiones unidades pedidos horarios
    "2" -> do opcionRutas conexiones
              menu conexiones unidades pedidos horarios
    "3" -> do putStrLn (textoReportePedidos unidades pedidos)
              menu conexiones unidades pedidos horarios
    "4" -> do opcionHorarios conexiones pedidos horarios
              menu conexiones unidades pedidos horarios
    "5" -> do opcionPlan conexiones unidades pedidos
              menu conexiones unidades pedidos horarios
    "6" -> do putStr (textoResumenGeneral conexiones unidades pedidos)
              menu conexiones unidades pedidos horarios
    _   -> do putStrLn "Opcion no valida."
              menu conexiones unidades pedidos horarios

preguntar :: String -> IO String
preguntar texto = do
  putStr texto
  hFlush stdout
  respuesta <- getLine
  return (limpiar respuesta)

opcionVecinos :: [Conexion] -> IO ()
opcionVecinos conexiones = do
  punto <- preguntar "Codigo del punto (ej. P01): "
  let lista = vecinos conexiones punto
  if null lista
    then putStrLn ("El punto " ++ punto ++ " no tiene vecinos por conexiones habilitadas.")
    else putStrLn ("Vecinos de " ++ punto ++ ": " ++ unirFlechasSimple lista)

unirFlechasSimple :: [String] -> String
unirFlechasSimple []     = ""
unirFlechasSimple [x]    = x
unirFlechasSimple (x:xs) = x ++ ", " ++ unirFlechasSimple xs

opcionRutas :: [Conexion] -> IO ()
opcionRutas conexiones = do
  origen  <- preguntar "Punto de origen: "
  destino <- preguntar "Punto de destino: "
  let rutas = rutasPosibles conexiones origen destino
  if null rutas
    then putStrLn "No hay rutas entre esos puntos."
    else do
      putStrLn ("Rutas posibles: " ++ show (length rutas))
      putStr (textoCostos conexiones rutas)

opcionPlan :: [Conexion] -> [Unidad] -> [Pedido] -> IO ()
opcionPlan conexiones unidades pedidos = do
  pedido <- preguntar "Codigo del pedido: "
  unidad <- preguntar "Codigo de la cisterna: "
  putStr (textoPrueba conexiones unidades pedidos pedido unidad)

-- =====================================================================
-- FA - Autor: Nicolas Kou Acosta
-- Aqui se unen las rutas de Valentino (4.2.4 y 4.2.5) con la evaluacion
-- de horarios de Nicolas. Esta es la unica parte que conoce a los dos.
-- =====================================================================

-- Para cada ruta posible toma su tiempo total con costoRuta.
-- Las rutas con un tramo cerrado devuelven RutaInvalida y se descartan.
rutasConTiempo :: [Conexion] -> String -> String -> [([String], Int)]
rutasConTiempo conexiones origen destino =
  soloValidas (map conTiempo (rutasPosibles conexiones origen destino))
  where
    conTiempo ruta = (ruta, costoRuta conexiones ruta)
    soloValidas [] = []
    soloValidas ((ruta, RutaValida _ minutos) : resto) = (ruta, minutos) : soloValidas resto
    soloValidas ((_, RutaInvalida _) : resto)          = soloValidas resto

opcionHorarios :: [Conexion] -> [Pedido] -> [Horario] -> IO ()
opcionHorarios conexiones pedidos horarios = do
  codigo <- preguntar "Codigo del pedido (ej. D001): "
  case buscarPedidoPorCodigo codigo pedidos of
    Nothing -> putStrLn ("El pedido " ++ codigo ++ " no existe en pedidos.csv.")
    Just pedido -> do
      texto <- preguntar "Hora de salida (HH:MM, ej. 09:00): "
      case leerHora texto of
        Nothing -> putStrLn "Hora invalida. Usa el formato HH:MM entre 00:00 y 23:59."
        Just salida -> mostrarEvaluacion conexiones horarios pedido salida

buscarPedidoPorCodigo :: String -> [Pedido] -> Maybe Pedido
buscarPedidoPorCodigo _ [] = Nothing
buscarPedidoPorCodigo codigo (p:ps)
  | pedidoCodigo p == codigo = Just p
  | otherwise                = buscarPedidoPorCodigo codigo ps

mostrarEvaluacion :: [Conexion] -> [Horario] -> Pedido -> Int -> IO ()
mostrarEvaluacion conexiones horarios pedido salida = do
  putStrLn ""
  putStrLn ("PEDIDO " ++ pedidoCodigo pedido)
  putStrLn ("Origen: " ++ pedidoOrigen pedido)
  putStrLn ("Destino: " ++ pedidoDestino pedido)
  putStrLn ("Salida: " ++ aTexto salida)
  case buscarHorario (pedidoDestino pedido) horarios of
    Nothing ->
      putStrLn ("\nEl punto " ++ pedidoDestino pedido
                ++ " no tiene horario de recepcion registrado.\n"
                ++ "Registralo desde la opcion 5 del menu de C++.")
    Just horario -> do
      putStrLn ("Recepcion: " ++ aTexto (horarioAperturaDe horario) ++ " a "
                ++ aTexto (horarioCierreDe horario) ++ ", descarga "
                ++ show (horarioDescargaDe horario) ++ " minutos")
      let rutas = rutasConTiempo conexiones (pedidoOrigen pedido) (pedidoDestino pedido)
      if null rutas
        then putStrLn "\nNo hay rutas habilitadas entre esos puntos."
        else do
          let validas = rutasValidasOrdenadas horario salida rutas
          putStrLn ("Rutas habilitadas: " ++ show (length rutas))
          if null validas
            then putStrLn "\nNinguna ruta termina la descarga antes del cierre."
            else do
              putStrLn "\nRUTAS VALIDAS:"
              mostrarNumeradas 1 validas

mostrarNumeradas :: Int -> [Evaluacion] -> IO ()
mostrarNumeradas _ [] = return ()
mostrarNumeradas n (e:es) = do
  putStrLn ""
  putStrLn (show n ++ ".")
  putStr (textoEvaluacion e)
  mostrarNumeradas (n + 1) es

horarioAperturaDe :: Horario -> Int
horarioAperturaDe (Horario _ a _ _) = a

horarioCierreDe :: Horario -> Int
horarioCierreDe (Horario _ _ c _) = c

horarioDescargaDe :: Horario -> Int
horarioDescargaDe (Horario _ _ _ d) = d

-- "09:00" -> Just 540
leerHora :: String -> Maybe Int
leerHora texto =
  case partirEnDosPuntos texto of
    Nothing -> Nothing
    Just (textoHora, textoMinuto) ->
      case (aEntero textoHora, aEntero textoMinuto) of
        (Just h, Just m)
          | h >= 0 && h <= 23 && m >= 0 && m <= 59 -> Just (h * 60 + m)
        _ -> Nothing

partirEnDosPuntos :: String -> Maybe (String, String)
partirEnDosPuntos texto = buscar texto ""
  where
    buscar [] _ = Nothing
    buscar (c:cs) acumulado
      | c == ':'  = Just (reverse acumulado, cs)
      | otherwise = buscar cs (c : acumulado)
