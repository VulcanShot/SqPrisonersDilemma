#ifndef INCLUDE_SQUIRREL_IMP
#define INCLUDE_SQUIRREL_IMP

#include <squirrel.h>
#include <stdbool.h>

#define STRATEGY_COUNT 2

typedef struct {
    int game_length;
    int temptation;
    int reward;
    int penalty;
    int sucker;
} GameSettings;

typedef struct {
    const char *name;
    const char *author;
    unsigned long score;
} StrategyInfo;

typedef struct {
    StrategyInfo *info;
    HSQUIRRELVM vm;
} Strategy;

typedef enum {
    NO_MOVE,
    COOPERATION,
    DEFECTION
} EMove;

typedef EMove Turn[STRATEGY_COUNT];

bool play_game(GameSettings settings, const char *scripts[STRATEGY_COUNT], StrategyInfo strategies_out[STRATEGY_COUNT]);
bool load_strategy(const char *filename, Strategy *strategy, StrategyInfo *strategy_info);
HSQUIRRELVM initialize_vm();
void push_history(HSQUIRRELVM v, Turn game_history[], int turn_count, int strat_index);
void push_strat_history(HSQUIRRELVM v, Turn game_history[], int turn_count, int strat_index);
EMove get_move(HSQUIRRELVM v, Turn game_history[], int turn_count, int strat_index);
void update_scores(StrategyInfo strategies[STRATEGY_COUNT], Turn turn, GameSettings settings);
void update_scores_helper(StrategyInfo strategies[STRATEGY_COUNT], int strat1_points, int strat2_points);
void printfunc(HSQUIRRELVM v, const SQChar *s, ...);
void errorfunc(HSQUIRRELVM v, const SQChar *s, ...);

#endif /* INCLUDE_SQUIRREL_IMP */
