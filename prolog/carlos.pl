% Compartimos esta regla para usarla desde main o desde las pruebas.
:- module(carlos, [puede_atender/4]).

% TODO 4.3.1 (Carlos Doig): leer los cuatro CSV y construir los hechos.

% 4.3.5 - Carlos Doig: por ahora recibimos las listas, sin leer archivos.
% unidad(Codigo, Tipo, CapacidadLitros, CostoKm, Estado).
% pedido(Codigo, Origen, Destino, Litros, Prioridad, Estado, Unidad).
puede_atender(CodigoUnidad, CodigoPedido, Unidades, Pedidos) :-
    member(unidad(CodigoUnidad, _, Capacidad, _, disponible), Unidades),
    member(pedido(CodigoPedido, _, _, Litros, _, _, _), Pedidos),
    % Si la capacidad es el mayor de los dos valores, alcanza para el pedido.
    max_list([Capacidad, Litros], Capacidad).
