#include <squirrel_imp.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>
#include <sqstdmath.h>
#include <sqstdstring.h>

#include "squirrel_utils.c"

#ifdef _MSC_VER
#pragma comment (lib ,"squirrel.lib")
#pragma comment (lib ,"sqstdlib.lib")
#endif

#ifdef SQUNICODE

#define scvprintf vfwprintf
#else

#define scvprintf vfprintf
#endif

#define VM_SIZE 1024
#define MISSING_DATA_STR "[MISSING]"

bool play_game(GameSettings settings, const char *scripts[STRATEGY_COUNT], StrategyInfo strategies_out[STRATEGY_COUNT])
{
    Strategy strategies[STRATEGY_COUNT];
    srand(settings.random_seed);

    for (int i = 0; i < STRATEGY_COUNT; i++)
    {
        if (!load_strategy(scripts[i], &strategies[i], &strategies_out[i]))
        {
            fprintf(stderr, "Failed to load strategy %d.\n", i);
            return false;
        }
    }

    Turn game_history[settings.game_length];

    // TODO: Simulate `w` discount parameter
    for (int turn_i = 0; turn_i < settings.game_length; turn_i++)
    {
        for (int strat_i = 0; strat_i < STRATEGY_COUNT; strat_i++)
        {
            game_history[turn_i][strat_i] = get_move(strategies[strat_i].vm, game_history, turn_i, strat_i);
            if (game_history[turn_i][strat_i] == NO_MOVE)
            {
                fprintf(stderr, "Strategy '%s' was not able to make move %d.\n", strategies[strat_i].info->name, turn_i);
                return false;
            }
        }
        
        update_scores(strategies_out, game_history[turn_i], settings);
    }

    for (int i_strat = 0; i_strat < STRATEGY_COUNT; i_strat++)
    {
        sq_close(strategies[i_strat].vm);
    }

    return true;
}

bool load_strategy(const char *filename, Strategy *strategy, StrategyInfo *strategy_info)
{
    strategy->info = strategy_info;
    HSQUIRRELVM v = strategy->vm = initialize_vm();

    sq_pushconsttable(v);
    sq_pushstring(v, "EMove", -1);

    sq_newtableex(v, 2);
    sq_push_integer_slot(v, "NONE", NO_MOVE);
    sq_push_integer_slot(v, "COOPERATION", COOPERATION);
    sq_push_integer_slot(v, "DEFECTION", DEFECTION);

    sq_newslot(v, -3, SQFalse);
    sq_setconsttable(v);

    sq_pushroottable(v);
    SQRESULT result = sqstd_dofile(v, _SC(filename), SQFalse, SQTrue); // Leaves the root table in the stack
    
    if (SQ_FAILED(result))
    {
        return false;
    }

    if ( SQ_FAILED(sq_get_string_slot(v, "NAME", &strategy->info->name)) )
        strategy->info->name = MISSING_DATA_STR;

    if ( SQ_FAILED(sq_get_string_slot(v, "AUTHOR", &strategy->info->author)) )
        strategy->info->author = MISSING_DATA_STR;

    sq_pop(v, 1); // Pop root table

    return true;
}

HSQUIRRELVM initialize_vm()
{
    HSQUIRRELVM v;
    v = sq_open(VM_SIZE);

    sq_pushroottable(v);
    sqstd_register_mathlib(v);
    sqstd_register_stringlib(v);
    sq_pop(v, 1);

    sqstd_seterrorhandlers(v);
    sq_setprintfunc(v, printfunc,errorfunc);

    return v;
}

EMove get_move(HSQUIRRELVM v, Turn game_history[], int turn_count, int strat_index)
{
    SQInteger initial_size = sq_gettop(v);

    // Push closure `make_move` to stack
    sq_pushroottable(v);
    sq_pushstring(v,_SC("make_move"),-1);
    if (SQ_FAILED(sq_get(v, -2))) {
        fprintf(stderr, "Failed to fetch function");
        sq_settop(v, initial_size);
        return NO_MOVE;
    }

    // Call closure `make_move`
    sq_pushroottable(v);
    push_history(v, game_history, turn_count, strat_index);
    if ( SQ_FAILED(sq_call(v, 2, SQTrue, SQTrue)) ) {
        fprintf(stderr, "Failed to call function\n");
        sq_settop(v, initial_size);
        return NO_MOVE;
    }

    // Get returning value
    SQInteger move;
    sq_getinteger(v, -1, &move);

    // Restore stack
    sq_settop(v,initial_size);

    return (EMove)move;
}

void push_history(HSQUIRRELVM v, Turn game_history[], int turn_count, int strat_index)
{
    sq_newtableex(v, 2);

    sq_pushstring(v, "self", -1); // TODO: Modify sq_pushstring for default len=-1
    push_strat_history(v, game_history, turn_count, strat_index);

    sq_pushstring(v, "other", -1);
    push_strat_history(v, game_history, turn_count, strat_index == 1 ? 0 : 1);
}

void push_strat_history(HSQUIRRELVM v, Turn game_history[], int turn_count, int strat_index)
{
    sq_newarray(v, turn_count);
    for (int i = 0; i < turn_count; i++)
    {
        if (game_history[i][strat_index] == NO_MOVE)
        {
            break;
        }

        sq_pushinteger(v, game_history[i][strat_index]);
        sq_arrayappend(v, -2);
    }
    sq_newslot(v, -3, SQFalse);
}

void update_scores(StrategyInfo strategies[STRATEGY_COUNT], Turn turn, GameSettings settings)
{
    if (turn[0] == COOPERATION && turn[1] == COOPERATION)
        update_scores_helper(strategies, settings.reward, settings.reward);
    
    else if (turn[0] == COOPERATION && turn[1] == DEFECTION)
        update_scores_helper(strategies, settings.sucker, settings.temptation);

    else if (turn[0] == DEFECTION && turn[1] == COOPERATION)
        update_scores_helper(strategies, settings.temptation, settings.sucker);

    else if (turn[0] == DEFECTION && turn[1] == DEFECTION)
        update_scores_helper(strategies, settings.penalty, settings.penalty);
}

void update_scores_helper(StrategyInfo strategies[STRATEGY_COUNT], int strat1_points, int strat2_points)
{
    strategies[0].score += strat1_points;
    strategies[1].score += strat2_points;
}

void printfunc(HSQUIRRELVM v,const SQChar *s,...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(stdout, s, vl);
    va_end(vl);
}

void errorfunc(HSQUIRRELVM v,const SQChar *s,...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(stderr, s, vl);
    va_end(vl);
}