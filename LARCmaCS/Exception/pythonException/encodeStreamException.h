#pragma once

#include "Exception/pythonException/pythonException.h"

class EncodeStreamException : public pythonException
{
public:
	explicit EncodeStreamException(const QString &str)
		: pythonException(QString("Could not encode %1.").arg(str))
	{}
};
