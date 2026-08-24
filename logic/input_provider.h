#pragma once

// Abstracts "is this key currently held down" away from any concrete
// windowing/console backend, so the logic layer (HumanPlayer in particular)
// has zero direct dependency on raylib or any other platform API.
class IInputProvider {
public:
    virtual ~IInputProvider() = default;
    virtual bool isKeyDown(int keyCode) const = 0;
};
