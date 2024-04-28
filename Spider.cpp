#include <iostream>
#include <cstdlib> // For rand()
#include "bug.h"

#include "spider.h"

Spider::Spider(int _id, std::pair<int, int> _position, Direction _direction, int _size, std::pair<int, int> _boardSize, Board& _board)
        : Bug(_id, _position, _direction, _size, _boardSize, _board) {}

std::pair<int, int> Spider::move() {


    std::pair<int, int> newPosition = position;


    int direction = rand() % 4;

    switch (direction) {
        case 0:
            newPosition.first--;
            break;
        case 1:
            newPosition.second++;
            break;
        case 2:
            newPosition.first++;
            break;
        case 3:
            newPosition.second--;
            break;
    }


    if (isValidMove(newPosition)) {

        position = newPosition;
    }

    return position;
}



bool Spider::isValidMove(const std::pair<int, int>& newPosition) {
    if (newPosition.first < 0 || newPosition.first >= boardSize.first ||
        newPosition.second < 0 || newPosition.second >= boardSize.second) {
        return false;
    }



    return newPosition.first >= 0 && newPosition.first < boardSize.first &&
           newPosition.second >= 0 && newPosition.second < boardSize.second;
}

bool Spider::isWayBlocked() {
    return false;
}

char Spider::getType() const {
    return 'S';
}
