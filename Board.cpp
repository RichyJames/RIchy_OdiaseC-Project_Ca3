#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "bug.h"
#include "board.h"
#include "crawler.h"
#include "hopper.h"
#include "spider.h"

Board::Board() {
    board = std::vector<std::vector<std::list<Bug*>>>(10, std::vector<std::list<Bug*>>(10));
}

Board::~Board() {
    for (Bug* bug : bugVector) {
        delete bug;
    }
}

void Board::initializeBoardFromFile(const std::string& filename) {
    // Open the file for reading
    std::ifstream file(filename);
    if (!file) {
        // If the file cannot be opened, print an error message and exit
        std::cerr << "Unable to open file " << filename << std::endl;
        exit(1);
    }
// Read each line from the file
    std::string line;
    // the size of the board
    std::pair<int, int> boardSize(10, 10);

    while (std::getline(file, line)) {
        // Parse the line using string stream
        std::istringstream iss(line);
        // Declare variables to store bug attributes
        char type;
        int id, x, y, size, directionValue, hopLength = 0;
        char delimiter;

        // Extract bug attributes from the line
        if (!(iss >> type >> delimiter >> id >> delimiter >> x >>
        delimiter >> y >> delimiter >> directionValue >> delimiter >> size)) {
            // If extraction fails, break out of the loop
            break;
        }
        // Extract additional attribute for Hopper bugs
        if (type == 'H' && !(iss >> delimiter >> hopLength)) {
            // If extraction fails, break out of the loop
            break;
        }

        // Create a pair to represent the bug's position
        std::pair<int, int> position(x, y);
        Direction direction = static_cast<Direction>(directionValue);
        Bug* newBug = nullptr;
        if (type == 'C')
            newBug = new Crawler(id, position, direction, size, boardSize, *this);
        else if (type == 'H')
            newBug = new Hopper(id, position, direction, size, boardSize, *this, hopLength);
        if (newBug) {
            bugVector.push_back(newBug);
            board[position.first][position.second].push_back(newBug);
        }
            // Handle Spider bugs
        else if (type == 'S') {
            Bug* newBug = new Spider(id, position, direction, size, boardSize, *this);
            bugVector.push_back(newBug);
            board[position.first][position.second].push_back(newBug);
        }
    }

    file.close();
}

std::vector<Bug*> Board::getBugVector() const {
    // Return the vector containing pointers to bugs
    return bugVector;
}

std::string directionToString(Direction direction) {
    // Switch case to determine the direction and return corresponding string
    switch (direction) {
        case Direction::North:
            return "North";
        case Direction::East:
            return "East";
        case Direction::South:
            return "South";
        case Direction::West:
            return "West";
        default:
            return "Wrong direction";
    }
}

void Board::bugDisplayed(int bugId) const {
    //track if the bug is found or not
    bool bugFound = false;
    // Iterate through each bug in the vector
    for (const Bug* bug : bugVector) {
        // Check if the current bug has the specified ID
        if (bug->getId() == bugId) {
            // Display information about the bug
            std::cout << "Bug ID: " << bug->getId() << "\n"
                      << "Position: (" << bug->getPosition().first << "," << bug->getPosition().second << ")\n"
                      << "Direction: " << directionToString(bug->getDirection()) << "\n"
                      << "Size: " << bug->getSize() << "\n"
                      << "Alive: " << (bug->isAlive() ? "Yes" : "No") << std::endl;

            // Check if the bug is a Hopper
            if (bug->getType() == 'H') {
                // If it is a Hopper, cast it to Hopper and display hop length
                const Hopper* hopper = dynamic_cast<const Hopper*>(bug);
                std::cout << "Hop Length: " << hopper->getHopLength() << std::endl;
            }
// Set to true to indicate that the bug is found
            bugFound = true;
            break;
        }
    }

    if (!bugFound) {
        std::cout << "Bug with ID " << bugId << " not found." << std::endl;
    }
}




void Board::displayLifeHistory() const {
    // Iterate through each bug in the vector
    for (const Bug* bug : bugVector) {
        // Display the bug ID
        std::cout << bug->getId() << " ";
        // Check if the bug is a Crawler
        if (auto crawler = dynamic_cast<const Crawler*>(bug)) {
            // If it is a Crawler, display Crawler information
            std::cout << "Crawler ";
            std::cout << "Location: (" << crawler->getPosition().first << "," << crawler->getPosition().second << ") ";
            // Print the bug's position
            std::cout << "Size: " << crawler->getSize() << " ";
            // Print the bug's size
            std::cout << "Direction: " << directionToString(crawler->getDirection()) << " ";
            std::cout << (crawler->isAlive() ? "Alive!" : "Dead!") << std::endl;
        }
        else if (auto hopper = dynamic_cast<const Hopper*>(bug)) {
            std::cout << "Hopper ";
            std::cout << "Location: (" << hopper->getPosition().first << "," << hopper->getPosition().second << ") ";
            std::cout << "Size: " << hopper->getSize() << " ";
            std::cout << "Direction: " << directionToString(hopper->getDirection()) << " ";
            std::cout << "HopLength: " << hopper->getHopLength() << " ";
            std::cout << (hopper->isAlive() ? "Alive!" : "Dead!") << std::endl;
        }
    }
}


void Board::moveBug(int bugId) {
    // Find the bug with the given ID
    Bug* bugToMove = nullptr;
    for (Bug* bug : bugVector) {
        if (bug->getId() == bugId) {
            bugToMove = bug;
            break;
        }
    }
    // If the bug is not found, print a message and return
    if (bugToMove == nullptr) {
        std::cout << "No bug found with ID " << bugId << std::endl;
        return;
    }
    // Check if the bug is alive
    if (bugToMove->isAlive()) {
        // Get the old position of the bug
        std::pair<int, int> oldPosition = bugToMove->getPosition();
        // Move the bug and get its new position
        std::pair<int, int> newPosition = bugToMove->move();

        // Print the movement of the bug
        std::cout << "Moving bug " << bugId << " from (" << oldPosition.first
        << ", " << oldPosition.second << ") to (" << newPosition.first << ", "
        << newPosition.second << ")" << std::endl;

        // Remove the bug from its old position in the board
        board[oldPosition.first][oldPosition.second].remove(bugToMove);
        // Add the bug to its new position in the board
        board[newPosition.first][newPosition.second].push_back(bugToMove);
        // Update the bug's position
        bugToMove->setPosition(newPosition);
    }
}


void Board::displayBoard() const {
// Loop through each cell in the board
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            // Print the coordinates of the cell
            std::cout << "(" << i << "," << j << "): ";
            // Check if the cell is empty
            if (board[i][j].empty()) {
                std::cout << "empty"; // Print "empty" if the cell is empty
            } else {
                // Loop through bugs in the cell
                for (Bug* bug : board[i][j]) {
                    // Check if the bug is alive
                    if (bug->isAlive()) {
                        // Identify the type of bug and print its ID
                        if (dynamic_cast<Crawler*>(bug)) {
                            std::cout << "Crawler ";// Print "Crawler" for Crawler bugs
                        } else if (dynamic_cast<Hopper*>(bug)) {
                            std::cout << "Hopper "; // Print "Hopper" for Hopper bugs
                        } else if (dynamic_cast<Spider*>(bug)) {
                            std::cout << "Spider ";
                        }
                        std::cout << bug->getId() << ", ";
                    }
                }
            }
            std::cout << std::endl;// Move to the next line for the next cell
        }
    }
}


Bug* Board::getBugAtPosition(std::pair<int, int> position) const {
    for (Bug* bug : bugVector) {
        if (bug->getPosition() == position && bug->isAlive()) {
            return bug;
        }
    }
    return nullptr;
}

void Board::tapBoard() {
    // Move all bugs first
    for (Bug *bug: bugVector) {
        if (bug->isAlive()) {
            std::pair<int, int> oldPosition = bug->getPosition();
            std::pair<int, int> newPosition = bug->move();

            // Print out the details of each bug as it's being moved
            std::cout << "Moving bug " << bug->getId() << " from (" << oldPosition.first << ", " << oldPosition.second
                      << ") to (" << newPosition.first << ", " << newPosition.second << ")" << std::endl;

            board[oldPosition.first][oldPosition.second].remove(bug);
            board[newPosition.first][newPosition.second].push_back(bug);
        }
    }
}
bool Board::isGameOver() const {
    for (const Bug* bug : bugVector) {
        if (bug->isAlive()) {
            return false;
        }
    }
    return true;
}


void Board::writeLifeHistoryToFile() const {
    // Get current time for unique filename
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    auto str = oss.str();
    // Open file for writing
    std::ofstream file("C:/Users/richy/CLionProjects/C++Project_RIchy_Odiase/lifehistory" + str + ".out");
    if (!file) {
        std::cerr << "Unable to open file for writing" << std::endl;
        return;
    }

    // Write life history of bugs to file
    for (const Bug* bug : bugVector) {
        file << bug->getId() << " ";
        if (dynamic_cast<const Crawler*>(bug)) {
            file << "Crawler ";
        } else if (dynamic_cast<const Hopper*>(bug)) {
            file << "Hopper ";
        }else if (dynamic_cast<const Spider*>(bug)) {
            file << "Spider ";
        }
        file << "Path: ";
        for (const auto& position : bug->getPath()) {
            file << "(" << position.first << "," << position.second << "),";
        }
        file << " " << (bug->isAlive() ? "Alive!" : "Dead!") << std::endl;
    }

    file.close();
}

