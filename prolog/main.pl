% Dejamos disponibles la lectura y las dos formas de consultar la regla de Carlos.
:- module(planificador, [leer_datos/4, puede_atender/2, puede_atender/4,
                         conectado/3, conectado/4,
                         alcanzable/2, alcanzable/3,
                         pedidos_inaccesibles/1, pedidos_inaccesibles/3]).

% Traemos la regla sin copiarla en este archivo.
:- use_module(carlos).
:- use_module(nicolas).
:- use_module(valentino).
:- use_module(diego).

% Ejecutar desde la raiz del repositorio para encontrar data/runtime.
