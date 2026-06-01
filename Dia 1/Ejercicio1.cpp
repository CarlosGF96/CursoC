#include <iostream>

bool par(int numero1){

    if (numero1 % 2 == 0) {
        return true;
    } else {
        return false;
    }
}

bool primo(int numero2) {
    if (numero2 <= 1)
        return false;
    for (int i = 2; i < numero2; i++) {
        if (numero2 % i == 0)
            return false;
    }
    return true;
}

int main()
{
	int numero;
    int nuemro_primo;
    int n_inicial;
    int n_siguiente;
    std::cout << "Introduzca un número: ";
	std::cin >> numero;
    if (par(numero)){
        std::cout << "El número " << numero << " es PAR." << std::endl;
    }else{
        std::cout << "El número " << numero << " es IMPAR." << std::endl;
    };
	
    std::cout << "Introduzca un número: ";
	std::cin >> nuemro_primo;

    if(primo(nuemro_primo)){
        std::cout << "El número " << nuemro_primo << " es primo." << std::endl;
    }else{
        std::cout << "El número " << nuemro_primo << " no es primo." << std::endl;
    };

    std::cout << "Introduzca un número inicial: ";
	std::cin >> n_inicial;

    do {
        std::cout << "Introduce un número: ";
        std::cin >> numero;
 
        if (numero > n_inicial)
            std::cout << "El número es mayor" << std::endl;
        else if (numero < n_inicial)
            std::cout << "El número es menor" << std::endl;
    } while (numero != n_inicial);
    std::cout << "Número correcto";


    

    
    return 0;
}