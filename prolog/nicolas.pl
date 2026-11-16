:- module(nicolas, [conectado/3, conectado/4]).
:- encoding(utf8).

/*
  4.3.2 Regla de conexion directa - Autor: Nicolas Kou Acosta.

  conectado(A, B, Distancia): A y B estan unidos por una conexion
  habilitada, en cualquiera de los dos sentidos.

  DEPENDE DE (no esta en este archivo):
    Carlos (4.3.1) - leer_datos(Puntos, Conexiones, Unidades, Pedidos)
                     conexion(Origen, Destino, DistanciaKm, TiempoMin, Estado)

  Los codigos llegan del CSV tal como estan escritos, asi que las consultas
  se escriben con comillas: conectado('P01', 'P06', D).
*/

:- use_module(carlos, [leer_datos/4]).

% Version para consultar: lee los CSV y usa la regla con listas.
conectado(A, B, Distancia) :-
    leer_datos(_, Conexiones, _, _),
    conectado(A, B, Distancia, Conexiones).

% Una via sirve en los dos sentidos, por eso hay dos reglas con el mismo
% nombre: la primera busca la conexion tal como esta escrita en el CSV y
% la segunda la busca al reves. Prolog prueba las dos por backtracking.
% Solo unifican las filas cuyo estado es habilitada: una conexion cerrada
% no deja conectados sus puntos.
conectado(A, B, Distancia, Conexiones) :-
    member(conexion(A, B, Distancia, _, habilitada), Conexiones).

conectado(A, B, Distancia, Conexiones) :-
    member(conexion(B, A, Distancia, _, habilitada), Conexiones).
