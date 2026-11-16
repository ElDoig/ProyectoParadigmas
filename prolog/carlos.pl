% Compartimos la lectura y las consultas sin copiar el codigo en otros archivos.
:- module(carlos, [leer_datos/4, leer_puntos/2, leer_conexiones/2,
                   leer_unidades/2, leer_pedidos/2, puede_atender/2, puede_atender/4,
                   ruta_disponible/2]).

% Esta biblioteca ya viene con SWI-Prolog y nos evita separar las comas a mano.
:- use_module(library(csv)).

% 4.3.1 - Carlos Doig: cada CSV se convierte en una lista de registros Prolog.
% La primera fila debe tener esta cabecera exacta; solo devolvemos las otras filas.
% functor pone el nombre del registro, arity revisa sus columnas y encoding lee UTF-8.
leer_puntos(Archivo, Puntos) :-
    csv_read_file(Archivo, Filas, [functor(punto), arity(4), encoding(utf8)]),
    Filas = [punto(codigo, nombre, tipo, zona)|Puntos].

leer_conexiones(Archivo, Conexiones) :-
    csv_read_file(Archivo, Filas, [functor(conexion), arity(5), encoding(utf8)]),
    Filas = [conexion(origen, destino, distancia_km, tiempo_min, estado)|Conexiones].

leer_unidades(Archivo, Unidades) :-
    csv_read_file(Archivo, Filas, [functor(unidad), arity(5), encoding(utf8)]),
    Filas = [unidad(codigo, tipo, capacidad_litros, costo_km, estado)|Unidades].

leer_pedidos(Archivo, Pedidos) :-
    csv_read_file(Archivo, Filas, [functor(pedido), arity(7), encoding(utf8)]),
    Filas = [pedido(codigo, origen, destino, litros, prioridad, estado, unidad)|Pedidos].

% Igual que la carga de C++ y Haskell: usamos data/runtime si el archivo ya
% existe ahi (lo escribio C++); si no, la copia versionada de data/examples.
% Las dos reglas son excluyentes: la segunda solo aplica si no existe la primera.
ruta_disponible(Nombre, Ruta) :-
    atom_concat('data/runtime/', Nombre, Ruta),
    exists_file(Ruta).
ruta_disponible(Nombre, Ruta) :-
    atom_concat('data/runtime/', Nombre, RutaRuntime),
    \+ exists_file(RutaRuntime),
    atom_concat('data/examples/', Nombre, Ruta).

% Usamos las copias locales; los ejemplos versionados no se modifican.
leer_datos(Puntos, Conexiones, Unidades, Pedidos) :-
    ruta_disponible('puntos.csv', RutaPuntos),
    ruta_disponible('conexiones.csv', RutaConexiones),
    ruta_disponible('unidades.csv', RutaUnidades),
    ruta_disponible('pedidos.csv', RutaPedidos),
    leer_puntos(RutaPuntos, Puntos),
    leer_conexiones(RutaConexiones, Conexiones),
    leer_unidades(RutaUnidades, Unidades),
    leer_pedidos(RutaPedidos, Pedidos).

% 4.3.5 - Carlos Doig: leemos los datos y reutilizamos la regla con listas.
% Se vuelven a leer en cada consulta para no guardar una copia desactualizada.
puede_atender(CodigoUnidad, CodigoPedido) :-
    leer_datos(_, _, Unidades, Pedidos),
    puede_atender(CodigoUnidad, CodigoPedido, Unidades, Pedidos).

% unidad(Codigo, Tipo, CapacidadLitros, CostoKm, Estado).
% pedido(Codigo, Origen, Destino, Litros, Prioridad, Estado, Unidad).
puede_atender(CodigoUnidad, CodigoPedido, Unidades, Pedidos) :-
    member(unidad(CodigoUnidad, _, Capacidad, _, disponible), Unidades),
    member(pedido(CodigoPedido, _, _, Litros, _, _, _), Pedidos),
    % Si la capacidad es el mayor de los dos valores, alcanza para el pedido.
    max_list([Capacidad, Litros], Capacidad).
