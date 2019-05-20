#pragma once

#include "Exception/pythonException/pythonException.h"

class CallMethodException : public pythonException
{
public:
	explicit CallMethodException(const QString &objectName, const QString &methodName)
		: pythonException(
			  QString("An error occurred while calling the method '%1' of the object '%2'.").arg(
				  methodName, objectName))
	{}
};
