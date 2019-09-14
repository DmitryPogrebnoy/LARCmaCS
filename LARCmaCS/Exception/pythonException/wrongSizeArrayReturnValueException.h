#pragma once

#include "Exception/pythonException/pythonException.h"

class WrongSizeArrayReturnValueException : public pythonException
{
public:
	explicit WrongSizeArrayReturnValueException(const int expectedValue, const int currentValue)
		: pythonException(QString(
						  "The returned array from the Python script has the wrong size. "
						  "Expected value: (array size > 0) and (array size <= %1). Current value: %2."
						  ).arg(QString::number(expectedValue), QString::number(currentValue))) {}
};
