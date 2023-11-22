///////////////////////////
///// Definitions.hpp /////
///////////////////////////

#pragma once
#include "Libraries.hpp"

// These type definitions are helpers to make code comprehensible
using Window_t = WINDOW*;
using ColorPairIndex_t = int;
using GameStatusCounter_t = int;
using WindowCoordinates_t = std::pair<int, int>;
using WindowSizes_t = std::pair<int, int>;
using GameObjectCoordinates_t = std::pair<int, int>;
using StageCounter_t = int;
using StageMissionKey_t = const char*;
using StageMissionCounter_t = int;
using GameStatusBoolean_t = bool;
