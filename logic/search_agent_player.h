#pragma once
#include <array>
#include <vector>
#include "data/wall.h"
#include "logic/player.h"

// Minimax-with-alpha-beta AI opponent ("Hard" difficulty), as a Player&
// sibling to ComputerPlayer ("Easy"). Like HumanPlayer, it only ever
// moves/fires its current getActiveTank() each tick (never both of its
// tanks at once) so it plays under the same restriction a human does;
// it decides for itself when to switch active tank.
//
// Each replan, it clones a small 1-vs-1 slice of the world (its active
// tank + its nearest alive enemy tank + shells/walls reconstructed from
// the live board) and searches a few rounds of "my action -> the
// opponent's worst-case reply" ahead, picking the action that maximizes
// a heuristic evaluation of the resulting position. Search depth and
// replan frequency are both capped (see SEARCH_AGENT_* in constants.h)
// so this stays cheap relative to the fixed game tick.
class SearchAgentPlayer : public Player {
public:
    SearchAgentPlayer(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol, Board& board);

    void decideActions(Board& board, Player& opponent, std::vector<Shell>& shells) override;

private:
    enum class SearchAction { ROTATE_LEFT, ROTATE_RIGHT, FORWARD, BACKWARD, STAY, FIRE };

    // A self-contained, disposable clone of the world for one my-tank-vs-
    // enemy-tank sub-search. Walls are reconstructed once (from the live
    // board) into a real Wall list here, since shell/tank movement can
    // transiently overwrite a wall's board cell mid-simulation and it
    // needs to be re-stamped afterward, exactly like the real game tick
    // does with the authoritative Wall objects. Mines aren't included:
    // decideActions() has no access to the live Mine array (Player's
    // interface doesn't expose it), so mine-avoidance instead uses the
    // fixed MINE_POS_X/MINE_POS_Y layout straight from constants.h — see
    // search_agent_player.cpp for the caveat that implies.
    struct SimState {
        Board board;
        Tank myTank;
        Tank enemyTank;
        std::vector<Shell> shells;
        std::vector<Wall> walls;

        SimState(Board board, Tank myTank, Tank enemyTank, std::vector<Shell> shells, std::vector<Wall> walls);
    };

    // Cached plan + replan throttle, one slot per own tank (TANK1_FLAG/TANK2_FLAG),
    // so each tank keeps its own last decision when the other one is active.
    std::array<SearchAction, 2> lastActions{SearchAction::STAY, SearchAction::STAY};
    std::array<int, 2> ticksSinceReplan{SEARCH_AGENT_REPLAN_TICKS, SEARCH_AGENT_REPLAN_TICKS};

    static std::vector<SearchAction> legalActions(const Tank& tank);
    static void applyTankAction(Tank& tank, SearchAction action, std::vector<Shell>& shells);
    static void stepState(SimState& state, SearchAction mine, SearchAction theirs, char mySymbol, char enemySymbol);
    static std::vector<Wall> synthesizeWallsFromBoard(const Board& board);
    // Clones a 1v1 slice of the world for `myTank` vs. `enemy` — shared by
    // the replan branch and the per-tick safety check below.
    static SimState buildSimState(const Board& board, const Tank& myTank, const Tank& enemy,
                                   const std::vector<Shell>& shells);

    // Minimax w/ alpha-beta over paired rounds: a MAX call tries each of my
    // candidate actions and forwards to a MIN call (same state, no copy
    // needed yet); the MIN call tries each enemy candidate action, clones
    // the state, applies both actions via stepState (consuming one round),
    // and recurses into the next MAX call.
    double search(const SimState& state, int roundsRemaining, double alpha, double beta, bool maximizingTurn,
                  SearchAction pendingMyAction, char mySymbol, char enemySymbol) const;

    // Root-level wrapper around search(): same MAX logic, but tracks which
    // action produced the best value instead of just the value. Drops FIRE
    // up front if it would hit `friendlyTank` (my other own tank), since
    // that tank isn't modeled inside the 1v1 SimState at all.
    SearchAction chooseBestAction(const SimState& rootState, const Tank* friendlyTank, char mySymbol,
                                   char enemySymbol) const;

    // One-shot (non-adversarial) safety check used every tick a shell
    // threatens the active tank: tries each legal action, applies it via
    // stepState with the enemy fixed to STAY (an already-fired shell's path
    // doesn't depend on the enemy's move this tick, and this is meant to
    // stay cheap, not another minimax search), and picks whichever surviving
    // result scores best under evaluate(). Falls back to STAY if none survive.
    SearchAction chooseSafeAction(const SimState& rootState, char mySymbol, char enemySymbol) const;

    double evaluate(const SimState& state) const;
    // Cheap 0-round standing of a single tank vs. its nearest enemy against
    // the REAL live board/shells, used only to decide whether to switch
    // active tank (never inside the minimax tree itself).
    double quickSelfEvaluate(const Tank& myTank, const std::vector<Tank>& enemyTanks, const Board& board,
                              const std::vector<Shell>& shells) const;

    static bool hasLineOfFire(Point from, Direction dir, Point target, const Board& board);
    static bool projectShellHits(const Shell& shell, Point target, int lookaheadSteps, int& stepsOut);
    static bool wouldHitFriendly(const Tank& shooter, const Tank& friendly, const Board& board);

    static const Tank* nearestAliveEnemy(const Tank& mine, const std::vector<Tank>& enemyTanks);
};
