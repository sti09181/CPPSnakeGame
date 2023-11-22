///////////////////////////
///// SnakeObject.hpp /////
///////////////////////////

#pragma once
#include "Libraries.hpp"
#include "Definitions.hpp"
#include "GameObjects.hpp"

// This enum definition will control heading direction of snake
enum class HeadingDirection_t { left = -20, down = -10, up = 10, right = 20 };

// This class is snake object for this game
class SnakeObject_t
{
private:
    // This field is queue that contains pieces of snake
    std::queue<SnakePiece_t, std::list<SnakePiece_t>> snake;

    // This field is heading direction of snake
    HeadingDirection_t headingDirection = HeadingDirection_t::right;

public:
    // This function will add head of snake
    void addPiece(SnakePiece_t snakePiece);

    // This function will remove tail of snake
    void removePiece();

    // This function will return tail of snake
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] SnakePiece_t getTail() const;

    // This function will return head of snake
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] SnakePiece_t getHead() const;

    // This function will return size of snake
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] int getSize() const;

    // This function will set heading direction of snake
    void setHeadingDirection(HeadingDirection_t headingDirectionInput);

    // This function will return heading direction of snake
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] HeadingDirection_t getHeadingDirection() const;

    // This function will return single piece that contains next head of snake coordinates
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] SnakePiece_t getNextHead() const;
};
