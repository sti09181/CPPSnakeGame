//////////////////////////
///// MainScreen.hpp /////
//////////////////////////

#pragma once
#include "Libraries.hpp"
#include "Definitions.hpp"
#include "GameObjects.hpp"

class MainScreen_t
{
private:
    // These fields are windows for this game
    Window_t gameWindow = nullptr;
    Window_t scoreWindow = nullptr;
    Window_t missionBorder = nullptr;
    Window_t missionWindow = nullptr;

    // These fields are indexes for color pairs
    static constexpr ColorPairIndex_t defaultColorPair = 1;
    static constexpr ColorPairIndex_t defaultWindowColorPair = 2;
    static constexpr ColorPairIndex_t statusWindowColorPair = 3;
    static constexpr ColorPairIndex_t missionWindowColorPair = 4;

    // This field is score counter for this game
    GameStatusCounter_t scoreCounter = 0;

    // These fields are coordinates and sizes for main windows
    const WindowSizes_t defaultWindowSizes = { 24, 80 };

    const WindowCoordinates_t gameWindowCoordinates = { 3, 3 };
    const WindowSizes_t gameWindowSizes = { defaultWindowSizes.first - 5, defaultWindowSizes.second - 35 };

    const WindowCoordinates_t scoreWindowCoordinates = { gameWindowCoordinates.first + 1, (gameWindowCoordinates.second + gameWindowSizes.second) + ((defaultWindowSizes.second - (gameWindowCoordinates.second + gameWindowSizes.second)) / 4) };
    const WindowSizes_t scoreWindowSizes = { 4, 15 };

    const WindowCoordinates_t missionBorderCoordinates = { scoreWindowCoordinates.first + 5, (gameWindowCoordinates.second + gameWindowSizes.second) + ((scoreWindowCoordinates.second - (gameWindowCoordinates.second + gameWindowSizes.second)) / 2) };
    const WindowSizes_t missionBorderSizes = { gameWindowCoordinates.first + gameWindowSizes.first - missionBorderCoordinates.first - 1, defaultWindowSizes.second - 5 - missionBorderCoordinates.second };

    const WindowCoordinates_t missionWindowCoordinates = { missionBorderCoordinates.first + 1, missionBorderCoordinates.second + 1 };
    const WindowSizes_t missionWindowSizes = { missionBorderSizes.first - 2, missionBorderSizes.second - 2 };

public:
    // This constructor will build main screen for this game
    // This constructor must not throw any exceptions!
    MainScreen_t() noexcept;

    // This destructor will free memory if this game screen needs to be deleted
    // This destructor must not throw any exceptions!
    ~MainScreen_t() noexcept;

    // This function will rebuild game window
    void rebuildGameWindow();

    // This function will rebuild score window
    void rebuildScoreWindow();

    // This function will rebuild border object for mission window
    void rebuildMissionBorder();

    // This function will rebuild mission window
    void rebuildMissionWindow();

    // This function will set value of score counter
    void setScoreCounter(GameStatusCounter_t scoreCounterInput);

    // This function will return game window
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] Window_t getGameWindow() const;

    // This function will return score window
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] Window_t getScoreWindow() const;

    // This function will return border object for mission window
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] Window_t getMissionBorder() const;

    // This function will return mission window
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] Window_t getMissionWindow() const;

    // This function will return index of default color pair
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] ColorPairIndex_t getDefaultColorPair() const;

    // This function will return index of default window color pair
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] ColorPairIndex_t getDefaultWindowColorPair() const;

    // This function will return index of status window color pair
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] ColorPairIndex_t getStatusWindowColorPair() const;

    // This function will return index of mission window color pair
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] ColorPairIndex_t getMissionWindowColorPair() const;

    // This function will return score counter
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] GameStatusCounter_t getScoreCounter() const;

    // This function will return default window sizes
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowSizes_t getDefaultWindowSizes() const;

    // This function will return game window coordinates
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowCoordinates_t getGameWindowCoordinates() const;

    // This function will return game window sizes
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowSizes_t getGameWindowSizes() const;

    // This function will return score window coordinates
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowCoordinates_t getScoreWindowCoordinates() const;

    // This function will return score window sizes
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowSizes_t getScoreWindowSizes() const;

    // This function will return border object for mission window coordinates
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowCoordinates_t getMissionBorderCoordinates() const;

    // This function will return border object for mission window sizes
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowSizes_t getMissionBorderSizes() const;

    // This function will return mission window coordinates
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowCoordinates_t getMissionWindowCoordinates() const;

    // This function will return mission window sizes
    // Return value of this function is cannot be able to discarded!
    [[nodiscard]] WindowSizes_t getMissionWindowSizes() const;
};
