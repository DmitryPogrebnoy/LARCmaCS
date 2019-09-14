#pragma once

#include "Exception/pythonException/pythonException.h"

class AttrNotFoundException : public pythonException
{
public:
	explicit AttrNotFoundException(const QString & objectName, const QString & methodName)
		: pythonException(QString("Could not found attribute '%1' in Python object '%2'.").arg(methodName, objectName))
	{}
};
