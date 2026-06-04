#pragma once  // Evita que el archivo de cabecera se incluya más de una vez en la compilación

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Interfaz para escribir texto (salida abstracta: consola, archivo, etc.)
class IEscritor {
public:
    virtual ~IEscritor() = default;
    // Método puro virtual: cualquier escritor debe implementar cómo escribir una línea
    virtual void Escribir(const std::string& linea) = 0;
};

// Interfaz base de cualquier figura geométrica
class IFigura {
public:
    virtual ~IFigura() = default;

    // Devuelve el área de la figura
    virtual double Area() const = 0;
    // Devuelve el color (aquí como entero)
    virtual int Color() const = 0;
    // Imprime la figura usando un escritor
    virtual void Imprimir(IEscritor& escritor) const = 0;
    // Mueve la figura a una nueva posición (x, y)
    virtual void Mover(int x, int y) = 0;
    // Devuelve la coordenada X actual
    virtual int X() const = 0;
    // Devuelve la coordenada Y actual
    virtual int Y() const = 0;
    // Crea una copia dinámica de la figura (patrón Prototype)
    virtual std::unique_ptr<IFigura> Clone() const = 0;
};

// Lienzo que contiene múltiples figuras
class Canvas {
public:
    // Añade una figura al vector, transfiriendo la propiedad (unique_ptr)
    void Add(std::unique_ptr<IFigura> figura);
    // Extrae una figura concreta (por puntero crudo) y devuelve su unique_ptr
    std::unique_ptr<IFigura> Extraer(IFigura* figura);
    // Devuelve un puntero crudo a la figura en la posición 'indice' (sin transferir propiedad)
    IFigura* ObtenerFigura(size_t indice) const;
    // Pide a todas las figuras que se impriman usando el escritor
    void ImprimirFiguras(IEscritor& escritor) const;

private:
    // Contenedor de figuras, cada una con propiedad exclusiva
    std::vector<std::unique_ptr<IFigura>> figuras_;
};

// Fábrica de figuras: crea figuras a partir de una línea de texto
class FabricaFiguras {
public:
    // Tipo de función constructora: recibe un istringstream y devuelve una figura
    using Constructor = std::function<std::unique_ptr<IFigura>(std::istringstream&)>;

    // Registra un tipo de figura con un nombre (string) y su constructor asociado
    static void Registrar(const std::string& nombre, Constructor ctor);
    // Crea una figura a partir de una línea de texto (ej: "cuadrado 0 0 4.0 1")
    static std::unique_ptr<IFigura> Crear(const std::string& linea);

private:
    // Devuelve el mapa estático que asocia nombre -> constructor
    static std::map<std::string, Constructor>& Registro();
};

// Interfaz de comando (patrón Command)
class ICommand {
public:
    virtual ~ICommand() = default;
    // Ejecuta la acción
    virtual void Ejecutar() = 0;
    // Deshace la acción
    virtual void Deshacer() = 0;
};

// Gestor de comandos con soporte de deshacer/rehacer
class GestorComandos {
public:
    // Ejecuta un comando, lo guarda en la pila de hechos y limpia la pila de deshechos
    void Ejecutar(std::unique_ptr<ICommand> comando);
    // Deshace el último comando ejecutado
    void Deshacer();
    // Rehace el último comando deshecho
    void Rehacer();

private:
    // Pila (vector) de comandos ya ejecutados
    std::vector<std::unique_ptr<ICommand>> hechos_;
    // Pila (vector) de comandos deshechos (para rehacer)
    std::vector<std::unique_ptr<ICommand>> deshechos_;
};

// Implementación concreta de IEscritor que escribe en la consola
class EscritorConsola : public IEscritor {
public:
    void Escribir(const std::string& linea) override;
};

// Implementación de un cuadrado
class Cuadrado : public IFigura {
public:
    // Constructor: posición (x, y), lado y color
    Cuadrado(int x, int y, double lado, int color);
    double Area() const override;
    int Color() const override;
    void Imprimir(IEscritor& escritor) const override;
    void Mover(int x, int y) override;
    int X() const override;
    int Y() const override;
    std::unique_ptr<IFigura> Clone() const override;

private:
    int x_;        // posición X
    int y_;        // posición Y
    double lado_;  // longitud del lado
    int color_;    // color como entero
};

// Implementación de un círculo
class Circulo : public IFigura {
public:
    // Constructor: posición (x, y), radio y color
    Circulo(int x, int y, double radio, int color);
    double Area() const override;
    int Color() const override;
    void Imprimir(IEscritor& escritor) const override;
    void Mover(int x, int y) override;
    int X() const override;
    int Y() const override;
    std::unique_ptr<IFigura> Clone() const override;

private:
    // Constante de PI para el cálculo del área
    static constexpr double kPi = 3.14159265358979;
    int x_;        // posición X
    int y_;        // posición Y
    double radio_; // radio del círculo
    int color_;    // color como entero
};

// Comando para agregar una figura al canvas
class CommandAgregarFigura : public ICommand {
public:
    // Recibe una referencia al canvas y la figura a agregar (propiedad exclusiva)
    CommandAgregarFigura(Canvas& canvas, std::unique_ptr<IFigura> figura);
    void Ejecutar() override;
    void Deshacer() override;

private:
    Canvas& canvas_;                 // referencia al canvas donde se agrega/extrae
    std::unique_ptr<IFigura> figura_; // figura que se va a agregar o que se recupera al deshacer
    IFigura* figuraPtr_;             // puntero crudo a la figura dentro del canvas
    bool ejecutado_;                 // indica si ya se ejecutó al menos una vez
};

// Comando para mover una figura
class CommandMoverFigura : public ICommand {
public:
    // Recibe un puntero a la figura y la nueva posición
    CommandMoverFigura(IFigura* figura, int x, int y);
    void Ejecutar() override;
    void Deshacer() override;

private:
    IFigura* figura_; // puntero a la figura a mover (no es dueño)
    int oldX_;        // posición X anterior
    int oldY_;        // posición Y anterior
    int newX_;        // nueva posición X
    int newY_;        // nueva posición Y
    bool ejecutado_;  // indica si ya se guardó la posición original
};
