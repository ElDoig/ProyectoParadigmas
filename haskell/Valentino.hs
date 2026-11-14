{-
  Valentino.hs - Parte de Valentino Mantilla (modulo Haskell)
  Proyecto G1: Abastecimiento de agua con cisternas

  Contiene:
    4.2.4 Rutas posibles      (rutasPosibles)
    4.2.5 Costo de una ruta   (costoRuta)
    FA    Plan de recarga     (planRecarga)
    Funciones para mostrar los resultados (textoCostos, textoPrueba, ...)

  DEPENDE DE (no estan en este archivo):
    Carlos (4.2.2) - tipos con estos nombres de campos:
      Conexion { conOrigen, conDestino :: String, conDistancia :: Double,
                 conTiempo :: Int, conEstado :: EstadoConexion }
      data EstadoConexion = Habilitada | Cerrada
      Unidad   { uniCodigo :: String, uniCapacidad :: Int, uniCostoKm :: Double, ... }
      Pedido   { pedCodigo, pedOrigen, pedDestino :: String, pedLitros :: Int,
                 pedEstado :: EstadoPedido, ... }
      data EstadoPedido = Pendiente | Asignado | Entregado
      type Ruta = [String]
    Nicolas (4.2.3) - vecinos :: [Conexion] -> String -> [String]
                      (solo conexiones habilitadas, en ambos sentidos)

  Si los nombres definitivos del grupo son distintos, solo hay que
  cambiar los nombres de los campos en este archivo.
-}

module Valentino
  ( -- 4.2.4
    rutasPosibles
    -- 4.2.5
  , ResultadoRuta (..)
  , costoRuta
    -- FA: plan de recarga
  , ResultadoPlan (..)
  , planRecarga
  , repartirCarga
  , rutaMasCorta
    -- Textos para mostrar resultados
  , textoCosto
  , textoCostos
  , textoPlan
  , textoPrueba
  , dosDecimales
  , unirFlechas
  ) where

import Carlos   -- tipos: Conexion, Unidad, Pedido, EstadoConexion, EstadoPedido, Ruta
import Nicolas (vecinos)

-- =====================================================================
-- 4.2.4 Rutas posibles - Autor: Valentino Mantilla
-- Genera todas las rutas entre 'inicio' y 'fin' sin repetir puntos.
-- =====================================================================

-- Indica si un punto ya esta en la lista (recursion con x:xs)
estaEn :: String -> [String] -> Bool
estaEn _ []     = False
estaEn x (y:ys) = x == y || estaEn x ys

-- Vecinos del punto actual que todavia no estan en el camino
noVisitados :: [Conexion] -> Ruta -> String -> [String]
noVisitados conexiones camino actual =
  filter (\v -> not (estaEn v camino)) (vecinos conexiones actual)

rutasPosibles :: [Conexion] -> String -> String -> [Ruta]
rutasPosibles conexiones inicio fin = extender conexiones fin [inicio]

-- 'camino' guarda los puntos recorridos AL REVES: el primero es el punto
-- donde estamos ahora (por eso se puede sacar con el patron actual:_).
--   Caso base: si el punto actual es el destino, devolvemos el camino
--              volteado (reverse), que es una ruta completa.
--   Caso recursivo: por cada vecino no visitado, extendemos el camino
--              y juntamos con ++ todas las rutas encontradas (foldr).
extender :: [Conexion] -> String -> Ruta -> [Ruta]
extender _ _ [] = []
extender conexiones fin camino@(actual:_)
  | actual == fin = [reverse camino]
  | otherwise     = foldr (\v acc -> extender conexiones fin (v : camino) ++ acc)
                          []
                          (noVisitados conexiones camino actual)

-- =====================================================================
-- 4.2.5 Costo de una ruta - Autor: Valentino Mantilla
-- Dada una ruta, calcula la distancia total (km) y el tiempo total (min).
-- Si un tramo no existe o esta cerrado, lo indica.
-- =====================================================================

-- Resultado con dos alternativas (TDA de suma con parametros)
data ResultadoRuta = RutaValida Double Int      -- km y minutos
                   | RutaInvalida String        -- mensaje de error
  deriving (Show, Eq)

-- Busca la conexion entre a y b en cualquiera de los dos sentidos.
-- Devuelve Maybe: Just conexion si la encuentra, Nothing si no existe.
buscarTramo :: [Conexion] -> String -> String -> Maybe Conexion
buscarTramo [] _ _ = Nothing
buscarTramo (c:cs) a b
  | conOrigen c == a && conDestino c == b = Just c     -- sentido de ida
  | conOrigen c == b && conDestino c == a = Just c     -- sentido de vuelta
  | otherwise                             = buscarTramo cs a b

-- Recursion sobre la ruta, tramo por tramo:
--   Ruta vacia: error.  Un solo punto: 0 km y 0 min (caso base).
--   (a:b:resto): costo del tramo a-b + costo del resto (b:resto).
costoRuta :: [Conexion] -> Ruta -> ResultadoRuta
costoRuta _ []  = RutaInvalida "La ruta esta vacia"
costoRuta _ [_] = RutaValida 0 0
costoRuta conexiones (a:b:resto) =
  case buscarTramo conexiones a b of
    Nothing    -> RutaInvalida ("No existe el tramo " ++ a ++ " - " ++ b)
    Just tramo -> agregarTramo tramo (costoRuta conexiones (b:resto))

-- Suma un tramo al resultado del resto de la ruta.
-- Los patrones se revisan de arriba hacia abajo: primero si el tramo
-- esta cerrado, luego si el resto ya tenia error, y al final la suma.
agregarTramo :: Conexion -> ResultadoRuta -> ResultadoRuta
agregarTramo tramo _
  | conEstado tramo == Cerrada =
      RutaInvalida ("El tramo " ++ conOrigen tramo ++ " - " ++ conDestino tramo ++ " esta cerrado")
agregarTramo _ (RutaInvalida mensaje) = RutaInvalida mensaje
agregarTramo tramo (RutaValida km minutos) =
  RutaValida (conDistancia tramo + km) (conTiempo tramo + minutos)

-- =====================================================================
-- FA - Plan de recarga para un pedido de gran volumen
-- Autor: Valentino Mantilla
--
-- Para una cisterna y un pedido pendiente que supera su capacidad,
-- calcula los viajes, los litros de cada viaje, la distancia total
-- (idas + retornos a la planta) y el costo. Usa la ruta habilitada mas
-- corta. No hay retorno despues de la ultima entrega, entonces:
--      n viajes = n idas + (n - 1) retornos = 2n - 1 tramos
-- Es solo una simulacion: no cambia estados ni exige que la cisterna
-- este disponible.
-- =====================================================================

-- PlanValido ruta litrosPorViaje kmTotal minutosTotal costoTotal
data ResultadoPlan = PlanValido Ruta [Int] Double Int Double
                   | PlanNoPosible String
  deriving (Show, Eq)

-- Reparte los litros en viajes de como maximo 'capacidad' litros.
-- repartirCarga 18000 7000 = [7000, 7000, 4000]
repartirCarga :: Int -> Int -> [Int]
repartirCarga litros capacidad
  | litros <= capacidad = [litros]
  | otherwise           = capacidad : repartirCarga (litros - capacidad) capacidad

-- A cada ruta le agrega su distancia y tiempo (usa costoRuta, 4.2.5).
-- Devuelve tuplas (ruta, km, minutos).
valorarRutas :: [Conexion] -> [Ruta] -> [(Ruta, Double, Int)]
valorarRutas _ [] = []
valorarRutas conexiones (r:rs) =
  case costoRuta conexiones r of
    RutaValida km minutos -> (r, km, minutos) : valorarRutas conexiones rs
    RutaInvalida _        -> valorarRutas conexiones rs

-- De dos rutas, la de menor distancia (patron @ como en clase)
menorDistancia :: (Ruta, Double, Int) -> (Ruta, Double, Int) -> (Ruta, Double, Int)
menorDistancia a@(_, kmA, _) b@(_, kmB, _)
  | kmA <= kmB = a
  | otherwise  = b

-- La ruta mas corta de una lista (foldr va comparando de dos en dos)
masCorta :: [(Ruta, Double, Int)] -> Maybe (Ruta, Double, Int)
masCorta []     = Nothing
masCorta (x:xs) = Just (foldr (\r mejor -> menorDistancia r mejor) x xs)

rutaMasCorta :: [Conexion] -> String -> String -> Maybe (Ruta, Double, Int)
rutaMasCorta conexiones inicio fin =
  masCorta (valorarRutas conexiones (rutasPosibles conexiones inicio fin))

-- Funcion principal de la FA: primero valida con guardas
planRecarga :: [Conexion] -> Unidad -> Pedido -> ResultadoPlan
planRecarga conexiones unidad pedido
  | pedEstado pedido /= Pendiente =
      PlanNoPosible ("El pedido " ++ pedCodigo pedido ++ " no esta pendiente")
  | uniCapacidad unidad <= 0 =
      PlanNoPosible "La cisterna tiene una capacidad invalida"
  | pedLitros pedido <= uniCapacidad unidad =
      PlanNoPosible ("El pedido cabe en un solo viaje de " ++ uniCodigo unidad
                     ++ "; no necesita plan de recarga")
  | otherwise =
      armarPlan unidad pedido (rutaMasCorta conexiones (pedOrigen pedido) (pedDestino pedido))

-- Arma el plan a partir de la ruta mas corta (pattern matching con Maybe)
armarPlan :: Unidad -> Pedido -> Maybe (Ruta, Double, Int) -> ResultadoPlan
armarPlan _ _ Nothing = PlanNoPosible "No hay ruta habilitada entre la planta y el destino"
armarPlan unidad pedido (Just (ruta, km, minutos)) =
  PlanValido ruta cargas kmTotal (minutos * tramos) (kmTotal * uniCostoKm unidad)
  where
    cargas  = repartirCarga (pedLitros pedido) (uniCapacidad unidad)
    tramos  = 2 * length cargas - 1
    kmTotal = km * fromIntegral tramos   -- fromIntegral: Int -> Double

-- =====================================================================
-- Funciones para armar el texto de salida (tambien son puras:
-- solo construyen Strings; 'main' es quien los imprime)
-- =====================================================================

-- Numero con dos decimales: 75.0 -> "75.00", 11.5 -> "11.50"
dosDecimales :: Double -> String
dosDecimales x
  | centavos < 10 = show enteros ++ ".0" ++ show centavos
  | otherwise     = show enteros ++ "." ++ show centavos
  where
    total    = round (x * 100) :: Int
    enteros  = div total 100
    centavos = mod total 100

-- ["P01","P07","P03"] -> "P01 -> P07 -> P03"
unirFlechas :: Ruta -> String
unirFlechas []     = ""
unirFlechas [x]    = x
unirFlechas (x:xs) = x ++ " -> " ++ unirFlechas xs

textoCosto :: [Conexion] -> Ruta -> String
textoCosto conexiones ruta =
  case costoRuta conexiones ruta of
    RutaValida km minutos ->
      "  " ++ unirFlechas ruta ++ ": " ++ dosDecimales km ++ " km, " ++ show minutos ++ " min\n"
    RutaInvalida mensaje ->
      "  " ++ unirFlechas ruta ++ ": ERROR - " ++ mensaje ++ "\n"

-- Una linea por cada ruta (recursion sobre la lista de rutas)
textoCostos :: [Conexion] -> [Ruta] -> String
textoCostos _ []              = ""
textoCostos conexiones (r:rs) = textoCosto conexiones r ++ textoCostos conexiones rs

-- "Viaje 1: 7000 L", "Viaje 2: ..." (el numero avanza en cada llamada)
textoViajes :: Int -> [Int] -> String
textoViajes _ []     = ""
textoViajes n (l:ls) = "    Viaje " ++ show n ++ ": " ++ show l ++ " L\n" ++ textoViajes (n + 1) ls

detallePlan :: ResultadoPlan -> String
detallePlan (PlanNoPosible mensaje) = "  No se genera plan: " ++ mensaje ++ "\n"
detallePlan (PlanValido ruta cargas km minutos costo) =
     "  Ruta usada: " ++ unirFlechas ruta ++ "\n"
  ++ "  Viajes: " ++ show (length cargas) ++ "\n"
  ++ textoViajes 1 cargas
  ++ "  Distancia total: " ++ dosDecimales km ++ " km (" ++ show minutos ++ " min de manejo)\n"
  ++ "  Costo estimado: S/ " ++ dosDecimales costo ++ "\n"

textoPlan :: Unidad -> Pedido -> ResultadoPlan -> String
textoPlan unidad pedido resultado =
     "Pedido " ++ pedCodigo pedido ++ " (" ++ show (pedLitros pedido) ++ " L) con cisterna "
  ++ uniCodigo unidad ++ " (" ++ show (uniCapacidad unidad) ++ " L)\n"
  ++ detallePlan resultado

-- Buscar por codigo con filter; si la lista resultante esta vacia: Nothing
buscarPedido :: String -> [Pedido] -> Maybe Pedido
buscarPedido codigo pedidos =
  case filter (\p -> pedCodigo p == codigo) pedidos of
    (p:_) -> Just p
    []    -> Nothing

buscarUnidad :: String -> [Unidad] -> Maybe Unidad
buscarUnidad codigo unidades =
  case filter (\u -> uniCodigo u == codigo) unidades of
    (u:_) -> Just u
    []    -> Nothing

-- Prueba de la FA para un pedido y una cisterna (pattern matching de tupla)
textoPrueba :: [Conexion] -> [Unidad] -> [Pedido] -> String -> String -> String
textoPrueba conexiones unidades pedidos codPedido codUnidad =
  case (buscarPedido codPedido pedidos, buscarUnidad codUnidad unidades) of
    (Just p, Just u) -> textoPlan u p (planRecarga conexiones u p)
    _                -> "No se encontro " ++ codPedido ++ " o " ++ codUnidad ++ "\n"

