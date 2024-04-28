#ifndef SPIDER_H
#define SPIDER_H

#include "bug.h"

class Spider : public Bug {
public:

    Spider(int _id, std::pair<int, int> _position, Direction _direction, int _size, std::pair<int, int> _boardSize, Board& _board);


    std::pair<int, int> move() override;
    bool isWayBlocked() override;
    char getType() const override;



private:

    bool isValidMove(const std::pair<int, int>& newPosition);
    void changeDirection();
};

#endif
