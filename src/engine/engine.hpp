#pragma once

int hello_world();

enum TRE_Odds {
    CERTAIN,
    NEARLY_CERTAIN,
    VERY_LIKELY,
    LIKELY,
    FIFTH_FIFTH,
    UNLIKELY,
    VERY_UNLIKELY,
    NEARLY_IMPOSSIBLE,
    IMPOSSIBLE
};

enum TRE_FateAnswer {
    EXCEPTIONAL_FAIL,
    FAIL,
    SUCCESS,
    EXCEPTIONAL_SUCESS
};

enum TRE_SceneTestResult {
    EXPECTED_SCENE,
    ALTERED_SCENE,
    INTERRUPT_SCENE
};

struct TRE_GMEmulator {
    TRE_FateAnswer AskFate(int diceRoll, TRE_Odds chosenOdds, int chaosFactor);
    TRE_SceneTestResult TestScene(int diceRoll, int chaosFactor);
};