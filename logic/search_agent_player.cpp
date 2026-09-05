#include "logic/search_agent_player.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>

#include "logic/shell_movement.h"
#include "logic/tank_movement.h"
#include "logic/wall_logic.h"

namespace {

int chebyshevDistance(Point a, Point b) {
    return std::max(std::abs(a.getX() - b.getX()), std::abs(a.getY() - b.getY()));
}

int countAdjacentWalls(Point p, const Board& board) {
    static const int dx[4] = {-1, 1, 0, 0};
    static const int dy[4] = {0, 0, -1, 1};
    int count = 0;
    for (int i = 0; i < 4; i++) {
        Point neighbor(p.getX() + dx[i], p.getY() + dy[i]);
        if (neighbor.getX() < 0 || neighbor.getX() >= BOARD_HEIGHT ||
            neighbor.getY() < 0 || neighbor.getY() >= BOARD_WIDTH)
            continue;
        char cell = board.getBoardCell(neighbor);
        if (cell == STRONG_WALL_SYMBOL || cell == WEAK_WALL_SYMBOL) count++;
    }
    return count;
}

} // namespace

SearchAgentPlayer::SimState::SimState(Board board, Tank myTank, Tank enemyTank, std::vector<Shell> shells,
                                       std::vector<Wall> walls)
    : board(std::move(board)), myTank(std::move(myTank)), enemyTank(std::move(enemyTank)),
      shells(std::move(shells)), walls(std::move(walls)) {}

SearchAgentPlayer::SearchAgentPlayer(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol,
                                     Board& board)
    : Player(tankPositions, tankIND, tankSymbol, board) {}

std::vector<SearchAgentPlayer::SearchAction> SearchAgentPlayer::legalActions(const Tank& tank) {
    std::vector<SearchAction> actions = {
        SearchAction::ROTATE_LEFT, SearchAction::ROTATE_RIGHT,
        SearchAction::FORWARD, SearchAction::BACKWARD, SearchAction::STAY
    };
    if (tank.canShoot()) actions.push_back(SearchAction::FIRE);
    return actions;
}

void SearchAgentPlayer::applyTankAction(Tank& tank, SearchAction action, std::vector<Shell>& shells) {
    switch (action) {
        case SearchAction::ROTATE_LEFT:
            tank.setLeftTrackStatus(TrackStatus::BACKWARD);
            tank.setRightTrackStatus(TrackStatus::STAY);
            break;
        case SearchAction::ROTATE_RIGHT:
            tank.setLeftTrackStatus(TrackStatus::STAY);
            tank.setRightTrackStatus(TrackStatus::BACKWARD);
            break;
        case SearchAction::FORWARD:
            tank.setLeftTrackStatus(TrackStatus::FORWARD);
            tank.setRightTrackStatus(TrackStatus::FORWARD);
            break;
        case SearchAction::BACKWARD:
            tank.setLeftTrackStatus(TrackStatus::BACKWARD);
            tank.setRightTrackStatus(TrackStatus::BACKWARD);
            break;
        case SearchAction::STAY:
            tank.setLeftTrackStatus(TrackStatus::STAY);
            tank.setRightTrackStatus(TrackStatus::STAY);
            break;
        case SearchAction::FIRE:
            tank.setLeftTrackStatus(TrackStatus::STAY);
            tank.setRightTrackStatus(TrackStatus::STAY);
            if (tank.canShoot()) {
                shells.emplace_back(tank.getCannonPosition().NextPointIs(tank.getDirection()), tank.getDirection());
                tank.resetCooldown();
            }
            break;
    }
}

std::vector<Wall> SearchAgentPlayer::synthesizeWallsFromBoard(const Board& board) {
    std::vector<Wall> walls;
    for (int row = MIN_ROW; row <= MAX_ROW; row++) {
        for (int col = MIN_COL; col <= MAX_COL; col++) {
            Point p(row, col);
            char cell = board.getBoardCell(p);
            if (cell != STRONG_WALL_SYMBOL && cell != WEAK_WALL_SYMBOL) continue;
            Wall wall(p);
            if (cell == WEAK_WALL_SYMBOL) wall.increaseHits(); // bring a fresh Wall(0 hits) up to WALL_HITS_WEAK(1)
            walls.push_back(std::move(wall));
        }
    }
    return walls;
}

void SearchAgentPlayer::stepState(SimState& state, SearchAction mine, SearchAction theirs, char mySymbol,
                                   char enemySymbol) {
    if (!state.myTank.isTankHit()) applyTankAction(state.myTank, mine, state.shells);
    if (!state.enemyTank.isTankHit()) applyTankAction(state.enemyTank, theirs, state.shells);

    if (!state.myTank.isTankHit()) TankMovementLogic::advance(state.myTank, state.board, mySymbol);
    if (!state.enemyTank.isTankHit()) TankMovementLogic::advance(state.enemyTank, state.board, enemySymbol);

    for (Shell& shell : state.shells) {
        if (shell.isActive()) ShellMovementLogic::advance(shell, state.board);
    }

    // Shell collisions: body -> cannon -> other shells -> walls, first match
    // wins per shell. Scoped subset of CollisionRules::resolveShellCollisions,
    // reimplemented here (rather than reused) since that takes Player& built
    // for arbitrary tank vectors, and this sub-search only ever has one tank
    // per side.
    for (size_t i = 0; i < state.shells.size(); i++) {
        Shell& shell = state.shells[i];
        if (!shell.isActive()) continue;

        bool resolved = false;
        for (Tank* t : {&state.myTank, &state.enemyTank}) {
            if (t->isTankHit()) continue;
            if (shell.getPosition() == t->getPosition()) {
                t->markAsHit();
                TankMovementLogic::removeDeadTankFromBoard(*t, state.board);
                shell.deactivate();
                resolved = true;
                break;
            }
            if (shell.getPosition() == t->getCannonPosition()) {
                TankMovementLogic::clearTankCannonFromBoard(*t, state.board);
                shell.deactivate();
                resolved = true;
                break;
            }
        }
        if (resolved) continue;

        for (size_t j = i + 1; j < state.shells.size(); j++) {
            if (!state.shells[j].isActive()) continue;
            if (shell.getPosition() == state.shells[j].getPosition()) {
                shell.deactivate();
                state.shells[j].deactivate();
                resolved = true;
                break;
            }
        }
        if (resolved) continue;

        for (Wall& wall : state.walls) {
            if (shell.getPosition() == wall.getPosition()) {
                shell.deactivate();
                wall.increaseHits();
                break;
            }
        }
    }

    state.shells.erase(
        std::remove_if(state.shells.begin(), state.shells.end(), [](const Shell& s) { return !s.isActive(); }),
        state.shells.end());

    state.walls.erase(
        std::remove_if(state.walls.begin(), state.walls.end(), [](const Wall& w) { return w.isDestroyed(); }),
        state.walls.end());
    // Shell/tank movement above can transiently overwrite a wall's board
    // cell; re-stamp every surviving wall now, exactly like the real tick's
    // end-of-tick WallLogic::syncToBoard pass.
    for (Wall& wall : state.walls) {
        WallLogic::syncToBoard(wall, state.board);
    }

    // Mine collisions: decideActions() has no access to the live Mine array,
    // so this uses the fixed spawn layout from constants.h directly (see the
    // header's SimState comment) rather than tracking live destroyed/visible
    // state.
    for (int m = 0; m < NUM_OF_MINES; m++) {
        Point minePos(MINE_POS_X[m], MINE_POS_Y[m]);
        for (Tank* t : {&state.myTank, &state.enemyTank}) {
            if (!t->isTankHit() && t->getPosition() == minePos) {
                t->markAsHit();
                TankMovementLogic::removeDeadTankFromBoard(*t, state.board);
            }
        }
    }
}

double SearchAgentPlayer::search(const SimState& state, int roundsRemaining, double alpha, double beta,
                                  bool maximizingTurn, SearchAction pendingMyAction, char mySymbol,
                                  char enemySymbol) const {
    if (state.myTank.isTankHit() || state.enemyTank.isTankHit() || roundsRemaining <= 0) {
        return evaluate(state);
    }

    if (maximizingTurn) {
        double best = -std::numeric_limits<double>::infinity();
        for (SearchAction action : legalActions(state.myTank)) {
            double value = search(state, roundsRemaining, alpha, beta, false, action, mySymbol, enemySymbol);
            best = std::max(best, value);
            alpha = std::max(alpha, value);
            if (alpha >= beta) break;
        }
        return best;
    }

    double worst = std::numeric_limits<double>::infinity();
    for (SearchAction action : legalActions(state.enemyTank)) {
        SimState next = state;
        stepState(next, pendingMyAction, action, mySymbol, enemySymbol);
        double value = search(next, roundsRemaining - 1, alpha, beta, true, SearchAction::STAY, mySymbol, enemySymbol);
        worst = std::min(worst, value);
        beta = std::min(beta, value);
        if (alpha >= beta) break;
    }
    return worst;
}

SearchAgentPlayer::SearchAction SearchAgentPlayer::chooseBestAction(const SimState& rootState,
                                                                     const Tank* friendlyTank, char mySymbol,
                                                                     char enemySymbol) const {
    SearchAction bestAction = SearchAction::STAY;
    double best = -std::numeric_limits<double>::infinity();
    double alpha = -std::numeric_limits<double>::infinity();
    const double beta = std::numeric_limits<double>::infinity();

    for (SearchAction action : legalActions(rootState.myTank)) {
        if (action == SearchAction::FIRE && friendlyTank != nullptr &&
            wouldHitFriendly(rootState.myTank, *friendlyTank, rootState.board)) {
            continue;
        }
        double value = search(rootState, SEARCH_AGENT_SEARCH_ROUNDS, alpha, beta, false, action, mySymbol, enemySymbol);
        if (value > best) {
            best = value;
            bestAction = action;
        }
        alpha = std::max(alpha, value);
    }
    return bestAction;
}

SearchAgentPlayer::SearchAction SearchAgentPlayer::chooseSafeAction(const SimState& rootState, char mySymbol,
                                                                     char enemySymbol) const {
    SearchAction bestSafe = SearchAction::STAY;
    double bestSafeScore = -std::numeric_limits<double>::infinity();
    bool foundSafe = false;

    for (SearchAction action : legalActions(rootState.myTank)) {
        SimState next = rootState;
        // The enemy's action is fixed to STAY here: an already-fired shell's
        // path doesn't depend on what the enemy does this tick, and this is
        // meant to stay a cheap one-shot safety check, not another
        // adversarial search.
        stepState(next, action, SearchAction::STAY, mySymbol, enemySymbol);
        if (next.myTank.isTankHit()) continue;

        double score = evaluate(next);
        if (!foundSafe || score > bestSafeScore) {
            bestSafeScore = score;
            bestSafe = action;
            foundSafe = true;
        }
    }
    return bestSafe;
}

bool SearchAgentPlayer::hasLineOfFire(Point from, Direction dir, Point target, const Board& board) {
    Point p = from;
    while (true) {
        if (p == target) return true;
        if (!(p.getX() >= MIN_ROW && p.getX() <= MAX_ROW && p.getY() >= MIN_COL && p.getY() <= MAX_COL))
            break;
        char cell = board.getBoardCell(p);
        if (cell == STRONG_WALL_SYMBOL || cell == WEAK_WALL_SYMBOL) break;
        Point next = p.NextPointIs(dir);
        if (next == p) break;
        p = next;
    }
    return false;
}

bool SearchAgentPlayer::projectShellHits(const Shell& shell, Point target, int lookaheadSteps, int& stepsOut) {
    if (!shell.isActive()) return false;
    Point p = shell.getPosition();
    Direction dir = shell.getDirection();
    for (int step = 0; step < lookaheadSteps; step++) {
        if (p == target) {
            stepsOut = step;
            return true;
        }
        Point next = p.NextPointIs(dir);
        if (next == p) break;
        p = next;
    }
    return false;
}

bool SearchAgentPlayer::wouldHitFriendly(const Tank& shooter, const Tank& friendly, const Board& board) {
    if (friendly.isTankHit()) return false;
    return hasLineOfFire(shooter.getCannonPosition(), shooter.getDirection(), friendly.getPosition(), board);
}

SearchAgentPlayer::SimState SearchAgentPlayer::buildSimState(const Board& board, const Tank& myTank,
                                                               const Tank& enemy, const std::vector<Shell>& shells) {
    return SimState(board, myTank, enemy, shells, synthesizeWallsFromBoard(board));
}

const Tank* SearchAgentPlayer::nearestAliveEnemy(const Tank& mine, const std::vector<Tank>& enemyTanks) {
    const Tank* nearest = nullptr;
    int bestDist = std::numeric_limits<int>::max();
    for (const Tank& enemy : enemyTanks) {
        if (enemy.isTankHit()) continue;
        int dist = chebyshevDistance(mine.getPosition(), enemy.getPosition());
        if (dist < bestDist) {
            bestDist = dist;
            nearest = &enemy;
        }
    }
    return nearest;
}

double SearchAgentPlayer::evaluate(const SimState& state) const {
    bool myDead = state.myTank.isTankHit();
    bool enemyDead = state.enemyTank.isTankHit();
    if (myDead && enemyDead) return 0.0;
    if (myDead) return -SEARCH_AGENT_WIN_SCORE;
    if (enemyDead) return SEARCH_AGENT_WIN_SCORE;

    double score = 0.0;

    bool iHaveLineOfFire = hasLineOfFire(state.myTank.getCannonPosition(), state.myTank.getDirection(),
                                          state.enemyTank.getPosition(), state.board);
    bool theyHaveLineOfFire = hasLineOfFire(state.enemyTank.getCannonPosition(), state.enemyTank.getDirection(),
                                             state.myTank.getPosition(), state.board);
    if (iHaveLineOfFire && state.myTank.canShoot()) score += SEARCH_AGENT_LINE_OF_FIRE_OFFENSE_WEIGHT;
    if (theyHaveLineOfFire && state.enemyTank.canShoot()) score -= SEARCH_AGENT_LINE_OF_FIRE_DEFENSE_WEIGHT;

    for (const Shell& shell : state.shells) {
        int steps;
        if (projectShellHits(shell, state.myTank.getPosition(), SEARCH_AGENT_SHELL_LOOKAHEAD_STEPS, steps))
            score -= SEARCH_AGENT_SHELL_THREAT_DEFENSE_WEIGHT / (steps + 1);
        if (projectShellHits(shell, state.enemyTank.getPosition(), SEARCH_AGENT_SHELL_LOOKAHEAD_STEPS, steps))
            score += SEARCH_AGENT_SHELL_THREAT_OFFENSE_WEIGHT / (steps + 1);
    }

    for (int m = 0; m < NUM_OF_MINES; m++) {
        Point minePos(MINE_POS_X[m], MINE_POS_Y[m]);
        if (chebyshevDistance(minePos, state.myTank.getPosition()) <= 1) score -= SEARCH_AGENT_MINE_PROXIMITY_WEIGHT;
        if (chebyshevDistance(minePos, state.enemyTank.getPosition()) <= 1)
            score += SEARCH_AGENT_MINE_PROXIMITY_WEIGHT * 0.5;
    }

    score -= SEARCH_AGENT_DISTANCE_WEIGHT * chebyshevDistance(state.myTank.getPosition(), state.enemyTank.getPosition());

    score += SEARCH_AGENT_WALL_COVER_WEIGHT * countAdjacentWalls(state.myTank.getPosition(), state.board);
    score -= SEARCH_AGENT_WALL_COVER_WEIGHT * 0.5 * countAdjacentWalls(state.enemyTank.getPosition(), state.board);

    return score;
}

double SearchAgentPlayer::quickSelfEvaluate(const Tank& myTank, const std::vector<Tank>& enemyTanks,
                                             const Board& board, const std::vector<Shell>& shells) const {
    if (myTank.isTankHit()) return -SEARCH_AGENT_WIN_SCORE;
    const Tank* enemy = nearestAliveEnemy(myTank, enemyTanks);
    if (enemy == nullptr) return 0.0;

    double score = 0.0;
    bool iHaveLineOfFire = hasLineOfFire(myTank.getCannonPosition(), myTank.getDirection(), enemy->getPosition(), board);
    bool theyHaveLineOfFire =
        hasLineOfFire(enemy->getCannonPosition(), enemy->getDirection(), myTank.getPosition(), board);
    if (iHaveLineOfFire && myTank.canShoot()) score += SEARCH_AGENT_LINE_OF_FIRE_OFFENSE_WEIGHT;
    if (theyHaveLineOfFire && enemy->canShoot()) score -= SEARCH_AGENT_LINE_OF_FIRE_DEFENSE_WEIGHT;

    for (const Shell& shell : shells) {
        int steps;
        if (projectShellHits(shell, myTank.getPosition(), SEARCH_AGENT_SHELL_LOOKAHEAD_STEPS, steps))
            score -= SEARCH_AGENT_SHELL_THREAT_DEFENSE_WEIGHT / (steps + 1);
    }

    score -= SEARCH_AGENT_DISTANCE_WEIGHT * chebyshevDistance(myTank.getPosition(), enemy->getPosition());
    return score;
}

void SearchAgentPlayer::decideActions(Board& board, Player& opponent, std::vector<Shell>& shells) {
    if (getActiveTank().isTankHit()) {
        changeTankFlag();
        if (getActiveTank().isTankHit()) return;
    }

    std::vector<Tank>& myTanks = getTankVector();
    std::vector<Tank>& enemyTanks = opponent.getTankVector();

    int otherIdx = (tankIndex == TANK1_FLAG) ? TANK2_FLAG : TANK1_FLAG;
    bool otherAlive = static_cast<size_t>(otherIdx) < myTanks.size() && !myTanks[otherIdx].isTankHit();

    if (otherAlive) {
        double activeScore = quickSelfEvaluate(myTanks[tankIndex], enemyTanks, board, shells);
        double otherScore = quickSelfEvaluate(myTanks[otherIdx], enemyTanks, board, shells);
        if (otherScore - activeScore > SEARCH_AGENT_SWITCH_MARGIN) {
            changeTankFlag();
            otherIdx = (tankIndex == TANK1_FLAG) ? TANK2_FLAG : TANK1_FLAG;
            otherAlive = static_cast<size_t>(otherIdx) < myTanks.size() && !myTanks[otherIdx].isTankHit();
        }
    }

    int i = tankIndex;
    Tank& activeTank = myTanks[i];

    const Tank* enemy = nearestAliveEnemy(activeTank, enemyTanks);
    const Tank* friendlyTank = otherAlive ? &myTanks[otherIdx] : nullptr;
    char mySymbol = tankSymbol;
    char enemySymbol = (mySymbol == TANK_SYMBOL_PLAYER_1) ? TANK_SYMBOL_PLAYER_2 : TANK_SYMBOL_PLAYER_1;

    ticksSinceReplan[i]++;
    if (ticksSinceReplan[i] >= SEARCH_AGENT_REPLAN_TICKS) {
        if (enemy != nullptr) {
            SimState rootState = buildSimState(board, activeTank, *enemy, shells);
            lastActions[i] = chooseBestAction(rootState, friendlyTank, mySymbol, enemySymbol);
        } else {
            lastActions[i] = SearchAction::STAY;
        }
        ticksSinceReplan[i] = 0;
    }

    bool threatened = false;
    for (const Shell& shell : shells) {
        int steps;
        if (projectShellHits(shell, activeTank.getPosition(), SEARCH_AGENT_SHELL_LOOKAHEAD_STEPS, steps)) {
            threatened = true;
            break;
        }
    }

    // Tier 1: evade a shell already on a collision course with us — this
    // overrides everything else, every tick, regardless of the replan
    // throttle.
    if (threatened && enemy != nullptr) {
        SimState rootState = buildSimState(board, activeTank, *enemy, shells);
        applyTankAction(activeTank, chooseSafeAction(rootState, mySymbol, enemySymbol), shells);
        return;
    }
    if (threatened) {
        // No alive enemy to build a SimState against (rare/edge case): just
        // hold still rather than act on stale board data.
        applyTankAction(activeTank, SearchAction::STAY, shells);
        return;
    }

    // Tier 2: take a free, safe shot the instant it's available, rather than
    // leaving it to the deep search — that search assumes an adversarial
    // opponent who dodges within its horizon, so a shot available *right
    // now* often doesn't score better than repositioning even though it's
    // free. A real, immediate opportunity shouldn't be gambled away like that.
    if (activeTank.canShoot() && enemy != nullptr &&
        hasLineOfFire(activeTank.getCannonPosition(), activeTank.getDirection(), enemy->getPosition(), board) &&
        (friendlyTank == nullptr || !wouldHitFriendly(activeTank, *friendlyTank, board))) {
        applyTankAction(activeTank, SearchAction::FIRE, shells);
        return;
    }

    // Tier 3: no immediate threat or shot — follow the deep search's plan.
    applyTankAction(activeTank, lastActions[i], shells);
}
