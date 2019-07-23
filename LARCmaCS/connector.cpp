#include "connector.h"

#include <QMap>

#include "grSimRobot.h"
#include "defaultRobot.h"

const QString Connector::robotBoxIP = QStringLiteral("192.168.0.10");

Connector::Connector(SharedRes * sharedRes)
	: mSharedRes(sharedRes)
	, mUdpSocket(this)
	, mStatisticsTimer(this)
{}

Connector::~Connector()
{}

const QString & Connector::getGrSimIP()
{
	return mGrSimIP;
}

unsigned short Connector::getGrSimPort()
{
	return mGrSimPort;
}

unsigned short Connector::getRobotPort()
{
	return mRobotPort;
}

void Connector::run(int N, const QByteArray & command)
{
	if (!mIsSim) {
		mUdpSocket.writeDatagram(command, QHostAddress(robotBoxIP), DefaultRobot::robotPort);
	} else {
		mUdpSocket.writeDatagram(command, QHostAddress(mGrSimIP), mGrSimPort);
	}
}

void Connector::onConnectorChange(bool isSim, const QString &ip, int port)
{
	mIsSim = isSim;

	if (mIsSim) {
		mGrSimIP = ip;
		mGrSimPort = port;
	}
}

void Connector::sendNewCommand(const QVector<Rule> & rule)
{
	if (!mIsPause) {
		for (int k = 0; k < rule.size(); k++) {
			QByteArray command;
			if (!mIsSim) {
				DefaultRobot::formControlPacket(command, k, rule[k].mSpeedX, rule[k].mSpeedY, rule[k].mSpeedR,
						rule[k].mKickUp, rule[k].mKickForward, rule[k].mKickerVoltageLevel,
						rule[k].mDribblerEnable, rule[k].mSpeedDribbler, rule[k].mAutoKick,
						rule[k].mKickerChargeEnable, rule[k].mBeep);
			} else {
				GrSimRobot::formControlPacket(command, k, rule[k].mSpeedX, rule[k].mSpeedY, rule[k].mSpeedR,
						rule[k].mKickUp, rule[k].mKickForward, rule[k].mKickerVoltageLevel,
						rule[k].mDribblerEnable, rule[k].mSpeedDribbler, rule[k].mAutoKick,
						rule[k].mKickerChargeEnable, rule[k].mBeep);
			}

			emit run(k, command);
		}
	}
}

void Connector::onPauseChanged(bool status)
{
	qDebug() << "onPauseChanged" << status;
	mIsPause = status;

	if (mIsPause) {
		QByteArray command;
		if (!mIsSim) {
			for (int i = 1; i <= Constants::maxNumOfRobots; i++) {
				DefaultRobot::formControlPacket(command, i, 0, 0, 0, 0, 0, 0, 0);
				run(i, command);
			}
		} else {
			for (int i = 0; i <= Constants::maxRobotsInTeam; i++) {
				GrSimRobot::formControlPacket(command, i, 0, 0, 0, 0, 0, 0, 0);
				run(i, command);
			}
		}
	}
}
