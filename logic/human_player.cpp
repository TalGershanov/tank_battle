#include "logic/human_player.h"

HumanPlayer::HumanPlayer(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol, Board& board,
                          const IInputProvider& input, KeyBindings keys)
    : Player(tankPositions, tankIND, tankSymbol, board), input(input), keys(keys) {}

void HumanPlayer::decideActions(Board& board, Player& opponent, std::vector<Shell>& shells) {
    (void)board;
    (void)opponent;

    if (input.isKeyDown(keys.switchTankKey)) {
        changeTankFlag();
    }

    Tank& activeTank = getActiveTank();
    TrackStatus curLeftStatus = activeTank.getLeftTrackStatus();
    TrackStatus curRightStatus = activeTank.getRightTrackStatus();
    activeTank.setLeftTrackStatus(TrackStatus::STAY);
    activeTank.setRightTrackStatus(TrackStatus::STAY);
    bool keyPressed = false;

    if (input.isKeyDown(keys.rightForwardKey)) {
        activeTank.setRightTrackStatus(TrackStatus::FORWARD);
        keyPressed = true;
    }
    if (input.isKeyDown(keys.rightBackwardKey)) {
        activeTank.setRightTrackStatus(TrackStatus::BACKWARD);
        keyPressed = true;
    }
    if (input.isKeyDown(keys.leftForwardKey)) {
        activeTank.setLeftTrackStatus(TrackStatus::FORWARD);
        keyPressed = true;
    }
    if (input.isKeyDown(keys.leftBackwardKey)) {
        activeTank.setLeftTrackStatus(TrackStatus::BACKWARD);
        keyPressed = true;
    }
    if (input.isKeyDown(keys.stayKey)) {
        activeTank.setRightTrackStatus(TrackStatus::STAY);
        activeTank.setLeftTrackStatus(TrackStatus::STAY);
        keyPressed = true;
    }
    if (input.isKeyDown(keys.fireKey)) {
        activeTank.setRightTrackStatus(curLeftStatus);
        activeTank.setLeftTrackStatus(curRightStatus);
        keyPressed = true;
        if (activeTank.canShoot()) {
            shells.emplace_back(activeTank.getCannonPosition().NextPointIs(activeTank.getDirection()),
                                 activeTank.getDirection());
            activeTank.resetCooldown();
        }
    }

    if (!keyPressed) {
        activeTank.setLeftTrackStatus(curLeftStatus);
        activeTank.setRightTrackStatus(curRightStatus);
    }
}
