#include "refereeClient.h"

RefereeClient::RefereeClient()
	: mWorker(new RefereeClientWorker())
{
	mWorker->moveToThread(&mThread);

	connect(&mThread, SIGNAL(started()), mWorker, SLOT(start()));
	connect(&mThread, SIGNAL(finished()), mWorker, SLOT(deleteLater()));

	connect(mWorker, SIGNAL(refereeInfoUpdate(const QSharedPointer<RefereeInfo> &)),
			this, SIGNAL(refereeInfoUpdate(const QSharedPointer<RefereeInfo> &)));

	mThread.start();
}

RefereeClient::~RefereeClient()
{
	mThread.quit();
	mThread.wait();
}
