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

signals:
	void refereeInfoUpdate(QSharedPointer<RefereeInfo> refInfo);

private:
	RefereeClientWorker * mWorker;
	QThread mThread;
};
