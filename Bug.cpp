#include "bug.h"
#include "board.h"

Bug::Bug(int _id, std::pair<int, int> _position, Direction _direction, int _size, std::pair<int, int> _boardSize, Board& _board)
        : id(_id), position(_position), direction(_direction), size(_size), boardSize(_boardSize), board(_board), alive(true) {}

Bug::~Bug() {}

int Bug::getId() const {
    return id;
}

std::pair<int, int> Bug::getPosition() const {
    return position;
}

void Bug::setPosition(std::pair<int, int> newPosition) {
    position = newPosition;
}

Direction Bug::getDirection() const {
    return direction;
}

int Bug::getSize() const {
    return size;
}

bool Bug::isAlive() const {
    return alive;
}

void Bug::kill() {
    alive = false;
}

void Bug::grow(int size) {
    this->size += size;
}

std::list<std::pair<int, int>> Bug::getPath() const {
    return path;
}
