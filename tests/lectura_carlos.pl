% Las pruebas leen los ejemplos, sin escribir ni cambiar data/runtime.
:- module(pruebas_lectura_carlos, [pruebas_lectura/0]).
:- use_module('../prolog/carlos.pl').

pruebas_lectura :-
    leer_puntos('data/examples/puntos.csv', Puntos),
    leer_conexiones('data/examples/conexiones.csv', Conexiones),
    leer_unidades('data/examples/unidades.csv', Unidades),
    leer_pedidos('data/examples/pedidos.csv', Pedidos),
    % Las cabeceras no deben aparecer como registros.
    length(Puntos, 10),
    length(Conexiones, 14),
    length(Unidades, 4),
    length(Pedidos, 9),
    % Las comillas conservan los codigos en mayusculas como atomos, no variables.
    member(punto('P01', _, planta, carabayllo), Puntos),
    member(conexion('P01', 'P06', 4.0, 15, habilitada), Conexiones),
    member(unidad('U01', cisterna, 10000, 3.5, disponible), Unidades),
    member(pedido('D001', 'P01', 'P03', 6000, alta, pendiente, ''), Pedidos),
    % Solo U01 y U03 pueden atender D001 con estos datos.
    findall(U, puede_atender(U, 'D001', Unidades, Pedidos), Compatibles),
    Compatibles == ['U01', 'U03'],
    findall(U, puede_atender(U, 'D004', Unidades, Pedidos), PedidoGrande),
    PedidoGrande == [],
    leer_puntos('tests/datos_csv/puntos_solo_cabecera.csv', []),
    findall(P, leer_puntos('tests/datos_csv/puntos_cabecera_incorrecta.csv', P), CabeceraIncorrecta),
    CabeceraIncorrecta == [].
