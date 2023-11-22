/////////////////////////
///// SnakeGame.hpp /////
/////////////////////////

#pragma once
#include "Libraries.hpp"
#include "Definitions.hpp"
#include "GameObjects.hpp"
#include "GateObjects.hpp"
#include "SnakeObject.hpp"
#include "MainScreen.hpp"

class SnakeGame_t
{
private:
    // This field is main screen for this game
    std::unique_ptr<MainScreen_t> mainScreen;

    // This field is index of current game stage
    StageCounter_t currentStageIndex = 0;

    // This field is count of stages
    static constexpr StageCounter_t countOfStages = 4;

    // This field is array of stage missions
    std::array<std::pair<StageMissionKey_t, StageMissionCounter_t>, countOfStages> stageMissions;

    // These fields are game objects for this game
    std::unique_ptr<SnakeObject_t> snakeObject;
    std::list<std::unique_ptr<GrowthObject_t>> growthObjects;
    std::list<std::unique_ptr<PoisonObject_t>> poisonObjects;
    std::unique_ptr<GateObjects_t> gateObjects;

    // This field is boolean value that check snake is located inside of gates
    GameStatusBoolean_t isSnakeIsLocatedInsideOfGates = false;

    // This field is count of snake pieces which located inside of gates
    GameStatusCounter_t countOfSnakePiecesInsideOfGates = 0;

    // This field is array of boolean values that check current stage is completed or not
    std::array<GameStatusBoolean_t, countOfStages> isCurrentStageIsCompleted = { false, };

    // This field is boolean value that check current stage is failed or not
    GameStatusBoolean_t isCurrentStageIsFailed = false;

public:
    // This constructor will act as main function for this game
    SnakeGame_t();

    // This destructor will print final instructions to player and free memory if this game needs to be terminated
    ~SnakeGame_t();

private:
    // This function will update game object coordinates to point random coordinates of empty object
    void getEmptyCoordinatesRandomly(GameObjectCoordinates_t& coordinates);

    // This function will update game object coordinates to point random coordinates of empty object or border object
    void getEmptyOrBorderCoordinatesRandomly(GameObjectCoordinates_t& coordinates);

    // This function will get game object character from specific window
    // Return value of this function is cannot be able to discarded
    [[nodiscard]] GameObjectCharacter_t getGameObjectCharacterFromWindow(Window_t window, const GameObjectCoordinates_t& coordinates);

    // This function will add game object character to game window
    void addGameObjectCharacterToWindow(Window_t window, const GameObject_t& gameObject);

    // This function will update snake object based on specific situations
    void handleNextSnakePiece(SnakePiece_t nextPiece);

    // This function will handle next head of snake if coordinates located in empty object
    void handlerForEmptyObject(SnakePiece_t nextPiece);

    // This function will handle next head of snake if coordinates located in growth object
    void handlerForGrowthObject(SnakePiece_t nextPiece);

    // This function will handle next head of snake if coordinates located in poison object
    void handlerForPoisonObject(SnakePiece_t nextPiece);

    // This function will handle next head of snake if coordinates located in gate objects
    void handlerForGateObjects(SnakePiece_t nextPiece);

    // This function will make growth object and add to random coordinates of empty object
    void createGrowthObject(std::unique_ptr<GrowthObject_t>& growthObject);

    // This function will make poison object and add to random coordinates of empty object
    void createPoisonObject(std::unique_ptr<PoisonObject_t>& poisonObject);

    // This function will make gate objects and add to random coordinates of game window
    void createGateObjects();

    // This function will remove growth object and add empty object to object coordinates
    void removeGrowthObject(std::unique_ptr<GrowthObject_t>& growthObject);

    // This function will remove poison object and add empty object to object coordinates
    void removePoisonObject(std::unique_ptr<PoisonObject_t>& poisonObject);

    // This function will remove gate objects and add empty object to object coordinates
    void removeGateObjects();

    // This function will initialize stage missions randomly
    void initializeStageMissions();

    // This function will clear game window and start to build current stage layout
    void initializeCurrentStageLayout();

    // This function will process inputs from game window
    void processInput();

    // This function will update game status
    void updateGameStatus();

    // This function will check current stage mission is completed or not
    void checkCurrentStageMission();
};
