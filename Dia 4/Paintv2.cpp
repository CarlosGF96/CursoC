#include "Paintv2.h"

// Alias de tipo: Constructor es FabricaFiguras::Constructor
using Constructor = FabricaFiguras::Constructor;

// Añade una figura al canvas
void Canvas::Add(std::unique_ptr<IFigura> figura) {
    // Se mueve el unique_ptr al vector, transfiriendo la propiedad
    figuras_.push_back(std::move(figura));
}

// Extrae una figura concreta del canvas
std::unique_ptr<IFigura> Canvas::Extraer(IFigura* figura) {
    // Recorre el vector de unique_ptr
    for (auto it = figuras_.begin(); it != figuras_.end(); ++it) {
        // it->get() devuelve el puntero crudo que posee el unique_ptr
        if (it->get() == figura) {
            // Mueve el unique_ptr fuera del vector (transferencia de propiedad)
            std::unique_ptr<IFigura> extraida = std::move(*it);
            // Elimina la posición del vector (ya no tiene la figura)
            figuras_.erase(it);
            // Devuelve la figura extraída
            return extraida;
        }
    }
    // Si no se encuentra, devuelve nullptr
    return nullptr;
}

// Devuelve un puntero crudo a la figura en el índice dado
IFigura* Canvas::ObtenerFigura(size_t indice) const {
    // Comprueba que el índice sea válido
    if (indice < figuras_.size())
        // Devuelve el puntero crudo sin transferir propiedad
        return figuras_[indice].get();
    return nullptr;
}

// Pide a todas las figuras que se impriman usando el escritor
void Canvas::ImprimirFiguras(IEscritor& escritor) const {
    // Recorre todas las figuras del canvas
    for (const auto& figura : figuras_)
        // Llama al método Imprimir de cada figura
        figura->Imprimir(escritor);
}

// Devuelve el mapa estático de registro de constructores
std::map<std::string, Constructor>& FabricaFiguras::Registro() {
    // Mapa estático: se crea una sola vez y se reutiliza
    static std::map<std::string, Constructor> registro;
    return registro;
}

// Registra un nuevo tipo de figura en la fábrica
void FabricaFiguras::Registrar(const std::string& nombre, Constructor ctor) {
    // Inserta o reemplaza la entrada en el mapa: nombre -> constructor
    Registro()[nombre] = std::move(ctor);
}

// Crea una figura a partir de una línea de texto
std::unique_ptr<IFigura> FabricaFiguras::Crear(const std::string& linea) {
    // Convierte la línea en un flujo de entrada para extraer tokens
    std::istringstream entrada(linea);
    std::string tipo;
    // Lee el primer token, que es el tipo de figura (ej: "cuadrado")
    entrada >> tipo;

    // Busca en el registro el constructor asociado a ese tipo
    auto it = Registro().find(tipo);
    if (it == Registro().end()) {
        // Si no existe, lanza una excepción
        throw std::runtime_error("Figura desconocida: " + tipo);
    }

    // Llama al constructor almacenado, pasándole el stream para que lea el resto de parámetros
    return it->second(entrada);
}

// Ejecuta un comando y lo guarda en la pila de hechos
void GestorComandos::Ejecutar(std::unique_ptr<ICommand> comando) {
    // Ejecuta la acción del comando
    comando->Ejecutar();
    // Mueve el comando a la lista de hechos (ya ejecutados)
    hechos_.push_back(std::move(comando));
    // Al ejecutar un nuevo comando, se limpia la pila de deshechos (no tiene sentido rehacer)
    deshechos_.clear();
}

// Deshace el último comando ejecutado
void GestorComandos::Deshacer() {
    // Si no hay comandos ejecutados, no hace nada
    if (hechos_.empty())
        return;

    // Toma el último comando de la pila de hechos, moviéndolo
    auto comando = std::move(hechos_.back());
    // Lo elimina de la pila de hechos
    hechos_.pop_back();
    // Llama a su método Deshacer
    comando->Deshacer();
    // Lo mueve a la pila de deshechos (para poder rehacerlo luego)
    deshechos_.push_back(std::move(comando));
}

// Rehace el último comando deshecho
void GestorComandos::Rehacer() {
    // Si no hay comandos deshechos, no hace nada
    if (deshechos_.empty())
        return;

    // Toma el último comando de la pila de deshechos
    auto comando = std::move(deshechos_.back());
    // Lo elimina de la pila de deshechos
    deshechos_.pop_back();
    // Vuelve a ejecutar el comando
    comando->Ejecutar();
    // Lo pasa de nuevo a la pila de hechos
    hechos_.push_back(std::move(comando));
}

// Implementación de EscritorConsola: escribe en std::cout
void EscritorConsola::Escribir(const std::string& linea) {
    std::cout << linea << "\n";
}

// Constructor de Cuadrado: inicializa miembros
Cuadrado::Cuadrado(int x, int y, double lado, int color)
    : x_(x), y_(y), lado_(lado), color_(color) {}

// Área del cuadrado: lado^2
double Cuadrado::Area() const {
    return lado_ * lado_;
}

// Devuelve el color
int Cuadrado::Color() const {
    return color_;
}

// Imprime la información del cuadrado
void Cuadrado::Imprimir(IEscritor& escritor) const {
    escritor.Escribir("Cuadrado | x: " + std::to_string(x_) +
                      " | y: " + std::to_string(y_) +
                      " | area: " + std::to_string(Area()) +
                      " | color: " + std::to_string(color_));
}

// Mueve el cuadrado a una nueva posición
void Cuadrado::Mover(int x, int y) {
    x_ = x;
    y_ = y;
}

// Devuelve X
int Cuadrado::X() const {
    return x_;
}

// Devuelve Y
int Cuadrado::Y() const {
    return y_;
}

// Clona el cuadrado creando una copia dinámica
std::unique_ptr<IFigura> Cuadrado::Clone() const {
    // Usa el constructor de copia de Cuadrado
    return std::make_unique<Cuadrado>(*this);
}

// Constructor de Circulo
Circulo::Circulo(int x, int y, double radio, int color)
    : x_(x), y_(y), radio_(radio), color_(color) {}

// Área del círculo: pi * r^2
double Circulo::Area() const {
    return kPi * radio_ * radio_;
}

// Devuelve el color
int Circulo::Color() const {
    return color_;
}

// Imprime la información del círculo
void Circulo::Imprimir(IEscritor& escritor) const {
    escritor.Escribir("Circulo  | x: " + std::to_string(x_) +
                      " | y: " + std::to_string(y_) +
                      " | area: " + std::to_string(Area()) +
                      " | color: " + std::to_string(color_));
}

// Mueve el círculo a una nueva posición
void Circulo::Mover(int x, int y) {
    x_ = x;
    y_ = y;
}

// Devuelve X
int Circulo::X() const {
    return x_;
}

// Devuelve Y
int Circulo::Y() const {
    return y_;
}

// Clona el círculo creando una copia dinámica
std::unique_ptr<IFigura> Circulo::Clone() const {
    return std::make_unique<Circulo>(*this);
}

// Comando para agregar una figura al canvas
CommandAgregarFigura::CommandAgregarFigura(Canvas& canvas, std::unique_ptr<IFigura> figura)
    : canvas_(canvas),          // guarda referencia al canvas
      figura_(std::move(figura)), // toma propiedad de la figura
      figuraPtr_(nullptr),      // aún no está en el canvas
      ejecutado_(false) {}      // aún no se ha ejecutado

void CommandAgregarFigura::Ejecutar() {
    // Solo ejecuta si no se ha ejecutado antes y aún tiene la figura
    if (!ejecutado_ && figura_) {
        // Guarda el puntero crudo antes de mover el unique_ptr al canvas
        figuraPtr_ = figura_.get();
        // Añade la figura al canvas, transfiriendo la propiedad
        canvas_.Add(std::move(figura_));
        // Marca como ejecutado
        ejecutado_ = true;
    }
}

void CommandAgregarFigura::Deshacer() {
    // Si no se ha ejecutado o no tenemos puntero a la figura, no hay nada que deshacer
    if (!ejecutado_ || figuraPtr_ == nullptr)
        return;

    // Extrae la figura del canvas (recupera el unique_ptr)
    figura_ = canvas_.Extraer(figuraPtr_);
    // Borra el puntero crudo (ya no está en el canvas)
    figuraPtr_ = nullptr;
    // Marca como no ejecutado (vuelve al estado inicial)
    ejecutado_ = false;
}

// Comando para mover una figura
CommandMoverFigura::CommandMoverFigura(IFigura* figura, int x, int y)
    : figura_(figura), // puntero a la figura (no es dueño)
      oldX_(0), oldY_(0), // se inicializan a 0, se rellenan en la primera ejecución
      newX_(x), newY_(y), // nuevas coordenadas
      ejecutado_(false) {} // aún no se ha guardado la posición original

void CommandMoverFigura::Ejecutar() {
    // Si no hay figura, no hace nada
    if (figura_ == nullptr)
        return;

    // Solo la primera vez guarda la posición original
    if (!ejecutado_) {
        oldX_ = figura_->X();
        oldY_ = figura_->Y();
        ejecutado_ = true;
    }

    // Mueve la figura a la nueva posición
    figura_->Mover(newX_, newY_);
}

void CommandMoverFigura::Deshacer() {
    // Si no hay figura o nunca se ejecutó, no se puede deshacer
    if (figura_ == nullptr || !ejecutado_)
        return;

    // Vuelve a la posición original
    figura_->Mover(oldX_, oldY_);
}

// Función principal
int main() {
    // Crea un escritor que imprime en consola
    EscritorConsola escritor;
    // Crea un canvas vacío
    Canvas canvas;
    // Crea el gestor de comandos (para deshacer/rehacer)
    GestorComandos gestor;

    // Registra el tipo "cuadrado" en la fábrica
    // El lambda recibe un istringstream con el resto de la línea
    FabricaFiguras::Registrar("cuadrado", [](std::istringstream& entrada) {
        int x, y, color;
        double lado;
        // Lee x, y, lado y color desde el stream
        entrada >> x >> y >> lado >> color;
        // Crea un Cuadrado con esos parámetros
        return std::make_unique<Cuadrado>(x, y, lado, color);
    });

    // Registra el tipo "circulo" en la fábrica
    FabricaFiguras::Registrar("circulo", [](std::istringstream& entrada) {
        int x, y, color;
        double radio;
        // Lee x, y, radio y color
        entrada >> x >> y >> radio >> color;
        // Crea un Circulo con esos parámetros
        return std::make_unique<Circulo>(x, y, radio, color);
    });

    // Crea un comando para agregar un cuadrado al canvas y lo ejecuta
    gestor.Ejecutar(std::make_unique<CommandAgregarFigura>(
        canvas, // referencia al canvas
        // La fábrica crea la figura a partir de la línea de texto
        FabricaFiguras::Crear("cuadrado 0 0 4.0 1")));

    // Crea un comando para agregar un círculo al canvas y lo ejecuta
    gestor.Ejecutar(std::make_unique<CommandAgregarFigura>(
        canvas,
        FabricaFiguras::Crear("circulo 10 5 2.0 2")));

    // Imprime el estado inicial del canvas
    escritor.Escribir("--- Estado inicial ---");
    canvas.ImprimirFiguras(escritor);

    // Obtiene un puntero a la primera figura del canvas (índice 0)
    IFigura* primeraFigura = canvas.ObtenerFigura(0);
    if (primeraFigura) {
        // Crea un comando para mover esa figura a (12, 8) y lo ejecuta
        gestor.Ejecutar(std::make_unique<CommandMoverFigura>(primeraFigura, 12, 8));
    }

    // Imprime el estado después de mover la primera figura
    escritor.Escribir("--- Después de mover la primera figura ---");
    canvas.ImprimirFiguras(escritor);

    // Deshace el último comando (el movimiento)
    gestor.Deshacer();
    escritor.Escribir("--- Después de deshacer el movimiento ---");
    canvas.ImprimirFiguras(escritor);

    // Rehace el movimiento
    gestor.Rehacer();
    escritor.Escribir("--- Después de rehacer el movimiento ---");
    canvas.ImprimirFiguras(escritor);

    return 0;
}
