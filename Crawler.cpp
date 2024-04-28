#include "crawler.h"
#include "board.h"

Crawler::Crawler(int _id, std::pair<int, int> _position, Direction _direction, int _size, std::pair<int, int> _boardSize, Board& _board)
        : board(_board), Bug(_id, _position, _direction, _size, _boardSize, _board) {

    direction = Direction::North;
}

std::pair<int, int> Crawler::move() {
    std::pair<int, int> newPosition;
    do {
        newPosition = position;
        direction = static_cast<Direction>((rand() % 4) + 1);
        switch (direction) {
            case Direction::North:
                if (newPosition.second > 0)
                    newPosition.second--;
                break;
            case Direction::East:
                if (newPosition.first < 9)
                    newPosition.first++;
                break;
            case Direction::South:
                if (newPosition.second < 9)
                    newPosition.second++;
                break;
            case Direction::West:
                if (newPosition.first > 0)
                    newPosition.first--;
                break;
        }
    } while (isWayBlocked());


    // Check if the new position is within the board boundaries
    if (newPosition.first >= 0 && newPosition.first < boardSize.first &&
        newPosition.second >= 0 && newPosition.second < boardSize.second) {
        position = newPosition;
        path.push_back(position);
        std::cout << "Crawler " << id << " moved to position: (" << position.first << ", " << position.second << ")" << std::endl;
    } else {
        std::cout << "Crawler " << id << " cannot move to that position: (" << newPosition.first << ", " << newPosition.second << ") bug is out of bounds!" << std::endl;
    }

    return position;
}

bool Crawler::isWayBlocked() {
    switch (direction) {
        case Direction::North:
            return position.first == 0;
        case Direction::East:
            return position.second == boardSize.second - 1;
        case Direction::South:
            return position.first == boardSize.first - 1;
        case Direction::West:
            return position.second == 0;
        default:
            return false;
    }
}

int Crawler::getId() const {
    return id;
}

std::pair<int, int> Crawler::getPosition() const {
    return position;
}

Direction Crawler::getDirection() const {
    return direction;
}

int Crawler::getSize() const {
    return size;
}

bool Crawler::isAlive() const {
    return alive;
}