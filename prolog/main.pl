% Punto de entrada del modulo Prolog. Ejecutar desde la raiz del repositorio:
%     swipl prolog/main.pl
% reexport deja disponibles en la consola TODAS las reglas que exporta cada
% integrante (Carlos, Nicolas, Valentino y Diego), sin copiarlas aqui ni
% repetir sus nombres en una lista.
:- module(planificador, []).

:- reexport(carlos).
:- reexport(nicolas).
:- reexport(valentino).
:- reexport(diego).

% Los datos se leen de data/runtime (escritos por C++) o, si aun no existen,
% de data/examples. Los codigos se escriben con comillas, por ejemplo:
%     ?- alcanzable('P01', 'P06').
%     ?- pedidos_inaccesibles(P).
%     ?- ruta_evitando('P01', 'P09', 'P07', Ruta).
