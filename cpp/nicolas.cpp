// 4.1.2 y 4.1.5 - Nicolas Kou Acosta: carga inicial y guardado de los CSV.
// Reutiliza las funciones de texto de diego.h para escribir las palabras
// de cada enumeracion igual que en los archivos del contrato.

#include <fstream>
#include <sstream>
#include <iomanip>
#include "tipos.h"
#include "nicolas.h"
#include "diego.h"

using namespace std;

string rutaRuntime(string nombre) {
    return "data/runtime/" + nombre;
}

string rutaEjemplo(string nombre) {
    return "data/examples/" + nombre;
}

// Quita espacios y el retorno de carro que deja Windows al final de la linea.
string limpiarCampo(string texto) {
    int inicio = 0;
    int fin = (int)texto.size();
    while (inicio < fin && (texto[inicio] == ' ' || texto[inicio] == '\r' || texto[inicio] == '\t')) {
        inicio++;
    }
    while (fin > inicio && (texto[fin - 1] == ' ' || texto[fin - 1] == '\r' || texto[fin - 1] == '\t')) {
        fin--;
    }
    return texto.substr(inicio, fin - inicio);
}

bool esEnteroTexto(string texto) {
    if (texto == "") return false;
    for (int i = 0; i < (int)texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') return false;
    }
    return true;
}

bool esDecimalTexto(string texto) {
    if (texto == "") return false;
    int puntos = 0;
    for (int i = 0; i < (int)texto.size(); i++) {
        if (texto[i] == '.') puntos++;
        else if (texto[i] < '0' || texto[i] > '9') return false;
    }
    if (puntos > 1) return false;
    if (texto[0] == '.' || texto[texto.size() - 1] == '.') return false;
    return true;
}

// stoi lanza una excepcion si el numero no cabe en un int y eso cerraria
// el programa. Convertimos a mano para poder rechazar el dato con un mensaje.
bool textoAEntero(string texto, int* valor) {
    if (valor == nullptr) return false;
    string limpio = limpiarCampo(texto);
    if (!esEnteroTexto(limpio)) return false;
    // El mayor int tiene 10 digitos, asi que con mas digitos ya no cabe.
    if (limpio.size() > 10) return false;
    long long numero = 0;
    for (int i = 0; i < (int)limpio.size(); i++) {
        numero = numero * 10 + (limpio[i] - '0');
        if (numero > 2147483647LL) return false;
    }
    *valor = (int)numero;
    return true;
}

bool textoADecimal(string texto, double* valor) {
    if (valor == nullptr) return false;
    string limpio = limpiarCampo(texto);
    if (!esDecimalTexto(limpio)) return false;
    int digitosEnteros = 0;
    for (int i = 0; i < (int)limpio.size() && limpio[i] != '.'; i++) {
        digitosEnteros++;
    }
    // Una distancia o un costo no tienen 16 cifras enteras; asi stod no desborda.
    if (digitosEnteros > 15) return false;
    *valor = stod(limpio);
    return true;
}

// Escribe 4.0 como "4.0" y 4.25 como "4.25": conserva el formato del CSV.
string textoDecimal(double valor) {
    ostringstream salida;
    salida << fixed << setprecision(2) << valor;
    string texto = salida.str();
    while (texto.size() > 2 && texto[texto.size() - 1] == '0' && texto[texto.size() - 2] != '.') {
        texto = texto.substr(0, texto.size() - 1);
    }
    return texto;
}

// Divide una linea en campos. Devuelve cuantos encontro.
int separarCampos(string linea, string campos[], int maximo) {
    stringstream separador(linea);
    string campo;
    int cantidad = 0;
    while (cantidad < maximo && getline(separador, campo, ',')) {
        campos[cantidad] = limpiarCampo(campo);
        cantidad++;
    }
    // Si quedan mas comas de las esperadas, la fila no cumple el contrato.
    if (getline(separador, campo, ',')) return maximo + 1;
    return cantidad;
}

bool tipoPuntoDesdeTexto(string texto, TipoPunto* tipo) {
    if (texto == "planta") { *tipo = TipoPunto::planta; return true; }
    if (texto == "colegio") { *tipo = TipoPunto::colegio; return true; }
    if (texto == "asentamiento") { *tipo = TipoPunto::asentamiento; return true; }
    if (texto == "mercado") { *tipo = TipoPunto::mercado; return true; }
    return false;
}

bool estadoConexionDesdeTexto(string texto, EstadoConexion* estado) {
    if (texto == "habilitada") { *estado = EstadoConexion::habilitada; return true; }
    if (texto == "cerrada") { *estado = EstadoConexion::cerrada; return true; }
    return false;
}

bool estadoUnidadDesdeTexto(string texto, EstadoUnidad* estado) {
    if (texto == "disponible") { *estado = EstadoUnidad::disponible; return true; }
    if (texto == "ocupada") { *estado = EstadoUnidad::ocupada; return true; }
    return false;
}

bool prioridadDesdeTexto(string texto, Prioridad* prioridad) {
    if (texto == "alta") { *prioridad = Prioridad::alta; return true; }
    if (texto == "media") { *prioridad = Prioridad::media; return true; }
    if (texto == "baja") { *prioridad = Prioridad::baja; return true; }
    return false;
}

// Abre el archivo y comprueba que la cabecera sea la del contrato.
// Si no coincide se rechaza el archivo entero, igual que hace Prolog.
string abrirConCabecera(ifstream* archivo, string ruta, string esperada) {
    archivo->open(ruta.c_str());
    if (!archivo->is_open()) {
        return "No se pudo abrir " + ruta;
    }
    string cabecera;
    if (!getline(*archivo, cabecera)) {
        archivo->close();
        return "El archivo " + ruta + " esta vacio.";
    }
    // Excel guarda a veces tres bytes invisibles al inicio (BOM): los saltamos.
    if (cabecera.size() >= 3 && (unsigned char)cabecera[0] == 0xEF &&
        (unsigned char)cabecera[1] == 0xBB && (unsigned char)cabecera[2] == 0xBF) {
        cabecera = cabecera.substr(3);
    }
    if (limpiarCampo(cabecera) != esperada) {
        archivo->close();
        return "La cabecera de " + ruta + " no coincide con el contrato. Se esperaba: " + esperada;
    }
    return "";
}

string cargarPuntos(string archivo, Punto puntos[], int* cantidad, int capacidad) {
    if (puntos == nullptr || cantidad == nullptr) return "Falta el arreglo de puntos o su contador.";
    ifstream entrada;
    string error = abrirConCabecera(&entrada, archivo, "codigo,nombre,tipo,zona");
    if (error != "") return error;

    *cantidad = 0;
    string linea;
    int numeroLinea = 1;
    while (getline(entrada, linea)) {
        numeroLinea++;
        if (limpiarCampo(linea) == "") continue;
        if (*cantidad == capacidad) {
            entrada.close();
            return "El archivo tiene mas puntos que el espacio disponible.";
        }
        string campos[4];
        if (separarCampos(linea, campos, 4) != 4) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " no tiene 4 campos.";
        }
        Punto nuevo;
        nuevo.codigo = campos[0];
        nuevo.nombre = campos[1];
        nuevo.zona = campos[3];
        if (nuevo.codigo == "" || nuevo.nombre == "" || nuevo.zona == "" ||
            !tipoPuntoDesdeTexto(campos[2], &nuevo.tipo)) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " tiene datos invalidos.";
        }
        puntos[*cantidad] = nuevo;
        *cantidad = *cantidad + 1;
    }
    entrada.close();
    return "";
}

string cargarConexiones(string archivo, Conexion conexiones[], int* cantidad, int capacidad) {
    if (conexiones == nullptr || cantidad == nullptr) return "Falta el arreglo de conexiones o su contador.";
    ifstream entrada;
    string error = abrirConCabecera(&entrada, archivo, "origen,destino,distancia_km,tiempo_min,estado");
    if (error != "") return error;

    *cantidad = 0;
    string linea;
    int numeroLinea = 1;
    while (getline(entrada, linea)) {
        numeroLinea++;
        if (limpiarCampo(linea) == "") continue;
        if (*cantidad == capacidad) {
            entrada.close();
            return "El archivo tiene mas conexiones que el espacio disponible.";
        }
        string campos[5];
        if (separarCampos(linea, campos, 5) != 5) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " no tiene 5 campos.";
        }
        Conexion nueva;
        nueva.origen = campos[0];
        nueva.destino = campos[1];
        if (nueva.origen == "" || nueva.destino == "" ||
            !textoADecimal(campos[2], &nueva.distancia_km) ||
            !textoAEntero(campos[3], &nueva.tiempo_min) ||
            !estadoConexionDesdeTexto(campos[4], &nueva.estado)) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " tiene datos invalidos.";
        }
        conexiones[*cantidad] = nueva;
        *cantidad = *cantidad + 1;
    }
    entrada.close();
    return "";
}

string cargarUnidades(string archivo, Unidad unidades[], int* cantidad, int capacidad) {
    if (unidades == nullptr || cantidad == nullptr) return "Falta el arreglo de unidades o su contador.";
    ifstream entrada;
    string error = abrirConCabecera(&entrada, archivo, "codigo,tipo,capacidad_litros,costo_km,estado");
    if (error != "") return error;

    *cantidad = 0;
    string linea;
    int numeroLinea = 1;
    while (getline(entrada, linea)) {
        numeroLinea++;
        if (limpiarCampo(linea) == "") continue;
        if (*cantidad == capacidad) {
            entrada.close();
            return "El archivo tiene mas cisternas que el espacio disponible.";
        }
        string campos[5];
        if (separarCampos(linea, campos, 5) != 5) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " no tiene 5 campos.";
        }
        Unidad nueva;
        nueva.codigo = campos[0];
        nueva.tipo = campos[1];
        if (nueva.codigo == "" || nueva.tipo == "" ||
            !textoAEntero(campos[2], &nueva.capacidad_litros) ||
            !textoADecimal(campos[3], &nueva.costo_km) ||
            !estadoUnidadDesdeTexto(campos[4], &nueva.estado)) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " tiene datos invalidos.";
        }
        unidades[*cantidad] = nueva;
        *cantidad = *cantidad + 1;
    }
    entrada.close();
    return "";
}

string cargarPedidos(string archivo, Pedido pedidos[], int* cantidad, int capacidad) {
    if (pedidos == nullptr || cantidad == nullptr) return "Falta el arreglo de pedidos o su contador.";
    ifstream entrada;
    string error = abrirConCabecera(&entrada, archivo, "codigo,origen,destino,litros,prioridad,estado,unidad");
    if (error != "") return error;

    *cantidad = 0;
    string linea;
    int numeroLinea = 1;
    while (getline(entrada, linea)) {
        numeroLinea++;
        if (limpiarCampo(linea) == "") continue;
        if (*cantidad == capacidad) {
            entrada.close();
            return "El archivo tiene mas pedidos que el espacio disponible.";
        }
        // La ultima columna puede venir vacia: pedido sin cisterna asignada.
        string campos[7];
        for (int i = 0; i < 7; i++) campos[i] = "";
        int encontrados = separarCampos(linea, campos, 7);
        if (encontrados != 7 && !(encontrados == 6 && linea[linea.size() - 1] == ',')) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " no tiene 7 campos.";
        }
        Pedido nuevo;
        nuevo.codigo = campos[0];
        nuevo.origen = campos[1];
        nuevo.destino = campos[2];
        nuevo.unidad = campos[6];
        EstadoPedido estado = EstadoPedido::pendiente;
        bool estadoCorrecto = true;
        if (campos[5] == "pendiente") estado = EstadoPedido::pendiente;
        else if (campos[5] == "asignado") estado = EstadoPedido::asignado;
        else if (campos[5] == "entregado") estado = EstadoPedido::entregado;
        else estadoCorrecto = false;
        if (nuevo.codigo == "" || nuevo.origen == "" || nuevo.destino == "" ||
            !textoAEntero(campos[3], &nuevo.litros) ||
            !prioridadDesdeTexto(campos[4], &nuevo.prioridad) || !estadoCorrecto) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " tiene datos invalidos.";
        }
        nuevo.estado = estado;
        pedidos[*cantidad] = nuevo;
        *cantidad = *cantidad + 1;
    }
    entrada.close();
    return "";
}

string cargarStockAgua(string archivo, StockAgua stocks[], int* cantidad, int capacidad) {
    if (stocks == nullptr || cantidad == nullptr) return "Falta el arreglo de stock o su contador.";
    ifstream entrada;
    string error = abrirConCabecera(&entrada, archivo, "punto_planta,litros_stock,litros_reservados");
    if (error != "") return error;

    *cantidad = 0;
    string linea;
    int numeroLinea = 1;
    while (getline(entrada, linea)) {
        numeroLinea++;
        if (limpiarCampo(linea) == "") continue;
        if (*cantidad == capacidad) {
            entrada.close();
            return "El archivo tiene mas plantas que el espacio disponible.";
        }
        string campos[3];
        if (separarCampos(linea, campos, 3) != 3) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " no tiene 3 campos.";
        }
        StockAgua nuevo;
        nuevo.punto_planta = campos[0];
        if (nuevo.punto_planta == "" || !textoAEntero(campos[1], &nuevo.litros_stock) ||
            !textoAEntero(campos[2], &nuevo.litros_reservados)) {
            entrada.close();
            return "La linea " + to_string(numeroLinea) + " de " + archivo + " tiene datos invalidos.";
        }
        stocks[*cantidad] = nuevo;
        *cantidad = *cantidad + 1;
    }
    entrada.close();
    return "";
}

// El archivo se reescribe entero: una sola cabecera y nunca filas a medias.
string guardarPuntos(string archivo, Punto puntos[], int cantidad) {
    if (cantidad > 0 && puntos == nullptr) return "Falta el arreglo de puntos.";
    ofstream salida(archivo.c_str());
    if (!salida.is_open()) return "No se pudo escribir " + archivo;
    salida << "codigo,nombre,tipo,zona\n";
    for (int i = 0; i < cantidad; i++) {
        salida << puntos[i].codigo << "," << puntos[i].nombre << ","
               << textoTipoPunto(puntos[i].tipo) << "," << puntos[i].zona << "\n";
    }
    salida.close();
    return "";
}

string guardarConexiones(string archivo, Conexion conexiones[], int cantidad) {
    if (cantidad > 0 && conexiones == nullptr) return "Falta el arreglo de conexiones.";
    ofstream salida(archivo.c_str());
    if (!salida.is_open()) return "No se pudo escribir " + archivo;
    salida << "origen,destino,distancia_km,tiempo_min,estado\n";
    for (int i = 0; i < cantidad; i++) {
        salida << conexiones[i].origen << "," << conexiones[i].destino << ","
               << textoDecimal(conexiones[i].distancia_km) << "," << conexiones[i].tiempo_min << ","
               << textoEstadoConexion(conexiones[i].estado) << "\n";
    }
    salida.close();
    return "";
}

string guardarUnidades(string archivo, Unidad unidades[], int cantidad) {
    if (cantidad > 0 && unidades == nullptr) return "Falta el arreglo de cisternas.";
    ofstream salida(archivo.c_str());
    if (!salida.is_open()) return "No se pudo escribir " + archivo;
    salida << "codigo,tipo,capacidad_litros,costo_km,estado\n";
    for (int i = 0; i < cantidad; i++) {
        salida << unidades[i].codigo << "," << unidades[i].tipo << ","
               << unidades[i].capacidad_litros << "," << textoDecimal(unidades[i].costo_km) << ","
               << textoEstadoUnidad(unidades[i].estado) << "\n";
    }
    salida.close();
    return "";
}

string guardarPedidos(string archivo, Pedido pedidos[], int cantidad) {
    if (cantidad > 0 && pedidos == nullptr) return "Falta el arreglo de pedidos.";
    ofstream salida(archivo.c_str());
    if (!salida.is_open()) return "No se pudo escribir " + archivo;
    salida << "codigo,origen,destino,litros,prioridad,estado,unidad\n";
    for (int i = 0; i < cantidad; i++) {
        salida << pedidos[i].codigo << "," << pedidos[i].origen << "," << pedidos[i].destino << ","
               << pedidos[i].litros << "," << textoPrioridad(pedidos[i].prioridad) << ","
               << textoEstadoPedido(pedidos[i].estado) << "," << pedidos[i].unidad << "\n";
    }
    salida.close();
    return "";
}

string guardarStockAgua(string archivo, StockAgua stocks[], int cantidad) {
    if (cantidad > 0 && stocks == nullptr) return "Falta el arreglo de stock.";
    ofstream salida(archivo.c_str());
    if (!salida.is_open()) return "No se pudo escribir " + archivo;
    salida << "punto_planta,litros_stock,litros_reservados\n";
    for (int i = 0; i < cantidad; i++) {
        salida << stocks[i].punto_planta << "," << stocks[i].litros_stock << ","
               << stocks[i].litros_reservados << "\n";
    }
    salida.close();
    return "";
}

// Si el archivo no esta en data/runtime se usa la copia de data/examples.
string rutaDisponible(string nombre) {
    ifstream prueba(rutaRuntime(nombre).c_str());
    if (prueba.is_open()) {
        prueba.close();
        return rutaRuntime(nombre);
    }
    return rutaEjemplo(nombre);
}

string cargarDatosIniciales(Punto puntos[], int* cantidadPuntos, int capacidadPuntos,
                            Conexion conexiones[], int* cantidadConexiones, int capacidadConexiones,
                            Unidad unidades[], int* cantidadUnidades, int capacidadUnidades,
                            Pedido pedidos[], int* cantidadPedidos, int capacidadPedidos) {
    string error = cargarPuntos(rutaDisponible("puntos.csv"), puntos, cantidadPuntos, capacidadPuntos);
    if (error != "") return error;
    error = cargarConexiones(rutaDisponible("conexiones.csv"), conexiones, cantidadConexiones, capacidadConexiones);
    if (error != "") return error;
    error = cargarUnidades(rutaDisponible("unidades.csv"), unidades, cantidadUnidades, capacidadUnidades);
    if (error != "") return error;
    error = cargarPedidos(rutaDisponible("pedidos.csv"), pedidos, cantidadPedidos, capacidadPedidos);
    if (error != "") return error;
    return "";
}

string guardarDatos(Punto puntos[], int cantidadPuntos,
                    Conexion conexiones[], int cantidadConexiones,
                    Unidad unidades[], int cantidadUnidades,
                    Pedido pedidos[], int cantidadPedidos) {
    string error = guardarPuntos(rutaRuntime("puntos.csv"), puntos, cantidadPuntos);
    if (error != "") return error;
    error = guardarConexiones(rutaRuntime("conexiones.csv"), conexiones, cantidadConexiones);
    if (error != "") return error;
    error = guardarUnidades(rutaRuntime("unidades.csv"), unidades, cantidadUnidades);
    if (error != "") return error;
    error = guardarPedidos(rutaRuntime("pedidos.csv"), pedidos, cantidadPedidos);
    if (error != "") return error;
    return "";
}
