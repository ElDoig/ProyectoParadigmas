:- module(valentino, [alcanzable/2, alcanzable/3,
                      pedidos_inaccesibles/1, pedidos_inaccesibles/3]).

/*
  DEPENDE DE (no estan en este archivo):
    Carlos  (4.3.1) - leer_datos(Puntos, Conexiones, Unidades, Pedidos)
                      conexion(Origen, Destino, DistanciaKm, TiempoMin, Estado)
                      pedido(Codigo, Origen, Destino, Litros, Prioridad, Estado, Unidad)
    Nicolas (4.3.2) - conectado(A, B, Distancia, Conexiones): A y B unidos por
                      una conexion habilitada de la lista, en cualquier sentido.

  Los codigos llegan del CSV tal como estan escritos ('P01', 'D006'), asi que
  las consultas se escriben con comillas: alcanzable('P01', 'P06').
*/

:- use_module(carlos,  [leer_datos/4]).
:- use_module(nicolas, [conectado/4]).

% ---------------------------------------------------------------------
% 4.3.3 Alcanzabilidad - Autor: Valentino Mantilla
% alcanzable(X, Y): se puede llegar del punto X al punto Y, directa o
% indirectamente, usando solo conexiones habilitadas.
% ---------------------------------------------------------------------

% Version para consultar: lee los CSV y usa la regla con listas.
alcanzable(X, Y) :-
    leer_datos(_, Conexiones, _, _),
    alcanzable(X, Y, Conexiones).

alcanzable(X, Y, Conexiones) :-
    camino(X, Y, [X], Conexiones).

% Regla recursiva con la misma forma que ancestro/2 de clase:
%   ancestro(X, Y) :- progenitor(X, Y).
%   ancestro(X, Y) :- progenitor(X, Z), ancestro(Z, Y).
% Diferencia: la red tiene ciclos (por ejemplo P04-P05-P06-P04), asi que
% se lleva una LISTA de puntos visitados para no repetir puntos; sin ella,
% Prolog podria dar vueltas para siempre.
% Como hay varios caminos, Prolog puede responder true mas de una vez
% (backtracking): cada true es un camino distinto.

% Caso base: X e Y estan conectados directamente.
camino(X, Y, _, Conexiones) :-
    conectado(X, Y, _, Conexiones).

% Caso recursivo: X se conecta con Z (variable puente), Z todavia no fue
% visitado, y desde Z se llega a Y. Z se agrega a la lista [Z | Visitados].
camino(X, Y, Visitados, Conexiones) :-
    conectado(X, Z, _, Conexiones),
    \+ member(Z, Visitados),
    camino(Z, Y, [Z | Visitados], Conexiones).

% ---------------------------------------------------------------------
% 4.3.6 Pedidos inaccesibles - Autor: Valentino Mantilla
% pedidos_inaccesibles(Codigos): pedidos pendientes cuyo destino no es
% alcanzable desde su origen por las conexiones cerradas.
% ---------------------------------------------------------------------

% Version para consultar: lee los CSV y usa la regla con listas.
pedidos_inaccesibles(Codigos) :-
    leer_datos(_, Conexiones, _, Pedidos),
    pedidos_inaccesibles(Codigos, Conexiones, Pedidos).

% findall con condiciones, como en clase:
%   findall(X, (member(X, [1,4]), X > 2), L).
% Junta los codigos que cumplen: (1) el pedido esta pendiente Y
% (2) su destino NO es alcanzable desde su origen.
% \+ es la negacion: es verdadera cuando Prolog NO puede demostrar la
% meta (presuncion de mundo cerrado: lo que no se demuestra es falso).
pedidos_inaccesibles(Codigos, Conexiones, Pedidos) :-
    findall(Codigo,
            ( member(pedido(Codigo, Origen, Destino, _, _, pendiente, _), Pedidos),
              \+ alcanzable(Origen, Destino, Conexiones) ),
            Codigos).
