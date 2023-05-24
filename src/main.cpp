#include <iostream>
#include "calculator/MatrixCalculator.h"

int main(int argc, char * argv[]){
    if (argc > 2){
        std::cerr << "Invalid number of command line arguments." << std::endl;
        return 0;
    }
    try {
        MatrixCalculator calculator(std::cin, std::cout, argc == 2 ? argv[1] : "");
        calculator.start();
    } catch (std::exception & e){
        std::cerr << e.what() << std::endl;
        return 0;
    }
    return 0;
}