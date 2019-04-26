#pragma once

#include "ssl_referee.pb.h"

enum TeamColour
{
	NEUTRAL,
	BLUETEAM,
	YELLOWTEAM
};

enum State
{
	HALT,
	STOP,
	RUNNING,
	TIMEOUT,
	POST_GAME,
	PREPARE_KICKOFF,
	KICKOFF,
	PREPARE_PENALTY,
	PENALTY,
	DIRECT_FREE,
	INDIRECT_FREE,
	BALL_PLACEMENT
};

struct Point
{
	float x;
	float y;
	Point() : x(0), y(0) {}

	Point(float x, float y) : x(x), y(y) {}

	Point(const ::Referee_Point& point) : x(point.x()), y(point.y()) {}
};


