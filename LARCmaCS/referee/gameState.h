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
	void setOurTeam(TeamColour ourTeam);
	void updateGameState(RefereeMessage & message);
	void updateRefereeInfoFromState(RefereeInfo & refInfo);


	State getState();
	TeamColour getForTeam();
	TeamColour getOurTeam();
	QPointF getBallPacementPostion();

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
	QPointF ballPlacementPosition;
};
