module Carlos
    ( EstadoConexion (..)
    , EstadoUnidad (..)
    , Prioridad (..)
    , EstadoPedido (..)
    , Conexion (..)
    , Unidad (..)
    , Pedido (..)
    ) where

import Data.Int (Int64)

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

-- Los prefijos de los selectores evitan nombres duplicados entre records.
-- El orden de campos coincide con el orden de columnas del contrato.
data Conexion = Conexion
    { conexionOrigen :: String
    , conexionDestino :: String
    , conexionDistanciaKm :: Double
    , conexionTiempoMin :: Int64
    , conexionEstado :: EstadoConexion
    } deriving (Eq, Show)

data Unidad = Unidad
    { unidadCodigo :: String
    , unidadTipo :: String
    , unidadCapacidadLitros :: Int64
    , unidadCostoKm :: Double
    , unidadEstado :: EstadoUnidad
    } deriving (Eq, Show)

data Pedido = Pedido
    { pedidoCodigo :: String
    , pedidoOrigen :: String
    , pedidoDestino :: String
    , pedidoLitros :: Int64
    , pedidoPrioridad :: Prioridad
    , pedidoEstado :: EstadoPedido
    , pedidoUnidad :: Maybe String
    } deriving (Eq, Show)

-- Nothing representa el campo CSV vacio; Just conserva el ID de la unidad.
-- Las validaciones y conversiones CSV se implementaran en sus modulos.
