#pragma once

#include <string>
#include <vector>

struct RollRange {
    int from;
    int to;
};

struct RollRow {
    RollRange range;
    std::string result;  // "Remove a character"
};

struct RollTable {
    std::string title;   // "Scene Adjustment Table"
    std::string dice;    // "1d10"
    std::vector<RollRow> rows;
};
