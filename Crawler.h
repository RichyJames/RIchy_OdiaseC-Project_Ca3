#ifndef CRAWLER_H
#define CRAWLER_H

#include "bug.h"
#include <cstdlib>

class Crawler : public Bug {
public:
    Crawler(int _id, std::pair<int, int> _position, Direction _direction, int _size, std::pair<int, int> _boardSize, Board& _board);

    std::pair<int, int>move() override;
    bool isWayBlocked() override;
    int getId() const;
    std::pair<int, int> getPosition() const;
    Direction getDirection() const;
    int getSize() const;
    bool isAlive() const;
    char getType() const override { return 'C'; }

private:
    Board& board;
};

#endif