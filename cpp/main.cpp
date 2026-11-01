#include <iostream>
#include <string>

int main() {
    // TODO 4.1.1 (Carlos Doig): integrar las opciones del menu completo.
    std::cout << "Planificador de rutas y logistica - agua con cisternas\n";

    std::string opcion;
    while (true) {
        std::cout << "\nMenu inicial\n0. Salir\nOpcion: ";
        if (!std::getline(std::cin, opcion)) {
            std::cout << "\nFin de entrada.\n";
            return 0;
        }
        if (opcion == "0") {
            std::cout << "Hasta luego.\n";
            return 0;
        }
        std::cout << "Opcion no disponible. Escribe 0 para salir.\n";
    }
}
