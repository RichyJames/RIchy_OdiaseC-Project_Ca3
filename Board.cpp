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
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Unable to open file " << filename << std::endl;
        exit(1);
    }

    std::string line;
    std::pair<int, int> boardSize(10, 10);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char type;
        int id, x, y, size, directionValue, hopLength = 0;
        char delimiter;

        if (!(iss >> type >> delimiter >> id >> delimiter >> x >>
        delimiter >> y >> delimiter >> directionValue >> delimiter >> size)) {
            break;
        }

        if (type == 'H' && !(iss >> delimiter >> hopLength)) {
            break;
        }

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
        else if (type == 'S') {
            Bug* newBug = new Spider(id, position, direction, size, boardSize, *this);
            bugVector.push_back(newBug);
            board[position.first][position.second].push_back(newBug);
        }
    }

    file.close();
}

std::vector<Bug*> Board::getBugVector() const {
    return bugVector;
}

std::string directionToString(Direction direction) {
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
    bool bugFound = false;
    for (const Bug* bug : bugVector) {
        if (bug->getId() == bugId) {
            std::cout << "Bug ID: " << bug->getId() << "\n"
                      << "Position: (" << bug->getPosition().first << "," << bug->getPosition().second << ")\n"
                      << "Direction: " << directionToString(bug->getDirection()) << "\n"
                      << "Size: " << bug->getSize() << "\n"
                      << "Alive: " << (bug->isAlive() ? "Yes" : "No") << std::endl;

            if (bug->getType() == 'H') {
                const Hopper* hopper = dynamic_cast<const Hopper*>(bug);
                std::cout << "Hop Length: " << hopper->getHopLength() << std::endl;
            }

            bugFound = true;
            break;
        }
    }

    if (!bugFound) {
        std::cout << "Bug with ID " << bugId << " not found." << std::endl;
    }
}




void Board::displayLifeHistory() const {
    for (const Bug* bug : bugVector) {
        std::cout << bug->getId() << " ";

        if (auto crawler = dynamic_cast<const Crawler*>(bug)) {
            std::cout << "Crawler ";
            std::cout << "Location: (" << crawler->getPosition().first << "," << crawler->getPosition().second << ") ";
            std::cout << "Size: " << crawler->getSize() << " ";
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
    Bug* bugToMove = nullptr;
    for (Bug* bug : bugVector) {
        if (bug->getId() == bugId) {
            bugToMove = bug;
            break;
        }
    }

    if (bugToMove == nullptr) {
        std::cout << "No bug found with ID " << bugId << std::endl;
        return;
    }

    if (bugToMove->isAlive()) {
        std::pair<int, int> oldPosition = bugToMove->getPosition();
        std::pair<int, int> newPosition = bugToMove->move();

        std::cout << "Moving bug " << bugId << " from (" << oldPosition.first
        << ", " << oldPosition.second << ") to (" << newPosition.first << ", "
        << newPosition.second << ")" << std::endl;

        board[oldPosition.first][oldPosition.second].remove(bugToMove);
        board[newPosition.first][newPosition.second].push_back(bugToMove);

        bugToMove->setPosition(newPosition);
    }
}


void Board::displayBoard() const {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::cout << "(" << i << "," << j << "): ";
            if (board[i][j].empty()) {
                std::cout << "empty";
            } else {
                for (Bug* bug : board[i][j]) {
                    if (bug->isAlive()) {
                        if (dynamic_cast<Crawler*>(bug)) {
                            std::cout << "Crawler ";
                        } else if (dynamic_cast<Hopper*>(bug)) {
                            std::cout << "Hopper ";
                        } else if (dynamic_cast<Spider*>(bug)) {
                            std::cout << "PerimeterCrawler ";
                        }
                        std::cout << bug->getId() << ", ";
                    }
                }
            }
            std::cout << std::endl;
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
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    auto str = oss.str();

    std::ofstream file("bugs_life_history_" + str + ".out");
    if (!file) {
        std::cerr << "Unable to open file for writing" << std::endl;
        return;
    }

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

