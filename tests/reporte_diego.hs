-- Prueba del reporte de pedidos de Diego con los datos de data/examples.
import Carlos
import Diego

conexiones :: [Conexion]
conexiones =
  [ Conexion "P01" "P06" 4.0 15 Habilitada, Conexion "P01" "P07" 3.5 12 Habilitada
  , Conexion "P01" "P08" 5.0 18 Habilitada, Conexion "P06" "P07" 2.0 8 Habilitada
  , Conexion "P07" "P09" 3.0 11 Habilitada, Conexion "P08" "P09" 2.5 10 Habilitada
  , Conexion "P06" "P03" 6.0 22 Habilitada, Conexion "P07" "P03" 5.5 20 Habilitada
  , Conexion "P03" "P04" 1.5 6 Habilitada,  Conexion "P03" "P05" 2.5 9 Habilitada
  , Conexion "P04" "P02" 2.0 8 Habilitada,  Conexion "P05" "P02" 3.0 11 Habilitada
  , Conexion "P04" "P05" 1.8 7 Cerrada,     Conexion "P09" "P10" 4.5 17 Cerrada
  ]

unidades :: [Unidad]
unidades =
  [ Unidad "U01" "cisterna" 10000 3.5 Disponible
  , Unidad "U02" "cisterna" 7000 3.0 Ocupada
  , Unidad "U03" "cisterna" 15000 4.5 Disponible
  , Unidad "U04" "cisterna" 5000 2.5 Ocupada
  ]

pedidos :: [Pedido]
pedidos =
  [ Pedido "D001" "P01" "P03" 6000 Alta Pendiente ""
  , Pedido "D002" "P01" "P05" 4000 Media Pendiente ""
  , Pedido "D003" "P01" "P07" 7000 Alta Asignado "U02"
  , Pedido "D004" "P01" "P08" 18000 Alta Pendiente ""
  , Pedido "D005" "P01" "P09" 5000 Baja Asignado "U04"
  , Pedido "D006" "P01" "P10" 3000 Alta Pendiente ""
  , Pedido "D007" "P01" "P02" 8000 Media Entregado "U01"
  , Pedido "D008" "P01" "P06" 9000 Baja Entregado "U03"
  , Pedido "D009" "P01" "P04" 2500 Baja Entregado "U01"
  ]

main :: IO ()
main = do
  putStr (textoResumenGeneral conexiones unidades pedidos)
  putStr (textoResumenRutas conexiones "P01" "P03")
  putStr (textoResumenRutas conexiones "P01" "P10")