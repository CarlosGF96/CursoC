#include <iostream>
#include <vector>
#include <memory>

class MiClase {
public:
    int id;

    // Constructor normal: se llama cuando creas un objeto "nuevo"
    MiClase(int v) : id(v) {
        std::cout << "Constructor NORMAL (" << id << ")\n";
    }

    // Constructor de copia: se llama cuando se COPIA un objeto existente
    MiClase(const MiClase& other) {
        id = other.id;
        std::cout << "Constructor COPIA (" << id << ")\n";
    }

    // Constructor de movimiento: se llama cuando se MUEVE un objeto (rvalue)
    MiClase(MiClase&& other) noexcept {
        id = other.id;
        other.id = -1; // marcamos el origen como "movido"
        std::cout << "Constructor MOVE (" << id << ")\n";
    }

    // Destructor para ver cuándo se destruyen los objetos
    ~MiClase() {
        std::cout << "Destructor (" << id << ")\n";
    }
};

// ---------------------------------------------------------
// PRUEBA 1: VECTOR usando COPIA
// ---------------------------------------------------------
void pruebaVectorCopia() {
    std::cout << "\n--- VECTOR (COPIA) ---\n";

    std::vector<MiClase> vec;

    // Se crean dos objetos normales
    MiClase a(10);  // Constructor NORMAL
    MiClase b(20);  // Constructor NORMAL

    // push_back con lvalues (a, b) → usa el CONSTRUCTOR DE COPIA
    vec.push_back(a); // Constructor COPIA (10)
    vec.push_back(b); // Constructor COPIA (20)

    // Al salir de la función:
    // - Se destruyen los elementos del vector
    // - Luego se destruyen a y b
}

// ---------------------------------------------------------
// PRUEBA 2: VECTOR usando MOVE (movimiento)
// ---------------------------------------------------------
void pruebaVectorMove() {
    std::cout << "\n--- VECTOR (MOVE) ---\n";

    std::vector<MiClase> vec;

    MiClase a(30);  // Constructor NORMAL
    MiClase b(40);  // Constructor NORMAL

    // Aquí usamos std::move(a) y std::move(b)
    // Eso convierte a y b en rvalues → se usa el CONSTRUCTOR DE MOVIMIENTO
    vec.push_back(std::move(a)); // Constructor MOVE (30)
    vec.push_back(std::move(b)); // Constructor MOVE (40)

    // a.id y b.id ahora valen -1 (los marcamos como "movidos" en el constructor MOVE)
}

// ---------------------------------------------------------
// PRUEBA 3: UNIQUE_PTR (no llama al MOVE de MiClase)
// ---------------------------------------------------------
void pruebaUniquePtr() {
    std::cout << "\n--- UNIQUE_PTR (mueve puntero, NO el objeto) ---\n";

    // Aquí se crean dos objetos MiClase en el heap
    auto p1 = std::make_unique<MiClase>(50); // Constructor NORMAL (50)
    auto p2 = std::make_unique<MiClase>(60); // Constructor NORMAL (60)

    // std::move(p1) mueve EL PUNTERO, no el objeto MiClase
    // NO se llama al constructor MOVE de MiClase
    std::unique_ptr<MiClase> destino = std::move(p1); // mueve el puntero
    destino = std::move(p2);                          // mueve el puntero otra vez

    // Los objetos MiClase se destruyen cuando los unique_ptr salen de ámbito
}

// ---------------------------------------------------------
// MAIN
// ---------------------------------------------------------
int main() {
    pruebaVectorCopia();  // NORMAL + COPIA
    pruebaVectorMove();   // NORMAL + MOVE
    pruebaUniquePtr();    // NORMAL + Destructores, sin MOVE

    return 0;
}
