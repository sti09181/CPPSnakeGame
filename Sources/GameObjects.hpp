///////////////////////////
///// GameObjects.hpp /////
///////////////////////////

#pragma once
#include "Libraries.hpp"
#include "Definitions.hpp"

class GameObject_t
{
private:
    // This field is game object coordinates
    GameObjectCoordinates_t coordinates;

    // This filed is game object character
    GameObjectCharacter_t character;

public:
    // This constructor will make specific game object
    // This constructor must not throw any exceptions!
    explicit GameObject_t(GameObjectCoordinates_t coordinates, GameObjectCharacter_t character) noexcept : coordinates(std::move(coordinates)), character(character) {}

    // This destructor will free memory if this game object needs to be deleted
    // This destructor must not throw any exceptions!
    // This destructor is virtual destructor
    virtual ~GameObject_t() noexcept = default;

    // This function will set value of game object coordinates
    void setCoordinates(GameObjectCoordinates_t coordinatesInput) { coordinates = std::move(coordinatesInput); }

    // This function will return game object coordinates
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] GameObjectCoordinates_t getCoordinates() const { return coordinates; }

    // This function will return game object character
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] GameObjectCharacter_t getCharacter() const { return character; }
};

class GrowthObject_t : public GameObject_t
{
private:
    // This field is timeout counter for repositioning growth object
    GameStatusCounter_t timeoutCounter = 0;

public:
    // This constructor will make growth object
    // This constructor must not throw any exceptions!
    explicit GrowthObject_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), 'O') {}

    // This function will set value of timeout counter
    void setTimeoutCounter(GameStatusCounter_t timeoutCounterInput) { timeoutCounter = timeoutCounterInput; }

    // This function will return timeout counter
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] GameStatusCounter_t getTimeoutCounter() const { return timeoutCounter; }
};

class PoisonObject_t : public GameObject_t
{
private:
    // This field is timeout counter for repositioning poison object
    GameStatusCounter_t timeoutCounter = 0;

public:
    // This constructor will make poison object
    // This constructor must not throw any exceptions!
    explicit PoisonObject_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), 'X') {}

    // This function will set value of timeout counter
    void setTimeoutCounter(GameStatusCounter_t timeoutCounterInput) { timeoutCounter = timeoutCounterInput; }

    // This function will return timeout counter
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] GameStatusCounter_t getTimeoutCounter() const { return timeoutCounter; }
};

class GatePiece_t : public GameObject_t
{
public:
    // This constructor will make single pieces of gate
    // This constructor must not throw any exceptions!
    explicit GatePiece_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), '?') {}
};

class EmptyObject_t : public GameObject_t
{
public:
    // This constructor will make empty object
    // This constructor must not throw any exceptions!
    explicit EmptyObject_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), ' ') {}
};

class SnakePiece_t : public GameObject_t
{
public:
    // This constructor will make single pieces of snake
    // This constructor must not throw any exceptions!
    explicit SnakePiece_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), '#') {}
};

class CornerWall_t : public GameObject_t
{
public:
    // This constructor will make single pieces of corner of wall
    // This constructor must not throw any exceptions!
    explicit CornerWall_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), '+') {}
};

class HorizontalWall_t : public GameObject_t
{
public:
    // This constructor will make single pieces of horizontal wall
    // This constructor must not throw any exceptions!
    explicit HorizontalWall_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), '-') {}
};

class VerticalWall_t : public GameObject_t
{
public:
    // This constructor will make single pieces of vertical wall
    // This constructor must not throw any exceptions!
    explicit VerticalWall_t(GameObjectCoordinates_t coordinates) noexcept : GameObject_t(std::move(coordinates), '|') {}
};
