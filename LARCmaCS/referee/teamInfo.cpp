#include "teamInfo.h"


TeamInfo::TeamInfo()
    : name(""),
      score(0),
      redCards(0),
      yellowCards(0),
      timeoutTime(0),
      timeouts(0),
      goalkeeper(0) {}

TeamInfo::TeamInfo(const Referee::TeamInfo &packet)
    : yellowCardTimes(packet.yellow_cards())
{
    name = QString::fromStdString(packet.name());
    score = packet.score();

    redCards = packet.red_cards();
    yellowCards = packet.yellow_cards();

    for (int i = 0; i < packet.yellow_card_times_size(); i++) {
        yellowCardTimes.push_back(packet.yellow_card_times(i));
    }

    timeouts = packet.timeouts();
    timeoutTime = packet.timeout_time();

    goalkeeper = packet.goalkeeper();

    foulCounter = packet.foul_counter();

    maxAllowedBots = packet.max_allowed_bots();

    ballPlacementFailures = packet.ball_placement_failures();
    canPlaceBall = packet.can_place_ball();
}


QString TeamInfo::getName()
{
    return name;
}

quint32 TeamInfo::getScore()
{
    return score;
}


quint32 TeamInfo::getRedCards()
{
    return redCards;
}

QVector<quint32> TeamInfo::getYellowCardTimes()
{
    return yellowCardTimes;
}

quint32 TeamInfo::getYellowCards()
{
    return yellowCards;
}

quint32 TeamInfo::getTimeouts()
{
    return timeouts;
}

quint32 TeamInfo::getTimeoutTime()
{
    return timeoutTime;
}

quint32 TeamInfo::getGoalkeeper()
{
    return goalkeeper;
}

quint32 TeamInfo::getFoulCounter()
{
    return foulCounter;
}

quint32 TeamInfo::getBallPlacementFailures()
{
    return ballPlacementFailures;
}

bool TeamInfo::getCanPlaceBall()
{
    return canPlaceBall;
}

quint32 TeamInfo::getMaxAllowedBots()
{
    return maxAllowedBots;
}
