#pragma once

#include "Exception/pythonException/pythonException.h"

class DecodeException : public pythonException
{
public:
	explicit DecodeException(const QString &str)
		: pythonException(QString("Could not decode string: %1.").arg(str))
	{}
};
