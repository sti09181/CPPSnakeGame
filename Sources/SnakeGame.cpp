/////////////////////////
///// SnakeGame.cpp /////
/////////////////////////

#include "SnakeGame.hpp"

// This constructor will act as main function for this game
SnakeGame_t::SnakeGame_t()
{
    // Initialize main screen for this game
    mainScreen = std::make_unique<MainScreen_t>();

    // Initialize stage missions
    initializeStageMissions();

    // Run multiple stages
    for (currentStageIndex = 0; currentStageIndex < countOfStages; currentStageIndex++)
    {
        // Clear game objects
        if (snakeObject != nullptr)
            snakeObject.reset();

        if (!growthObjects.empty())
            growthObjects.clear();

        if (!poisonObjects.empty())
            poisonObjects.clear();

        if (gateObjects != nullptr)
            gateObjects.reset();

        // Rebuild game window
        mainScreen->rebuildGameWindow();

        // Print instructions to game window
        wattron(mainScreen->getGameWindow(), COLOR_PAIR(mainScreen->getDefaultWindowColorPair()));
        mvwprintw(mainScreen->getGameWindow(), 1, 1, "Stage %d will be started!", currentStageIndex + 1);
        mvwprintw(mainScreen->getGameWindow(), 2, 1, "Press ENTER key to start this game...");
        wattroff(mainScreen->getGameWindow(), COLOR_PAIR(mainScreen->getDefaultWindowColorPair()));
        wrefresh(mainScreen->getGameWindow());

        // Rebuild mission window
        mainScreen->rebuildMissionWindow();

        // Hold starting this game until press enter key
        while (wgetch(mainScreen->getGameWindow()) != 10);

        // Initialize current stage layout
        initializeCurrentStageLayout();

        // Initialize snake object
        snakeObject = std::make_unique<SnakeObject_t>();
        snakeObject->setHeadingDirection(HeadingDirection_t::right);
        handleNextSnakePiece(SnakePiece_t({ 3, 3 }));
        handleNextSnakePiece(snakeObject->getNextHead());
        handleNextSnakePiece(snakeObject->getNextHead());

        // Add growth objects to random coordinates
        if (growthObjects.empty())
        {
            growthObjects.resize(4);

            for (auto& growthObject : growthObjects)
                createGrowthObject(growthObject);
        }

        // Add poison objects to random coordinates
        if (poisonObjects.empty())
        {
            poisonObjects.resize(2);

            for (auto& poisonObject : poisonObjects)
                createPoisonObject(poisonObject);
        }

        // Disable keyboard input delays in game window
        nodelay(mainScreen->getGameWindow(), true);

        // Enable more keyboard inputs from game window
        keypad(mainScreen->getGameWindow(), true);

        // Do while loop until current stage is failed or completed
        while (!isCurrentStageIsFailed and !isCurrentStageIsCompleted[currentStageIndex])
        {
            // Get keyboard input and processing it
            processInput();

            // Update game status
            updateGameStatus();

            // Check current stage mission is completed or not
            checkCurrentStageMission();

            // Refresh game window
            wrefresh(mainScreen->getGameWindow());

            // Run delay command
            std::this_thread::sleep_for(std::chrono::microseconds(500000));
        }

        // If current stage is failed then immediately prepare to terminate this game
        if (isCurrentStageIsFailed)
            return;
    }
}

// This destructor will print final instructions to player and free memory if this game needs to be terminated
SnakeGame_t::~SnakeGame_t()
{
    // Rebuild game window
    mainScreen->rebuildGameWindow();

    // Print instructions to game window
    wattron(mainScreen->getGameWindow(), COLOR_PAIR(mainScreen->getDefaultWindowColorPair()));
    mvwprintw(mainScreen->getGameWindow(), 1, 1, "Game Over!");
    mvwprintw(mainScreen->getGameWindow(), 2, 1, "You scored %d points!", mainScreen->getScoreCounter());
    mvwprintw(mainScreen->getGameWindow(), 3, 1, "Press ENTER key to terminate this game...");

    for (int i = 0; i < static_cast<int>(isCurrentStageIsCompleted.size()); i++)
        mvwprintw(mainScreen->getGameWindow(), i + 5, 1, "You %s Stage %d!", (isCurrentStageIsCompleted[i]) ? "completed" : "not completed", i + 1);

    wattroff(mainScreen->getGameWindow(), COLOR_PAIR(mainScreen->getDefaultWindowColorPair()));
    wrefresh(mainScreen->getGameWindow());

    // Hold terminate this game until press enter key
    while (wgetch(mainScreen->getGameWindow()) != 10);
}

// This function will update game object coordinates to point random coordinates of empty object
void SnakeGame_t::getEmptyCoordinatesRandomly(GameObjectCoordinates_t& coordinates)
{
    std::random_device rd;
    std::ranlux48 gen(rd());
    std::uniform_int_distribution<int> distFirst(0, mainScreen->getGameWindowSizes().first - 1);
    std::uniform_int_distribution<int> distSecond(0, mainScreen->getGameWindowSizes().second - 1);

    GameObjectCharacter_t character = GameObjectCharacter_t::NullObject_t;

    do
    {
        character = getGameObjectCharacterFromWindow(mainScreen->getGameWindow(), { coordinates.first = distFirst(gen), coordinates.second = distSecond(gen) });

    } while (character != GameObjectCharacter_t::EmptyObject_t);
}

// This function will update game object coordinates to point random coordinates of empty object or border object
void SnakeGame_t::getEmptyOrBorderCoordinatesRandomly(GameObjectCoordinates_t& coordinates)
{
    std::random_device rd;
    std::ranlux48 gen(rd());
    std::uniform_int_distribution<int> distFirst(0, mainScreen->getGameWindowSizes().first - 1);
    std::uniform_int_distribution<int> distSecond(0, mainScreen->getGameWindowSizes().second - 1);

    GameObjectCharacter_t character = GameObjectCharacter_t::NullObject_t;

    do
    {
        character = getGameObjectCharacterFromWindow(mainScreen->getGameWindow(), { coordinates.first = distFirst(gen), coordinates.second = distSecond(gen) });

    } while (character != GameObjectCharacter_t::EmptyObject_t and character != GameObjectCharacter_t::HorizontalWall_t and character != GameObjectCharacter_t::VerticalWall_t);
}

// This function will get game object character from game window
// Return value of this function is cannot be able to discarded
[[nodiscard]] GameObjectCharacter_t SnakeGame_t::getGameObjectCharacterFromWindow(Window_t window, const GameObjectCoordinates_t& coordinates)
{
    return static_cast<GameObjectCharacter_t>(mvwinch(window, coordinates.first, coordinates.second));
}

// This function will add game object character to game window
void SnakeGame_t::addGameObjectCharacterToWindow(Window_t window, const GameObject_t& gameObject)
{
    mvwaddch(window, gameObject.getCoordinates().first, gameObject.getCoordinates().second, static_cast<char>(gameObject.getCharacter()));
}

// This function will update snake object based on specific situations
void SnakeGame_t::handleNextSnakePiece(SnakePiece_t nextPiece)
{
    // If growth objects and poison objects is existing in some other coordinates
    if (!growthObjects.empty() and !poisonObjects.empty())
    {
        // Get game object from next head of snake coordinates and run switch statement
        switch (getGameObjectCharacterFromWindow(mainScreen->getGameWindow(), nextPiece.getCoordinates()))
        {
            case GameObjectCharacter_t::EmptyObject_t:
                handlerForEmptyObject(nextPiece);
                return;

            case GameObjectCharacter_t::GrowthObject_t:
                handlerForGrowthObject(nextPiece);
                return;

            case GameObjectCharacter_t::PoisonObject_t:
                handlerForPoisonObject(nextPiece);
                return;

            case GameObjectCharacter_t::GatePiece_t:
                handlerForGateObjects(nextPiece);
                return;

            default:
                isCurrentStageIsFailed = true;
                return;
        }
    }

    // Add single piece to next head of snake coordinates
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), nextPiece);
    snakeObject->addPiece(nextPiece);
}

// This function will handle next head of snake if coordinates located in empty object
void SnakeGame_t::handlerForEmptyObject(SnakePiece_t nextPiece)
{
    // Remove tail of snake
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(snakeObject->getTail().getCoordinates()));
    snakeObject->removePiece();

    // Add single piece to next head of snake coordinates
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), nextPiece);
    snakeObject->addPiece(nextPiece);

    // If snake is located inside of gates then decrease counter for snake pieces
    if (gateObjects != nullptr and (isSnakeIsLocatedInsideOfGates and countOfSnakePiecesInsideOfGates != 0))
        countOfSnakePiecesInsideOfGates--;
}

// This function will handle next head of snake if coordinates located in growth object
void SnakeGame_t::handlerForGrowthObject(SnakePiece_t nextPiece)
{
    // Remove growth object and create it to another random coordinates
    for (auto& growthObject : growthObjects)
        if (growthObject->getCoordinates() == nextPiece.getCoordinates())
        {
            removeGrowthObject(growthObject);
            createGrowthObject(growthObject);
        }

    // Increase score counter
    mainScreen->setScoreCounter(mainScreen->getScoreCounter() + 10);

    // Rebuild score window
    mainScreen->rebuildScoreWindow();

    // Set snake size limit
    if (snakeObject->getSize() >= 20)
    {
        addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(snakeObject->getTail().getCoordinates()));
        snakeObject->removePiece();
    }

    // Add single piece to next head of snake coordinates
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), nextPiece);
    snakeObject->addPiece(nextPiece);

    // Update current stage missions
    if (std::strcmp(stageMissions[currentStageIndex].first, "Growth") == 0)
        stageMissions[currentStageIndex].second--;
}

// This function will handle next head of snake if coordinates located in poison object
void SnakeGame_t::handlerForPoisonObject(SnakePiece_t nextPiece)
{
    // Remove poison object and create it to another random coordinates
    for (auto& poisonObject : poisonObjects)
        if (poisonObject->getCoordinates() == nextPiece.getCoordinates())
        {
            removePoisonObject(poisonObject);
            createPoisonObject(poisonObject);
        }

    // Decrease score counter
    mainScreen->setScoreCounter(mainScreen->getScoreCounter() - 5);

    // Rebuild score window
    mainScreen->rebuildScoreWindow();

    // Remove tail of snake
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(snakeObject->getTail().getCoordinates()));
    snakeObject->removePiece();

    // If snake is located inside of gates then decrease counter for snake pieces
    if (gateObjects != nullptr and (isSnakeIsLocatedInsideOfGates and countOfSnakePiecesInsideOfGates != 0))
        countOfSnakePiecesInsideOfGates -= 2;

    // Update current stage missions
    if (std::strcmp(stageMissions[currentStageIndex].first, "Poison") == 0)
        stageMissions[currentStageIndex].second--;
}

// This function will handle next head of snake if coordinates located in gate objects
void SnakeGame_t::handlerForGateObjects(SnakePiece_t nextPiece)
{
    // Set next head of snake coordinates to another gate
    if (nextPiece.getCoordinates() == gateObjects->getFirstGate().getCoordinates())
        nextPiece.setCoordinates(gateObjects->getSecondGate().getCoordinates());
    else
        nextPiece.setCoordinates(gateObjects->getFirstGate().getCoordinates());

    if (nextPiece.getCoordinates().second == 0)
    {
        snakeObject->setHeadingDirection(HeadingDirection_t::right);
        nextPiece.setCoordinates({ nextPiece.getCoordinates().first, nextPiece.getCoordinates().second + 1 });
    }
    else if (nextPiece.getCoordinates().second == mainScreen->getGameWindowSizes().second - 1)
    {
        snakeObject->setHeadingDirection(HeadingDirection_t::left);
        nextPiece.setCoordinates({ nextPiece.getCoordinates().first, nextPiece.getCoordinates().second - 1 });
    }
    else if (nextPiece.getCoordinates().first == 0)
    {
        snakeObject->setHeadingDirection(HeadingDirection_t::down);
        nextPiece.setCoordinates({ nextPiece.getCoordinates().first + 1, nextPiece.getCoordinates().second });
    }
    else if (nextPiece.getCoordinates().first == mainScreen->getGameWindowSizes().first - 1)
    {
        snakeObject->setHeadingDirection(HeadingDirection_t::up);
        nextPiece.setCoordinates({ nextPiece.getCoordinates().first - 1, nextPiece.getCoordinates().second });
    }
    else
    {
        GameObjectCharacter_t character = GameObjectCharacter_t::NullObject_t;

        do
        {
            if (snakeObject->getHeadingDirection() == HeadingDirection_t::right and ((character = getGameObjectCharacterFromWindow(mainScreen->getGameWindow(), { nextPiece.getCoordinates().first, nextPiece.getCoordinates().second + 1 })) != GameObjectCharacter_t::EmptyObject_t))
                snakeObject->setHeadingDirection(HeadingDirection_t::down);
            if (snakeObject->getHeadingDirection() == HeadingDirection_t::left and ((character = getGameObjectCharacterFromWindow(mainScreen->getGameWindow(), { nextPiece.getCoordinates().first, nextPiece.getCoordinates().second - 1 })) != GameObjectCharacter_t::EmptyObject_t))
                snakeObject->setHeadingDirection(HeadingDirection_t::up);
            if (snakeObject->getHeadingDirection() == HeadingDirection_t::down and ((character = getGameObjectCharacterFromWindow(mainScreen->getGameWindow(), { nextPiece.getCoordinates().first + 1, nextPiece.getCoordinates().second })) != GameObjectCharacter_t::EmptyObject_t))
                snakeObject->setHeadingDirection(HeadingDirection_t::left);
            if (snakeObject->getHeadingDirection() == HeadingDirection_t::up and ((character = getGameObjectCharacterFromWindow(mainScreen->getGameWindow(), { nextPiece.getCoordinates().first - 1, nextPiece.getCoordinates().second })) != GameObjectCharacter_t::EmptyObject_t))
                snakeObject->setHeadingDirection(HeadingDirection_t::right);
        } while (character != GameObjectCharacter_t::EmptyObject_t);

        if (snakeObject->getHeadingDirection() == HeadingDirection_t::right)
            nextPiece.setCoordinates({ nextPiece.getCoordinates().first, nextPiece.getCoordinates().second + 1 });
        else if (snakeObject->getHeadingDirection() == HeadingDirection_t::left)
            nextPiece.setCoordinates({ nextPiece.getCoordinates().first, nextPiece.getCoordinates().second - 1 });
        else if (snakeObject->getHeadingDirection() == HeadingDirection_t::down)
            nextPiece.setCoordinates({ nextPiece.getCoordinates().first + 1, nextPiece.getCoordinates().second });
        else if (snakeObject->getHeadingDirection() == HeadingDirection_t::up)
            nextPiece.setCoordinates({ nextPiece.getCoordinates().first - 1, nextPiece.getCoordinates().second });
    }

    // Increase score counter
    mainScreen->setScoreCounter(mainScreen->getScoreCounter() + 5);

    // Rebuild score window
    mainScreen->rebuildScoreWindow();

    // Remove tail of snake
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(snakeObject->getTail().getCoordinates()));
    snakeObject->removePiece();

    // Add single piece to next head of snake coordinates
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), nextPiece);
    snakeObject->addPiece(nextPiece);

    // Set this boolean value that check snake is located inside of gates to true
    isSnakeIsLocatedInsideOfGates = true;

    // Set counter for snake pieces which located inside of gates
    countOfSnakePiecesInsideOfGates = static_cast<GameStatusCounter_t>(snakeObject->getSize());
}

// This function will make growth object and add to random coordinates of empty object
void SnakeGame_t::createGrowthObject(std::unique_ptr<GrowthObject_t>& growthObject)
{
    // Get random coordinates of empty object and store it
    GameObjectCoordinates_t coordinates;
    getEmptyCoordinatesRandomly(coordinates);

    // Create growth object with updated coordinates
    growthObject = std::make_unique<GrowthObject_t>(coordinates);

    // Add growth object to game window
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), *growthObject);
}

// This function will make poison object and add to random coordinates of empty object
void SnakeGame_t::createPoisonObject(std::unique_ptr<PoisonObject_t>& poisonObject)
{
    // Get random coordinates of empty object and store it
    GameObjectCoordinates_t coordinates;
    getEmptyCoordinatesRandomly(coordinates);

    // Create poison object with updated coordinates
    poisonObject = std::make_unique<PoisonObject_t>(coordinates);

    // Add poison object to game window
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), *poisonObject);
}

// This function will make gate objects and add to random coordinates of game window
void SnakeGame_t::createGateObjects()
{
    // Get random coordinates of empty object or border object and store it
    std::pair<GameObjectCoordinates_t, GameObjectCoordinates_t> coordinates;
    getEmptyOrBorderCoordinatesRandomly(coordinates.first);
    getEmptyOrBorderCoordinatesRandomly(coordinates.second);

    // Create gate objects with updated coordinates
    gateObjects = std::make_unique<GateObjects_t>(coordinates.first, coordinates.second);

    // Add gate objects to game window
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), gateObjects->getFirstGate());
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), gateObjects->getSecondGate());
}

// This function will remove growth object and add empty object to object coordinates
void SnakeGame_t::removeGrowthObject(std::unique_ptr<GrowthObject_t>& growthObject)
{
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(growthObject->getCoordinates()));
    growthObject.reset();
}

// This function will remove poison object and add empty object to object coordinates
void SnakeGame_t::removePoisonObject(std::unique_ptr<PoisonObject_t>& poisonObject)
{
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(poisonObject->getCoordinates()));
    poisonObject.reset();
}

// This function will remove gate objects and add empty object to object coordinates
void SnakeGame_t::removeGateObjects()
{
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(gateObjects->getFirstGate().getCoordinates()));
    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t(gateObjects->getSecondGate().getCoordinates()));

    switch (currentStageIndex % 4)
    {
        case 0:
            break;

        case 1:
            if (gateObjects->getFirstGate().getCoordinates().second == (mainScreen->getGameWindowSizes().second - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t(gateObjects->getFirstGate().getCoordinates()));

            if (gateObjects->getSecondGate().getCoordinates().second == (mainScreen->getGameWindowSizes().second - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t(gateObjects->getSecondGate().getCoordinates()));

            break;

        case 2:
            if (gateObjects->getFirstGate().getCoordinates().first == (mainScreen->getGameWindowSizes().first - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t(gateObjects->getFirstGate().getCoordinates()));

            if (gateObjects->getSecondGate().getCoordinates().first == (mainScreen->getGameWindowSizes().first - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t(gateObjects->getSecondGate().getCoordinates()));

            break;

        case 3:
            if (gateObjects->getFirstGate().getCoordinates().second == (mainScreen->getGameWindowSizes().second - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t(gateObjects->getFirstGate().getCoordinates()));

            if (gateObjects->getSecondGate().getCoordinates().second == (mainScreen->getGameWindowSizes().second - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t(gateObjects->getSecondGate().getCoordinates()));

            if (gateObjects->getFirstGate().getCoordinates().first == (mainScreen->getGameWindowSizes().first - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t(gateObjects->getFirstGate().getCoordinates()));

            if (gateObjects->getSecondGate().getCoordinates().first == (mainScreen->getGameWindowSizes().first - 1) / 2)
                addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t(gateObjects->getSecondGate().getCoordinates()));

            break;

    }

    if (gateObjects->getFirstGate().getCoordinates().second == 0 or gateObjects->getFirstGate().getCoordinates().second == mainScreen->getGameWindowSizes().second - 1)
        addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t(gateObjects->getFirstGate().getCoordinates()));

    if (gateObjects->getSecondGate().getCoordinates().second == 0 or gateObjects->getSecondGate().getCoordinates().second == mainScreen->getGameWindowSizes().second - 1)
        addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t(gateObjects->getSecondGate().getCoordinates()));

    if (gateObjects->getFirstGate().getCoordinates().first == 0 or gateObjects->getFirstGate().getCoordinates().first == mainScreen->getGameWindowSizes().first - 1)
        addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t(gateObjects->getFirstGate().getCoordinates()));

    if (gateObjects->getSecondGate().getCoordinates().first == 0 or gateObjects->getSecondGate().getCoordinates().first == mainScreen->getGameWindowSizes().first - 1)
        addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t(gateObjects->getSecondGate().getCoordinates()));

    gateObjects.reset();
}

// This function will initialize stage missions randomly
void SnakeGame_t::initializeStageMissions()
{
    std::random_device rd;
    std::ranlux48 gen(rd());
    std::uniform_int_distribution<int> distMissionKeyIndex(0, 3);
    std::uniform_int_distribution<int> distMissionCounter(5, 20);

    std::array<StageMissionKey_t, 4> stageMissionKeys = { "Size", "Growth", "Poison", "Gates" };

    for (auto& stageMission : stageMissions)
        stageMission = { stageMissionKeys[distMissionKeyIndex(gen)], distMissionCounter(gen) };
}

// This function will clear game window and start to build current stage layout
void SnakeGame_t::initializeCurrentStageLayout()
{
    // Rebuild game window
    mainScreen->rebuildGameWindow();

    // Start to build current stage
    switch (currentStageIndex % 4)
    {
        case 0:
            break;

        case 1:
            for (int i = 0; i < mainScreen->getGameWindowSizes().first; i++)
            {
                if (i == 0 or i == mainScreen->getGameWindowSizes().first - 1)
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), CornerWall_t({ i, (mainScreen->getGameWindowSizes().second - 1) / 2 }));
                else
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t({ i, (mainScreen->getGameWindowSizes().second - 1) / 2 }));
            }

            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) / 3, (mainScreen->getGameWindowSizes().second - 1) / 2 }));
            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) - ((mainScreen->getGameWindowSizes().first - 1) / 3), (mainScreen->getGameWindowSizes().second - 1) / 2 }));
            break;

        case 2:
            for (int i = 0; i < mainScreen->getGameWindowSizes().second; i++)
            {
                if (i == 0 or i == mainScreen->getGameWindowSizes().second - 1)
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), CornerWall_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, i }));
                else
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, i }));
            }

            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, (mainScreen->getGameWindowSizes().second - 1) / 3 }));
            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, (mainScreen->getGameWindowSizes().second - 1) - ((mainScreen->getGameWindowSizes().second - 1) / 3) }));
            break;

        case 3:
            for (int i = 0; i < mainScreen->getGameWindowSizes().first; i++)
            {
                if (i == 0 or i == mainScreen->getGameWindowSizes().first - 1)
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), CornerWall_t({ i, (mainScreen->getGameWindowSizes().second - 1) / 2 }));
                else
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), VerticalWall_t({ i, (mainScreen->getGameWindowSizes().second - 1) / 2 }));
            }

            for (int i = 0; i < mainScreen->getGameWindowSizes().second; i++)
            {
                if (i == 0 or i == mainScreen->getGameWindowSizes().second - 1)
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), CornerWall_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, i }));
                else
                    addGameObjectCharacterToWindow(mainScreen->getGameWindow(), HorizontalWall_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, i }));
            }

            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), CornerWall_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, (mainScreen->getGameWindowSizes().second - 1) / 2 }));

            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) / 3, (mainScreen->getGameWindowSizes().second - 1) / 2 }));
            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) - ((mainScreen->getGameWindowSizes().first - 1) / 3), (mainScreen->getGameWindowSizes().second - 1) / 2 }));

            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, (mainScreen->getGameWindowSizes().second - 1) / 3 }));
            addGameObjectCharacterToWindow(mainScreen->getGameWindow(), EmptyObject_t({ (mainScreen->getGameWindowSizes().first - 1) / 2, (mainScreen->getGameWindowSizes().second - 1) - ((mainScreen->getGameWindowSizes().second - 1) / 3) }));
            break;

        default:
            break;
    }
}

// This function will process inputs from game window
void SnakeGame_t::processInput()
{
    switch (wgetch(mainScreen->getGameWindow()))
    {
        case KEY_UP: snakeObject->setHeadingDirection(HeadingDirection_t::up); break;
        case KEY_DOWN: snakeObject->setHeadingDirection(HeadingDirection_t::down); break;
        case KEY_LEFT: snakeObject->setHeadingDirection(HeadingDirection_t::left); break;
        case KEY_RIGHT: snakeObject->setHeadingDirection(HeadingDirection_t::right); break;
        default: break;
    }
}

// This function will update game status
void SnakeGame_t::updateGameStatus()
{
    // Update next head of snake based on specific situations
    handleNextSnakePiece(snakeObject->getNextHead());

    // If size of snake is less than 3 or score counter is less than 0, prepare to terminate this game
    if (snakeObject->getSize() < 3 or mainScreen->getScoreCounter() < 0)
        isCurrentStageIsFailed = true;

    // If snake was located inside of gates and now fully get out from gates then remove gate objects
    if (gateObjects != nullptr and (isSnakeIsLocatedInsideOfGates and countOfSnakePiecesInsideOfGates <= 0))
    {
        // Set this boolean value that check snake is located inside of gates to false
        isSnakeIsLocatedInsideOfGates = false;

        // Update current stage missions
        if (std::strcmp(stageMissions[currentStageIndex].first, "Gates") == 0)
            stageMissions[currentStageIndex].second--;

        // Remove gate objects and create it to another random coordinates
        removeGateObjects();
    }

    // Increase timeout counter and compare it with 5000000, if timeout counter is equal to 5000000, recreate object to another random coordinates
    for (auto& growthObject : growthObjects)
    {
        growthObject->setTimeoutCounter(growthObject->getTimeoutCounter() + 500000);

        if (growthObject->getTimeoutCounter() == 5000000)
        {
            removeGrowthObject(growthObject);
            createGrowthObject(growthObject);
        }
    }

    // Increase timeout counter and compare it with 5000000, if timeout counter is equal to 5000000, recreate object to another random coordinates
    for (auto& poisonObject : poisonObjects)
    {
        poisonObject->setTimeoutCounter(poisonObject->getTimeoutCounter() + 500000);

        if (poisonObject->getTimeoutCounter() == 5000000)
        {
            removePoisonObject(poisonObject);
            createPoisonObject(poisonObject);
        }
    }

    // If gate objects is deleted and snake size is not less than specific size, it will add another gate objects to random coordinates
    if (gateObjects == nullptr and snakeObject->getSize() >= 5)
        createGateObjects();
}

// This function will check current stage mission is completed or not
void SnakeGame_t::checkCurrentStageMission()
{
    // Rebuild mission window
    mainScreen->rebuildMissionWindow();

    // Print instructions to mission window
    wattron(mainScreen->getMissionWindow(), COLOR_PAIR(mainScreen->getMissionWindowColorPair()));

    if (std::strcmp(stageMissions[currentStageIndex].first, "Size") == 0)
        mvwprintw(mainScreen->getMissionWindow(), 1, 0, "Size of snake are must to reach %d!", stageMissions[currentStageIndex].second);
    else if (std::strcmp(stageMissions[currentStageIndex].first, "Growth") == 0)
        mvwprintw(mainScreen->getMissionWindow(), 1, 0, "You have to get %d growth Objects!", stageMissions[currentStageIndex].second);
    else if (std::strcmp(stageMissions[currentStageIndex].first, "Poison") == 0)
        mvwprintw(mainScreen->getMissionWindow(), 1, 0, "You have to get %d poison Objects!", stageMissions[currentStageIndex].second);
    else if (std::strcmp(stageMissions[currentStageIndex].first, "Gates") == 0)
        mvwprintw(mainScreen->getMissionWindow(), 1, 0, "You have to pass %d gates!", stageMissions[currentStageIndex].second);

    wattroff(mainScreen->getMissionWindow(), COLOR_PAIR(mainScreen->getMissionWindowColorPair()));
    wrefresh(mainScreen->getMissionWindow());

    // If current mission is completed, prepare to end current stage
    if (stageMissions[currentStageIndex].second == 0 or (std::strcmp(stageMissions[currentStageIndex].first, "Size") == 0 and snakeObject->getSize() == stageMissions[currentStageIndex].second))
        isCurrentStageIsCompleted[currentStageIndex] = true;
}
