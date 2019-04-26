#pragma once
#include <QObject>

#include "refereeInfo.h"
#include "refereeUtils.h"
#include "refereeMessage.h"

class GameState : public QObject
{
    Q_OBJECT
public:

    GameState();
    void updateOurTeam(TeamColour ourTeam);
    void updateGameState(RefereeMessage &message);
    void updateRefereeInfoFromState(RefereeInfo &refInfo);


    State getState();
    TeamColour getForTeam();
    TeamColour getOurTeam();
    Point getBallPacementPostion();

    bool isGameStateForOurTeam();
    bool isGameStateForOtherTeam();

    bool isDistanceToBallRequired();
    bool isPenaltyShootout();
    bool isPartOfFieldLeft();
    bool isRunning();

private:
    State state;
    TeamColour forTeam;
    TeamColour ourTeam;
    bool partOfFieldLeft;
    bool penaltyShootout;
    Point ballPlacementPosition;
};
