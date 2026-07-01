#pragma once

class GameState
{
public:
    bool isCastleGateUnlocked() const;
    void unlockCastleGate();
    void setCastleGateUnlocked(bool value);

    bool hasReachedCourtyard() const;
    void setReachedCourtyard(bool value);

private:
    bool castleGateUnlocked_ = false;
    bool reachedCourtyard_ = false;
};
