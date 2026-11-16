// FA - Autor: Nicolas Kou Acosta.
// Horarios de recepcion de cada punto de destino.
// Incluir tipos.h antes de este archivo, una sola vez por .cpp.

// Los horarios se guardan en minutos desde medianoche: 08:00 son 480.
struct HorarioRecepcion {
    std::string puntoDestino;
    int aperturaMin;
    int cierreMin;
    int descargaMin;
};

int aMinutos(int hora, int minuto);
std::string textoHora(int minutos);

// Acepta "09:00" y tambien "9". Devuelve false si el texto no es una hora.
bool interpretarHora(std::string texto, int* minutos);

std::string cargarHorarios(std::string archivo, HorarioRecepcion horarios[],
                           int* cantidad, int capacidad);
std::string guardarHorarios(std::string archivo, HorarioRecepcion horarios[], int cantidad);

bool existeHorario(HorarioRecepcion horarios[], int cantidad, std::string destino);

// Valida y agrega el horario al arreglo. Devuelve "" si lo registro,
// o el motivo del rechazo sin modificar nada.
std::string registrarHorario(HorarioRecepcion horarios[], int* cantidad, int capacidad,
                             Punto puntos[], int cantidadPuntos, HorarioRecepcion nuevo);

void mostrarHorarios(HorarioRecepcion horarios[], int cantidad);

// Submenu de la opcion 5 del menu principal.
// Devuelve false si la lectura de entrada falla, como los menus de Carlos.
bool menuHorariosNicolas(HorarioRecepcion horarios[], int* cantidad, int capacidad,
                         Punto puntos[], int cantidadPuntos);
