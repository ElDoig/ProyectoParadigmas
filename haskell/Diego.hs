module Diego
  ( pedidosPorEstado
  , cantidadPorEstado
  , cargaAsignada
  , cargaPorUnidad
  , pendientesPorPrioridad
  , textoReportePedidos
  , costoPedido
  , textoResumenRutas
  , textoResumenGeneral
  ) where

import Carlos
import Valentino (Ruta, rutasPosibles, rutaMasCorta, textoCostos, dosDecimales, unirFlechas)

-- 4.2.6 (Diego Rodriguez): pedidos que tienen el estado indicado.
pedidosPorEstado :: EstadoPedido -> [Pedido] -> [Pedido]
pedidosPorEstado estado pedidos = filter (\p -> pedidoEstado p == estado) pedidos

-- Cantidad de pedidos en cada estado.
cantidadPorEstado :: [Pedido] -> [(EstadoPedido, Int)]
cantidadPorEstado pedidos =
  map (\e -> (e, length (pedidosPorEstado e pedidos))) [Pendiente, Asignado, Entregado]

-- Litros de los pedidos asignados a la unidad indicada.
cargaAsignada :: String -> [Pedido] -> Int
cargaAsignada unidad pedidos =
  sum (map pedidoLitros (filter (\p -> pedidoUnidad p == unidad) (pedidosPorEstado Asignado pedidos)))

-- Carga asignada de cada unidad registrada.
cargaPorUnidad :: [Unidad] -> [Pedido] -> [(String, Int)]
cargaPorUnidad unidades pedidos =
  map (\u -> (unidadCodigo u, cargaAsignada (unidadCodigo u) pedidos)) unidades

-- Pedidos pendientes ordenados por prioridad (alta, media, baja).
pendientesPorPrioridad :: [Pedido] -> [Pedido]
pendientesPorPrioridad pedidos = foldr insertar [] (pedidosPorEstado Pendiente pedidos)

-- Inserta un pedido antes del primero de menor prioridad (orden por insercion).
insertar :: Pedido -> [Pedido] -> [Pedido]
insertar p [] = [p]
insertar p (q:qs)
  | pedidoPrioridad p <= pedidoPrioridad q = p : q : qs
  | otherwise                              = q : insertar p qs

-- Texto del reporte de pedidos para mostrar en consola.
textoReportePedidos :: [Unidad] -> [Pedido] -> String
textoReportePedidos unidades pedidos = unlines (
  ["Pedidos por estado:"] ++
  map (\(e, n) -> "  " ++ show e ++ ": " ++ show n) (cantidadPorEstado pedidos) ++
  ["Carga asignada por unidad:"] ++
  map (\(u, l) -> "  " ++ u ++ ": " ++ show l ++ " litros") (cargaPorUnidad unidades pedidos) ++
  ["Pendientes por prioridad:"] ++
  map (\p -> "  " ++ pedidoCodigo p ++ " -> " ++ pedidoDestino p ++ " ("
             ++ show (pedidoPrioridad p) ++ ", " ++ show (pedidoLitros p) ++ " litros)")
      (pendientesPorPrioridad pedidos))
-- 4.2.7 (Diego Rodriguez): costo del pedido por su ruta mas corta con una cisterna.
costoPedido :: [Conexion] -> Unidad -> Pedido -> Maybe Double
costoPedido conexiones unidad pedido =
  case rutaMasCorta conexiones (pedidoOrigen pedido) (pedidoDestino pedido) of
    Nothing         -> Nothing
    Just (_, km, _) -> Just (km * unidadCostoKm unidad)

-- Todas las rutas habilitadas entre dos puntos con su costo, y la mas corta.
textoResumenRutas :: [Conexion] -> String -> String -> String
textoResumenRutas conexiones origen destino =
  case rutasPosibles conexiones origen destino of
    []    -> "Rutas de " ++ origen ++ " a " ++ destino ++ ": no hay rutas habilitadas\n"
    rutas -> "Rutas de " ++ origen ++ " a " ++ destino ++ ":\n" ++ textoCostos conexiones rutas
             ++ textoMasCorta (rutaMasCorta conexiones origen destino)

-- Linea con la ruta mas corta, si existe.
textoMasCorta :: Maybe (Ruta, Double, Int) -> String
textoMasCorta Nothing = ""
textoMasCorta (Just (ruta, km, minutos)) =
  "  Mas corta: " ++ unirFlechas ruta ++ " (" ++ dosDecimales km ++ " km, " ++ show minutos ++ " min)\n"

-- Cisternas disponibles con capacidad suficiente para el pedido.
cisternasCandidatas :: [Unidad] -> Pedido -> [Unidad]
cisternasCandidatas unidades pedido =
  filter (\u -> unidadEstado u == Disponible && unidadCapacidadLitros u >= pedidoLitros pedido) unidades

-- Ruta mas corta de un pedido y el costo con cada cisterna candidata.
textoResumenPedido :: [Conexion] -> [Unidad] -> Pedido -> String
textoResumenPedido conexiones unidades pedido =
  "  " ++ pedidoCodigo pedido ++ " (" ++ pedidoOrigen pedido ++ " -> " ++ pedidoDestino pedido
  ++ ", " ++ show (pedidoLitros pedido) ++ " litros): " ++
  case rutaMasCorta conexiones (pedidoOrigen pedido) (pedidoDestino pedido) of
    Nothing -> "sin ruta habilitada\n"
    Just (ruta, km, minutos) ->
      unirFlechas ruta ++ ", " ++ dosDecimales km ++ " km, " ++ show minutos ++ " min\n"
      ++ textoCandidatas conexiones pedido (cisternasCandidatas unidades pedido)

-- Costo con cada cisterna, o aviso si ninguna alcanza.
textoCandidatas :: [Conexion] -> Pedido -> [Unidad] -> String
textoCandidatas _ _ [] = "    Ninguna cisterna disponible alcanza; revisar plan de recarga\n"
textoCandidatas conexiones pedido unidades =
  concat (map (\u -> "    " ++ unidadCodigo u ++ ": S/ " ++ textoSoles (costoPedido conexiones u pedido) ++ "\n") unidades)

-- Costo en texto con dos decimales.
textoSoles :: Maybe Double -> String
textoSoles Nothing      = "sin ruta"
textoSoles (Just costo) = dosDecimales costo

-- Resumen final: reporte de pedidos y ruta y costo de cada pendiente por prioridad.
textoResumenGeneral :: [Conexion] -> [Unidad] -> [Pedido] -> String
textoResumenGeneral conexiones unidades pedidos =
  textoReportePedidos unidades pedidos
  ++ "Pendientes con ruta mas corta y costo:\n"
  ++ concat (map (textoResumenPedido conexiones unidades) (pendientesPorPrioridad pedidos))