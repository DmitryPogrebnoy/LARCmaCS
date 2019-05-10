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
	mEngine = new MatlabEngine(mSharedRes);
	connect(mEngine, SIGNAL(algoStatus(const QString &)), this, SIGNAL(newPauseState(const QString &)));
	connect(mEngine, SIGNAL(isPause(bool)), this, SIGNAL(pause(bool)));
	connect(mEngine, SIGNAL(consoleMessage(const QString &)), this, SLOT(receiveMSGToConsole(const QString &)));
	connect(mEngine, SIGNAL(newData(const QVector<Rule> &)), this, SIGNAL(newData(const QVector<Rule> &)));
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
	mEngine->setDirectory(path);
}

void MainAlgWorker::formStatistics()
{
	QString tmp;
	QString ToStatus = "Using Matlab: PacketsPerSec = ";
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
		mEngine->evaluate();
		mPacketsPerSecond++;
		mTotalPacketsNum++;
		QApplication::processEvents();
	}
	delete mEngine;
}

<<<<<<< HEAD
void MainAlgWorker::pauseUnpause()
=======
void MainAlgWorker::updatePauseState()
{
	evalString("ispause=RP.Pause");
	mxArray *mxitpause = engGetVariable(fmldata.ep, "ispause");
	bool pauseStatus = true;
	if (mxitpause != 0) {
		double* itpause = mxGetPr(mxitpause);
		if (itpause != 0) {
			if ((*itpause) == 0) {
				mxArray* mxzMain_End = engGetVariable(fmldata.ep, "zMain_End");
				if (mxzMain_End != 0) {
					double* zMain_End = mxGetPr(mxzMain_End);
					if (zMain_End != 0) {
						if ((*zMain_End) == 0) {
							emit newPauseState("main br");
						} else {
							pauseStatus = false;
							emit newPauseState("WORK");
						}
					} else {
						emit newPauseState("-err-z");
					}
				} else {
					emit newPauseState("-err-mz");
				}
			} else {
				emit newPauseState("PAUSE");
			}
		} else {
			emit newPauseState("-err-p"); //Matlab answer corrupted
		}
	} else {
		emit newPauseState("-err-mp"); //Matlab does not respond
	}

	if (mIsPause != pauseStatus) {
		emit pause(pauseStatus);
		mIsPause = pauseStatus;
	}
}

void MainAlgWorker::processPacket(const QSharedPointer<PacketSSL> & packetssl)
{
	if (packetssl.isNull()) {
		return;
	}
// Заполнение массивов Balls Blues и Yellows и запуск main-функции
	mPacketsPerSecond++;
	mTotalPacketsNum++;

	memcpy(mxGetPr(fmldata.Ball), packetssl->balls, Constants::ballAlgoPacketSize * sizeof(double));
	memcpy(mxGetPr(fmldata.Blue), packetssl->robots_blue, Constants::robotAlgoPacketSize * sizeof(double));
	memcpy(mxGetPr(fmldata.Yellow), packetssl->robots_yellow, Constants::robotAlgoPacketSize * sizeof(double));
	memcpy(mxGetPr(fmldata.ballInside), &mIsBallInside, sizeof(double));

	double state = mSharedRes->getRefereeState();
	memcpy(mxGetPr(fmldata.state), &state, sizeof(double));

	double team = mSharedRes->getRefereeTeam();
	memcpy(mxGetPr(fmldata.team), &team, sizeof(double));

	double partOfFied = mSharedRes->getRefereePartOfFildLeft();
	memcpy(mxGetPr(fmldata.partOfFieldLeft), &partOfFied, sizeof(double));

	engPutVariable(fmldata.ep, "Balls", fmldata.Ball);
	engPutVariable(fmldata.ep, "Blues", fmldata.Blue);
	engPutVariable(fmldata.ep, "Yellows", fmldata.Yellow);
	engPutVariable(fmldata.ep, "ballInside", fmldata.ballInside);
	engPutVariable(fmldata.ep, "state", fmldata.state);
	engPutVariable(fmldata.ep, "commandForTeam", fmldata.team);
	engPutVariable(fmldata.ep, "partOfFieldLeft", fmldata.partOfFieldLeft);

	evalString(fmldata.config.file_of_matlab);
// Забираем Rules и очищаем его в воркспейсе

	fmldata.Rule = engGetVariable(fmldata.ep, "Rules");
	double *ruleArray =
			(double *)malloc(Constants::ruleAmount * Constants::ruleLength * sizeof(double));
	if (fmldata.Rule != 0) {
		memcpy(ruleArray, mxGetPr(fmldata.Rule), Constants::ruleAmount * Constants::ruleLength * sizeof(double));
	} else {
		memset(ruleArray, 0, Constants::ruleAmount * Constants::ruleLength * sizeof(double));
	}
	char sendString[256];
	sprintf(sendString, "Rules=zeros(%d, %d);", Constants::ruleAmount, Constants::ruleLength);
	evalString(sendString);

// Разбор пришедшего пакета и переправка его строк на connector

	for (int i = 0; i < Constants::ruleAmount; i++) {
		QVector<double> newmess;
		newmess.resize(Constants::ruleLength);

		for (int j = 0; j < Constants::ruleLength; j++) {
			newmess[j] = ruleArray[j * Constants::ruleAmount + i];
		}

		emit newData(newmess);
	}
	free(ruleArray);
	mxDestroyArray(fmldata.Rule);

	updatePauseState();
}

void MainAlgWorker::Pause()
{
	evalString("PAUSE();");
}

void MainAlgWorker::runMatlab()
{
	if (!(fmldata.ep = engOpen(NULL))) {
		qDebug() << "Can't open Matlab Engine" << endl;
		fmtlab = false;
		return;
	}

	mMatlabOutputBuffer[Constants::matlabOutputBufferSize - 1] = '\0';
	engOutputBuffer(fmldata.ep, mMatlabOutputBuffer, Constants::matlabOutputBufferSize - 1);
	printf("Matlab Engine is opened\n");

	//-----create Rules-----
	char sendString[256];
	sprintf (sendString, "Rules=zeros(%d, %d)", Constants::ruleAmount, Constants::ruleLength);
	evalString(sendString);

	QString dirPath = "cd " + QCoreApplication::applicationDirPath() + "/MLscripts";
	evalString(dirPath);
	fmtlab = true;
}

void MainAlgWorker::stop_matlab()
>>>>>>> Add crucial referee info to matlab
{
	mEngine->pauseUnpause();
}
