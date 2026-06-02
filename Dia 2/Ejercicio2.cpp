#include <iostream>
#include <vector>


/*Realiza una aplicación que pida por pantalla números, hasta que el usuario escriba 0, 
almacénalos en un array y luego recorre el array mostrándolos todos por pantalla.*/
int main()
{
    int n_inicial;
    int numero;
    std::vector<int> numeros;

    std::cout << "Introduzca un número inicial: ";
	std::cin >> n_inicial;
    numeros.push_back(n_inicial);
    if (n_inicial==0){


        std::cout << "Numeros introducidos:" << n_inicial << std::endl;

    }else{
        do {
            std::cout << "Introduce un número: ";
            std::cin >> numero;
    
            numeros.push_back(numero);

        } while (numero != 0);
    }

    for (int i = 0; i < sizeof(numeros); i++) {
        std::cout << numeros[i] << " ";
    } 

    return 0;



}