#include "engine.hpp"
#include <iostream>

int hello_world() {
    std::cout << "from library";
    return 0;
}

/// Internal struct to store the probability table
struct OddsProbability {
    TRE_Odds odd;
    int chaos;
    int lower;
    int normal;
    int upper;
};

/// We must declare it somewhere
OddsProbability oddsProbability[] = {
        // Certain
    { TRE_Odds::CERTAIN, 1, 10, 50, 91 },
    { TRE_Odds::CERTAIN, 2, 13, 65, 94 },
    { TRE_Odds::CERTAIN, 3, 15, 75, 96 },
    { TRE_Odds::CERTAIN, 4, 17, 85, 98 },
    { TRE_Odds::CERTAIN, 5, 18, 90, 99 },
    { TRE_Odds::CERTAIN, 6, 19, 95, 100 },
    { TRE_Odds::CERTAIN, 7, 20, 99, -1 },
    { TRE_Odds::CERTAIN, 8, 20, 99, -1 },
    { TRE_Odds::CERTAIN, 9, 20, 99, -1 },

    // Nearly Certain
    { TRE_Odds::NEARLY_CERTAIN, 1, 7, 35, 88 },
    { TRE_Odds::NEARLY_CERTAIN, 2, 10, 50, 91 },
    { TRE_Odds::NEARLY_CERTAIN, 3, 13, 65, 94 },
    { TRE_Odds::NEARLY_CERTAIN, 4, 15, 75, 96 },
    { TRE_Odds::NEARLY_CERTAIN, 5, 17, 85, 98 },
    { TRE_Odds::NEARLY_CERTAIN, 6, 18, 90, 99 },
    { TRE_Odds::NEARLY_CERTAIN, 7, 19, 95, 100 },
    { TRE_Odds::NEARLY_CERTAIN, 8, 20, 99, -1 },
    { TRE_Odds::NEARLY_CERTAIN, 9, 20, 99, -1 },

    // Very Likely
    { TRE_Odds::VERY_LIKELY, 1, 5, 25, 86 },
    { TRE_Odds::VERY_LIKELY, 2, 7, 35, 88 },
    { TRE_Odds::VERY_LIKELY, 3, 10, 50, 91 },
    { TRE_Odds::VERY_LIKELY, 4, 13, 65, 94 },
    { TRE_Odds::VERY_LIKELY, 5, 15, 75, 96 },
    { TRE_Odds::VERY_LIKELY, 6, 17, 85, 98 },
    { TRE_Odds::VERY_LIKELY, 7, 18, 90, 99 },
    { TRE_Odds::VERY_LIKELY, 8, 19, 95, 100 },
    { TRE_Odds::VERY_LIKELY, 9, 20, 99, -1 },

    // Likely
    { TRE_Odds::LIKELY, 1, 3, 15, 84 },
    { TRE_Odds::LIKELY, 2, 5, 25, 86 },
    { TRE_Odds::LIKELY, 3, 7, 35, 88 },
    { TRE_Odds::LIKELY, 4, 10, 50, 91 },
    { TRE_Odds::LIKELY, 5, 13, 65, 94 },
    { TRE_Odds::LIKELY, 6, 15, 75, 96 },
    { TRE_Odds::LIKELY, 7, 17, 85, 98 },
    { TRE_Odds::LIKELY, 8, 18, 90, 99 },
    { TRE_Odds::LIKELY, 9, 19, 95, 100 },

    // 50/50
    { TRE_Odds::FIFTH_FIFTH, 1, 2, 10, 83 },
    { TRE_Odds::FIFTH_FIFTH, 2, 3, 15, 84 },
    { TRE_Odds::FIFTH_FIFTH, 3, 5, 25, 86 },
    { TRE_Odds::FIFTH_FIFTH, 4, 7, 35, 88 },
    { TRE_Odds::FIFTH_FIFTH, 5, 10, 50, 91 },
    { TRE_Odds::FIFTH_FIFTH, 6, 13, 65, 94 },
    { TRE_Odds::FIFTH_FIFTH, 7, 15, 75, 96 },
    { TRE_Odds::FIFTH_FIFTH, 8, 17, 85, 98 },
    { TRE_Odds::FIFTH_FIFTH, 9, 18, 90, 99 },

    // Unlikely
    { TRE_Odds::UNLIKELY, 1, 1, 5, 82 },
    { TRE_Odds::UNLIKELY, 2, 2, 10, 83 },
    { TRE_Odds::UNLIKELY, 3, 3, 15, 84 },
    { TRE_Odds::UNLIKELY, 4, 5, 25, 86 },
    { TRE_Odds::UNLIKELY, 5, 7, 35, 88 },
    { TRE_Odds::UNLIKELY, 6, 10, 50, 91 },
    { TRE_Odds::UNLIKELY, 7, 13, 65, 94 },
    { TRE_Odds::UNLIKELY, 8, 15, 75, 96 },
    { TRE_Odds::UNLIKELY, 9, 17, 85, 98 },

    // Very Unlikely
    { TRE_Odds::VERY_UNLIKELY, 1, 0, 1, 81 },
    { TRE_Odds::VERY_UNLIKELY, 2, 1, 5, 82 },
    { TRE_Odds::VERY_UNLIKELY, 3, 2, 10, 83 },
    { TRE_Odds::VERY_UNLIKELY, 4, 3, 15, 84 },
    { TRE_Odds::VERY_UNLIKELY, 5, 5, 25, 86 },
    { TRE_Odds::VERY_UNLIKELY, 6, 7, 35, 88 },
    { TRE_Odds::VERY_UNLIKELY, 7, 10, 50, 91 },
    { TRE_Odds::VERY_UNLIKELY, 8, 13, 65, 94 },
    { TRE_Odds::VERY_UNLIKELY, 9, 15, 75, 96 },

    // Nearly Impossible
    { TRE_Odds::NEARLY_IMPOSSIBLE, 1, 0, 1, 81 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 2, 0, 1, 81 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 3, 1, 5, 82 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 4, 2, 10, 83 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 5, 3, 15, 84 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 6, 5, 25, 86 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 7, 7, 35, 88 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 8, 10, 50, 91 },
    { TRE_Odds::NEARLY_IMPOSSIBLE, 9, 13, 65, 94 },

    // Impossible
    { TRE_Odds::IMPOSSIBLE, 1, -1, 1, 81 },
    { TRE_Odds::IMPOSSIBLE, 2, -1, 1, 81 },
    { TRE_Odds::IMPOSSIBLE, 3, -1, 1, 81 },
    { TRE_Odds::IMPOSSIBLE, 4, 1, 5, 82 },
    { TRE_Odds::IMPOSSIBLE, 5, 2, 10, 83 },
    { TRE_Odds::IMPOSSIBLE, 6, 3, 15, 84 },
    { TRE_Odds::IMPOSSIBLE, 7, 5, 25, 86 },
    { TRE_Odds::IMPOSSIBLE, 8, 7, 35, 88 },
    { TRE_Odds::IMPOSSIBLE, 9, 10, 50, 91 },
};

// Ask something to the oracle
TRE_FateAnswer TRE_GMEmulator::AskFate(int diceRoll, TRE_Odds chosenOdd, int chaosFactor){
    for(int i = 0; i < sizeof(oddsProbability)/sizeof(OddsProbability); i++) {
        const auto odd = oddsProbability[i];
        if(odd.odd == chosenOdd && odd.chaos == chaosFactor) {
            if(odd.lower > 0) {
                if(diceRoll < odd.lower) {
                    return TRE_FateAnswer::EXCEPTIONAL_FAIL;
                }
            }

            if(diceRoll >= odd.upper) {
                return TRE_FateAnswer::EXCEPTIONAL_SUCESS;
            }

            if (diceRoll < odd.normal) {
                return TRE_FateAnswer::FAIL;
            }

            return TRE_FateAnswer::SUCCESS;
        }
    }

    return TRE_FateAnswer::SUCCESS;
}

// Test if the scene can proceed as expected
TRE_SceneTestResult TRE_GMEmulator::TestScene(int diceRoll, int chaosFactor){
    if(diceRoll > chaosFactor) {
        return  TRE_SceneTestResult::EXPECTED_SCENE;
    }

    if(diceRoll % 2 == 0) {
        return TRE_SceneTestResult::INTERRUPT_SCENE;
    }

    return TRE_SceneTestResult::ALTERED_SCENE;
}