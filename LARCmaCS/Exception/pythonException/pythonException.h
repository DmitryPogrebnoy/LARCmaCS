#pragma once

#include "Exception/baseException/baseException.h"

class pythonException : public Exception
{
public:
	explicit pythonException(const QString &str) : Exception(str)
	{}
};
