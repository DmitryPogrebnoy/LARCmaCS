#pragma once

#include <QObject>
#include <QThread>
#include <QtCore>
#include <QtNetwork>
#include "sharedRes.h"

#include "connectorWorker.h"

class Connector : public QObject
{
	Q_OBJECT

public:
	explicit Connector(SharedRes * sharedRes);
	~Connector();
	static const unsigned short robotPort = 10000;
	const QString & getGrSimIP();
	unsigned short getGrSimPort();

public slots:
	void changeGrSimIP(const QString & IP);
	void changeGrSimPort(unsigned short port);
	void run(int N, const QByteArray & command);
	void runSim(const QByteArray & command);

signals:
	void wstop();
	void sendPacket(int N, const QByteArray & command);
	void sendSimPacket(const QByteArray & command);
	void setGrSimIP(const QString & IP);
	void setGrSimPort(unsigned short port);

private:
	SharedRes * mSharedRes;
	ConnectorWorker * mWorker;
	QThread mThread;
};
