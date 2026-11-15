:- module(diego, [ruta_evitando/4, ruta_evitando/5, ruta_compatible/4,
                  cisternas_compatibles/2, restriccion_que_impide/5]).
:- encoding(utf8).

:- use_module(library(csv)).
:- use_module(carlos,  [leer_datos/4]).
:- use_module(nicolas, [conectado/4]).

% 4.3.4 (Diego Rodriguez): ruta de Origen a Destino que no pasa por Evitar.
ruta_evitando(Origen, Destino, Evitar, Ruta) :-
    leer_datos(_, Conexiones, _, _),
    ruta_evitando(Origen, Destino, Evitar, Ruta, Conexiones).

% Version con lista de conexiones; falla si Origen o Destino son el punto a evitar.
ruta_evitando(Origen, Destino, Evitar, Ruta, Conexiones) :-
    Origen \== Evitar,
    Destino \== Evitar,
    recorrido(Origen, Destino, Evitar, [Origen], Conexiones, RutaInvertida),
    reverse(RutaInvertida, Ruta).

% Caso base: se llego al destino y los visitados forman la ruta.
recorrido(Destino, Destino, _, Visitados, _, Visitados).

% Avanza a un vecino no visitado distinto de Evitar (la lista evita ciclos).
recorrido(Actual, Destino, Evitar, Visitados, Conexiones, Ruta) :-
    Actual \== Destino,
    conectado(Actual, Siguiente, _, Conexiones),
    Siguiente \== Evitar,
    \+ member(Siguiente, Visitados),
    recorrido(Siguiente, Destino, Evitar, [Siguiente|Visitados], Conexiones, Ruta).

% Adicional (Diego Rodriguez): lee alturas de cisternas y limites de altura de vias.
leer_alturas(Alturas, Limites) :-
    csv_read_file('data/runtime/dimensiones_cisternas.csv', FilasA,
                  [functor(altura), arity(2), encoding(utf8)]),
    FilasA = [altura(unidad, altura_m)|Alturas],
    csv_read_file('data/runtime/restricciones_altura.csv', FilasL,
                  [functor(limite), arity(3), encoding(utf8)]),
    FilasL = [limite(origen, destino, altura_max_m)|Limites].

% Cualquier ruta entre dos puntos (ruta_evitando sin punto a evitar).
ruta(Origen, Destino, Ruta, Conexiones) :-
    ruta_evitando(Origen, Destino, ninguno, Ruta, Conexiones).

% Tramo A-B formado por dos puntos seguidos de la ruta.
tramo([A, B|_], A, B).
tramo([_|Resto], A, B) :- tramo(Resto, A, B).

% Limite de altura de la via A-B, registrada en cualquier sentido.
limite_via(A, B, Max, Limites) :- member(limite(A, B, Max), Limites).
limite_via(A, B, Max, Limites) :- member(limite(B, A, Max), Limites).

% Tramo A-B de la ruta cuyo limite Max es menor que la altura de la unidad.
bloqueo(Unidad, Ruta, A, B, Max, Alturas, Limites) :-
    member(altura(Unidad, Altura), Alturas),
    tramo(Ruta, A, B),
    limite_via(A, B, Max, Limites),
    Altura > Max.

% Ruta de Origen a Destino por la que la unidad puede circular segun su altura.
ruta_compatible(Unidad, Origen, Destino, Ruta) :-
    leer_datos(_, Conexiones, _, _),
    leer_alturas(Alturas, Limites),
    member(altura(Unidad, _), Alturas),
    ruta(Origen, Destino, Ruta, Conexiones),
    \+ bloqueo(Unidad, Ruta, _, _, _, Alturas, Limites).

% Lista de cisternas que pueden recorrer la ruta completa.
cisternas_compatibles(Ruta, Unidades) :-
    leer_alturas(Alturas, Limites),
    findall(Unidad,
            ( member(altura(Unidad, _), Alturas),
              \+ bloqueo(Unidad, Ruta, _, _, _, Alturas, Limites) ),
            Unidades).

% Restriccion (tramo A-B y su limite Max) que impide a la unidad usar la ruta.
restriccion_que_impide(Unidad, Ruta, A, B, Max) :-
    leer_alturas(Alturas, Limites),
    bloqueo(Unidad, Ruta, A, B, Max, Alturas, Limites).