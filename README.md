# SqPrisonersDilemma

SqPrisonersDilemma is a recreation of Robert Axelrod's famous Prisoner's Dilemma tournament, implemented in C with strategies written in the Squirrel scripting language. WIP.

## What is the Prisoner's Dilemma?

The [Prisoner's Dilemma](https://www.youtube.com/watch?v=qf7ws2DF-zk) is a game theory scenario where two players must independently choose to cooperate or defect. The payoff matrix creates a tension between individual rationality and collective benefit:

- Both cooperate: Each gets a moderate reward
- Both defect: Each gets a small punishment
- One cooperates, one defects: The defector gets the highest reward, the cooperator gets the worst outcome

"The Evolution of Cooperation" (Axelrod, 1984) is as absolutely recommended read.

## Available Strategies

The tournament currently includes the following classic strategies, implemented as Squirrel scripts:

### Basic Strategies
- **all_c.nut** - Always Cooperate
- **all_d.nut** - Always Defect
- **random.nut** - Random

### Reactive Strategies
- **tit_for_tat.nut** - Tit for Tat (winner of Axelrod's original tournament)
- **tit_for_two_tats.nut** - Tit for Two Tats

### Sophisticated Strategies
- **downing.nut** - Downing: Attempts to model and exploit the opponent's behavior
- **friedman.nut** - Friedman: Cooperates until opponent defects once, then defects forever
- **joss.nut** - Joss: Like Tit for Tat but occasionally defects to test if it can exploit the opponent

## Build from Source

```bash
$ make 64
$ ./bin/sq_imp
```
