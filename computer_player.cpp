
#include "computer_player.h"
#include <cstdlib>
#include <ctime>

ComputerPlayer::ComputerPlayer(const std::vector<Point> &tankPositions_, int _tankIND, char _tankSymbol, Board &board_, int _printColor)
        : Player(tankPositions_, _tankIND, _tankSymbol, board_, _printColor)
{

    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
}

// Try not to shoot own tanks, shoot opponents when possible, and move if a shell is chasing
void ComputerPlayer::movementDecision(Board &board, Player &opponent, std::vector<Shell> &shells) {
    for (Tank &tank : getTankVector()) {
        if (tank.isTankHit()) continue;

        // 1. Evade if a shell is chasing
        if (isShellChasingTank(tank, shells)) {
            evadeShell(tank, board);
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
        }int action = rand() % 5;
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
        // (After setting track status, tank.move() will be called by moveTanks())

        // 3. Optionally: random movement or rotate for fun (not required)
    }
}

void ComputerPlayer::moveTanks(Board &board,  Player &opponent, std::vector<Shell> &shells) {
movementDecision(board,opponent,shells);
Player::moveTanks(board,opponent,shells);// base function
};

// Returns true if an opponent tank is in the line of fire (no wall in between)
bool ComputerPlayer::isOpponentInLineOfFire(const Tank &tank,  Player &opponent, const Board &board) const {
    Point p = tank.getCannonPosition();
    Direction dir = tank.getDirection();

    // Check along the line until hitting a wall or end of board
    while (true) {
        // Check if any opponent tank is at this point
        for (const Tank &otank : opponent.getTankVector()) {
            if (!otank.isTankHit() && otank.getPosition() == p)
                return true;
        }
        char cell = board.getBoardCell(p);
        if (cell == STRONG_WALL_SYMBOL || cell == WEAK_WALL_SYMBOL)
            break;
        // Move to next cell in direction
        Point next = p.NextPointIs(dir);
        if (next == p) break; // No movement, out of bounds
        p = next;
    }
    return false;
}

// Returns true if any friendly tank is in the line of fire
bool ComputerPlayer::isFriendlyInLineOfFire( Tank &tank, const Board &board)  {
    Point p = tank.getCannonPosition();
    Direction dir = tank.getDirection();
    for ( Tank &ftank : getTankVector()) {
        if (&ftank == &tank || ftank.isTankHit()) continue;
        Point line = p;
        // Check the line of fire
        while (true) {
            if (ftank.getPosition() == line) return true;
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
bool ComputerPlayer::isShellChasingTank(const Tank &tank, const std::vector<Shell> &shells) const {
    for (const Shell &shell : shells) {
        if (!shell.isActive()) continue;
        // If the shell's direction would eventually hit this tank
        Point shellPos = shell.getPosition();
        Direction dir = shell.getDirection();
        for (int step = 0; step < 10; ++step) { // Arbitrary max range
            if (shellPos == tank.getPosition())
                return true;
            Point next = shellPos.NextPointIs(dir);
            if (next == shellPos) break;
            shellPos = next;
        }
    }
    return false;
}

// Simple evade: turn the tank away (rotate left)
void ComputerPlayer::evadeShell(Tank &tank, Board &board) {
    if (rand() % 2 == 0) {
        tank.setLeftTrackStatus(TrackStatus::BACKWARD);
        tank.setRightTrackStatus(TrackStatus::STAY);
    } else {
        tank.setLeftTrackStatus(TrackStatus::STAY);
        tank.setRightTrackStatus(TrackStatus::BACKWARD);
    }};
