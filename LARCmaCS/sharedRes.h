// Copyright 2019 Dmitrii Iarosh

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "messages_robocup_ssl_wrapper.pb.h"

#include <QVector>
#include <QSharedPointer>
#include <QMutex>
#include <QMap>
#include <QReadWriteLock>

#include "constants.h"

class SharedRes : public QObject
{
	Q_OBJECT

public:
	SharedRes();
	QString getRobotIP(int id);
	void setRobotIP(int id, const QString & ip);
	void setBallInsideData(const QString & ip, bool isBallInside);
	void setDetection(const QSharedPointer<SSL_WrapperPacket> & detection, int camID);
	void setGeometry(const QSharedPointer<SSL_WrapperPacket> & geometry);
	const QSharedPointer<QVector<bool> > & getBallInsideData();
	QSharedPointer<QVector<QSharedPointer<SSL_WrapperPacket> > > getDetection();
	QSharedPointer<SSL_WrapperPacket> getGeometry();
	QSharedPointer<SSL_WrapperPacket> getDetection(int camID);

private:
	QMutex mIPMutex;
	QMap<int, QString> mIPRobotList;
	QSharedPointer<QVector<QSharedPointer<SSL_WrapperPacket> > > mDetection;
	QSharedPointer<SSL_WrapperPacket> mGeometry;
	QSharedPointer<QVector<bool> > mBallInsideData;
	QReadWriteLock mDetectionLock;
};
