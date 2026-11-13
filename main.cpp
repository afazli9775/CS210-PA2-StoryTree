
#include <iostream>
#include <fstream>
#include <string>
#include "Adventure.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "Usage: ./adventure <story_file>"
                  << std::endl;
        return 1;
    }

    // ===================== PART 0 =====================
    // Disabled because the game is now ready.

    /*
    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cout << "Error: could not open file "
                  << argv[1] << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
    */

    // ===================== PART 1: the game =====================

    Adventure game;

    if (!game.load(argv[1])) {
        return 1;
    }

    std::cout << "Story loaded." << std::endl;

    std::string choice;

    while (true) {
        std::cout << std::endl;
        std::cout << "==== MENU ====" << std::endl;
        std::cout << "1. Play" << std::endl;
        std::cout << "2. Story stats" << std::endl;
        std::cout << "3. Spoiler guide" << std::endl;
        std::cout << "4. Greedy vs optimal" << std::endl;
        std::cout << "5. Quit" << std::endl;
        std::cout << "Choose: ";

        if (!std::getline(std::cin, choice)) {
            break;
        }

        if (choice == "1") {
            game.play();
        } else if (choice == "2") {
            game.printStats();
        } else if (choice == "3") {
            game.printSpoilers();
        } else if (choice == "4") {
            game.greedyRun();
            game.optimalRun();
        } else if (choice == "5") {
            break;
        } else {
            std::cout << "Please type a number from 1 to 5."
                      << std::endl;
        }
    }

    std::cout << "Goodbye!" << std::endl;

    return 0;
}
