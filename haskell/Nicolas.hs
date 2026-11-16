{-
  Nicolas.hs - Parte de Nicolas Kou Acosta (modulo Haskell)

  Contiene:
    4.2.1 Lectura de CSV      (leerConexiones, leerUnidades, leerPedidos)
    4.2.3 Vecinos             (vecinos)
    FA    Horarios de recepcion (Horario, Evaluacion, rutasValidasOrdenadas)

  Usa de Carlos (4.2.2) los tipos Conexion, Unidad y Pedido.
  Este modulo NO importa Valentino: el es quien importa vecinos de aqui.
  La FA recibe las rutas ya calculadas, asi que tampoco necesita sus rutas.
-}

module Nicolas
  ( -- 4.2.1
    leerConexiones
  , leerUnidades
  , leerPedidos
  , dividirPorComas
  , limpiar
  , aEntero
    -- 4.2.3
  , vecinos
    -- FA
  , Horario (..)
  , Evaluacion (..)
  , leerHorarios
  , buscarHorario
  , calcularLlegada
  , calcularEspera
  , evaluarRuta
  , esValida
  , rutasValidasOrdenadas
  , aTexto
  , textoEvaluacion
  ) where

import Carlos

-- =====================================================================
-- 4.2.1 Lectura de archivos CSV - Autor: Nicolas Kou Acosta
-- Reciben el CONTENIDO del archivo como texto: son funciones puras.
-- El readFile lo hace Main.hs. Haskell nunca escribe los CSV.
-- =====================================================================

-- "P01,P06,4.0,15,habilitada" -> ["P01","P06","4.0","15","habilitada"]
dividirPorComas :: String -> [String]
dividirPorComas [] = [""]
dividirPorComas (c:cs)
  | c == ','  = "" : resto
  | otherwise = (c : head resto) : tail resto
  where
    resto = dividirPorComas cs

-- Quita espacios y el retorno de carro que deja Windows al final de la linea.
limpiar :: String -> String
limpiar texto = reverse (quitarInicio (reverse (quitarInicio texto)))
  where
    quitarInicio [] = []
    quitarInicio (x:xs)
      | x == ' ' || x == '\r' || x == '\t' = quitarInicio xs
      | otherwise                          = x : xs

esDigito :: Char -> Bool
esDigito c = c >= '0' && c <= '9'

-- Devuelve Nothing si el texto esta vacio o no son todos digitos.
aEntero :: String -> Maybe Int
aEntero texto
  | not (null limpio) && all esDigito limpio = Just (read limpio)
  | otherwise                                = Nothing
  where
    limpio = limpiar texto

-- Acepta "4" y "4.0"; cualquier otra cosa da Nothing.
aDecimal :: String -> Maybe Double
aDecimal texto
  | null limpio                     = Nothing
  | all esDigito limpio             = Just (read (limpio ++ ".0"))
  | enteros /= "" && decimales /= "" && all esDigito enteros && all esDigito decimales
                                    = Just (read limpio)
  | otherwise                       = Nothing
  where
    limpio    = limpiar texto
    enteros   = takeWhile (/= '.') limpio
    resto     = dropWhile (/= '.') limpio
    decimales = drop 1 resto

estadoConexionDe :: String -> Maybe EstadoConexion
estadoConexionDe texto
  | limpiar texto == "habilitada" = Just Habilitada
  | limpiar texto == "cerrada"    = Just Cerrada
  | otherwise                     = Nothing

estadoUnidadDe :: String -> Maybe EstadoUnidad
estadoUnidadDe texto
  | limpiar texto == "disponible" = Just Disponible
  | limpiar texto == "ocupada"    = Just Ocupada
  | otherwise                     = Nothing

prioridadDe :: String -> Maybe Prioridad
prioridadDe texto
  | limpiar texto == "alta"  = Just Alta
  | limpiar texto == "media" = Just Media
  | limpiar texto == "baja"  = Just Baja
  | otherwise                = Nothing

estadoPedidoDe :: String -> Maybe EstadoPedido
estadoPedidoDe texto
  | limpiar texto == "pendiente" = Just Pendiente
  | limpiar texto == "asignado"  = Just Asignado
  | limpiar texto == "entregado" = Just Entregado
  | otherwise                    = Nothing

-- Las filas mal formadas se descartan en vez de cortar el programa.
soloBuenos :: [Maybe a] -> [a]
soloBuenos []              = []
soloBuenos (Nothing : xs)  = soloBuenos xs
soloBuenos (Just x  : xs)  = x : soloBuenos xs

-- Salta la cabecera y convierte cada fila con la funcion recibida.
leerCsv :: (String -> Maybe a) -> String -> [a]
leerCsv parsear contenido =
  case lines contenido of
    []            -> []
    (_cabecera:filas) -> soloBuenos (map parsear filas)

parsearConexion :: String -> Maybe Conexion
parsearConexion linea =
  case dividirPorComas linea of
    [origen, destino, distancia, tiempo, estado] ->
      case (aDecimal distancia, aEntero tiempo, estadoConexionDe estado) of
        (Just d, Just t, Just e)
          | limpiar origen /= "" && limpiar destino /= "" ->
              Just (Conexion (limpiar origen) (limpiar destino) d t e)
        _ -> Nothing
    _ -> Nothing

leerConexiones :: String -> [Conexion]
leerConexiones = leerCsv parsearConexion

parsearUnidad :: String -> Maybe Unidad
parsearUnidad linea =
  case dividirPorComas linea of
    [codigo, tipo, capacidad, costo, estado] ->
      case (aEntero capacidad, aDecimal costo, estadoUnidadDe estado) of
        (Just cap, Just cos, Just e)
          | limpiar codigo /= "" ->
              Just (Unidad (limpiar codigo) (limpiar tipo) cap cos e)
        _ -> Nothing
    _ -> Nothing

leerUnidades :: String -> [Unidad]
leerUnidades = leerCsv parsearUnidad

-- La columna unidad puede venir vacia: significa pedido sin cisterna asignada.
parsearPedido :: String -> Maybe Pedido
parsearPedido linea =
  case dividirPorComas linea of
    [codigo, origen, destino, litros, prioridad, estado, unidad] ->
      case (aEntero litros, prioridadDe prioridad, estadoPedidoDe estado) of
        (Just l, Just p, Just e)
          | limpiar codigo /= "" && limpiar origen /= "" && limpiar destino /= "" ->
              Just (Pedido (limpiar codigo) (limpiar origen) (limpiar destino) l p e (limpiar unidad))
        _ -> Nothing
    _ -> Nothing

leerPedidos :: String -> [Pedido]
leerPedidos = leerCsv parsearPedido

-- =====================================================================
-- 4.2.3 Vecinos - Autor: Nicolas Kou Acosta
-- Puntos unidos directamente con uno dado. Solo conexiones habilitadas
-- y en los dos sentidos. No se repite un vecino aunque haya varias vias.
-- =====================================================================

habilitadas :: [Conexion] -> [Conexion]
habilitadas conexiones = filter (\c -> conexionEstado c == Habilitada) conexiones

sinRepetidos :: [String] -> [String]
sinRepetidos []     = []
sinRepetidos (x:xs) = x : sinRepetidos (filter (/= x) xs)

vecinos :: [Conexion] -> String -> [String]
vecinos conexiones punto =
  sinRepetidos (map otroExtremo (filter tocaElPunto (habilitadas conexiones)))
  where
    tocaElPunto c = conexionOrigen c == punto || conexionDestino c == punto
    otroExtremo c = if conexionOrigen c == punto then conexionDestino c else conexionOrigen c

-- =====================================================================
-- FA - Autor: Nicolas Kou Acosta
-- Evaluacion de entregas dentro del horario de recepcion.
-- Los horarios estan en minutos desde medianoche: 08:00 = 480.
-- Esta parte NO asigna pedidos ni cambia estados: solo evalua rutas.
-- =====================================================================

-- Horario: destino, apertura, cierre, descarga.
data Horario = Horario String Int Int Int
  deriving (Eq, Show)

horarioDestino :: Horario -> String
horarioDestino (Horario d _ _ _) = d

horarioApertura :: Horario -> Int
horarioApertura (Horario _ a _ _) = a

horarioCierre :: Horario -> Int
horarioCierre (Horario _ _ c _) = c

horarioDescarga :: Horario -> Int
horarioDescarga (Horario _ _ _ d) = d

-- Evaluacion: ruta, tiempo de recorrido, llegada, espera, inicio y fin de descarga.
data Evaluacion = Evaluacion [String] Int Int Int Int Int
  deriving (Eq, Show)

evaluacionRuta :: Evaluacion -> [String]
evaluacionRuta (Evaluacion r _ _ _ _ _) = r

evaluacionTiempoRuta :: Evaluacion -> Int
evaluacionTiempoRuta (Evaluacion _ t _ _ _ _) = t

evaluacionLlegada :: Evaluacion -> Int
evaluacionLlegada (Evaluacion _ _ l _ _ _) = l

evaluacionEspera :: Evaluacion -> Int
evaluacionEspera (Evaluacion _ _ _ e _ _) = e

evaluacionInicio :: Evaluacion -> Int
evaluacionInicio (Evaluacion _ _ _ _ i _) = i

evaluacionFin :: Evaluacion -> Int
evaluacionFin (Evaluacion _ _ _ _ _ f) = f

parsearHorario :: String -> Maybe Horario
parsearHorario linea =
  case dividirPorComas linea of
    [destino, apertura, cierre, descarga] ->
      case (aEntero apertura, aEntero cierre, aEntero descarga) of
        (Just a, Just c, Just d)
          | limpiar destino /= "" && a < c && d > 0 && c <= 1440 ->
              Just (Horario (limpiar destino) a c d)
        _ -> Nothing
    _ -> Nothing

leerHorarios :: String -> [Horario]
leerHorarios = leerCsv parsearHorario

-- Nothing cuando el destino no tiene ventana registrada.
buscarHorario :: String -> [Horario] -> Maybe Horario
buscarHorario _ [] = Nothing
buscarHorario codigo (h:hs)
  | horarioDestino h == codigo = Just h
  | otherwise                  = buscarHorario codigo hs

calcularLlegada :: Int -> Int -> Int
calcularLlegada salida tiempoRuta = salida + tiempoRuta

-- Si el camion llega antes de la apertura espera; nunca es negativa.
calcularEspera :: Int -> Int -> Int
calcularEspera apertura llegada
  | apertura > llegada = apertura - llegada
  | otherwise          = 0

-- Recibe la ruta y su tiempo total ya calculados por Valentino (4.2.4 y 4.2.5).
evaluarRuta :: Horario -> Int -> ([String], Int) -> Evaluacion
evaluarRuta horario salida (ruta, tiempoRuta) =
  Evaluacion ruta tiempoRuta llegada espera inicio (inicio + horarioDescarga horario)
  where
    llegada = calcularLlegada salida tiempoRuta
    espera  = calcularEspera (horarioApertura horario) llegada
    inicio  = llegada + espera

-- Si termina exactamente a la hora de cierre la ruta se acepta: por eso <=.
esValida :: Horario -> Evaluacion -> Bool
esValida horario evaluacion = evaluacionFin evaluacion <= horarioCierre horario

-- Insercion ordenada por hora de fin de descarga.
insertarOrdenado :: Evaluacion -> [Evaluacion] -> [Evaluacion]
insertarOrdenado nueva [] = [nueva]
insertarOrdenado nueva (x:xs)
  | evaluacionFin nueva <= evaluacionFin x = nueva : x : xs
  | otherwise                              = x : insertarOrdenado nueva xs

ordenarPorFin :: [Evaluacion] -> [Evaluacion]
ordenarPorFin evaluaciones = foldr insertarOrdenado [] evaluaciones

-- map evalua todas, filter deja las validas y foldr las ordena.
rutasValidasOrdenadas :: Horario -> Int -> [([String], Int)] -> [Evaluacion]
rutasValidasOrdenadas horario salida rutas =
  ordenarPorFin (filter (esValida horario) (map (evaluarRuta horario salida) rutas))

-- 540 -> "09:00"
aTexto :: Int -> String
aTexto minutos = dosDigitos (div minutos 60) ++ ":" ++ dosDigitos (mod minutos 60)
  where
    dosDigitos n
      | n < 10    = '0' : show n
      | otherwise = show n

unirFlechas :: [String] -> String
unirFlechas []     = ""
unirFlechas [x]    = x
unirFlechas (x:xs) = x ++ " -> " ++ unirFlechas xs

textoEvaluacion :: Evaluacion -> String
textoEvaluacion e =
     "   " ++ unirFlechas (evaluacionRuta e) ++ "\n"
  ++ "   Recorrido: " ++ show (evaluacionTiempoRuta e) ++ " min\n"
  ++ "   Llegada: " ++ aTexto (evaluacionLlegada e) ++ "\n"
  ++ "   Espera: " ++ show (evaluacionEspera e) ++ " minutos\n"
  ++ "   Inicio de descarga: " ++ aTexto (evaluacionInicio e) ++ "\n"
  ++ "   Fin de descarga: " ++ aTexto (evaluacionFin e) ++ "\n"
