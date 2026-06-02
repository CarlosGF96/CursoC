#include <iostream>

/*Realizar la práctica de creación de una lista enlazada, y su prueba*/
template <typename T>
class ListaEnlazada {
private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
        Nodo(const T& valor) : dato(valor), siguiente(nullptr) {}
    };
 
    Nodo* cabeza;
    size_t tamano;
 
public:
    ListaEnlazada() : cabeza(nullptr), tamano(0) {}
 
    ~ListaEnlazada() {
        limpiar();
    }
 
    void insertarInicio(const T& valor) {
        Nodo* nuevo = new Nodo(valor);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        ++tamano;
    }
 
    void insertarFinal(const T& valor) {
        Nodo* nuevo = new Nodo(valor);
        if (!cabeza) {
            cabeza = nuevo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente) actual = actual->siguiente;
            actual->siguiente = nuevo;
        }
        ++tamano;
    }
 
    bool eliminar(const T& valor) {
        Nodo* actual = cabeza;
        Nodo* anterior = nullptr;
        while (actual) {
            if (actual->dato == valor) {
                if (anterior) anterior->siguiente = actual->siguiente;
                else cabeza = actual->siguiente;
                delete actual;
                --tamano;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }
 
    bool contiene(const T& valor) const {
        Nodo* actual = cabeza;
        while (actual) {
            if (actual->dato == valor) return true;
            actual = actual->siguiente;
        }
        return false;
    }
 
    void limpiar() {
        while (cabeza) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
        tamano = 0;
    }
 
    void imprimir() const {
        Nodo* actual = cabeza;
        while (actual) {
            std::cout << actual->dato << " -> ";
            actual = actual->siguiente;
        }
        std::cout << "nullptr\n";
    }
 
    size_t size() const { return tamano; }
    bool vacia() const { return cabeza == nullptr; }
};
 
int main() {
    ListaEnlazada<int> lista;
    lista.insertarFinal(10);
    lista.insertarFinal(20);
    lista.insertarInicio(5);
    lista.imprimir();          // 5 -> 10 -> 20 -> nullptr
    lista.eliminar(10);
    lista.imprimir();          // 5 -> 20 -> nullptr
    std::cout << "Tamaño: " << lista.size() << "\n";
    return 0;
}