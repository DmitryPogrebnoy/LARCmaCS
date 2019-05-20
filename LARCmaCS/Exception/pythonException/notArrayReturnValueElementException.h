#pragma once

#include "Exception/pythonException/pythonException.h"

class NotArrayReturnValueElementException : public pythonException
{
public:
	explicit NotArrayReturnValueElementException(const int elementPosition)
		: pythonException(
			  QString("The %1 element of the returned array from the Python script is not an array."
					  ).arg(QString::number(elementPosition)))
	{}
};
