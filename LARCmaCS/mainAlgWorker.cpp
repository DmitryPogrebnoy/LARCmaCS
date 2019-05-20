// Bug lib. Include first of all - pythonEngine.h.
#include "pythonEngine.h"

#include "mainAlgWorker.h"

#include <QDebug>
#include <QApplication>

#include "message.h"
#include "constants.h"
#include "matlabEngine.h"

MainAlgWorker::MainAlgWorker(SharedRes * sharedRes)
	: mStatisticsTimer(this)
	, mSharedRes(sharedRes)
{}

MainAlgWorker::~MainAlgWorker(){}

void MainAlgWorker::start()
{
	mShutdownFlag = false;
	mMainEngine = new MatlabEngine(mSharedRes);
	mSecondaryEngine = new PythonEngine(mSharedRes);
	connect(mMainEngine, SIGNAL(algoStatus(const QString &)), this, SIGNAL(newPauseState(const QString &)));
	connect(mMainEngine, SIGNAL(isPause(bool)), this, SIGNAL(pause(bool)));
	connect(mMainEngine, SIGNAL(consoleMessage(const QString &)), this, SLOT(receiveMSGToConsole(const QString &)));
	connect(mMainEngine, SIGNAL(newData(const QVector<Rule> &)), this, SIGNAL(newData(const QVector<Rule> &)));
	mStatisticsTimer.setInterval(1000);
	connect(&mStatisticsTimer, SIGNAL(timeout()), this, SLOT(formStatistics()));
	mStatisticsTimer.start();
	run();
}

void MainAlgWorker::stop()
{
	mStatisticsTimer.stop();
	mShutdownFlag = true;
}

void MainAlgWorker::setDebugFrequency(int frequency) {
	mFrequency = frequency;
}

void MainAlgWorker::receiveMSGToConsole(const QString & str)
{
	if (mTotalPacketsNum % mFrequency == 0) {
		emit toConsole(str);
	}
}

void MainAlgWorker::setDirectory(const QString & path)
{
	mMainEngine->setDirectory(path);
}

void MainAlgWorker::formStatistics()
{
	QString tmp;
	QString ToStatus = "PacketsPerSec = ";
	tmp.setNum(mPacketsPerSecond);
	mPacketsPerSecond = 0;
	ToStatus += tmp;
	ToStatus += " Total packets = ";
	tmp.setNum(mTotalPacketsNum);
	ToStatus += tmp;
	emit sendStatistics(ToStatus);
}

void MainAlgWorker::run()
{
	while (!mShutdownFlag) {
		mMainEngine->evaluate();
		mPacketsPerSecond++;
		mTotalPacketsNum++;
		QApplication::processEvents();
	}
	delete mMainEngine;
}

void MainAlgWorker::pauseUnpause()
{
	mMainEngine->pauseUnpause();
}

void MainAlgWorker::switchEngine()
{
	disconnect(mMainEngine, SIGNAL(algoStatus(const QString &)), this, SIGNAL(newPauseState(const QString &)));
	disconnect(mMainEngine, SIGNAL(isPause(bool)), this, SIGNAL(pause(bool)));
	disconnect(mMainEngine, SIGNAL(consoleMessage(const QString &)), this, SLOT(receiveMSGToConsole(const QString &)));
	disconnect(mMainEngine, SIGNAL(newData(const QVector<Rule> &)), this, SIGNAL(newData(const QVector<Rule> &)));
	EngineInterface * tmp = mMainEngine;
	mMainEngine = mSecondaryEngine;
	mSecondaryEngine = tmp;
	connect(mMainEngine, SIGNAL(algoStatus(const QString &)), this, SIGNAL(newPauseState(const QString &)));
	connect(mMainEngine, SIGNAL(isPause(bool)), this, SIGNAL(pause(bool)));
	connect(mMainEngine, SIGNAL(consoleMessage(const QString &)), this, SLOT(receiveMSGToConsole(const QString &)));
	connect(mMainEngine, SIGNAL(newData(const QVector<Rule> &)), this, SIGNAL(newData(const QVector<Rule> &)));
}
