#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;

/* ==============================================================
   SISTEMA DE ARBOL GENEALOGICO CON AVL
   
   CARACTERISTICAS:
   - Arbol Binario de Busqueda (ABB) con balanceo AVL
   - Insercion, busqueda, modificacion y eliminacion de miembros
   - Recorridos: Preorden, Inorden, Postorden y Por Niveles
   - Estadisticas avanzadas y conteo por relaciones
   - Sistema de validaciones completo
   - Menu organizado con submenus
   ============================================================== */

/* ---------------------------
   ESTRUCTURA DEL NODO: Miembro
   Representa a un miembro de la familia con sus atributos
   --------------------------- */
struct Miembro {
    string nombre;           // Nombre completo (clave de busqueda)
    int edad;                // Edad en anos
    string genero;           // "Masculino" o "Femenino"
    string relacionFamiliar; // Tipo de relacion (ej: "Sapa Inca", "Hijo")
    string ocupacion;        // Profesion u oficio
    string lugarNacimiento;  // Lugar de origen

    Miembro* izquierdo;      // Puntero al hijo izquierdo
    Miembro* derecho;        // Puntero al hijo derecho

    int altura;              // Altura del nodo (para balanceo AVL)

    /**
     * Constructor del nodo Miembro
     * Inicializa todos los atributos y punteros
     */
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
   CLASE: ArbolGenealogico
   Implementa un arbol AVL para gestionar miembros familiares
   --------------------------- */
class ArbolGenealogico {
private:
    Miembro* raiz;                  // Nodo raiz del arbol
    vector<string> historial;       // Registro de operaciones

    /**
     * Busca un miembro por nombre de forma recursiva
     * @param nodo Nodo actual en la recursion
     * @param nombre Nombre a buscar
     * @return Puntero al miembro encontrado o NULL
     */
    Miembro* buscarRec(Miembro* nodo, const string &nombre) {
        if (nodo == NULL) return NULL;
        if (nombre == nodo->nombre) return nodo;
        if (nombre < nodo->nombre) return buscarRec(nodo->izquierdo, nombre);
        return buscarRec(nodo->derecho, nombre);
    }

    /**
     * Recorrido Preorden: Raiz -> Izquierdo -> Derecho
     * Util para mostrar jerarquia de ancestros a descendientes
     */
    void preordenRec(Miembro* nodo, int &contador) {
        if (nodo == NULL) return;
        imprimirLineaEnumerada(nodo, contador);
        contador++;
        preordenRec(nodo->izquierdo, contador);
        preordenRec(nodo->derecho, contador);
    }

    /**
     * Recorrido Inorden: Izquierdo -> Raiz -> Derecho
     * Muestra los miembros en orden alfabetico
     */
    void inordenRec(Miembro* nodo, int &contador) {
        if (nodo == NULL) return;
        inordenRec(nodo->izquierdo, contador);
        imprimirLineaEnumerada(nodo, contador);
        contador++;
        inordenRec(nodo->derecho, contador);
    }

    /**
     * Recorrido Postorden: Izquierdo -> Derecho -> Raiz
     * Util para mostrar de descendientes hacia ancestros
     */
    void postordenRec(Miembro* nodo, int &contador) {
        if (nodo == NULL) return;
        postordenRec(nodo->izquierdo, contador);
        postordenRec(nodo->derecho, contador);
        imprimirLineaEnumerada(nodo, contador);
        contador++;
    }

    /**
     * Calcula la profundidad maxima del arbol
     * @param nodo Nodo actual
     * @return Profundidad desde el nodo hasta la hoja mas lejana
     */
    int profundidadRec(Miembro* nodo) {
        if (nodo == NULL) return 0;
        int iz = profundidadRec(nodo->izquierdo);
        int dr = profundidadRec(nodo->derecho);
        return 1 + ((iz > dr) ? iz : dr);
    }

    /**
     * Cuenta el total de nodos en el arbol
     * @param nodo Nodo actual
     * @return Cantidad de nodos desde el nodo actual
     */
    int contarRec(Miembro* nodo) {
        if (nodo == NULL) return 0;
        return 1 + contarRec(nodo->izquierdo) + contarRec(nodo->derecho);
    }

    /**
     * Imprime la cabecera de las tablas de miembros
     */
    void imprimirCabeceraTabla() {
        cout << "\n-----------------------------------------------------------------\n";
        cout << left << setw(4)  << "No."
             << left << setw(22) << "Nombre"
             << left << setw(22) << "Relacion"
             << left << setw(8)  << "Edad"
             << left << setw(20) << "Ocupacion"
             << "\n";
        cout << "-----------------------------------------------------------------\n";
    }

    /**
     * Imprime una fila con los datos de un miembro
     * @param m Puntero al miembro
     * @param num Numero de fila
     */
    void imprimirLineaEnumerada(Miembro* m, int num) {
        if (m == NULL) return;
        cout << left << setw(4)  << (toStringNum(num) + ".")
             << left << setw(22) << m->nombre
             << left << setw(22) << m->relacionFamiliar
             << left << setw(8)  << (toStringNum(m->edad) + " anos")
             << left << setw(20) << m->ocupacion
             << "\n";
    }

    /**
     * Convierte un entero a string (compatible con C++98)
     * @param x Numero a convertir
     * @return String con el numero
     */
    string toStringNum(int x) {
        char buffer[20];
        sprintf(buffer, "%d", x);
        return string(buffer);
    }

    /* ========== FUNCIONES AVL ========== */

    /**
     * Obtiene la altura de un nodo
     * @param nodo Nodo a consultar
     * @return Altura del nodo (0 si es NULL)
     */
    int obtenerAltura(Miembro* nodo) {
        if (nodo == NULL) return 0;
        return nodo->altura;
    }

    /**
     * Calcula el factor de balance de un nodo
     * Balance = Altura(izquierdo) - Altura(derecho)
     * @param nodo Nodo a evaluar
     * @return Factor de balance (-1, 0, 1 en arbol balanceado)
     */
    int obtenerBalance(Miembro* nodo) {
        if (nodo == NULL) return 0;
        return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
    }

    /**
     * Actualiza la altura de un nodo basandose en sus hijos
     * @param nodo Nodo a actualizar
     */
    void actualizarAltura(Miembro* nodo) {
        if (nodo == NULL) return;
        int altIzq = obtenerAltura(nodo->izquierdo);
        int altDer = obtenerAltura(nodo->derecho);
        nodo->altura = 1 + (altIzq > altDer ? altIzq : altDer);
    }

    /**
     * Rotacion simple a la derecha (caso LL)
     * Se usa cuando el subarbol izquierdo esta desbalanceado a la izquierda
     * @param y Nodo desbalanceado
     * @return Nueva raiz del subarbol
     */
    Miembro* rotacionDerecha(Miembro* y) {
        Miembro* x = y->izquierdo;
        Miembro* T2 = x->derecho;

        x->derecho = y;
        y->izquierdo = T2;

        actualizarAltura(y);
        actualizarAltura(x);

        return x;
    }

    /**
     * Rotacion simple a la izquierda (caso RR)
     * Se usa cuando el subarbol derecho esta desbalanceado a la derecha
     * @param x Nodo desbalanceado
     * @return Nueva raiz del subarbol
     */
    Miembro* rotacionIzquierda(Miembro* x) {
        Miembro* y = x->derecho;
        Miembro* T2 = y->izquierdo;

        y->izquierdo = x;
        x->derecho = T2;

        actualizarAltura(x);
        actualizarAltura(y);

        return y;
    }

    /**
     * Balancea un nodo aplicando las rotaciones necesarias
     * Maneja los 4 casos: LL, RR, LR, RL
     * @param nodo Nodo a balancear
     * @return Nodo balanceado
     */
    Miembro* balancear(Miembro* nodo) {
        if (nodo == NULL) return nodo;

        actualizarAltura(nodo);
        int balance = obtenerBalance(nodo);

        // Caso LL: Rotacion derecha
        if (balance > 1 && obtenerBalance(nodo->izquierdo) >= 0)
            return rotacionDerecha(nodo);

        // Caso LR: Rotacion izquierda-derecha
        if (balance > 1 && obtenerBalance(nodo->izquierdo) < 0) {
            nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
            return rotacionDerecha(nodo);
        }

        // Caso RR: Rotacion izquierda
        if (balance < -1 && obtenerBalance(nodo->derecho) <= 0)
            return rotacionIzquierda(nodo);

        // Caso RL: Rotacion derecha-izquierda
        if (balance < -1 && obtenerBalance(nodo->derecho) > 0) {
            nodo->derecho = rotacionDerecha(nodo->derecho);
            return rotacionIzquierda(nodo);
        }

        return nodo;
    }

    /**
     * Inserta un nuevo miembro en el arbol con balanceo AVL
     * @param nodo Nodo actual en la recursion
     * @param nuevo Nuevo miembro a insertar
     * @return Raiz del subarbol modificado
     */
    Miembro* insertarRecAVL(Miembro* nodo, Miembro* nuevo) {
        if (nodo == NULL) return nuevo;

        if (nuevo->nombre < nodo->nombre)
            nodo->izquierdo = insertarRecAVL(nodo->izquierdo, nuevo);
        else if (nuevo->nombre > nodo->nombre)
            nodo->derecho = insertarRecAVL(nodo->derecho, nuevo);
        else
            return nodo; // Nombre duplicado, no se inserta

        return balancear(nodo);
    }

    /**
     * Encuentra el nodo con el valor minimo (mas a la izquierda)
     * @param nodo Nodo inicial
     * @return Nodo con el valor minimo
     */
    Miembro* encontrarMinimo(Miembro* nodo) {
        while (nodo->izquierdo != NULL) 
            nodo = nodo->izquierdo;
        return nodo;
    }

    /**
     * Elimina un miembro del arbol y rebalancea
     * @param nodo Nodo actual
     * @param nombre Nombre del miembro a eliminar
     * @param eliminado Bandera que indica si se elimino
     * @return Raiz del subarbol modificado
     */
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

            // Caso 1: Nodo sin hijo derecho
            if (nodo->izquierdo == NULL) {
                Miembro* temp = nodo->derecho;
                delete nodo;
                return temp;
            } 
            // Caso 2: Nodo sin hijo izquierdo
            else if (nodo->derecho == NULL) {
                Miembro* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }

            // Caso 3: Nodo con dos hijos
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

    /* ========== FUNCIONES DE ESTADISTICAS ========== */

    /**
     * Suma las edades de todos los miembros
     * @param nodo Nodo actual
     * @param suma Acumulador de la suma
     * @param contador Contador de miembros
     */
    void sumarEdades(Miembro* nodo, int &suma, int &contador) {
        if (nodo == NULL) return;
        suma += nodo->edad;
        contador++;
        sumarEdades(nodo->izquierdo, suma, contador);
        sumarEdades(nodo->derecho, suma, contador);
    }

    /**
     * Encuentra la edad maxima en el arbol
     * @param nodo Nodo actual
     * @param maxEdad Variable que almacena el maximo
     */
    void encontrarEdadMaxima(Miembro* nodo, int &maxEdad) {
        if (nodo == NULL) return;
        if (nodo->edad > maxEdad) maxEdad = nodo->edad;
        encontrarEdadMaxima(nodo->izquierdo, maxEdad);
        encontrarEdadMaxima(nodo->derecho, maxEdad);
    }

    /**
     * Encuentra la edad minima en el arbol
     * @param nodo Nodo actual
     * @param minEdad Variable que almacena el minimo
     */
    void encontrarEdadMinima(Miembro* nodo, int &minEdad) {
        if (nodo == NULL) return;
        if (nodo->edad < minEdad) minEdad = nodo->edad;
        encontrarEdadMinima(nodo->izquierdo, minEdad);
        encontrarEdadMinima(nodo->derecho, minEdad);
    }

    /**
     * Cuenta miembros con una relacion familiar especifica
     * @param nodo Nodo actual
     * @param relacion Relacion a buscar
     * @param contador Acumulador de coincidencias
     */
    void contarPorRelacion(Miembro* nodo, const string &relacion, int &contador) {
        if (nodo == NULL) return;
        if (nodo->relacionFamiliar == relacion) contador++;
        contarPorRelacion(nodo->izquierdo, relacion, contador);
        contarPorRelacion(nodo->derecho, relacion, contador);
    }

    /**
     * Verifica si el arbol cumple las propiedades AVL
     * @param nodo Nodo actual
     * @return true si esta balanceado, false en caso contrario
     */
    bool esAVLBalanceado(Miembro* nodo) {
        if (nodo == NULL) return true;
        int balance = obtenerBalance(nodo);
        if (balance < -1 || balance > 1) return false;
        return esAVLBalanceado(nodo->izquierdo) && esAVLBalanceado(nodo->derecho);
    }

/* ========== FUNCIONES DE VISUALIZACION DEL DIAGRAMA ========== */

    /**
     * Obtiene el nombre abreviado para el diagrama (primeras letras)
     * @param nombre Nombre completo
     * @return Nombre abreviado (max 3 caracteres)
     */
    string obtenerAbreviatura(const string &nombre) {
        if (nombre.length() <= 3) return nombre;
        
        string abrev = "";
        bool tomarLetra = true;
        
        for (size_t i = 0; i < nombre.length() && abrev.length() < 3; i++) {
            if (nombre[i] == ' ') {
                tomarLetra = true;
            } else if (tomarLetra && nombre[i] != ' ') {
                abrev += nombre[i];
                tomarLetra = false;
            }
        }
        
        if (abrev.length() == 0) abrev = nombre.substr(0, 3);
        return abrev;
    }

    /**
     * Imprime el arbol de forma horizontal recursiva
     * @param nodo Nodo actual
     * @param espacio Espaciado para la indentacion
     * @param esIzquierdo Indica si es hijo izquierdo o derecho
     */
    void imprimirArbolHorizontal(Miembro* nodo, string espacio, bool esIzquierdo) {
        if (nodo == NULL) return;

        // Procesar primero el subarbol derecho (arriba en la visualizacion)
        if (nodo->derecho != NULL) {
            imprimirArbolHorizontal(nodo->derecho, espacio + "        ", false);
        }

        // Imprimir el nodo actual
        cout << espacio;
        if (espacio != "") {
            cout << (esIzquierdo ? "\\_____ " : "/_____ ");
        }
        cout << "[" << obtenerAbreviatura(nodo->nombre) << "]" << endl;

        // Procesar el subarbol izquierdo (abajo en la visualizacion)
        if (nodo->izquierdo != NULL) {
            imprimirArbolHorizontal(nodo->izquierdo, espacio + "        ", true);
        }
    }

    /**
     * Estructura para almacenar informacion de posicion de nodos
     */
    struct NodoPosicion {
        Miembro* nodo;
        int x, y;
        NodoPosicion(Miembro* n, int px, int py) : nodo(n), x(px), y(py) {}
    };

    /**
     * Imprime el diagrama del arbol de forma vertical (mejorado)
     */
    void imprimirDiagramaVertical() {
        if (raiz == NULL) {
            cout << "\n(Arbol vacio)\n";
            return;
        }

        const int ANCHO_CELDA = 8;
        const int ALTURA_NIVEL = 3;
        
        // Calcular dimensiones
        int profundidad = profundidadRec(raiz);
        int anchoTotal = (1 << profundidad) * ANCHO_CELDA;
        int altoTotal = profundidad * ALTURA_NIVEL + 2;

        // Crear matriz de caracteres
        vector<string> canvas(altoTotal, string(anchoTotal, ' '));

        // Cola para BFS con posiciones
        queue<NodoPosicion> cola;
        int posicionInicial = anchoTotal / 2;
        cola.push(NodoPosicion(raiz, posicionInicial, 0));

        // Procesar nodos por nivel
        while (!cola.empty()) {
            NodoPosicion actual = cola.front();
            cola.pop();

            if (actual.nodo == NULL) continue;

            // Dibujar el nodo
            string abrev = obtenerAbreviatura(actual.nodo->nombre);
            int y = actual.y * ALTURA_NIVEL;
            int x = actual.x;

            // Dibujar el circulo con el nombre
            if (y < altoTotal && x >= 2 && x + 4 < anchoTotal) {
                canvas[y][x-2] = '.';
                canvas[y][x-1] = '-';
                canvas[y][x] = '-';
                canvas[y][x+1] = '-';
                canvas[y][x+2] = '.';
                
                if (y+1 < altoTotal) {
                    canvas[y+1][x-2] = '|';
                    for (size_t i = 0; i < abrev.length() && i < 3; i++) {
                        if (x-1+i < anchoTotal) {
                            canvas[y+1][x-1+i] = abrev[i];
                        }
                    }
                    canvas[y+1][x+2] = '|';
                }
                
                if (y+2 < altoTotal) {
                    canvas[y+2][x-2] = '\'';
                    canvas[y+2][x-1] = '-';
                    canvas[y+2][x] = '-';
                    canvas[y+2][x+1] = '-';
                    canvas[y+2][x+2] = '\'';
                }
            }

            // Calcular posiciones de los hijos
            int separacion = anchoTotal / (1 << (actual.y + 2));
            
            // Dibujar conexiones y agregar hijos a la cola
            if (actual.nodo->izquierdo != NULL) {
                int xIzq = x - separacion;
                int yHijo = (actual.y + 1) * ALTURA_NIVEL;
                
                // Dibujar linea diagonal hacia la izquierda
                if (y+3 < altoTotal && xIzq < anchoTotal) {
                    int pasos = (xIzq < x) ? (x - xIzq) : (xIzq - x);
                    for (int i = 1; i <= pasos && y+2+i < altoTotal; i++) {
                        int posX = x - i;
                        if (posX >= 0 && posX < anchoTotal) {
                            canvas[y+2+i][posX] = '/';
                        }
                    }
                }
                
                cola.push(NodoPosicion(actual.nodo->izquierdo, xIzq, actual.y + 1));
            }

            if (actual.nodo->derecho != NULL) {
                int xDer = x + separacion;
                int yHijo = (actual.y + 1) * ALTURA_NIVEL;
                
                // Dibujar linea diagonal hacia la derecha
                if (y+3 < altoTotal && xDer < anchoTotal) {
                    int pasos = (xDer > x) ? (xDer - x) : (x - xDer);
                    for (int i = 1; i <= pasos && y+2+i < altoTotal; i++) {
                        int posX = x + i;
                        if (posX >= 0 && posX < anchoTotal) {
                            canvas[y+2+i][posX] = '\\';
                        }
                    }
                }
                
                cola.push(NodoPosicion(actual.nodo->derecho, xDer, actual.y + 1));
            }
        }

        // Imprimir el canvas
        for (size_t i = 0; i < canvas.size(); i++) {
            cout << canvas[i] << endl;
        }
    }

public:
    /**
     * Constructor: Inicializa el arbol vacio
     */
    ArbolGenealogico() {
        raiz = NULL;
        historial.clear();
    }

    /**
     * Busca un miembro por nombre
     * @param nombre Nombre a buscar
     * @return Puntero al miembro o NULL si no existe
     */
    Miembro* buscarMiembro(const string &nombre) {
        return buscarRec(raiz, nombre);
    }

    /**
     * Modifica los datos de un miembro existente
     * @param nombre Nombre del miembro
     * @param nuevaEdad Nueva edad
     * @param nuevaOcupacion Nueva ocupacion
     * @param nuevaRelacion Nueva relacion familiar
     * @return true si se modifico, false si no existe
     */
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

    /**
     * Muestra todos los datos de un miembro
     * @param m Puntero al miembro
     */
    void imprimirMiembroCompleto(Miembro* m) {
        if (m == NULL) {
            cout << "(Miembro nulo)\n";
            return;
        }
        cout << "\n----------------- FICHA DEL MIEMBRO -----------------\n";
        cout << "Nombre: " << m->nombre << "\n";
        cout << "Edad: "  << m->edad << " anos\n";
        cout << "Genero: " << m->genero << "\n";
        cout << "Relacion familiar: " << m->relacionFamiliar << "\n";
        cout << "Ocupacion: " << m->ocupacion << "\n";
        cout << "Lugar de nacimiento: " << m->lugarNacimiento << "\n";
        cout << "Altura (campo para AVL): " << m->altura << "\n";
        cout << "----------------------------------------------------\n";
    }

    /* ========== RECORRIDOS DEL ARBOL ========== */

    /**
     * Muestra el recorrido en Preorden
     * Util para ver la jerarquia de ancestros a descendientes
     */
    void mostrarPreorden() {
        cout << "\n=== RECORRIDO PREORDEN (Antiguedad: ancestros -> descendientes) ===\n";
        cout << "Explicacion: Preorden visita la raiz primero, por eso es util\n";
        cout << "             para presentar la genealogia del ancestro hacia\n";
        cout << "             sus descendientes en orden temporal.\n";
        imprimirCabeceraTabla();
        int contador = 1;
        preordenRec(raiz, contador);
        cout << "-----------------------------------------------------------------\n";
    }

    /**
     * Muestra el recorrido Inorden
     * Muestra los miembros en orden alfabetico
     */
    void mostrarInorden() {
        cout << "\n=== RECORRIDO INORDEN (Orden alfabetico) ===\n";
        cout << "Explicacion: Inorden muestra los miembros ordenados\n";
        cout << "             alfabeticamente por nombre.\n";
        imprimirCabeceraTabla();
        int contador = 1;
        inordenRec(raiz, contador);
        cout << "-----------------------------------------------------------------\n";
    }

    /**
     * Muestra el recorrido Postorden
     * Util para ver de descendientes hacia ancestros
     */
    void mostrarPostorden() {
        cout << "\n=== RECORRIDO POSTORDEN (Descendientes -> Ancestro) ===\n";
        cout << "Explicacion: Postorden visita los descendientes antes que el\n";
        cout << "             ancestro; es util para ver generaciones recientes\n";
        cout << "             hacia los mas antiguos.\n";
        imprimirCabeceraTabla();
        int contador = 1;
        postordenRec(raiz, contador);
        cout << "-----------------------------------------------------------------\n";
    }

    /**
     * Muestra el recorrido Por Niveles (BFS)
     * Muestra miembros por distancia a la raiz
     */
    void mostrarPorNiveles() {
        cout << "\n=== RECORRIDO POR NIVELES (Generaciones aproximadas) ===\n";
        cout << "Explicacion: Por niveles muestra nodos por distancia a la raiz\n";
        cout << "             (nivel 0 = raiz, nivel 1 = hijos, etc.).\n";
        imprimirCabeceraTabla();
        if (raiz == NULL) {
            cout << "(arbol vacio)\n";
            cout << "-----------------------------------------------------------------\n";
            return;
        }
        queue<Miembro*> q;
        q.push(raiz);
        int contador = 1;
        while (!q.empty()) {
            Miembro* cur = q.front(); 
            q.pop();
            imprimirLineaEnumerada(cur, contador);
            contador++;
            if (cur->izquierdo) q.push(cur->izquierdo);
            if (cur->derecho) q.push(cur->derecho);
        }
        cout << "-----------------------------------------------------------------\n";
    }

    /**
     * Muestra el historial de operaciones realizadas
     */
    void mostrarHistorial() {
        cout << "\n=== HISTORIAL DE OPERACIONES ===\n";
        if (historial.empty()) {
            cout << "(vacio)\n";
        } else {
            for (size_t i = 0; i < historial.size(); ++i)
                cout << (i+1) << ". " << historial[i] << "\n";
        }
        cout << "=================================\n";
    }

    /**
     * Carga datos de ejemplo del Tahuantinsuyo
     */
    void cargarDatosEjemplo() {
        insertarMiembroAVL("Manco Capac", 70, "Masculino", "Fundador", "Sapa Inca legendario", "Titicaca");
        insertarMiembroAVL("Sinchi Roca", 48, "Masculino", "Sucesor", "Noble", "Cusco");
        insertarMiembroAVL("Lloque Yupanqui", 45, "Masculino", "Ancestro legendario", "Noble", "Cusco");
        insertarMiembroAVL("Mayta Capac", 60, "Masculino", "Antepasado", "Sapa Inca", "Cusco");
        insertarMiembroAVL("Pachacutec", 55, "Masculino", "Sapa Inca", "Emperador - Reformador", "Cusco");
        insertarMiembroAVL("Tupac Yupanqui", 50, "Masculino", "Hijo", "Emperador", "Cusco");
        insertarMiembroAVL("Huayna Capac", 45, "Masculino", "Nieto", "Ultimo gran Sapa Inca", "Cusco");

        insertarmeHistorial("CARGA_INICIAL: Datos del Tahuantinsuyo insertados");
    }

    /**
     * Agrega una entrada al historial
     * @param s Texto a agregar
     */
    void insertarmeHistorial(const string &s) {
        historial.push_back(s);
    }

    /**
     * Inserta un nuevo miembro con balanceo AVL
     * @return true si se inserto, false si ya existe o datos invalidos
     */
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

    /**
     * Elimina un miembro del arbol
     * @param nombre Nombre del miembro a eliminar
     * @return true si se elimino, false si no existe
     */
    bool eliminarMiembro(const string &nombre) {
        if (nombre.empty()) return false;
        bool eliminado = false;
        raiz = eliminarRec(raiz, nombre, eliminado);
        if (eliminado) historial.push_back(string("ELIMINAR: ") + nombre);
        return eliminado;
    }

    /**
     * Muestra estadisticas avanzadas del arbol
     */
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
        cout << "Edad promedio: " << promedio << " anos\n";

        int maxEdad = 0;
        encontrarEdadMaxima(raiz, maxEdad);
        cout << "Edad maxima: " << maxEdad << " anos\n";

        int minEdad = 999;
        encontrarEdadMinima(raiz, minEdad);
        cout << "Edad minima: " << minEdad << " anos\n";

        cout << "Profundidad del arbol: " << profundidadRec(raiz) << " niveles\n";

        bool balanceado = esAVLBalanceado(raiz);
        cout << "Estado AVL: " << (balanceado ? "BALANCEADO" : "DESBALANCEADO") << "\n";

        cout << "===========================================\n";
    }

    /**
     * Muestra conteo de miembros por relacion familiar
     */
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

    /**
     * Muestra el diagrama visual del arbol
     */
    void mostrarDiagramaArbol() {
        cout << "\n+---------------------------------------------------------------+\n";
        cout << "¦       DIAGRAMA VISUAL DEL ARBOL GENEALOGICO (AVL)            ¦\n";
        cout << "+---------------------------------------------------------------+\n\n";
        
        if (raiz == NULL) {
            cout << "  (Arbol vacio - No hay miembros registrados)\n\n";
            return;
        }

        cout << "Leyenda: [Abr] = Abreviatura del nombre\n";
        cout << "         /     = Rama izquierda (menor alfabeticamente)\n";
        cout << "         \\     = Rama derecha (mayor alfabeticamente)\n\n";
        
        cout << "VISTA HORIZONTAL (izquierda=arriba, derecha=abajo):\n";
        cout << "---------------------------------------------------\n";
        imprimirArbolHorizontal(raiz, "", false);
        
        cout << "\n\nVISTA VERTICAL (estructura de arbol):\n";
        cout << "--------------------------------------\n";
        imprimirDiagramaVertical();
        
        cout << "\n---------------------------------------------------------------\n";
        cout << "Estadisticas: " << contarRec(raiz) << " miembros, ";
        cout << "Profundidad: " << profundidadRec(raiz) << " niveles\n";
        cout << "Balance AVL: " << (esAVLBalanceado(raiz) ? "CORRECTO" : "REQUIERE AJUSTE") << "\n";
        cout << "---------------------------------------------------------------\n";
    }
};

/* ========== FUNCIONES DE VALIDACION ========== */

/**
 * Limpia el buffer de entrada
 */
void limpiarEntradas() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * Valida que una cadena no este vacia
 * @param texto Texto a validar
 * @return true si es valido
 */
bool validarTextoNoVacio(const string &texto) {
    if (texto.empty()) {
        cout << "ERROR: El campo no puede estar vacio.\n";
        return false;
    }
    // Verificar que no sea solo espacios
    bool soloEspacios = true;
    for (size_t i = 0; i < texto.length(); i++) {
        if (texto[i] != ' ' && texto[i] != '\t') {
            soloEspacios = false;
            break;
        }
    }
    if (soloEspacios) {
        cout << "ERROR: El campo no puede contener solo espacios.\n";
        return false;
    }
    return true;
}

/**
 * Valida que la edad este en un rango razonable
 * @param edad Edad a validar
 * @return true si es valido
 */
bool validarEdad(int edad) {
    if (edad < 0 || edad > 150) {
        cout << "ERROR: La edad debe estar entre 0 y 150 anos.\n";
        return false;
    }
    return true;
}

/**
 * Valida que el genero sea Masculino o Femenino
 * @param genero Genero a validar
 * @return true si es valido
 */
bool validarGenero(const string &genero) {
    if (genero != "Masculino" && genero != "Femenino") {
        cout << "ERROR: El genero debe ser 'Masculino' o 'Femenino'.\n";
        return false;
    }
    return true;
}

/**
 * Lee un numero entero con validacion
 * @param prompt Mensaje a mostrar
 * @return Numero ingresado valido
 */
int leerEntero(const string &prompt) {
    int valor;
    while (true) {
        cout << prompt;
        if (cin >> valor) {
            limpiarEntradas();
            return valor;
        } else {
            cout << "ERROR: Debe ingresar un numero entero valido.\n";
            limpiarEntradas();
        }
    }
}

/**
 * Lee una cadena con validacion de no vacio
 * @param prompt Mensaje a mostrar
 * @return Cadena ingresada valida
 */
string leerTexto(const string &prompt) {
    string texto;
    while (true) {
        cout << prompt;
        getline(cin, texto);
        if (validarTextoNoVacio(texto)) {
            return texto;
        }
    }
}

/**
 * Lee y valida una edad
 * @param prompt Mensaje a mostrar
 * @return Edad valida
 */
int leerEdad(const string &prompt) {
    int edad;
    while (true) {
        edad = leerEntero(prompt);
        if (validarEdad(edad)) {
            return edad;
        }
    }
}

/**
 * Lee y valida un genero
 * @param prompt Mensaje a mostrar
 * @return Genero valido
 */
string leerGenero(const string &prompt) {
    string genero;
    while (true) {
        cout << prompt << " (Masculino/Femenino): ";
        getline(cin, genero);
        if (validarGenero(genero)) {
            return genero;
        }
    }
}

/**
 * Pausa la ejecucion hasta que el usuario presione Enter
 */
void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

/**
 * Limpia la pantalla (compatible con Windows y Linux)
 */
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* ========== MENUS DEL SISTEMA ========== */

/**
 * Muestra el submenu de recorridos
 * @param arbol Referencia al arbol genealogico
 */
void submenuRecorridos(ArbolGenealogico &arbol) {
    int opcion;
    do {
        limpiarPantalla();
        cout << "\n+----------------------------------------+\n";
        cout << "?     SUBMENU: RECORRIDOS DEL ARBOL      ?\n";
        cout << "+----------------------------------------+\n";
        cout << "  1. Recorrido PREORDEN\n";
        cout << "  2. Recorrido INORDEN (Orden alfabetico)\n";
        cout << "  3. Recorrido POSTORDEN\n";
        cout << "  4. Recorrido POR NIVELES\n";
        cout << "  0. Volver al menu principal\n";
        cout << "-----------------------------------------\n";
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch(opcion) {
            case 1:
                arbol.mostrarPreorden();
                pausar();
                break;
            case 2:
                arbol.mostrarInorden();
                pausar();
                break;
            case 3:
                arbol.mostrarPostorden();
                pausar();
                break;
            case 4:
                arbol.mostrarPorNiveles();
                pausar();
                break;
            case 0:
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "ERROR: Opcion no valida.\n";
                pausar();
        }
    } while (opcion != 0);
}

/**
 * Muestra el submenu de estadisticas
 * @param arbol Referencia al arbol genealogico
 */
void submenuEstadisticas(ArbolGenealogico &arbol) {
    int opcion;
    do {
        limpiarPantalla();
        cout << "\n+----------------------------------------+\n";
        cout << "?      SUBMENU: ESTADISTICAS             ?\n";
        cout << "+----------------------------------------+\n";
        cout << "  1. Estadisticas avanzadas\n";
        cout << "  2. Conteo por relacion familiar\n";
        cout << "  3. Historial de operaciones\n";
        cout << "  0. Volver al menu principal\n";
        cout << "-----------------------------------------\n";
        
        opcion = leerEntero("Seleccione una opcion: ");
        
        switch(opcion) {
            case 1:
                arbol.mostrarEstadisticasAvanzadas();
                pausar();
                break;
            case 2:
                arbol.mostrarConteoRelaciones();
                pausar();
                break;
            case 3:
                arbol.mostrarHistorial();
                pausar();
                break;
            case 0:
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "ERROR: Opcion no valida.\n";
                pausar();
        }
    } while (opcion != 0);
}

/**
 * Funcion para insertar un nuevo miembro con validaciones
 * @param arbol Referencia al arbol genealogico
 */
void insertarNuevoMiembro(ArbolGenealogico &arbol) {
    limpiarPantalla();
    cout << "\n+----------------------------------------+\n";
    cout << "?      INSERTAR NUEVO MIEMBRO            ?\n";
    cout << "+----------------------------------------+\n\n";
    
    string nombre = leerTexto("Nombre completo: ");
    
    // Verificar si ya existe
    if (arbol.buscarMiembro(nombre) != NULL) {
        cout << "\nERROR: Ya existe un miembro con ese nombre.\n";
        pausar();
        return;
    }
    
    int edad = leerEdad("Edad: ");
    string genero = leerGenero("Genero");
    string relacion = leerTexto("Relacion familiar: ");
    string ocupacion = leerTexto("Ocupacion: ");
    string lugar = leerTexto("Lugar de nacimiento: ");
    
    bool ok = arbol.insertarMiembroAVL(nombre, edad, genero, relacion, ocupacion, lugar);
    if (ok) {
        cout << "\n? Miembro insertado exitosamente con balanceo AVL.\n";
    } else {
        cout << "\nERROR: No se pudo insertar el miembro.\n";
    }
    pausar();
}

/**
 * Funcion para buscar un miembro
 * @param arbol Referencia al arbol genealogico
 */
void buscarMiembroPorNombre(ArbolGenealogico &arbol) {
    limpiarPantalla();
    cout << "\n+----------------------------------------+\n";
    cout << "?       BUSCAR MIEMBRO POR NOMBRE        ?\n";
    cout << "+----------------------------------------+\n\n";
    
    string nombre = leerTexto("Nombre a buscar: ");
    Miembro* m = arbol.buscarMiembro(nombre);
    
    if (m) {
        arbol.imprimirMiembroCompleto(m);
    } else {
        cout << "\nMiembro no encontrado en el arbol.\n";
    }
    pausar();
}

/**
 * Funcion para modificar un miembro existente
 * @param arbol Referencia al arbol genealogico
 */
void modificarMiembroExistente(ArbolGenealogico &arbol) {
    limpiarPantalla();
    cout << "\n+----------------------------------------+\n";
    cout << "?        MODIFICAR MIEMBRO               ?\n";
    cout << "+----------------------------------------+\n\n";
    
    string nombre = leerTexto("Nombre del miembro a modificar: ");
    Miembro* m = arbol.buscarMiembro(nombre);
    
    if (!m) {
        cout << "\nERROR: Miembro no encontrado.\n";
        pausar();
        return;
    }
    
    cout << "\n--- Datos actuales ---\n";
    arbol.imprimirMiembroCompleto(m);
    
    cout << "\n--- Ingrese los nuevos datos ---\n";
    int nuevaEdad = leerEdad("Nueva edad: ");
    string nuevaOcupacion = leerTexto("Nueva ocupacion: ");
    string nuevaRelacion = leerTexto("Nueva relacion familiar: ");
    
    bool ok = arbol.modificarMiembro(nombre, nuevaEdad, nuevaOcupacion, nuevaRelacion);
    if (ok) {
        cout << "\n? Miembro modificado correctamente.\n";
    } else {
        cout << "\nERROR: No se pudo modificar el miembro.\n";
    }
    pausar();
}

/**
 * Funcion para eliminar un miembro
 * @param arbol Referencia al arbol genealogico
 */
void eliminarMiembroDelArbol(ArbolGenealogico &arbol) {
    limpiarPantalla();
    cout << "\n+----------------------------------------+\n";
    cout << "?         ELIMINAR MIEMBRO               ?\n";
    cout << "+----------------------------------------+\n\n";
    
    string nombre = leerTexto("Nombre del miembro a ELIMINAR: ");
    Miembro* m = arbol.buscarMiembro(nombre);
    
    if (!m) {
        cout << "\nERROR: Miembro no encontrado.\n";
        pausar();
        return;
    }
    
    arbol.imprimirMiembroCompleto(m);
    cout << "\n?Esta seguro de eliminar este miembro? (S/N): ";
    string confirmacion;
    getline(cin, confirmacion);
    
    if (confirmacion == "S" || confirmacion == "s") {
        bool ok = arbol.eliminarMiembro(nombre);
        if (ok) {
            cout << "\n? Miembro eliminado y arbol rebalanceado (AVL).\n";
        } else {
            cout << "\nERROR: No se pudo eliminar el miembro.\n";
        }
    } else {
        cout << "\nEliminacion cancelada.\n";
    }
    pausar();
}

/* ========== MENU PRINCIPAL ========== */

int main() {
    ArbolGenealogico arbol;
    int opcion = -1;

    limpiarPantalla();
    cout << "+-------------------------------------------------------+\n";
    cout << "?   SISTEMA DE ARBOL GENEALOGICO CON BALANCEO AVL       ?\n";
    cout << "?           Tahuantinsuyo - Imperio Inca                ?\n";
    cout << "+-------------------------------------------------------+\n\n";
    cout << "Cargando datos de ejemplo...\n";
    arbol.cargarDatosEjemplo();
    cout << "? Datos cargados exitosamente.\n";
    pausar();

    while (true) {
        limpiarPantalla();
        cout << "\n+----------------------------------------+\n";
        cout << "|         MENU PRINCIPAL                 |\n";
        cout << "+----------------------------------------+\n";
        cout << "  1. Insertar nuevo miembro\n";
        cout << "  2. Buscar miembro por nombre\n";
        cout << "  3. Modificar miembro\n";
        cout << "  4. Eliminar miembro\n";
        cout << "  5. Recorridos del arbol [SUBMENU]\n";
        cout << "  6. Estadisticas [SUBMENU]\n";
        cout << "  7. Mostrar diagrama del arbol\n";
        cout << "  0. Salir del sistema\n";
        cout << "-----------------------------------------\n";

        opcion = leerEntero("Seleccione una opcion: ");

        switch(opcion) {
            case 0:
                limpiarPantalla();
                cout << "\n+----------------------------------------+\n";
                cout << "|  Gracias por usar el sistema.          |\n";
                cout << "|  ¡Hasta pronto!                        |\n";
                cout << "+----------------------------------------+\n";
                return 0;
                
            case 1:
                insertarNuevoMiembro(arbol);
                break;
                
            case 2:
                buscarMiembroPorNombre(arbol);
                break;
                
            case 3:
                modificarMiembroExistente(arbol);
                break;
                
            case 4:
                eliminarMiembroDelArbol(arbol);
                break;
                
            case 5:
                submenuRecorridos(arbol);
                break;
                
            case 6:
                submenuEstadisticas(arbol);
                break;
                
            case 7:
                arbol.mostrarDiagramaArbol();
                pausar();
                break;
                
            default:
                cout << "\nERROR: Opcion no valida. Intente nuevamente.\n";
                pausar();
        }
    }

    return 0;
}
