#include "gameState.h"

GameState::GameState()
	: state(HALT)
	, forTeam(NEUTRAL)
	, ourTeam(NEUTRAL)
	, penaltyShootout(false)
	, partOfFieldLeft(false)
	, ballPlacementPosition(QPointF(0, 0))
{}

void GameState::setOurTeam(TeamColour team)
{
	ourTeam = team;
}

void GameState::updateGameState(RefereeMessage &message)
{

	switch (message.getCommand())
	{
		case Referee::HALT:
			state = HALT;
			forTeam = NEUTRAL;
			break;
		case Referee::STOP:
			state = STOP;
			forTeam = NEUTRAL;
			break;
		case Referee::NORMAL_START:
			if (state == PREPARE_KICKOFF) state = KICKOFF;
			if (state == PREPARE_PENALTY) state = PENALTY;
			break;
		case Referee::FORCE_START:
			state = RUNNING; // neutral restart
			forTeam = NEUTRAL;
			break;
		case Referee::PREPARE_KICKOFF_YELLOW:
			state = PREPARE_KICKOFF;
			forTeam = YELLOWTEAM;
			break;
		case Referee::PREPARE_KICKOFF_BLUE:
			state = PREPARE_KICKOFF;
			forTeam = BLUETEAM;
			break;
		case Referee::PREPARE_PENALTY_YELLOW:
			state = PREPARE_PENALTY;
			forTeam = YELLOWTEAM;
			break;
		case Referee::PREPARE_PENALTY_BLUE:
			state = PREPARE_PENALTY;
			forTeam = BLUETEAM;
			break;
		case Referee::DIRECT_FREE_YELLOW:
			state = DIRECT_FREE;
			forTeam = YELLOWTEAM;
			break;
		case Referee::DIRECT_FREE_BLUE:
			state = DIRECT_FREE;
			forTeam = BLUETEAM;
			break;
		case Referee::INDIRECT_FREE_YELLOW:
			state = INDIRECT_FREE;
			forTeam = YELLOWTEAM;
			break;
		case Referee::INDIRECT_FREE_BLUE:
			state = INDIRECT_FREE;
			forTeam = BLUETEAM;
			break;
		case Referee::TIMEOUT_YELLOW:
			state = TIMEOUT;
			forTeam = YELLOWTEAM;
			break;
		case Referee::TIMEOUT_BLUE:
			state = TIMEOUT;
			forTeam = BLUETEAM;
			break;
		case Referee::GOAL_YELLOW:
			state = STOP;
			forTeam = NEUTRAL;
			break;
		case Referee::GOAL_BLUE:
			state = STOP;
			forTeam = NEUTRAL;
			break;
		case Referee::BALL_PLACEMENT_YELLOW:
			state = BALL_PLACEMENT;
			forTeam = YELLOWTEAM;
			ballPlacementPosition = message.getBallPlacementPosition();
			break;
		case Referee::BALL_PLACEMENT_BLUE:
			state = BALL_PLACEMENT;
			forTeam = BLUETEAM;
			ballPlacementPosition = message.getBallPlacementPosition();
			break;
	}

	switch (message.getStage())
	{
		case Referee::PENALTY_SHOOTOUT:
			penaltyShootout = true;
			break;
		case Referee::POST_GAME:
			state = POST_GAME;
			forTeam = NEUTRAL;
			break;
		default:
			penaltyShootout = false;
	}

	if (ourTeam == BLUETEAM) {
		partOfFieldLeft = message.getBlueTeamOnPositiveHalf();
	} else {
		partOfFieldLeft = !message.getBlueTeamOnPositiveHalf();
	}
}

void GameState::updateRefereeInfoFromState(RefereeInfo & refInfo)
{
	refInfo.state = this->state;
	refInfo.commandForTeam = this->forTeam;
	refInfo.isPartOfFieldLeft = this->partOfFieldLeft;
}

State GameState::getState()
{
	return state;
}

TeamColour GameState::getForTeam()
{
	return forTeam;
}

TeamColour GameState::getOurTeam()
{
	return ourTeam;
}

QPointF GameState::getBallPacementPostion()
{
	return ballPlacementPosition;
}

bool GameState::isGameStateForOurTeam()
{
	return (forTeam == NEUTRAL) || (forTeam == ourTeam);
}

bool GameState::isGameStateForOtherTeam()
{
	return !isGameStateForOurTeam();
}

bool GameState::isDistanceToBallRequired()
{
	if (state == STOP || state == PREPARE_KICKOFF) {
			return true;
	}


	if (ourTeam != forTeam)
	{
		if (state == BALL_PLACEMENT || state == INDIRECT_FREE
				|| state == DIRECT_FREE || state == PREPARE_PENALTY) {
			return true;
		}
	}

	return false;
}

bool GameState::isPenaltyShootout()
{
	return penaltyShootout;
}

bool GameState::isPartOfFieldLeft()
{
	return partOfFieldLeft;
}

bool GameState::isRunning()
{
	return state == RUNNING;
}
