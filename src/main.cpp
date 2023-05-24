#include <iostream>
#include "calculator/MatrixCalculator.h"

int main(int argc, char * argv[]){
    if (argc > 1){
        std::cerr << "Invalid number of command line arguments." << std::endl;
        return 0;
    }
    try {
        MatrixCalculator calculator(std::cin, std::cerr, argc ? argv[0] : "");
        calculator.start();
    } catch (std::exception & e){
        std::cerr << e.what() << std::endl;
        return 0;
    }
    return 0;
}
