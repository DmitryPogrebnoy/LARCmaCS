#pragma once

#include "Exception/pythonException/pythonException.h"

class ObjectNotFoundException : public pythonException
{
public:
	explicit ObjectNotFoundException(const QString & objectName)
		: pythonException(QString("Could not found Python module '%1'.").arg(objectName)) {}
};
