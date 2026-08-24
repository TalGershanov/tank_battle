#include "logic/collision_rules.h"
#include "logic/tank_movement.h"

namespace {

// Checks shells[i] against one player's tanks (body then cannon). Returns
// true and deactivates the shell the instant a match is found.
bool resolveShellVsPlayerTanks(Shell& shell, Player& player, Board& board) {
    for (Tank& t : player.getTankVector()) {
        if (shell.getPosition() == t.getPosition()) {
            t.markAsHit();
            TankMovementLogic::removeDeadTankFromBoard(t, board);
            player.changeTankFlag();
            shell.deactivate();
            return true;
        }
        if (shell.getPosition() == t.getCannonPosition()) {
            TankMovementLogic::clearTankCannonFromBoard(t, board);
            shell.deactivate();
            return true;
        }
    }
    return false;
}

} // namespace

void CollisionRules::resolveShellCollisions(std::vector<Shell>& shells, Player& player1, Player& player2,
                                             std::vector<Wall>& walls, Board& board) {
    for (size_t i = 0; i < shells.size(); i++) {
        if (!shells[i].isActive()) continue;

        if (resolveShellVsPlayerTanks(shells[i], player1, board)) continue;
        if (resolveShellVsPlayerTanks(shells[i], player2, board)) continue;

        bool hitShell = false;
        for (size_t j = i + 1; j < shells.size(); j++) {
            if (!shells[j].isActive()) continue;
            if (shells[i].getPosition() == shells[j].getPosition()) {
                shells[i].deactivate();
                shells[j].deactivate();
                hitShell = true;
                break;
            }
        }
        if (hitShell) continue;

        for (auto& wall : walls) {
            if (shells[i].getPosition() == wall.getPosition()) {
                shells[i].deactivate();
                wall.increaseHits();
                break;
            }
        }
    }
}

void CollisionRules::resolveMineCollisions(std::array<Mine, NUM_OF_MINES>& mines, Player& player1, Player& player2,
                                            const std::vector<Shell>& shells, Board& board) {
    for (auto& mine : mines) {
        // player1
        if (!mine.isDestroyed()) {
            for (Tank& t1 : player1.getTankVector()) {
                if (mine.getPosition() == t1.getPosition()) {
                    t1.markAsHit();
                    TankMovementLogic::removeDeadTankFromBoard(t1, board);
                    player1.changeTankFlag();
                    mine.setDestroyed();
                }
                if (mine.getPosition() == t1.getCannonPosition()) {
                    mine.markAsInvisible();
                }
            }
        }

        // player2
        for (Tank& t2 : player2.getTankVector()) {
            if (!mine.isDestroyed()) {
                if (mine.getPosition() == t2.getPosition()) {
                    t2.markAsHit();
                    TankMovementLogic::removeDeadTankFromBoard(t2, board);
                    player2.changeTankFlag();
                    mine.setDestroyed();
                }
                if (mine.getPosition() == t2.getCannonPosition()) {
                    mine.markAsInvisible();
                }

                for (const auto& shell : shells) {
                    if (shell.getPosition() == mine.getPosition())
                        mine.markAsInvisible();
                }
            }
        }

        if (!mine.isDestroyed()) {
            mine.markAsVisible();
        }
    }
}
