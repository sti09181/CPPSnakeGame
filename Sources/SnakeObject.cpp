///////////////////////////
///// SnakeObject.cpp /////
///////////////////////////

#include "SnakeObject.hpp"

// This function will add head of snake
void SnakeObject_t::addPiece(SnakePiece_t snakePiece)
{
    snake.push(snakePiece);
}

// This function will remove tail of snake
void SnakeObject_t::removePiece()
{
    snake.pop();
}

// This function will return tail of snake
// Return value of this function is cannot be able to discarded!
[[nodiscard]] SnakePiece_t SnakeObject_t::getTail() const
{
    return snake.front();
}

// This function will return head of snake
// Return value of this function is cannot be able to discarded!
[[nodiscard]] SnakePiece_t SnakeObject_t::getHead() const
{
    return snake.back();
}

// This function will return size of snake
// Return value of this function is cannot be able to discarded!
[[nodiscard]] int SnakeObject_t::getSize() const
{
    return static_cast<int>(snake.size());
}

// This function will set heading direction of snake
void SnakeObject_t::setHeadingDirection(HeadingDirection_t headingDirectionInput)
{
    headingDirection = headingDirectionInput;
}

// This function will return heading direction of snake
// Return value of this function is cannot be able to discarded!
[[nodiscard]] HeadingDirection_t SnakeObject_t::getHeadingDirection() const
{
    return headingDirection;
}

// This function will return single piece that contains next head of snake coordinates
// Return value of this function is cannot be able to discarded!
[[nodiscard]] SnakePiece_t SnakeObject_t::getNextHead() const
{
    GameObjectCoordinates_t nextCoordinates = getHead().getCoordinates();

    switch (headingDirection)
    {
        case HeadingDirection_t::up: nextCoordinates.first--; break;
        case HeadingDirection_t::down: nextCoordinates.first++; break;
        case HeadingDirection_t::left: nextCoordinates.second--; break;
        case HeadingDirection_t::right: nextCoordinates.second++; break;
        default: break;
    }

    return SnakePiece_t(nextCoordinates);
}
