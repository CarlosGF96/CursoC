#include <iostream>
#include <vector>
#include <memory>
#include <string>

class IEscritor {
public:
    virtual ~IEscritor() = default;
    virtual void Escribir(const std::string& linea) = 0;
};

class IFigura {
public:
    virtual ~IFigura() = default;
    virtual double Area() const = 0;
    virtual int Color() const = 0;
    virtual void Imprimir(IEscritor& escritor) const = 0;
};

class Cuadrado : public IFigura {
    double lado_;
    int color_;
public:
    Cuadrado(double lado, int color) : lado_(lado), color_(color) {}
    double Area() const override { return lado_ * lado_; }
    int Color() const override { return color_; }
    void Imprimir(IEscritor& escritor) const override {
        escritor.Escribir("Cuadrado | area: " + std::to_string(Area()) +
                          " | color: " + std::to_string(color_));
    }
};

class Circulo : public IFigura {
    static constexpr double kPi = 3.14159265358979;
    double radio_;
    int color_;
public:
    Circulo(double radio, int color) : radio_(radio), color_(color) {}
    double Area() const override { return kPi * radio_ * radio_; }
    int Color() const override { return color_; }
    void Imprimir(IEscritor& escritor) const override {
        escritor.Escribir("Circulo | area: " + std::to_string(Area()) +
                          " | color: " + std::to_string(color_));
    }
};

class Canvas {
    std::vector<std::unique_ptr<IFigura>> figuras_;
public:
    void Add(std::unique_ptr<IFigura> figura) {
        figuras_.push_back(std::move(figura));
    }
    void ImprimirFiguras(IEscritor& escritor) const {
        for (const auto& f : figuras_)
            f->Imprimir(escritor);
    }
};

class EscritorConsola : public IEscritor {
public:
    void Escribir(const std::string& linea) override {
        std::cout << linea << "\n";
    }
};

int main() {
    EscritorConsola escritor;

    Canvas canvas;
    canvas.Add(std::make_unique<Cuadrado>(4.0, 1));
    canvas.Add(std::make_unique<Circulo>(2.0, 2));
    canvas.Add(std::make_unique<Cuadrado>(1.5, 3));

    canvas.ImprimirFiguras(escritor);
    return 0;
}