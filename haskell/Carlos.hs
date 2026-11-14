-- Esto permite que los otros archivos usen nuestros tipos y funciones
-- con import Carlos, sin tener que copiarlos.
module Carlos where

-- 4.2.2 - Carlos Doig: tipos compartidos segun docs/CONTRATO_CSV.md.
-- Los constructores usan mayusculas por sintaxis Haskell. Show sirve
-- para depurar: NO serializa las palabras en minuscula del contrato CSV.
data EstadoConexion = Habilitada | Cerrada
    deriving (Eq, Show)

data EstadoUnidad = Disponible | Ocupada
    deriving (Eq, Show)

-- Orden de atencion: Alta < Media < Baja al ordenar ascendentemente.
data Prioridad = Alta | Media | Baja
    deriving (Eq, Ord, Show)

data EstadoPedido = Pendiente | Asignado | Entregado
    deriving (Eq, Show)

-- Constructores normales, como data Figura en los apuntes de Semana 04.
-- El orden de campos coincide con el orden de columnas del contrato.
-- Conexion: origen, destino, distancia_km, tiempo_min, estado.
data Conexion = Conexion String String Double Int EstadoConexion
    deriving (Eq, Show)

-- Unidad: codigo, tipo, capacidad_litros, costo_km, estado.
data Unidad = Unidad String String Int Double EstadoUnidad
    deriving (Eq, Show)

-- Pedido: codigo, origen, destino, litros, prioridad, estado, unidad.
data Pedido = Pedido String String String Int Prioridad EstadoPedido String
    deriving (Eq, Show)

-- Acceso a campos mediante pattern matching (Semanas 04 y 05).
conexionOrigen :: Conexion -> String
conexionOrigen (Conexion origen _ _ _ _) = origen

conexionDestino :: Conexion -> String
conexionDestino (Conexion _ destino _ _ _) = destino

conexionDistanciaKm :: Conexion -> Double
conexionDistanciaKm (Conexion _ _ distancia _ _) = distancia

conexionTiempoMin :: Conexion -> Int
conexionTiempoMin (Conexion _ _ _ tiempo _) = tiempo

conexionEstado :: Conexion -> EstadoConexion
conexionEstado (Conexion _ _ _ _ estado) = estado

unidadCodigo :: Unidad -> String
unidadCodigo (Unidad codigo _ _ _ _) = codigo

unidadTipo :: Unidad -> String
unidadTipo (Unidad _ tipo _ _ _) = tipo

unidadCapacidadLitros :: Unidad -> Int
unidadCapacidadLitros (Unidad _ _ capacidad _ _) = capacidad

unidadCostoKm :: Unidad -> Double
unidadCostoKm (Unidad _ _ _ costo _) = costo

unidadEstado :: Unidad -> EstadoUnidad
unidadEstado (Unidad _ _ _ _ estado) = estado

pedidoCodigo :: Pedido -> String
pedidoCodigo (Pedido codigo _ _ _ _ _ _) = codigo

pedidoOrigen :: Pedido -> String
pedidoOrigen (Pedido _ origen _ _ _ _ _) = origen

pedidoDestino :: Pedido -> String
pedidoDestino (Pedido _ _ destino _ _ _ _) = destino

pedidoLitros :: Pedido -> Int
pedidoLitros (Pedido _ _ _ litros _ _ _) = litros

pedidoPrioridad :: Pedido -> Prioridad
pedidoPrioridad (Pedido _ _ _ _ prioridad _ _) = prioridad

pedidoEstado :: Pedido -> EstadoPedido
pedidoEstado (Pedido _ _ _ _ _ estado _) = estado

pedidoUnidad :: Pedido -> String
pedidoUnidad (Pedido _ _ _ _ _ _ unidad) = unidad

-- "" representa el campo CSV vacio; otro texto conserva el codigo de unidad.
-- Las validaciones y conversiones CSV se implementaran en sus modulos.
