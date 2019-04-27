#pragma once

#include <QThread>

#include "refereeClientWorker.h"

struct RefereeClient : public QObject
{
	Q_OBJECT

public:
	explicit RefereeClient();
	~RefereeClient();

signals:
	void refereeInfoUpdate(const QSharedPointer<RefereeInfo> & refInfo);

private:
	RefereeClientWorker * mWorker;
	QThread mThread;
};
