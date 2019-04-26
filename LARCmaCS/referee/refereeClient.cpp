#include "refereeClient.h"

RefereeClient::RefereeClient()
{
    mWorker = new RefereeClientWorker();
    mWorker->moveToThread(&mThread);

    connect(mWorker, SIGNAL(finished()), mWorker, SLOT(deleteLater()));
    connect(mWorker, SIGNAL(finished()), &mThread, SLOT(quit()));

    connect(&mThread, SIGNAL(started()), mWorker, SLOT(start()));
    connect(&mThread, SIGNAL(finished()), mWorker, SLOT(deleteLater()));

    connect(mWorker, SIGNAL(refereeInfoUpdate()), this, SIGNAL(refereeInfoUpdate()));
}

RefereeClient::~RefereeClient()
{
    mThread.quit();
    mThread.wait();
}

void RefereeClient::start()
{
    mThread.start();
}

void RefereeClient::stop()
{
    emit wstop();
}
