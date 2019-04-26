#include "refereeClientWorker.h"

const QString RefereeClientWorker::hostName = QStringLiteral("224.5.23.2");

RefereeClientWorker::RefereeClientWorker()
	: mSocket(this)
	, mGroupAddress(hostName)
{
	connect(&mSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

RefereeClientWorker::~RefereeClientWorker()
{}

void RefereeClientWorker::start()
{
	open(Constants::refereePort);
}

bool RefereeClientWorker::open(qint16 port)
{
	close();
	if (mSocket.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress)
			&& mSocket.joinMulticastGroup(mGroupAddress)) {
		return true;
	}
	return false;
}

void RefereeClientWorker::close()
{
	mSocket.close();
}

void RefereeClientWorker::processPendingDatagrams()
{
	GameState gState;
	QSharedPointer<RefereeInfo> refInfo;
	RefereeInfo tempRefInfo;

	while (mSocket.hasPendingDatagrams()) {
		QByteArray datagram;
		int datagramSize = static_cast<int>(mSocket.pendingDatagramSize());
		datagram.resize(datagramSize);

		Referee refPacket;
		mSocket.readDatagram(datagram.data(), datagramSize);
		refPacket.ParseFromArray(datagram.data(), datagramSize);

		RefereeMessage message;
		message = RefereeMessage(refPacket);
		gState.updateGameState(message);
		gState.updateRefereeInfoFromState(tempRefInfo);

		if (tempRefInfo.state != refInfo->state
				|| tempRefInfo.commandForTeam != refInfo->commandForTeam
				|| tempRefInfo.isPartOfFieldLeft != refInfo->isPartOfFieldLeft) {
			refInfo = QSharedPointer<RefereeInfo>(new RefereeInfo(tempRefInfo));
			emit refereeInfoUpdate(refInfo);
		}
	}
}
