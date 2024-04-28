#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <list>

#include "bug.h"

class Board {
private:
    std::vector<Bug*> bugVector;
    std::vector<std::vector<std::list<Bug*>>> board;

public:
    Board();
    ~Board();

    void initializeBoardFromFile(const std::string& filename);
    std::vector<Bug*> getBugVector() const;
    void bugDisplayed(int bugId) const;
    void displayLifeHistory() const;
    void moveBug(int bugId);
    void displayBoard() const;
    Bug* getBugAtPosition(std::pair<int, int> position) const;
    void tapBoard();
    void writeLifeHistoryToFile() const;
    bool isGameOver() const;
};

#endif

