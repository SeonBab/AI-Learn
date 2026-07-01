#include "GameState.h"

bool GameState::isCastleGateUnlocked() const
{
    return castleGateUnlocked_;
}

void GameState::unlockCastleGate()
{
    castleGateUnlocked_ = true;
}

void GameState::setCastleGateUnlocked(bool value)
{
    castleGateUnlocked_ = value;
}

bool GameState::hasReachedCourtyard() const
{
    return reachedCourtyard_;
}

void GameState::setReachedCourtyard(bool value)
{
    reachedCourtyard_ = value;
}
