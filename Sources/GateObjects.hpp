///////////////////////////
///// GateObjects.hpp /////
///////////////////////////

#pragma once
#include "Libraries.hpp"
#include "Definitions.hpp"
#include "GameObjects.hpp"

class GateObjects_t
{
private:
    // This field is pair of gates
    std::pair<GatePiece_t, GatePiece_t> gates;

public:
    // This constructor will make gate objects
    // This constructor must not throw any exceptions!
    explicit GateObjects_t(GameObjectCoordinates_t firstGateCoordinates, GameObjectCoordinates_t secondGateCoordinates) noexcept : gates(GatePiece_t(firstGateCoordinates), GatePiece_t(secondGateCoordinates)) {}

    // This function will set value of first gate
    void setFirstGate(GatePiece_t firstGateInput) { gates.first = firstGateInput; }

    // This function will set value of second gate
    void setSecondGate(GatePiece_t secondGateInput) { gates.second = secondGateInput; }

    // This function will return first gate
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] GatePiece_t getFirstGate() const { return gates.first; }

    // This function will return second gate
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] GatePiece_t getSecondGate() const { return gates.second; }
};
