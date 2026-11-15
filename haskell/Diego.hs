module Diego
  ( pedidosPorEstado
  , cantidadPorEstado
  , cargaAsignada
  , cargaPorUnidad
  , pendientesPorPrioridad
  , textoReportePedidos
  ) where

import Carlos

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