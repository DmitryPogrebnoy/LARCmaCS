#pragma once

#include "Exception/pythonException/pythonException.h"

class NotArrayReturnValueException : public pythonException
{
public:
	explicit NotArrayReturnValueException(const QString & objectName, const QString & methodName)
		: pythonException(
			  QString("The returned value of the method '%1' object '%2' not an array").arg(methodName, objectName)) {}
};
