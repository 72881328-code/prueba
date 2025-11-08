#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <direct.h>
#include <cctype> // Para isdigit()
using namespace std;

// ============================================
// TDA: ESTRUCTURAS BÁSICAS
// ============================================

struct Fecha {
    int dia;
    int mes;
    int anio;
};

struct Paciente {
    int id;
    string nombre;
    string dni;
    int edad;
    char genero;
    string telefono;
    string motivo;
    string diagnostico;
    string estado;
    int prioridad;
    Fecha fechaIngreso;
    string rutaReceta;
    bool tieneReceta;
};

// ============================================
// LISTA ENLAZADA PARA PACIENTES REGISTRADOS
// ============================================

struct NodoLista {
    Paciente datos;
    NodoLista* siguiente;
    
    NodoLista(Paciente p) {
        datos = p;
        siguiente = NULL;
    }
};

class ListaPacientes {
private:
    NodoLista* cabeza;
    int cantidad;
    int idContador;

public:
    ListaPacientes() {
        cabeza = NULL;
        cantidad = 0;
        idContador = 1000;
    }

    bool estaVacia() {
        return cabeza == NULL;
    }

    void insertar(Paciente p) {
        p.id = ++idContador;
        NodoLista* nuevo = new NodoLista(p);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        cantidad++;
    }

    NodoLista* buscarPorDNI(string dni) {
        NodoLista* actual = cabeza;
        while (actual != NULL) {
            if (actual->datos.dni == dni) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return NULL;
    }

    NodoLista* buscarPorNombre(string nombre) {
        NodoLista* actual = cabeza;
        while (actual != NULL) {
            if (actual->datos.nombre == nombre) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return NULL;
    }

    NodoLista* buscarPorID(int id) {
        NodoLista* actual = cabeza;
        while (actual != NULL) {
            if (actual->datos.id == id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return NULL;
    }

    bool eliminarPorDNI(string dni) {
        if (estaVacia()) return false;

        NodoLista* actual = cabeza;
        NodoLista* anterior = NULL;

        while (actual != NULL) {
            if (actual->datos.dni == dni) {
                if (anterior == NULL) {
                    cabeza = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                cantidad--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }

    void mostrarTodos() {
        if (estaVacia()) {
            cout << "\nNo hay pacientes registrados.\n";
            return;
        }

        cout << "\n========== LISTA DE PACIENTES REGISTRADOS ==========\n";
        NodoLista* actual = cabeza;
        int posicion = 1;

        while (actual != NULL) {
            cout << "\n--- Paciente #" << posicion << " ---\n";
            cout << "ID: " << actual->datos.id << endl;
            cout << "Nombre: " << actual->datos.nombre << endl;
            cout << "DNI: " << actual->datos.dni << endl;
            cout << "Edad: " << actual->datos.edad << " anos | Genero: " 
                 << actual->datos.genero << endl;
            cout << "Telefono: " << actual->datos.telefono << endl;
            cout << "Motivo: " << actual->datos.motivo << endl;
            cout << "Diagnostico: " << actual->datos.diagnostico << endl;
            cout << "Estado: " << actual->datos.estado << endl;
            
            string prioridadTexto;
            if (actual->datos.prioridad == 1) {
                prioridadTexto = "URGENTE";
            } else if (actual->datos.prioridad == 2) {
                prioridadTexto = "Normal";
            } else {
                prioridadTexto = "Baja";
            }
            cout << "Prioridad: " << prioridadTexto << endl;
            cout << "Receta: " << (actual->datos.tieneReceta ? "SI" : "NO") << endl;
            cout << "Fecha ingreso: " << actual->datos.fechaIngreso.dia << "/" 
                 << actual->datos.fechaIngreso.mes << "/" 
                 << actual->datos.fechaIngreso.anio << endl;
            
            actual = actual->siguiente;
            posicion++;
        }
    }

    int obtenerCantidad() {
        return cantidad;
    }

    void actualizarEstado(string dni, string nuevoEstado) {
        NodoLista* paciente = buscarPorDNI(dni);
        if (paciente != NULL) {
            paciente->datos.estado = nuevoEstado;
            cout << "\nEstado actualizado correctamente.\n";
        } else {
            cout << "\nPaciente no encontrado.\n";
        }
    }

    void actualizarPrioridad(string dni, int nuevaPrioridad) {
        NodoLista* paciente = buscarPorDNI(dni);
        if (paciente != NULL) {
            paciente->datos.prioridad = nuevaPrioridad;
            cout << "\nPrioridad actualizada correctamente.\n";
        } else {
            cout << "\nPaciente no encontrado.\n";
        }
    }

    void actualizarDiagnostico(string dni, string nuevoDiagnostico) {
        NodoLista* paciente = buscarPorDNI(dni);
        if (paciente != NULL) {
            paciente->datos.diagnostico = nuevoDiagnostico;
            cout << "\nDiagnostico actualizado correctamente.\n";
        } else {
            cout << "\nPaciente no encontrado.\n";
        }
    }

    ~ListaPacientes() {
        while (!estaVacia()) {
            NodoLista* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
};

// ============================================
// COLA PARA PACIENTES EN ESPERA
// ============================================

struct NodoCola {
    Paciente datos;
    NodoCola* siguiente;
    
    NodoCola(Paciente p) {
        datos = p;
        siguiente = NULL;
    }
};

class ColaEspera {
private:
    NodoCola* frente;
    NodoCola* final;
    int cantidad;

public:
    ColaEspera() {
        frente = NULL;
        final = NULL;
        cantidad = 0;
    }

    bool estaVacia() {
        return frente == NULL;
    }

    void encolar(Paciente p) {
        NodoCola* nuevo = new NodoCola(p);

        if (estaVacia()) {
            frente = nuevo;
            final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
        cantidad++;
        cout << "\n[COLA] Paciente agregado a la cola de espera.\n";
    }

    Paciente desencolar() {
        if (estaVacia()) {
            Paciente vacio;
            vacio.id = -1;
            return vacio;
        }

        NodoCola* temp = frente;
        Paciente p = frente->datos;
        frente = frente->siguiente;

        if (frente == NULL) {
            final = NULL;
        }

        delete temp;
        cantidad--;
        return p;
    }

    void mostrar() {
        if (estaVacia()) {
            cout << "\nNo hay pacientes en espera.\n";
            return;
        }

        cout << "\n========== COLA DE ESPERA ==========\n";
        NodoCola* actual = frente;
        int posicion = 1;

        while (actual != NULL) {
            cout << posicion << ". " << actual->datos.nombre 
                 << " (Prioridad: " << actual->datos.prioridad 
                 << ", DNI: " << actual->datos.dni << ")\n";
            actual = actual->siguiente;
            posicion++;
        }
    }

    int obtenerCantidad() {
        return cantidad;
    }

    // Ordenamiento por prioridad usando metodo de insercion
    void ordenarPorPrioridad() {
        if (estaVacia() || cantidad == 1) return;
        
        cout << "\n[ORDENAMIENTO] Ordenando por prioridad...\n";
        
        // Convertir cola a array temporal
        Paciente temp[100];
        int n = 0;
        NodoCola* actual = frente;
        
        while (actual != NULL && n < 100) {
            temp[n] = actual->datos;
            actual = actual->siguiente;
            n++;
        }
        
        // Algoritmo de insercion por prioridad
        for (int i = 1; i < n; i++) {
            Paciente key = temp[i];
            int j = i - 1;
            
            while (j >= 0 && temp[j].prioridad > key.prioridad) {
                temp[j + 1] = temp[j];
                j--;
            }
            temp[j + 1] = key;
        }
        
        // Reconstruir cola ordenada
        while (!estaVacia()) {
            desencolar();
        }
        
        for (int i = 0; i < n; i++) {
            encolar(temp[i]);
        }
        
        cout << "[ORDENAMIENTO] Cola ordenada por prioridad.\n";
    }
};

// ============================================
// PILA PARA HISTORIAL DE ATENCIONES
// ============================================

struct NodoPila {
    Paciente datos;
    NodoPila* siguiente;
    
    NodoPila(Paciente p) {
        datos = p;
        siguiente = NULL;
    }
};

class PilaHistorial {
private:
    NodoPila* tope;
    int cantidad;

public:
    PilaHistorial() {
        tope = NULL;
        cantidad = 0;
    }

    bool estaVacia() {
        return tope == NULL;
    }

    void apilar(Paciente p) {
        NodoPila* nuevo = new NodoPila(p);
        nuevo->siguiente = tope;
        tope = nuevo;
        cantidad++;
    }

    Paciente desapilar() {
        if (estaVacia()) {
            Paciente vacio;
            vacio.id = -1;
            return vacio;
        }

        NodoPila* temp = tope;
        Paciente p = tope->datos;
        tope = tope->siguiente;
        delete temp;
        cantidad--;
        return p;
    }

    void mostrarUltimos(int n) {
        if (estaVacia()) {
            cout << "\nNo hay historial de atenciones.\n";
            return;
        }

        cout << "\n========== ULTIMOS " << n << " PACIENTES ATENDIDOS ==========\n";
        NodoPila* actual = tope;
        int contador = 0;

        while (actual != NULL && contador < n) {
            cout << (contador + 1) << ". " << actual->datos.nombre
                 << " (DNI: " << actual->datos.dni << ")\n";
            cout << "   Diagnostico: " << actual->datos.diagnostico << endl;
            cout << "   Fecha: " << actual->datos.fechaIngreso.dia << "/"
                 << actual->datos.fechaIngreso.mes << "/"
                 << actual->datos.fechaIngreso.anio << endl;
            actual = actual->siguiente;
            contador++;
        }
    }

    int obtenerCantidad() {
        return cantidad;
    }
};

// ============================================
// LISTA PARA MEDICAMENTOS
// ============================================

struct NodoMedicamento {
    string nombre;
    NodoMedicamento* siguiente;
    
    NodoMedicamento(string med) {
        nombre = med;
        siguiente = NULL;
    }
};

class ListaMedicamentos {
private:
    NodoMedicamento* cabeza;
    int cantidad;

public:
    ListaMedicamentos() {
        cabeza = NULL;
        cantidad = 0;
        // Insertar medicamentos iniciales
        string medicamentosIniciales[] = {
            "Paracetamol", "Ibuprofeno", "Amoxicilina", "Omeprazol", 
            "Loratadina", "Salbutamol", "Metformina", "Enalapril",
            "Atorvastatina", "Losartan", "Aspirina", "Diclofenaco",
            "Cetirizina", "Ranitidina", "Ciprofloxacino"
        };
        
        for (int i = 14; i >= 0; i--) {
            insertar(medicamentosIniciales[i]);
        }
    }

    void insertar(string nombre) {
        NodoMedicamento* nuevo = new NodoMedicamento(nombre);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        cantidad++;
    }

    bool eliminar(string nombre) {
        if (cabeza == NULL) return false;

        NodoMedicamento* actual = cabeza;
        NodoMedicamento* anterior = NULL;

        while (actual != NULL) {
            if (actual->nombre == nombre) {
                if (anterior == NULL) {
                    cabeza = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                cantidad--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }

    bool buscar(string nombre) {
        NodoMedicamento* actual = cabeza;
        while (actual != NULL) {
            if (actual->nombre == nombre) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    void mostrar() {
        if (cabeza == NULL) {
            cout << "\nNo hay medicamentos registrados.\n";
            return;
        }

        cout << "\n========== LISTA DE MEDICAMENTOS ==========\n";
        NodoMedicamento* actual = cabeza;
        int posicion = 1;

        while (actual != NULL) {
            cout << posicion << ". " << actual->nombre << endl;
            actual = actual->siguiente;
            posicion++;
        }
    }

    void ordenar() {
        if (cabeza == NULL || cabeza->siguiente == NULL) return;
        
        cout << "\n[ORDENAMIENTO] Ordenando medicamentos...\n";
        
        // Convertir lista a array temporal
        string temp[50];
        int n = 0;
        NodoMedicamento* actual = cabeza;
        
        while (actual != NULL && n < 50) {
            temp[n] = actual->nombre;
            actual = actual->siguiente;
            n++;
        }
        
        // Algoritmo de burbuja
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n-i-1; j++) {
                if (temp[j] > temp[j+1]) {
                    string aux = temp[j];
                    temp[j] = temp[j+1];
                    temp[j+1] = aux;
                }
            }
        }
        
        // Reconstruir lista ordenada
        while (cabeza != NULL) {
            NodoMedicamento* tempNode = cabeza;
            cabeza = cabeza->siguiente;
            delete tempNode;
        }
        cantidad = 0;
        
        for (int i = 0; i < n; i++) {
            insertar(temp[i]);
        }
        
        cout << "Medicamentos ordenados alfabeticamente.\n";
    }

    int obtenerCantidad() {
        return cantidad;
    }
};

// ============================================
// FUNCIONES DE VALIDACION Y UTILIDAD
// ============================================

bool validarDNI(string dni) {
    if (dni.length() != 8) return false;
    for (int i = 0; i < dni.length(); i++) {
        char c = dni[i];
        if (c < '0' || c > '9') return false;
    }
    return true;
}

bool validarTelefono(string tel) {
    if (tel.length() < 7 || tel.length() > 15) return false;
    for (int i = 0; i < tel.length(); i++) {
        char c = tel[i];
        if (!isdigit(c) && c != '-' && c != ' ') return false;
    }
    return true;
}

void crearDirectorioRecetas() {
    _mkdir("Recetas_Medicas");
}

// ============================================
// FUNCIONES PRINCIPALES DEL SISTEMA
// ============================================

Paciente registrarPaciente() {
    Paciente nuevo;
    
    cout << "\n========== REGISTRAR NUEVO PACIENTE ==========\n";
    cin.ignore();
    
    cout << "Nombre completo: ";
    getline(cin, nuevo.nombre);
    
    do {
        cout << "DNI (8 digitos): ";
        getline(cin, nuevo.dni);
    } while (!validarDNI(nuevo.dni));
    
    cout << "Edad: ";
    cin >> nuevo.edad;
    
    cout << "Genero (M/F): ";
    cin >> nuevo.genero;
    
    cin.ignore();
    do {
        cout << "Telefono: ";
        getline(cin, nuevo.telefono);
    } while (!validarTelefono(nuevo.telefono));
    
    cout << "Motivo de consulta: ";
    getline(cin, nuevo.motivo);
    
    cout << "Diagnostico inicial: ";
    getline(cin, nuevo.diagnostico);
    
    cout << "Prioridad (1=Urgente, 2=Normal, 3=Baja): ";
    cin >> nuevo.prioridad;
    
    cout << "Fecha de ingreso (dia mes anio): ";
    cin >> nuevo.fechaIngreso.dia >> nuevo.fechaIngreso.mes >> nuevo.fechaIngreso.anio;
    
    nuevo.estado = "Registrado";
    nuevo.tieneReceta = false;
    nuevo.rutaReceta = "";
    
    cout << "\n*** Paciente registrado exitosamente ***\n";
    return nuevo;
}

void atenderPaciente(ColaEspera& cola, PilaHistorial& historial, ListaPacientes& lista) {
    if (cola.estaVacia()) {
        cout << "\nNo hay pacientes para atender.\n";
        return;
    }
    
    Paciente p = cola.desencolar();
    p.estado = "Atendido";
    
    // Actualizar en lista principal
    NodoLista* pacienteLista = lista.buscarPorDNI(p.dni);
    if (pacienteLista != NULL) {
        pacienteLista->datos.estado = "Atendido";
    }
    
    historial.apilar(p);
    
    cout << "\n========== ATENDIENDO PACIENTE ==========\n";
    cout << "ID: " << p.id << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "DNI: " << p.dni << endl;
    cout << "Edad: " << p.edad << " anos\n";
    cout << "Motivo: " << p.motivo << endl;
    cout << "Diagnostico: " << p.diagnostico << endl;
    cout << "\n*** Paciente atendido correctamente ***\n";
}

void buscarPaciente(ListaPacientes& lista) {
    int opcion;
    cout << "\n=== BUSCAR PACIENTE ===\n";
    cout << "1. Por Nombre\n";
    cout << "2. Por DNI\n";
    cout << "3. Por ID\n";
    cout << "Opcion: ";
    cin >> opcion;
    cin.ignore();
    
    if (opcion == 1) {
        string nombre;
        cout << "Ingrese nombre: ";
        getline(cin, nombre);
        
        NodoLista* encontrado = lista.buscarPorNombre(nombre);
        
        if (encontrado != NULL) {
            cout << "\n*** PACIENTE ENCONTRADO ***\n";
            cout << "ID: " << encontrado->datos.id << endl;
            cout << "Nombre: " << encontrado->datos.nombre << endl;
            cout << "DNI: " << encontrado->datos.dni << endl;
            cout << "Estado: " << encontrado->datos.estado << endl;
        } else {
            cout << "\nPaciente no encontrado.\n";
        }
    } else if (opcion == 2) {
        string dni;
        cout << "Ingrese DNI: ";
        getline(cin, dni);
        
        NodoLista* encontrado = lista.buscarPorDNI(dni);
        
        if (encontrado != NULL) {
            cout << "\n*** PACIENTE ENCONTRADO ***\n";
            cout << "ID: " << encontrado->datos.id << endl;
            cout << "Nombre: " << encontrado->datos.nombre << endl;
            cout << "DNI: " << encontrado->datos.dni << endl;
            cout << "Estado: " << encontrado->datos.estado << endl;
        } else {
            cout << "\nPaciente no encontrado.\n";
        }
    } else if (opcion == 3) {
        int id;
        cout << "Ingrese ID: ";
        cin >> id;
        
        NodoLista* encontrado = lista.buscarPorID(id);
        
        if (encontrado != NULL) {
            cout << "\n*** PACIENTE ENCONTRADO ***\n";
            cout << "ID: " << encontrado->datos.id << endl;
            cout << "Nombre: " << encontrado->datos.nombre << endl;
            cout << "DNI: " << encontrado->datos.dni << endl;
            cout << "Estado: " << encontrado->datos.estado << endl;
        } else {
            cout << "\nPaciente no encontrado.\n";
        }
    }
}

void menuPrincipal() {
    cout << "+------------------------------------------+\n";
    cout << "|  SISTEMA DE GESTION DE PACIENTES        |\n";
    cout << "+------------------------------------------+\n";
    cout << "1.  Registrar paciente\n";
    cout << "2.  Agregar paciente a cola de espera\n";
    cout << "3.  Atender paciente\n";
    cout << "4.  Mostrar todos los pacientes\n";
    cout << "5.  Mostrar cola de espera\n";
    cout << "6.  Buscar paciente\n";
    cout << "7.  Mostrar historial de atenciones\n";
    cout << "8.  Ordenar cola por prioridad\n";
    cout << "9.  Mostrar medicamentos\n";
    cout << "10. Buscar medicamento\n";
    cout << "11. Ordenar medicamentos\n";
    cout << "12. Agregar medicamento\n";
    cout << "13. Eliminar medicamento\n";
    cout << "14. Actualizar datos de paciente\n";
    cout << "15. Estadisticas del sistema\n";
    cout << "0.  Salir\n";
    cout << "-------------------------------------------\n";
    cout << "Opcion: ";
}

// ============================================
// FUNCION PRINCIPAL
// ============================================

int main() {
    ListaPacientes listaPacientes;
    ColaEspera colaEspera;
    PilaHistorial historial;
    ListaMedicamentos medicamentos;
    
    crearDirectorioRecetas();
    
    cout << "\n*** SISTEMA INICIADO ***\n";
    cout << "Estructuras: Lista, Cola y Pila inicializadas.\n";
    
    int opcion;
    
    do {
        system("cls");
        menuPrincipal();
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                Paciente nuevo = registrarPaciente();
                listaPacientes.insertar(nuevo);
                break;
            }
                
            case 2: {
                string dni;
                cout << "\nIngrese DNI del paciente a agregar a cola: ";
                cin.ignore();
                getline(cin, dni);
                
                NodoLista* paciente = listaPacientes.buscarPorDNI(dni);
                if (paciente != NULL) {
                    paciente->datos.estado = "En espera";
                    colaEspera.encolar(paciente->datos);
                    cout << "\n*** Paciente agregado a la cola de espera ***\n";
                } else {
                    cout << "\nPaciente no encontrado.\n";
                }
                break;
            }
                
            case 3:
                atenderPaciente(colaEspera, historial, listaPacientes);
                break;
                
            case 4:
                listaPacientes.mostrarTodos();
                break;
                
            case 5:
                colaEspera.mostrar();
                break;
                
            case 6:
                buscarPaciente(listaPacientes);
                break;
                
            case 7:
                historial.mostrarUltimos(10);
                break;
                
            case 8:
                colaEspera.ordenarPorPrioridad();
                break;
                
            case 9:
                medicamentos.mostrar();
                break;
                
            case 10: {
                string nombre;
                cout << "\nNombre del medicamento a buscar: ";
                cin.ignore();
                getline(cin, nombre);
                if (medicamentos.buscar(nombre)) {
                    cout << "\n*** Medicamento encontrado ***\n";
                } else {
                    cout << "\nMedicamento no encontrado.\n";
                }
                break;
            }
                
            case 11:
                medicamentos.ordenar();
                break;
                
            case 12: {
                string nombre;
                cout << "\nNombre del medicamento: ";
                cin.ignore();
                getline(cin, nombre);
                medicamentos.insertar(nombre);
                cout << "\n*** Medicamento agregado ***\n";
                break;
            }
                
            case 13: {
                string nombre;
                cout << "\nNombre del medicamento a eliminar: ";
                cin.ignore();
                getline(cin, nombre);
                if (medicamentos.eliminar(nombre)) {
                    cout << "\n*** Medicamento eliminado ***\n";
                } else {
                    cout << "\nMedicamento no encontrado.\n";
                }
                break;
            }
                
            case 14: {
                int subopcion;
                cout << "\n=== ACTUALIZAR DATOS ===\n";
                cout << "1. Actualizar estado\n";
                cout << "2. Actualizar prioridad\n";
                cout << "3. Actualizar diagnostico\n";
                cout << "Opcion: ";
                cin >> subopcion;
                cin.ignore();
                
                string dni;
                cout << "DNI del paciente: ";
                getline(cin, dni);
                
                if (subopcion == 1) {
                    string estado;
                    cout << "Nuevo estado: ";
                    getline(cin, estado);
                    listaPacientes.actualizarEstado(dni, estado);
                } else if (subopcion == 2) {
                    int prioridad;
                    cout << "Nueva prioridad (1-3): ";
                    cin >> prioridad;
                    listaPacientes.actualizarPrioridad(dni, prioridad);
                } else if (subopcion == 3) {
                    string diagnostico;
                    cout << "Nuevo diagnostico: ";
                    getline(cin, diagnostico);
                    listaPacientes.actualizarDiagnostico(dni, diagnostico);
                }
                break;
            }
                
            case 15: {
                cout << "\n========== ESTADISTICAS DEL SISTEMA ==========\n";
                cout << "Pacientes registrados: " << listaPacientes.obtenerCantidad() << endl;
                cout << "Pacientes en espera: " << colaEspera.obtenerCantidad() << endl;
                cout << "Pacientes atendidos: " << historial.obtenerCantidad() << endl;
                cout << "Medicamentos disponibles: " << medicamentos.obtenerCantidad() << endl;
                break;
            }
                
            case 0:
                cout << "\nSaliendo del sistema...\n";
                break;
                
            default:
                cout << "\nOpcion invalida.\n";
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 0);
    
    return 0;
}
