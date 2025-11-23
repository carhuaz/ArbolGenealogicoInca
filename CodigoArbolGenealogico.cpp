#include <iostream>
#include <string>
#include <queue>
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

    /* Inserción recursiva (ABB por nombre) */
    Miembro* insertarRec(Miembro* nodo, Miembro* nuevo) {
        if (nodo == NULL) return nuevo;

        if (nuevo->nombre < nodo->nombre) {
            nodo->izquierdo = insertarRec(nodo->izquierdo, nuevo);
        } else if (nuevo->nombre > nodo->nombre) {
            nodo->derecho = insertarRec(nodo->derecho, nuevo);
        } else {
            // duplicado: no insertar
            return nodo;
        }
        return nodo;
    }

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

public:
    ArbolGenealogico() {
        raiz = NULL;
        historial.clear();
    }

    /* ---------- Inserción pública (verifica duplicados) ---------- */
    bool insertarMiembro(const string &nombre, int edad,
                         const string &genero, const string &relacion,
                         const string &ocupacion, const string &lugarNacimiento)
    {
        if (nombre.size() == 0) return false;
        if (buscarMiembro(nombre) != NULL) {
            // no insertar duplicados
            return false;
        }
        Miembro* nuevo = new Miembro(nombre, edad, genero, relacion, ocupacion, lugarNacimiento);
        raiz = insertarRec(raiz, nuevo);
        historial.push_back(string("INSERTAR: ") + nombre);
        return true;
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

    /* ---------- Estadísticas (parte 1) ---------- */
    int contarMiembros() {
        return contarRec(raiz);
    }

    int profundidadArbol() {
        return profundidadRec(raiz);
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
        insertarMiembro("Manco Cápac", 70, "Masculino", "Fundador", "Sapa Inca legendario", "Titicaca");
        insertarMiembro("Sinchi Roca", 48, "Masculino", "Sucesor", "Noble", "Cusco");
        insertarMiembro("Lloque Yupanqui", 45, "Masculino", "Ancestro legendario", "Noble", "Cusco");
        insertarMiembro("Mayta Capac", 60, "Masculino", "Antepasado", "Sapa Inca", "Cusco");
        insertarMiembro("Pachacutec", 55, "Masculino", "Sapa Inca", "Emperador - Reformador", "Cusco");
        insertarmeHistorial("CARGA_INICIAL: Datos del Tahuantinsuyo insertados");
    }

    /* helper para agregar evento al historial sin repetición */
    void insertarmeHistorial(const string &s) {
        historial.push_back(s);
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
        cout << "\n--- MENÚ PRINCIPAL ---\n";
        cout << "1. Insertar nuevo miembro\n";
        cout << "2. Buscar miembro por nombre\n";
        cout << "3. Modificar miembro\n";
        cout << "4. Mostrar PREORDEN (Antigüedad)\n";
        cout << "5. Mostrar INORDEN (Estructura lateral)\n";
        cout << "6. Mostrar POSTORDEN (Descendientes ? Ancestro)\n";
        cout << "7. Mostrar POR NIVELES\n";
        cout << "8. Estadísticas (total, profundidad)\n";
        cout << "9. Historial de operaciones\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";

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
            cout << "Edad: "; cin >> edad; limpar:
            // NOTE: Dev-C++ 5.11 sometimes needs clearing after reading int
            // We'll clear the newline after reading edad
            limpiarEntradas();
            cout << "Género: "; getline(cin, genero);
            cout << "Relación familiar: "; getline(cin, relacion);
            cout << "Ocupación: "; getline(cin, ocupacion);
            cout << "Lugar de nacimiento: "; getline(cin, lugar);
            bool ok = arbol.insertarMiembro(nombre, edad, genero, relacion, ocupacion, lugar);
            if (ok) cout << "Miembro insertado correctamente.\n";
            else cout << "No se insertó (posible duplicado o nombre vacío).\n";
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
            cout << "Nueva ocupación: "; getline(cin, nuevaOcup);
            cout << "Nueva relación familiar: "; getline(cin, nuevaRel);
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
            cout << "\n=== Estadísticas ===\n";
            cout << "Total de miembros: " << arbol.contarMiembros() << "\n";
            cout << "Profundidad (generaciones aprox.): " << arbol.profundidadArbol() << "\n";
        }
        else if (opcion == 9) {
            arbol.mostrarHistorial();
        }
        else {
            cout << "Opción no válida. Intente nuevamente.\n";
        }
    }

    return 0;
}

