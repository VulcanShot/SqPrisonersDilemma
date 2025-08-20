#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "squirrel_imp.h"

int main(int argc, char** argv)
{
    GameSettings settings = {
        .game_length = 100,
        .temptation  = 5,
        .reward      = 3,
        .penalty     = 1,
        .sucker      = 0,
        .random_seed = time(NULL)
    };
    const char *scripts[] = { "strategies/tit_for_tat.nut", "strategies/friedman.nut" };
    StrategyInfo strategies[2] = {};

    if (!play_game(settings, scripts, strategies))
    {
        fprintf(stderr, "Game did not go well, exiting.\n");
        return 1;
    }

    for (int i = 0; i < 2; i++)
    {
        printf("%s : %ld\n", strategies[i].name, strategies[i].score);
    }

    return 0;
}