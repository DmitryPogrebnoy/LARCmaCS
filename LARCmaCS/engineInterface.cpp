#include "engineInterface.h"

#include <QDebug>

QSharedPointer<PacketSSL> EngineInterface::loadVisionData()
{
	QSharedPointer<PacketSSL> packetSSL(new PacketSSL());

	packetSSL->balls[0] = 0;

	int balls_n, idCam, robots_blue_n, robots_yellow_n;
	for (int i = 0; i < mSharedRes->getDetectionSize(); i++) {
		QSharedPointer<SSL_WrapperPacket> packet = mSharedRes->getDetection(i);
		if (!packet || !packet->IsInitialized()) {
			if (packet)
				qDebug() << "Packet is uninitialized!";
			continue;
		}

		if (packet->has_detection()) {
			SSL_DetectionFrame mDetection = packet->detection();

			idCam = mDetection.camera_id() + 1;
			balls_n = mDetection.balls_size();

			// [Start] Ball info
			if (balls_n != 0) {
				packetSSL->balls[0] = idCam;
				SSL_DetectionBall ball = mDetection.balls(0);
				packetSSL->balls[1] = ball.x();
				packetSSL->balls[2] = ball.y();
			}
			// [End] Ball info

			// [Start] Robot info
			robots_blue_n = mDetection.robots_blue_size();
			robots_yellow_n = mDetection.robots_yellow_size();

			for (int i = 0; i < robots_blue_n; i++) {
				SSL_DetectionRobot robot = mDetection.robots_blue(i);
				if (robot.has_robot_id() && robot.robot_id() >= 0 && robot.robot_id() <= Constants::maxRobotsInTeam) {
					packetSSL->robots_blue[robot.robot_id()] = idCam;
					packetSSL->robots_blue[robot.robot_id() + Constants::maxRobotsInTeam] = robot.x();
					packetSSL->robots_blue[robot.robot_id() + Constants::maxRobotsInTeam * 2] = robot.y();
					packetSSL->robots_blue[robot.robot_id() + Constants::maxRobotsInTeam * 3] = robot.orientation();
				} else {
					if (robot.has_robot_id()) {
						qDebug() << "Error: incorrect blue robot id" << robot.robot_id();
					}
				}
			}

			for (int i = 0; i < robots_yellow_n; i++) {
				SSL_DetectionRobot robot = mDetection.robots_yellow(i);
				if (robot.has_robot_id() && robot.robot_id() >= 0 && robot.robot_id() <= Constants::maxRobotsInTeam) {
					packetSSL->robots_yellow[robot.robot_id()] = idCam;
					packetSSL->robots_yellow[robot.robot_id() + Constants::maxRobotsInTeam] = robot.x();
					packetSSL->robots_yellow[robot.robot_id() + Constants::maxRobotsInTeam * 2] = robot.y();
					packetSSL->robots_yellow[robot.robot_id() + Constants::maxRobotsInTeam * 3] = robot.orientation();
				} else {
					if (robot.has_robot_id()) {
						qDebug() << "Error: incorrect yellow robot id" << robot.robot_id();
					}
				}
			}
			// [End] Robot info
		}
	}

	return packetSSL;
}
