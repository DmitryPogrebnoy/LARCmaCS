#pragma once

#include <QString>
#include <QUdpSocket>

#include "constants.h"
#include "gameState.h"
#include "refereeInfo.h"
#include "refereeUtils.h"
#include "refereeMessage.h"

struct RefereeClientWorker : public QObject
{
    Q_OBJECT

public:
    RefereeClientWorker();
    ~RefereeClientWorker();

public slots:
    void processPendingDatagrams();
    bool open(qint16 port);
    void close();
    void start();
    void stop();

signals:
    void refereeInfoUpdate(QSharedPointer<RefereeInfo> refInfo);

private:
    void init();
    static const QString hostName;
    QUdpSocket mSocket;
    QHostAddress mGroupAddress;
};
