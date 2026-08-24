#pragma once
#include "logic/input_provider.h"

// Real physical key-held polling via raylib's IsKeyDown(), matching the
// original GetAsyncKeyState-based continuous-hold input model.
//
// Game logic only advances once per fixed tick (every GAME_TICK_SECONDS),
// but rendering/input polling runs every real frame. To make sure a brief
// tap that falls between two ticks still registers exactly once (not zero,
// not twice), isKeyDown() combines two signals: a press *event* latched
// across the whole tick window (IsKeyPressed fires on exactly one frame per
// physical press, so it can only ever land in one tick's window), OR'd with
// the live held-state at the instant the tick fires (so a key still held
// down keeps registering every tick, for continuous movement/rotation).
class RaylibInputProvider : public IInputProvider {
public:
    // Call once per rendered frame.
    void pollFrame();
    // Call once a game logic tick has consumed the latched state.
    void clearLatch();

    bool isKeyDown(int keyCode) const override;

private:
    static constexpr int kLatchSize = 256;
    bool latched[kLatchSize] = {};
};
