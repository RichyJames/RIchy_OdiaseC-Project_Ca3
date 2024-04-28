
#include <iostream>
#include "board.h"

int main() {
    Board board;

    board.initializeBoardFromFile("C:/Users/richy/CLionProjects/C++Project_RIchy_Odiase/bugs.txt");

    while (true) {
        std::cout << "Please choose an option:" << std::endl;
        std::cout << "1: Display bug" << std::endl;
        std::cout << "2: Display life history" << std::endl;
        std::cout << "3: Move bug" << std::endl;
        std::cout << "4: Display board" << std::endl;
        std::cout << "5: Tap board" << std::endl;
        std::cout << "6: Write life history to file" << std::endl;
        std::cout << "7: Run Simulation" << std::endl;
        std::cout << "0: Exit" << std::endl;
        std::cout << "Enter your choice: ";

        int option;
        std::cin >> option;
        switch (option) {
            case 0:
                std::cout << "Exiting..." << std::endl;
                return 0;
            case 1: {
                int bugId;
                std::cout << "Enter the ID of the bug you want to display: ";
                std::cin >> bugId;
                board.bugDisplayed(bugId);
                break;
            }
            case 2:
                board.displayLifeHistory();
                break;
            case 3: {
                int bugId;
                std::cout << "Enter the ID of the bug you want to move: ";
                std::cin >> bugId;
                board.moveBug(bugId);
                break;
            }
            case 4:
                board.displayBoard();
                break;
            case 5:
                board.tapBoard();
                break;
            case 6:
                board.writeLifeHistoryToFile();
                std::cout << "Life history written to file." << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }
}
