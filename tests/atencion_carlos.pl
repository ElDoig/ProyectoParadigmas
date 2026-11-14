% Separamos las pruebas para no poner datos de ejemplo en el programa.
:- module(pruebas_atencion_carlos, [datos_atencion/2, pruebas_atencion/0, puede_atender/4]).
:- use_module('../prolog/carlos.pl').

% Estos datos son solo de prueba. El ultimo campo no interviene en la regla.
datos_atencion(
    [unidad(grande, cisterna, 10000, 3, disponible),
     unidad(justa, cisterna, 6000, 3, disponible),
     unidad(pequena, cisterna, 5000, 3, disponible),
     unidad(ocupada, cisterna, 10000, 3, ocupada)],
    [pedido(normal, planta, colegio, 6000, alta, pendiente, _),
     pedido(excesivo, planta, mercado, 18000, media, pendiente, _)]
).

pruebas_atencion :-
    datos_atencion(Unidades, Pedidos),
    % Solo deben salir grande y justa para el pedido normal.
    findall(resultado(U, P), puede_atender(U, P, Unidades, Pedidos), Resultados),
    Resultados == [resultado(grande, normal), resultado(justa, normal)],
    % Un codigo inexistente o una lista vacia no debe producir soluciones.
    findall(P, puede_atender(inexistente, P, Unidades, Pedidos), SinUnidad),
    SinUnidad == [],
    findall(U, puede_atender(U, inexistente, Unidades, Pedidos), SinPedido),
    SinPedido == [],
    findall(U, puede_atender(U, normal, [], Pedidos), ListaUnidadesVacia),
    ListaUnidadesVacia == [],
    findall(P, puede_atender(grande, P, Unidades, []), ListaPedidosVacia),
    ListaPedidosVacia == [].
