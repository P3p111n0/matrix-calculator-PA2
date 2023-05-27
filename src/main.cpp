#include "calculator/MatrixCalculator.h"
#include <csignal>
#include <iostream>
#include "exceptions/QuitSignal.h"

//http://vyuka.bernhauer.cz/pa2-clanky/semestralni-prace
void signal_handler(int) {
    throw QuitSignal();
}

int main(int argc, char * argv[]){
    if (argc > 2){
        std::cerr << "Invalid number of command line arguments." << std::endl;
        return EXIT_FAILURE;
    }

    std::signal(SIGTERM, signal_handler);
    std::signal(SIGINT, signal_handler);

    try {
        MatrixCalculator calculator(std::cin, std::cout, argc == 2 ? argv[1] : "");
        calculator.start();
    } catch (std::exception & e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
