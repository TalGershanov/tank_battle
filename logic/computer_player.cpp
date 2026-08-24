#include "logic/computer_player.h"
#include <cstdlib>

ComputerPlayer::ComputerPlayer(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol, Board& board)
    : Player(tankPositions, tankIND, tankSymbol, board) {}

// Try not to shoot own tanks, shoot opponents when possible, and move if a shell is chasing
void ComputerPlayer::decideActions(Board& board, Player& opponent, std::vector<Shell>& shells) {
    for (Tank& tank : getTankVector()) {
        if (tank.isTankHit()) continue;

        // 1. Evade if a shell is chasing
        if (isShellChasingTank(tank, shells)) {
            evadeShell(tank);
            continue; // Don't shoot & move in same turn
        }

        // 2. Try to shoot at opponent (but never own tank)
        if (tank.canShoot() &&
            isOpponentInLineOfFire(tank, opponent, board) &&
            !isFriendlyInLineOfFire(tank, board)) {
            shells.emplace_back(
                    tank.getCannonPosition().NextPointIs(tank.getDirection()),
                    tank.getDirection()
            );
            tank.resetCooldown();
            continue;
        }

        int action = rand() % NUM_AI_ACTIONS;
        switch (action) {
            case 0:
                // Rotate left
                tank.setLeftTrackStatus(TrackStatus::BACKWARD);
                tank.setRightTrackStatus(TrackStatus::STAY);
                break;
            case 1:
                // Rotate right
                tank.setLeftTrackStatus(TrackStatus::STAY);
                tank.setRightTrackStatus(TrackStatus::BACKWARD);
                break;
            case 2:
                // Move forward
                tank.setLeftTrackStatus(TrackStatus::FORWARD);
                tank.setRightTrackStatus(TrackStatus::FORWARD);
                break;
            case 3:
                // Move backward
                tank.setLeftTrackStatus(TrackStatus::BACKWARD);
                tank.setRightTrackStatus(TrackStatus::BACKWARD);
                break;
            default:
                // Stay still
                tank.setLeftTrackStatus(TrackStatus::STAY);
                tank.setRightTrackStatus(TrackStatus::STAY);
                break;
        }
        // (After setting track status, applyMovement() will move the tank)
    }
}

// Returns true if an opponent tank is in the line of fire (no wall in between)
bool ComputerPlayer::isOpponentInLineOfFire(const Tank& tank, Player& opponent, const Board& board) const {
    Point p = tank.getCannonPosition();
    Direction dir = tank.getDirection();

    while (true) {
        for (const Tank& otank : opponent.getTankVector()) {
            if (!otank.isTankHit() && otank.getPosition() == p)
                return true;
        }
        if (!(p.getX() >= MIN_ROW && p.getX() <= MAX_ROW && p.getY() >= MIN_COL && p.getY() <= MAX_COL))
            break; // left the playable board area
        char cell = board.getBoardCell(p);
        if (cell == STRONG_WALL_SYMBOL || cell == WEAK_WALL_SYMBOL)
            break;
        Point next = p.NextPointIs(dir);
        if (next == p) break; // No movement, out of bounds
        p = next;
    }
    return false;
}

// Returns true if any friendly tank is in the line of fire
bool ComputerPlayer::isFriendlyInLineOfFire(Tank& tank, const Board& board) {
    Point p = tank.getCannonPosition();
    Direction dir = tank.getDirection();
    for (Tank& ftank : getTankVector()) {
        if (&ftank == &tank || ftank.isTankHit()) continue;
        Point line = p;
        while (true) {
            if (ftank.getPosition() == line) return true;
            if (!(line.getX() >= MIN_ROW && line.getX() <= MAX_ROW &&
                  line.getY() >= MIN_COL && line.getY() <= MAX_COL))
                break; // left the playable board area
            char cell = board.getBoardCell(line);
            if (cell == STRONG_WALL_SYMBOL || cell == WEAK_WALL_SYMBOL)
                break;
            Point next = line.NextPointIs(dir);
            if (next == line) break;
            line = next;
        }
    }
    return false;
}

// Simple: returns true if any shell is heading toward the tank's position (direct collision course)
bool ComputerPlayer::isShellChasingTank(const Tank& tank, const std::vector<Shell>& shells) const {
    for (const Shell& shell : shells) {
        if (!shell.isActive()) continue;
        Point shellPos = shell.getPosition();
        Direction dir = shell.getDirection();
        for (int step = 0; step < SHELL_CHASE_LOOKAHEAD_STEPS; ++step) {
            if (shellPos == tank.getPosition())
                return true;
            Point next = shellPos.NextPointIs(dir);
            if (next == shellPos) break;
            shellPos = next;
        }
    }
    return false;
}

// Simple evade: turn the tank away
void ComputerPlayer::evadeShell(Tank& tank) {
    if (rand() % NUM_EVADE_OPTIONS == 0) {
        tank.setLeftTrackStatus(TrackStatus::BACKWARD);
        tank.setRightTrackStatus(TrackStatus::STAY);
    } else {
        tank.setLeftTrackStatus(TrackStatus::STAY);
        tank.setRightTrackStatus(TrackStatus::BACKWARD);
    }
}
