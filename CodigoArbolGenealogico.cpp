#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <iomanip>   // setw, left
using namespace std;

/* ==============================================================
   PARTE 1 — INTEGRANTE 1
   main.cpp (único archivo)
   - Estructura del nodo "Miembro"
   - ABB básico (inserción por nombre), búsqueda, modificación
   - Recorridos: PREORDEN / INORDEN / POSTORDEN / POR NIVELES
   - Cada recorrido muestra enumeración (1., 2., ...) que se
     REINICIA en 1 para cada recorrido (como pediste).
   - Salidas en tablas alineadas y textos explicativos para la
     exposición (qué representa cada recorrido).
   Compatible: Dev-C++ 5.11 (C++98)
   ============================================================== */

/* ---------------------------
   ESTRUCTURA DEL NODO: Miembro
   --------------------------- */
struct Miembro {
    string nombre;           // clave de búsqueda (ordenación por nombre)
    int edad;
    string genero;           // "Masculino" / "Femenino"
    string relacionFamiliar; // e.g. "Sapa Inca", "Hijo", "Nieto"
    string ocupacion;        // e.g. "Emperador", "General", "Sacerdote"
    string lugarNacimiento;  // e.g. "Cusco", "Quito", "Titicaca"

    Miembro* izquierdo;
    Miembro* derecho;

    int altura; // campo reservado para AVL (Parte 2)

    Miembro(const string &_nombre, int _edad,
            const string &_genero, const string &_relacion,
            const string &_ocupacion, const string &_lugar)
    {
        nombre = _nombre;
        edad = _edad;
        genero = _genero;
        relacionFamiliar = _relacion;
        ocupacion = _ocupacion;
        lugarNacimiento = _lugar;

        izquierdo = NULL;
        derecho   = NULL;
        altura = 1;
    }
};

/* ---------------------------
   CLASE: ArbolGenealogico (PARTE 1)
   --------------------------- */
class ArbolGenealogico {
private:
    Miembro* raiz;
    // historial simple de operaciones (cadena)
    vector<string> historial;

  

    /* Búsqueda recursiva por nombre */
    Miembro* buscarRec(Miembro* nodo, const string &nombre) {
        if (nodo == NULL) return NULL;
        if (nombre == nodo->nombre) return nodo;
        if (nombre < nodo->nombre) return buscarRec(nodo->izquierdo, nombre);
        return buscarRec(nodo->derecho, nombre);
    }

    /* Recorridos recursivos que imprimen con enumeración compartida */
    void preordenRec(Miembro* nodo, int &contador) {
        if (nodo == NULL) return;
        // imprimir línea numerada (ej.: "1. Nombre | Relacion | Edad | Ocupacion")
        imprimirLineaEnumerada(nodo, contador);
        contador++;
        preordenRec(nodo->izquierdo, contador);
        preordenRec(nodo->derecho, contador);
    }

    void inordenRec(Miembro* nodo, int &contador) {
        if (nodo == NULL) return;
        inordenRec(nodo->izquierdo, contador);
        imprimirLineaEnumerada(nodo, contador);
        contador++;
        inordenRec(nodo->derecho, contador);
    }

    void postordenRec(Miembro* nodo, int &contador) {
        if (nodo == NULL) return;
        postordenRec(nodo->izquierdo, contador);
        postordenRec(nodo->derecho, contador);
        imprimirLineaEnumerada(nodo, contador);
        contador++;
    }

    /* Profundidad (altura) recursiva */
    int profundidadRec(Miembro* nodo) {
        if (nodo == NULL) return 0;
        int iz = profundidadRec(nodo->izquierdo);
        int dr = profundidadRec(nodo->derecho);
        return 1 + ((iz > dr) ? iz : dr);
    }

    /* Conteo recursivo */
    int contarRec(Miembro* nodo) {
        if (nodo == NULL) return 0;
        return 1 + contarRec(nodo->izquierdo) + contarRec(nodo->derecho);
    }

    /* Imprime la cabecera de tabla (alineada) */
    void imprimirCabeceraTabla() {
        cout << "\n-----------------------------------------------------------------\n";
        cout << left << setw(4)  << "No."
             << left << setw(22) << "Nombre"
             << left << setw(22) << "Relación"
             << left << setw(8)  << "Edad"
             << left << setw(20) << "Ocupación"
             << "\n";
        cout << "-----------------------------------------------------------------\n";
    }

    /* Imprime una línea numerada para un miembro (formato compacto, para recorridos) */
    void imprimirLineaEnumerada(Miembro* m, int num) {
        if (m == NULL) return;
        cout << left << setw(4)  << (toStringNum(num) + ".")
             << left << setw(22) << m->nombre
             << left << setw(22) << m->relacionFamiliar
             << left << setw(8)  << (toStringNum(m->edad) + " años")
             << left << setw(20) << m->ocupacion
             << "\n";
    }

    /* Helper: convertir int a string sin usar to_string (C++98) */
    string toStringNum(int x) {
        // pequeño método para convertir int a string (compatible C++98)
        char buffer[20];
        sprintf(buffer, "%d", x);
        return string(buffer);
    }
    /* ---------- FUNCIONES AVL (PARTE 2) ---------- */

// Obtener altura de un nodo
int obtenerAltura(Miembro* nodo) {
    if (nodo == NULL) return 0;
    return nodo->altura;
}

// Calcular factor de balance
int obtenerBalance(Miembro* nodo) {
    if (nodo == NULL) return 0;
    return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
}

// Actualizar altura de un nodo
void actualizarAltura(Miembro* nodo) {
    if (nodo == NULL) return;
    int altIzq = obtenerAltura(nodo->izquierdo);
    int altDer = obtenerAltura(nodo->derecho);
    nodo->altura = 1 + (altIzq > altDer ? altIzq : altDer);
}

// Rotación simple derecha (LL)
Miembro* rotacionDerecha(Miembro* y) {
    Miembro* x = y->izquierdo;
    Miembro* T2 = x->derecho;

    x->derecho = y;
    y->izquierdo = T2;

    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

// Rotación simple izquierda (RR)
Miembro* rotacionIzquierda(Miembro* x) {
    Miembro* y = x->derecho;
    Miembro* T2 = y->izquierdo;

    y->izquierdo = x;
    x->derecho = T2;

    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}

// Balancear nodo (LL, RR, LR, RL)
Miembro* balancear(Miembro* nodo) {
    if (nodo == NULL) return nodo;

    actualizarAltura(nodo);
    int balance = obtenerBalance(nodo);

    if (balance > 1 && obtenerBalance(nodo->izquierdo) >= 0)
        return rotacionDerecha(nodo);

    if (balance > 1 && obtenerBalance(nodo->izquierdo) < 0) {
        nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
        return rotacionDerecha(nodo);
    }

    if (balance < -1 && obtenerBalance(nodo->derecho) <= 0)
        return rotacionIzquierda(nodo);

    if (balance < -1 && obtenerBalance(nodo->derecho) > 0) {
        nodo->derecho = rotacionDerecha(nodo->derecho);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

// Inserción recursiva con balanceo AVL
Miembro* insertarRecAVL(Miembro* nodo, Miembro* nuevo) {
    if (nodo == NULL) return nuevo;

    if (nuevo->nombre < nodo->nombre)
        nodo->izquierdo = insertarRecAVL(nodo->izquierdo, nuevo);
    else if (nuevo->nombre > nodo->nombre)
        nodo->derecho = insertarRecAVL(nodo->derecho, nuevo);
    else
        return nodo; // duplicado

    return balancear(nodo);
}

// Encontrar nodo mínimo (sucesor inorden)
Miembro* encontrarMinimo(Miembro* nodo) {
    while (nodo->izquierdo != NULL) nodo = nodo->izquierdo;
    return nodo;
}

// Eliminación recursiva con balanceo AVL
Miembro* eliminarRec(Miembro* nodo, const string &nombre, bool &eliminado) {
    if (nodo == NULL) {
        eliminado = false;
        return NULL;
    }

    if (nombre < nodo->nombre)
        nodo->izquierdo = eliminarRec(nodo->izquierdo, nombre, eliminado);
    else if (nombre > nodo->nombre)
        nodo->derecho = eliminarRec(nodo->derecho, nombre, eliminado);
    else {
        eliminado = true;

        if (nodo->izquierdo == NULL) {
            Miembro* temp = nodo->derecho;
            delete nodo;
            return temp;
        } else if (nodo->derecho == NULL) {
            Miembro* temp = nodo->izquierdo;
            delete nodo;
            return temp;
        }

        Miembro* sucesor = encontrarMinimo(nodo->derecho);
        nodo->nombre = sucesor->nombre;
        nodo->edad = sucesor->edad;
        nodo->genero = sucesor->genero;
        nodo->relacionFamiliar = sucesor->relacionFamiliar;
        nodo->ocupacion = sucesor->ocupacion;
        nodo->lugarNacimiento = sucesor->lugarNacimiento;

        nodo->derecho = eliminarRec(nodo->derecho, sucesor->nombre, eliminado);
    }

    return balancear(nodo);
}

/* ---------- ESTADÍSTICAS AVANZADAS (PARTE 2) ---------- */

void sumarEdades(Miembro* nodo, int &suma, int &contador) {
    if (nodo == NULL) return;
    suma += nodo->edad;
    contador++;
    sumarEdades(nodo->izquierdo, suma, contador);
    sumarEdades(nodo->derecho, suma, contador);
}

void encontrarEdadMaxima(Miembro* nodo, int &maxEdad) {
    if (nodo == NULL) return;
    if (nodo->edad > maxEdad) maxEdad = nodo->edad;
    encontrarEdadMaxima(nodo->izquierdo, maxEdad);
    encontrarEdadMaxima(nodo->derecho, maxEdad);
}

void encontrarEdadMinima(Miembro* nodo, int &minEdad) {
    if (nodo == NULL) return;
    if (nodo->edad < minEdad) minEdad = nodo->edad;
    encontrarEdadMinima(nodo->izquierdo, minEdad);
    encontrarEdadMinima(nodo->derecho, minEdad);
}

void contarPorRelacion(Miembro* nodo, const string &relacion, int &contador) {
    if (nodo == NULL) return;
    if (nodo->relacionFamiliar == relacion) contador++;
    contarPorRelacion(nodo->izquierdo, relacion, contador);
    contarPorRelacion(nodo->derecho, relacion, contador);
}

bool esAVLBalanceado(Miembro* nodo) {
    if (nodo == NULL) return true;
    int balance = obtenerBalance(nodo);
    if (balance < -1 || balance > 1) return false;
    return esAVLBalanceado(nodo->izquierdo) && esAVLBalanceado(nodo->derecho);
}

public:
    ArbolGenealogico() {
        raiz = NULL;
        historial.clear();
    }


    /* ---------- Búsqueda pública ---------- */
    Miembro* buscarMiembro(const string &nombre) {
        return buscarRec(raiz, nombre);
    }

    /* ---------- Modificar miembro ---------- */
    bool modificarMiembro(const string &nombre, int nuevaEdad,
                         const string &nuevaOcupacion, const string &nuevaRelacion)
    {
        Miembro* m = buscarMiembro(nombre);
        if (m == NULL) return false;
        m->edad = nuevaEdad;
        m->ocupacion = nuevaOcupacion;
        m->relacionFamiliar = nuevaRelacion;
        historial.push_back(string("MODIFICAR: ") + nombre);
        return true;
    }

    /* ---------- Mostrar ficha completa (para búsqueda) ---------- */
    void imprimirMiembroCompleto(Miembro* m) {
        if (m == NULL) {
            cout << "(Miembro nulo)\n";
            return;
        }
        cout << "\n----------------- FICHA DEL MIEMBRO -----------------\n";
        cout << "Nombre: " << m->nombre << "\n";
        cout << "Edad: "  << m->edad << " años\n";
        cout << "Género: " << m->genero << "\n";
        cout << "Relación familiar: " << m->relacionFamiliar << "\n";
        cout << "Ocupación: " << m->ocupacion << "\n";
        cout << "Lugar de nacimiento: " << m->lugarNacimiento << "\n";
        cout << "Altura (campo para AVL): " << m->altura << "\n";
        cout << "----------------------------------------------------\n";
    }

    /* ---------- RECORRIDOS PÚBLICOS (con numeración reiniciada) ---------- */

    void mostrarPreorden() {
        cout << "\n=== RECORRIDO PREORDEN (Antigüedad: ancestros ? descendientes) ===\n";
        cout << "Explicación: Preorden visita la raíz primero, por eso es útil\n";
        cout << "             para presentar la genealogía del ancestro hacia\n";
        cout << "             sus descendientes en orden temporal.\n";
        imprimirCabeceraTabla();
        int contador = 1; // reiniciar enumeración
        preordenRec(raiz, contador);
        cout << "-----------------------------------------------------------------\n";
    }

    void mostrarInorden() {
        cout << "\n=== RECORRIDO INORDEN (Estructura lateral: izquierda ? nodo ? derecha) ===\n";
        cout << "Explicación: Inorden no representa antigüedad; muestra cómo\n";
        cout << "             se distribuyen los miembros entre la rama izquierda\n";
        cout << "             y la rama derecha (vista lateral/estructura).\n";
        imprimirCabeceraTabla();
        int contador = 1;
        inordenRec(raiz, contador);
        cout << "-----------------------------------------------------------------\n";
    }

    void mostrarPostorden() {
        cout << "\n=== RECORRIDO POSTORDEN (Descendientes ? Ancestro) ===\n";
        cout << "Explicación: Postorden visita los descendientes antes que el\n";
        cout << "             ancestro; es útil para ver generaciones recientes\n";
        cout << "             hacia los más antiguos.\n";
        imprimirCabeceraTabla();
        int contador = 1;
        postordenRec(raiz, contador);
        cout << "-----------------------------------------------------------------\n";
    }

    /* Mostrar por niveles (BFS) con enumeración reiniciada */
    void mostrarPorNiveles() {
        cout << "\n=== RECORRIDO POR NIVELES (Generaciones aproximadas) ===\n";
        cout << "Explicación: Por niveles muestra nodos por distancia a la raíz\n";
        cout << "             (nivel 0 = raíz, nivel 1 = hijos, etc.).\n";
        imprimirCabeceraTabla();
        if (raiz == NULL) {
            cout << "(árbol vacío)\n";
            cout << "-----------------------------------------------------------------\n";
            return;
        }
        queue<Miembro*> q;
        q.push(raiz);
        int contador = 1;
        while (!q.empty()) {
            Miembro* cur = q.front(); q.pop();
            imprimirLineaEnumerada(cur, contador);
            contador++;
            if (cur->izquierdo) q.push(cur->izquierdo);
            if (cur->derecho) q.push(cur->derecho);
        }
        cout << "-----------------------------------------------------------------\n";
    }
    /* ---------- Historial simple ---------- */
    void mostrarHistorial() {
        cout << "\n=== Historial de operaciones (sesión) ===\n";
        if (historial.empty()) {
            cout << "(vacío)\n";
        } else {
            for (size_t i = 0; i < historial.size(); ++i)
                cout << (i+1) << ". " << historial[i] << "\n";
        }
        cout << "=========================================\n";
    }

    /* ---------- Cargar datos de ejemplo (útil para demo) ---------- */
   void cargarDatosEjemplo() {
    // Inserciones representativas
    insertarMiembroAVL("Manco Cápac", 70, "Masculino", "Fundador", "Sapa Inca legendario", "Titicaca");
    insertarMiembroAVL("Sinchi Roca", 48, "Masculino", "Sucesor", "Noble", "Cusco");
    insertarMiembroAVL("Lloque Yupanqui", 45, "Masculino", "Ancestro legendario", "Noble", "Cusco");
    insertarMiembroAVL("Mayta Capac", 60, "Masculino", "Antepasado", "Sapa Inca", "Cusco");
    insertarMiembroAVL("Pachacutec", 55, "Masculino", "Sapa Inca", "Emperador - Reformador", "Cusco");

    insertarmeHistorial("CARGA_INICIAL: Datos del Tahuantinsuyo insertados");
}
    /* helper para agregar evento al historial sin repetición */
    void insertarmeHistorial(const string &s) {
        historial.push_back(s);
    }
    /* ---------- INSERCIÓN CON AVL ---------- */
bool insertarMiembroAVL(const string &nombre, int edad,
                        const string &genero, const string &relacion,
                        const string &ocupacion, const string &lugarNacimiento)
{
    if (nombre.size() == 0) return false;
    if (buscarMiembro(nombre) != NULL) return false;

    Miembro* nuevo = new Miembro(nombre, edad, genero, relacion, ocupacion, lugarNacimiento);
    raiz = insertarRecAVL(raiz, nuevo);
    historial.push_back(string("INSERTAR AVL: ") + nombre);
    return true;
}

/* ---------- ELIMINACIÓN ---------- */
bool eliminarMiembro(const string &nombre) {
    if (nombre.empty()) return false;
    bool eliminado = false;
    raiz = eliminarRec(raiz, nombre, eliminado);
    if (eliminado) historial.push_back(string("ELIMINAR: ") + nombre);
    return eliminado;
}

/* ---------- ESTADÍSTICAS AVANZADAS ---------- */
void mostrarEstadisticasAvanzadas() {
    cout << "\n========== ESTADISTICAS AVANZADAS ==========\n";

    int total = contarRec(raiz);
    cout << "Total de miembros: " << total << "\n";

    if (total == 0) {
        cout << "No hay miembros en el arbol.\n";
        cout << "===========================================\n";
        return;
    }

    int suma = 0, contador = 0;
    sumarEdades(raiz, suma, contador);
    double promedio = (double)suma / contador;
    cout << "Edad promedio: " << promedio << " años\n";

    int maxEdad = 0;
    encontrarEdadMaxima(raiz, maxEdad);
    cout << "Edad maxima: " << maxEdad << " años\n";

    int minEdad = 999;
    encontrarEdadMinima(raiz, minEdad);
    cout << "Edad minima: " << minEdad << " años\n";

    cout << "Profundidad del arbol: " << profundidadRec(raiz) << " niveles\n";

    bool balanceado = esAVLBalanceado(raiz);
    cout << "Estado AVL: " << (balanceado ? "BALANCEADO" : "DESBALANCEADO") << "\n";

    cout << "===========================================\n";
}

void mostrarConteoRelaciones() {
    cout << "\n========== CONTEO POR RELACION FAMILIAR ==========\n";

    string relaciones[] = {"Sapa Inca", "Fundador", "Sucesor", "Principe",
                           "Hijo", "Nieto", "Emperador", "Noble", 
                           "Ancestro legendario", "Antepasado"};
    int numRelaciones = 10;
    bool hayResultados = false;

    for (int i = 0; i < numRelaciones; i++) {
        int count = 0;
        contarPorRelacion(raiz, relaciones[i], count);
        if (count > 0) {
            cout << relaciones[i] << ": " << count << " miembro(s)\n";
            hayResultados = true;
        }
    }

    if (!hayResultados) cout << "No se encontraron relaciones registradas.\n";
    cout << "==================================================\n";
	}	
};

/* ---------------------------
   MENÚ PRINCIPAL (INTERACTIVO)
   --------------------------- */
void limpiarEntradas() {
    cin.clear();
    cin.ignore(1024, '\n');
}

int main() {
    ArbolGenealogico arbol;
    int opcion = -1;

    cout << "=== Sistema: Árbol Genealógico (Parte 1) ===\n";
    cout << "Se cargaron datos de ejemplo para la demostración.\n";
    arbol.cargarDatosEjemplo();

    while (true) {
        cout << "\n--- MENU PRINCIPAL ---\n";
        cout << "1. Insertar nuevo miembro con AVL\n";
        cout << "2. Buscar miembro por nombre\n";
        cout << "3. Modificar miembro\n";
        cout << "4. Mostrar PREORDEN (Antigüedad)\n";
        cout << "5. Mostrar INORDEN (Estructura lateral)\n";
        cout << "6. Mostrar POSTORDEN (Descendientes ? Ancestro)\n";
        cout << "7. Mostrar POR NIVELES\n";
        cout << "8. Estadísticas\n";
        cout << "9. Historial de operaciones\n";
		cout << "10. Eliminar miembro\n";
		cout << "11. Conteo por relacion familiar\n";	
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            cout << "Entrada inválida. Intentar de nuevo.\n";
            limpiarEntradas();
            continue;
        }
        limpiarEntradas();

        if (opcion == 0) {
            cout << "Saliendo...\n";
            break;
        }

        if (opcion == 1) {
           string nombre, genero, relacion, ocupacion, lugar;
    	int edad;
    	cout << "Nombre: "; getline(cin, nombre);
    	cout << "Edad: "; cin >> edad; limpiarEntradas();
    	cout << "Genero: "; getline(cin, genero);
    	cout << "Relación familiar: "; getline(cin, relacion);
    	cout << "Ocupacion: "; getline(cin, ocupacion);
    	cout << "Lugar de nacimiento: "; getline(cin, lugar);
    	bool ok = arbol.insertarMiembroAVL(nombre, edad, genero, relacion, ocupacion, lugar);
    	if (ok) cout << "Miembro insertado con balanceo AVL.\n";
    	else cout << "No se inserto (posible duplicado o nombre vacio).\n";
		}
        else if (opcion == 2) {
            string nombre;
            cout << "Nombre a buscar: "; getline(cin, nombre);
            Miembro* m = arbol.buscarMiembro(nombre);
            if (m) arbol.imprimirMiembroCompleto(m);
            else cout << "Miembro no encontrado.\n";
        }
        else if (opcion == 3) {
            string nombre, nuevaOcup, nuevaRel;
            int nuevaEdad;
            cout << "Nombre del miembro a modificar: "; getline(cin, nombre);
            cout << "Nueva edad: "; cin >> nuevaEdad; limpiarEntradas();
            cout << "Nueva ocupacion: "; getline(cin, nuevaOcup);
            cout << "Nueva relacion familiar: "; getline(cin, nuevaRel);
            bool ok = arbol.modificarMiembro(nombre, nuevaEdad, nuevaOcup, nuevaRel);
            if (ok) cout << "Miembro modificado correctamente.\n";
            else cout << "Miembro no encontrado.\n";
        }
        else if (opcion == 4) {
            arbol.mostrarPreorden();
        }
        else if (opcion == 5) {
            arbol.mostrarInorden();
        }
        else if (opcion == 6) {
            arbol.mostrarPostorden();
        }
        else if (opcion == 7) {
            arbol.mostrarPorNiveles();
        }
        else if (opcion == 8) {
            arbol.mostrarEstadisticasAvanzadas();
        }
        else if (opcion == 9) {
            arbol.mostrarHistorial();
        }
		else if (opcion == 10) {
    	string nombre;
    	cout << "Nombre del miembro a ELIMINAR: "; getline(cin, nombre);
    	bool ok = arbol.eliminarMiembro(nombre);
    	if (ok) cout << "Miembro eliminado y arbol rebalanceado (AVL).\n";
    	else cout << "Miembro no encontrado.\n";
		}
		else if (opcion == 11) {
    	arbol.mostrarConteoRelaciones();
		}
        else {
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    }

    return 0;
}

