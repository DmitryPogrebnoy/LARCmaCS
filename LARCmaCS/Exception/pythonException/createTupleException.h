#pragma once

#include "Exception/pythonException/pythonException.h"

class CreateTupleException : public pythonException
{
public:
	explicit CreateTupleException(const int lengthTuple)
		: pythonException(QString("Failed to create a tuple of length %1.").arg(QString::number(lengthTuple)))
	{}
};
