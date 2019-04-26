#pragma once

#include <QObject>
#include <QThread>

#include "refereeClientWorker.h"

struct RefereeClient : public QObject
{
	Q_OBJECT

public:
	explicit RefereeClient();
	~RefereeClient();
	void start();
	void stop();

signals:
	void refereeInfoUpdate(QSharedPointer<RefereeInfo> refInfo);
	void wstop();

private:
	RefereeClientWorker * mWorker;
	QThread mThread;
};
