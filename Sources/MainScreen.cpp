//////////////////////////
///// MainScreen.cpp /////
//////////////////////////

#include "MainScreen.hpp"

// This constructor will build main screen for this game
// This constructor must not throw any exceptions
MainScreen_t::MainScreen_t() noexcept
{
    // Initialize curses
    initscr();

    // Enable color mode
    start_color();

    // Resize current terminal
    resize_term(defaultWindowSizes.first, defaultWindowSizes.second);

    // Disable cursor
    curs_set(0);

    // Disable echo for every single inputs
    noecho();

    // Enable Ctrl + C keyboard shortcut to terminate program
    cbreak();

    // Initialize color pairs
    init_pair(defaultColorPair, COLOR_WHITE, COLOR_BLACK);
    init_pair(defaultWindowColorPair, COLOR_CYAN, COLOR_BLACK);
    init_pair(statusWindowColorPair, COLOR_YELLOW, COLOR_BLACK);
    init_pair(missionWindowColorPair, COLOR_BLACK, COLOR_WHITE);

    // Initialize default window
    wborder(stdscr, static_cast<char>(GameObjectCharacter_t::VerticalWall_t), static_cast<char>(GameObjectCharacter_t::VerticalWall_t),
                    static_cast<char>(GameObjectCharacter_t::HorizontalWall_t), static_cast<char>(GameObjectCharacter_t::HorizontalWall_t),
                    static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t),
                    static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t));
    wattron(stdscr, COLOR_PAIR(defaultWindowColorPair));
    mvwprintw(stdscr, 1, defaultWindowSizes.second / 2 - 10, "Snake Game");
    wattroff(stdscr, COLOR_PAIR(defaultWindowColorPair));
    wrefresh(stdscr);

    // Initialize game window
    gameWindow = newwin(gameWindowSizes.first, gameWindowSizes.second, gameWindowCoordinates.first, gameWindowCoordinates.second);
    wborder(gameWindow, static_cast<char>(GameObjectCharacter_t::VerticalWall_t), static_cast<char>(GameObjectCharacter_t::VerticalWall_t),
                        static_cast<char>(GameObjectCharacter_t::HorizontalWall_t), static_cast<char>(GameObjectCharacter_t::HorizontalWall_t),
                        static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t),
                        static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t));
    wrefresh(gameWindow);

    // Initialize score window
    scoreWindow = newwin(scoreWindowSizes.first, scoreWindowSizes.second, scoreWindowCoordinates.first, scoreWindowCoordinates.second);
    wborder(scoreWindow, static_cast<char>(GameObjectCharacter_t::VerticalWall_t), static_cast<char>(GameObjectCharacter_t::VerticalWall_t),
                         static_cast<char>(GameObjectCharacter_t::HorizontalWall_t), static_cast<char>(GameObjectCharacter_t::HorizontalWall_t),
                         static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t),
                         static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t));
    wattron(scoreWindow, COLOR_PAIR(statusWindowColorPair));
    mvwprintw(scoreWindow, 1, 1, "Score:");
    mvwprintw(scoreWindow, 2, 1, "%d", scoreCounter);
    wattroff(scoreWindow, COLOR_PAIR(statusWindowColorPair));
    wrefresh(scoreWindow);

    // Initialize border object for mission window
    missionBorder = newwin(missionBorderSizes.first, missionBorderSizes.second, missionBorderCoordinates.first, missionBorderCoordinates.second);
    wborder(missionBorder, static_cast<char>(GameObjectCharacter_t::VerticalWall_t), static_cast<char>(GameObjectCharacter_t::VerticalWall_t),
                           static_cast<char>(GameObjectCharacter_t::HorizontalWall_t), static_cast<char>(GameObjectCharacter_t::HorizontalWall_t),
                           static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t),
                           static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t));
    wbkgd(missionBorder, COLOR_PAIR(missionWindowColorPair));
    wrefresh(missionBorder);

    // Initialize mission window
    missionWindow = newwin(missionWindowSizes.first, missionWindowSizes.second, missionWindowCoordinates.first, missionWindowCoordinates.second);
    wbkgd(missionWindow, COLOR_PAIR(missionWindowColorPair));
    wattron(missionWindow, COLOR_PAIR(missionWindowColorPair));
    mvwprintw(missionWindow, 0, 0, "Missions:");
    wattroff(missionWindow, COLOR_PAIR(missionWindowColorPair));
    wrefresh(missionWindow);
}

// This destructor will free memory if this game screen needs to be deleted
// This destructor must not throw any exceptions!
MainScreen_t::~MainScreen_t() noexcept
{
    delwin(gameWindow);
    delwin(scoreWindow);
    delwin(missionBorder);
    delwin(missionWindow);
    endwin();
}

// This function will rebuild game window
void MainScreen_t::rebuildGameWindow()
{
    wborder(gameWindow, static_cast<char>(GameObjectCharacter_t::VerticalWall_t), static_cast<char>(GameObjectCharacter_t::VerticalWall_t),
                        static_cast<char>(GameObjectCharacter_t::HorizontalWall_t), static_cast<char>(GameObjectCharacter_t::HorizontalWall_t),
                        static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t),
                        static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t));

    for (int i = 1; i < gameWindowSizes.first - 1; i++)
        for (int j = 1; j < gameWindowSizes.second - 1; j++)
            mvwaddch(gameWindow, i, j, ' ');

    wrefresh(gameWindow);
}

// This function will rebuild score window
void MainScreen_t::rebuildScoreWindow()
{
    wborder(scoreWindow, static_cast<char>(GameObjectCharacter_t::VerticalWall_t), static_cast<char>(GameObjectCharacter_t::VerticalWall_t),
                         static_cast<char>(GameObjectCharacter_t::HorizontalWall_t), static_cast<char>(GameObjectCharacter_t::HorizontalWall_t),
                         static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t),
                         static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t));

    for (int i = 1; i < scoreWindowSizes.first - 1; i++)
        for (int j = 1; j < scoreWindowSizes.second - 1; j++)
            mvwaddch(scoreWindow, i, j, ' ');

    wattron(scoreWindow, COLOR_PAIR(statusWindowColorPair));
    mvwprintw(scoreWindow, 1, 1, "Score:");
    mvwprintw(scoreWindow, 2, 1, "%d", scoreCounter);
    wattroff(scoreWindow, COLOR_PAIR(statusWindowColorPair));
    wrefresh(scoreWindow);
}

// This function will rebuild border object for mission window
void MainScreen_t::rebuildMissionBorder()
{
    wborder(missionBorder, static_cast<char>(GameObjectCharacter_t::VerticalWall_t), static_cast<char>(GameObjectCharacter_t::VerticalWall_t),
                           static_cast<char>(GameObjectCharacter_t::HorizontalWall_t), static_cast<char>(GameObjectCharacter_t::HorizontalWall_t),
                           static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t),
                           static_cast<char>(GameObjectCharacter_t::CornerWall_t), static_cast<char>(GameObjectCharacter_t::CornerWall_t));

    for (int i = 1; i < missionBorderSizes.first - 1; i++)
        for (int j = 1; j < missionBorderSizes.second - 1; j++)
            mvwaddch(missionBorder, i, j, ' ');

    wbkgd(missionBorder, COLOR_PAIR(missionWindowColorPair));
    wrefresh(missionBorder);
}

// This function will rebuild mission window
void MainScreen_t::rebuildMissionWindow()
{
    for (int i = 0; i < missionWindowSizes.first; i++)
        for (int j = 0; j < missionWindowSizes.second; j++)
            mvwaddch(missionWindow, i, j, ' ');

    wbkgd(missionWindow, COLOR_PAIR(missionWindowColorPair));
    wattron(missionWindow, COLOR_PAIR(missionWindowColorPair));
    mvwprintw(missionWindow, 0, 0, "Missions:");
    wattroff(missionWindow, COLOR_PAIR(missionWindowColorPair));
    wrefresh(missionWindow);
}

// This function will set value of score counter
void MainScreen_t::setScoreCounter(GameStatusCounter_t scoreCounterInput)
{
    scoreCounter = scoreCounterInput;
}

// This function will return game window
// Return value of this function is cannot be able to discarded!
[[nodiscard]] Window_t MainScreen_t::getGameWindow() const
{
    return gameWindow;
}

// This function will return score window
// Return value of this function is cannot be able to discarded!
[[nodiscard]] Window_t MainScreen_t::getScoreWindow() const
{
    return scoreWindow;
}

// This function will return border object for mission window
// Return value of this function is cannot be able to discarded!
[[nodiscard]] Window_t MainScreen_t::getMissionBorder() const
{
    return missionBorder;
}

// This function will return mission window
// Return value of this function is cannot be able to discarded!
[[nodiscard]] Window_t MainScreen_t::getMissionWindow() const
{
    return missionWindow;
}

// This function will return index of default color pair
// Return value of this function is cannot be able to discarded!
[[nodiscard]] ColorPairIndex_t MainScreen_t::getDefaultColorPair() const
{
    return defaultColorPair;
}

// This function will return index of default window color pair
// Return value of this function is cannot be able to discarded!
[[nodiscard]] ColorPairIndex_t MainScreen_t::getDefaultWindowColorPair() const
{
    return defaultWindowColorPair;
}

// This function will return index of status window color pair
// Return value of this function is cannot be able to discarded!
[[nodiscard]] ColorPairIndex_t MainScreen_t::getStatusWindowColorPair() const
{
    return statusWindowColorPair;
}

// This function will return index of mission window color pair
// Return value of this function is cannot be able to discarded!
[[nodiscard]] ColorPairIndex_t MainScreen_t::getMissionWindowColorPair() const
{
    return missionWindowColorPair;
}

// This function will return score counter
// Return value of this function is cannot be able to discarded!
[[nodiscard]] GameStatusCounter_t MainScreen_t::getScoreCounter() const
{
    return scoreCounter;
}

// This function will return default window sizes
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowSizes_t MainScreen_t::getDefaultWindowSizes() const
{
    return defaultWindowSizes;
}

// This function will return game window coordinates
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowCoordinates_t MainScreen_t::getGameWindowCoordinates() const
{
    return gameWindowCoordinates;
}

// This function will return game window sizes
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowSizes_t MainScreen_t::getGameWindowSizes() const
{
    return gameWindowSizes;
}

// This function will return score window coordinates
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowCoordinates_t MainScreen_t::getScoreWindowCoordinates() const
{
    return scoreWindowCoordinates;
}

// This function will return score window sizes
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowSizes_t MainScreen_t::getScoreWindowSizes() const
{
    return scoreWindowSizes;
}

// This function will return border object for mission window coordinates
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowCoordinates_t MainScreen_t::getMissionBorderCoordinates() const
{
    return missionBorderCoordinates;
}

// This function will return border object for mission window sizes
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowSizes_t MainScreen_t::getMissionBorderSizes() const
{
    return missionBorderSizes;
}

// This function will return mission window coordinates
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowCoordinates_t MainScreen_t::getMissionWindowCoordinates() const
{
    return missionWindowCoordinates;
}

// This function will return mission window sizes
// Return value of this function is cannot be able to discarded!
[[nodiscard]] WindowSizes_t MainScreen_t::getMissionWindowSizes() const
{
    return missionWindowSizes;
}
