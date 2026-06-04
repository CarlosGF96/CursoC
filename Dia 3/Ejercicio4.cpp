#include <iostream>
#include <set>

class MiClase {
public:
    int id;

    MiClase(int param) : id(param) {}

    // El set usa este operador para ordenar y detectar duplicados
    bool operator<(const MiClase& other) const {
        return this->id < other.id;
    }

};

int main() {
    std::set<MiClase> MiSet;

    MiClase m(1);
    MiClase m1(1);
    MiClase m2(4);

    MiSet.insert(m);   // OK
    MiSet.insert(m1);  // NO se inserta (id duplicado)
    MiSet.insert(m2);  //OK

    std::cout << "Tamaño del set: " << MiSet.size() << std::endl;
}