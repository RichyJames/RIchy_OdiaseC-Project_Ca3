#include "hopper.h"
#include "board.h"

Hopper::Hopper(int _id, std::pair<int, int> _position, Direction _direction, int _size, std::pair<int, int> _boardSize, Board& _board, int _hopLength)
        : board(_board), Bug(_id, _position, _direction, _size, _boardSize, _board), hopLength(_hopLength) {}

std::pair<int, int> Hopper::move() {
    std::pair<int, int> newPosition;
    Direction originalDirection = direction;
    int steps = hopLength;
    do {
        newPosition = position;
        direction = static_cast<Direction>((rand() % 4) + 1);
        switch (direction) {
            case Direction::North:
                newPosition.first -= hopLength;
                break;
            case Direction::East:
                newPosition.second += hopLength;
                break;
            case Direction::South:
                newPosition.first += hopLength;
                break;
            case Direction::West:
                newPosition.second -= hopLength;
                break;
        }
    } while (isWayBlocked());


    if (newPosition.first >= 0 && newPosition.first < boardSize.first &&
        newPosition.second >= 0 && newPosition.second < boardSize.second) {
        position = newPosition;
        path.push_back(position);
        std::cout << "Hopper " << id << " moved to position: (" << position.first << ", " << position.second << ")" << std::endl;
    } else {
        std::cout << "Hopper " << id << " cannot move to position: (" << newPosition.first << ", " << newPosition.second << ")Out of bounds!" << std::endl;
    }

    return position;
}
bool Hopper::isWayBlocked() {
    switch (direction) {
        case Direction::North:
            return position.first - 1 < 0;
        case Direction::East:
            return position.second + 1 > boardSize.second - 1;
        case Direction::South:
            return position.first + 1 > boardSize.first - 1;
        case Direction::West:
            return position.second - 1 < 0;
        default:
            return false;
    }
}

int Hopper::getId() const {
    return id;
}

std::pair<int, int> Hopper::getPosition() const {
    return position;
}

Direction Hopper::getDirection() const {
    return direction;
}

int Hopper::getSize() const {
    return size;
}

bool Hopper::isAlive() const {
    return alive;
}

int Hopper::getHopLength() const {
    return hopLength;
}