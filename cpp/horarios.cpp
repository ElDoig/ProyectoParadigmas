// FA - Autor: Nicolas Kou Acosta.
// Registro y consulta de los horarios de recepcion.
// Esta parte solo registra datos: la evaluacion de rutas se hace en Haskell.

#include <iostream>
#include <fstream>
#include <sstream>
#include "tipos.h"
#include "diego.h"
#include "horarios.h"
#include "nicolas.h"

using namespace std;

int aMinutos(int hora, int minuto) {
    return hora * 60 + minuto;
}

string textoHora(int minutos) {
    int hora = minutos / 60;
    int minuto = minutos % 60;
    string textoHoras = to_string(hora);
    string textoMinutos = to_string(minuto);
    if (hora < 10) textoHoras = "0" + textoHoras;
    if (minuto < 10) textoMinutos = "0" + textoMinutos;
    return textoHoras + ":" + textoMinutos;
}

bool soloDigitos(string texto) {
    if (texto == "") return false;
    for (int i = 0; i < (int)texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') return false;
    }
    return true;
}

// "09:00" se parte por los dos puntos; "9" se toma como la hora en punto.
bool interpretarHora(string texto, int* minutos) {
    if (minutos == nullptr || texto == "") return false;
    int posicion = (int)texto.find(':');
    string parteHora = texto;
    string parteMinuto = "0";
    if (posicion >= 0 && posicion < (int)texto.size()) {
        parteHora = texto.substr(0, posicion);
        parteMinuto = texto.substr(posicion + 1);
    }
    int hora = 0;
    int minuto = 0;
    if (!textoAEntero(parteHora, &hora) || !textoAEntero(parteMinuto, &minuto)) return false;
    if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) return false;
    *minutos = aMinutos(hora, minuto);
    return true;
}

string cargarHorarios(string archivo, HorarioRecepcion horarios[], int* cantidad, int capacidad) {
    if (horarios == nullptr || cantidad == nullptr) return "Falta el arreglo de horarios o su contador.";
    ifstream entrada(archivo.c_str());
    if (!entrada.is_open()) {
        // Todavia no existe el archivo: se empieza sin horarios y no es un error.
        *cantidad = 0;
        return "";
    }
    string cabecera;
    if (!getline(entrada, cabecera)) {
        entrada.close();
        *cantidad = 0;
        return "";
    }

    *cantidad = 0;
    string linea;
    while (getline(entrada, linea)) {
        if (linea == "" || linea == "\r") continue;
        if (*cantidad == capacidad) {
            entrada.close();
            return "El archivo tiene mas horarios que el espacio disponible.";
        }
        stringstream separador(linea);
        string destino, apertura, cierre, descarga;
        getline(separador, destino, ',');
        getline(separador, apertura, ',');
        getline(separador, cierre, ',');
        getline(separador, descarga, ',');
        // Una fila incompleta o con texto se salta: el programa no se detiene.
        HorarioRecepcion nuevo;
        nuevo.puntoDestino = destino;
        if (!textoAEntero(apertura, &nuevo.aperturaMin) ||
            !textoAEntero(cierre, &nuevo.cierreMin) ||
            !textoAEntero(descarga, &nuevo.descargaMin)) continue;
        if (nuevo.puntoDestino == "" || nuevo.aperturaMin >= nuevo.cierreMin ||
            nuevo.descargaMin <= 0 || nuevo.cierreMin > 1440) continue;
        horarios[*cantidad] = nuevo;
        *cantidad = *cantidad + 1;
    }
    entrada.close();
    return "";
}

string guardarHorarios(string archivo, HorarioRecepcion horarios[], int cantidad) {
    if (cantidad > 0 && horarios == nullptr) return "Falta el arreglo de horarios.";
    ofstream salida(archivo.c_str());
    if (!salida.is_open()) return "No se pudo escribir " + archivo;
    salida << "punto_destino,apertura_min,cierre_min,descarga_min\n";
    for (int i = 0; i < cantidad; i++) {
        salida << horarios[i].puntoDestino << "," << horarios[i].aperturaMin << ","
               << horarios[i].cierreMin << "," << horarios[i].descargaMin << "\n";
    }
    salida.close();
    return "";
}

bool existeHorario(HorarioRecepcion horarios[], int cantidad, string destino) {
    for (int i = 0; i < cantidad; i++) {
        if (horarios[i].puntoDestino == destino) return true;
    }
    return false;
}

bool existePuntoHorario(Punto puntos[], int cantidad, string codigo) {
    for (int i = 0; i < cantidad; i++) {
        if (puntos[i].codigo == codigo) return true;
    }
    return false;
}

string registrarHorario(HorarioRecepcion horarios[], int* cantidad, int capacidad,
                        Punto puntos[], int cantidadPuntos, HorarioRecepcion nuevo) {
    if (horarios == nullptr || cantidad == nullptr) {
        return "Falta el arreglo de horarios o su contador.";
    }
    if (*cantidad == capacidad) {
        return "No hay espacio para otro horario.";
    }
    if (nuevo.puntoDestino == "") {
        return "El codigo del punto de destino es obligatorio.";
    }
    if (!existePuntoHorario(puntos, cantidadPuntos, nuevo.puntoDestino)) {
        return "El punto " + nuevo.puntoDestino + " no esta registrado.";
    }
    // Un dia completo son 1440 minutos: no se aceptan entregas del dia siguiente.
    if (nuevo.aperturaMin < 0 || nuevo.aperturaMin > 1439 ||
        nuevo.cierreMin < 1 || nuevo.cierreMin > 1440) {
        return "La apertura y el cierre deben estar dentro del mismo dia.";
    }
    if (nuevo.aperturaMin >= nuevo.cierreMin) {
        return "La apertura debe ser anterior al cierre.";
    }
    if (nuevo.descargaMin <= 0) {
        return "El tiempo de descarga debe ser mayor que cero.";
    }
    if (nuevo.descargaMin > nuevo.cierreMin - nuevo.aperturaMin) {
        return "La descarga no cabe dentro de la ventana de recepcion.";
    }
    if (existeHorario(horarios, *cantidad, nuevo.puntoDestino)) {
        return "El punto " + nuevo.puntoDestino + " ya tiene un horario registrado.";
    }

    horarios[*cantidad] = nuevo;
    *cantidad = *cantidad + 1;
    return "";
}

void mostrarHorarios(HorarioRecepcion horarios[], int cantidad) {
    if (cantidad == 0) {
        cout << "No hay horarios de recepcion registrados.\n";
        return;
    }
    for (int i = 0; i < cantidad; i++) {
        cout << "Destino: " << horarios[i].puntoDestino << "\n";
        cout << "Apertura: " << textoHora(horarios[i].aperturaMin) << "\n";
        cout << "Cierre: " << textoHora(horarios[i].cierreMin) << "\n";
        cout << "Descarga: " << horarios[i].descargaMin << " minutos\n";
        cout << "-----------------------------\n";
    }
}

// Pide una hora por consola aceptando "09:00" o "9 0".
// Devuelve false solo si se acabo la entrada; una hora mal escrita se avisa.
bool pedirHora(string titulo, int* minutos, bool* correcta) {
    string texto;
    cout << titulo;
    if (!(cin >> texto)) return false;
    *correcta = interpretarHora(texto, minutos);
    if (!*correcta) {
        cout << "Hora invalida. Usa 09:00 o 9 0, entre 00:00 y 23:59.\n";
        return true;
    }
    // Con el formato "9 0" falta leer los minutos, que vienen aparte.
    if (texto.find(':') == string::npos) {
        string textoMinuto;
        int minuto = 0;
        if (!(cin >> textoMinuto)) return false;
        if (!textoAEntero(textoMinuto, &minuto) || minuto < 0 || minuto > 59) {
            cout << "Los minutos deben estar entre 0 y 59.\n";
            *correcta = false;
            return true;
        }
        *minutos = *minutos + minuto;
    }
    return true;
}

bool registrarHorarioMenu(HorarioRecepcion horarios[], int* cantidad, int capacidad,
                          Punto puntos[], int cantidadPuntos) {
    HorarioRecepcion nuevo;
    bool correcta = true;

    cout << "Codigo del punto de destino (ej. P03): ";
    if (!(cin >> nuevo.puntoDestino)) return false;

    if (!pedirHora("Hora de apertura (09:00 o 9 0): ", &nuevo.aperturaMin, &correcta)) return false;
    if (!correcta) return true;
    if (!pedirHora("Hora de cierre (10:00 o 10 0): ", &nuevo.cierreMin, &correcta)) return false;
    if (!correcta) return true;

    string textoDescarga;
    cout << "Tiempo de descarga en minutos: ";
    if (!(cin >> textoDescarga)) return false;
    if (!textoAEntero(textoDescarga, &nuevo.descargaMin)) {
        cout << "El tiempo de descarga debe ser un numero de minutos.\n";
        return true;
    }

    string error = registrarHorario(horarios, cantidad, capacidad, puntos, cantidadPuntos, nuevo);
    if (error != "") {
        cout << "No se realizo la operacion: " << error << "\n";
        return true;
    }
    error = guardarHorarios(rutaRuntime("horarios_recepcion.csv"), horarios, *cantidad);
    if (error != "") cout << "El horario quedo en memoria, pero " << error << "\n";
    else cout << "Horario registrado y guardado.\n";
    return true;
}

bool menuHorariosNicolas(HorarioRecepcion horarios[], int* cantidad, int capacidad,
                         Punto puntos[], int cantidadPuntos) {
    string opcion;
    while (true) {
        cout << "\nHORARIOS DE RECEPCION (FA - Nicolas Kou)\n"
             << "1. Registrar horario de recepcion\n"
             << "2. Consultar horarios registrados\n"
             << "0. Volver\nOpcion: ";
        if (!(cin >> opcion)) return false;
        if (opcion == "0") return true;
        if (opcion == "1") {
            if (!registrarHorarioMenu(horarios, cantidad, capacidad, puntos, cantidadPuntos)) return false;
        } else if (opcion == "2") {
            cout << "\n";
            mostrarHorarios(horarios, *cantidad);
        } else {
            cout << "Opcion no valida.\n";
        }
    }
}
