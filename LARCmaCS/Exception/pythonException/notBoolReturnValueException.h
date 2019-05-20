#pragma once

#include "Exception/pythonException/pythonException.h"

class NotBoolReturnValueException : public pythonException
{
public:
	explicit NotBoolReturnValueException(const QString &objectName, const QString &methodName)
		: pythonException(
			  QString("The returned value of the method '%1' object '%2' is not a bool").arg(methodName, objectName))
	{}
};
