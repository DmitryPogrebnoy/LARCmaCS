#pragma once

#include "Exception/pythonException/pythonException.h"

class WrongSizeArrayReturnValueElementException : public pythonException
{
public:
	explicit WrongSizeArrayReturnValueElementException(const int elementPosition,const int expectedValue,
													   const int currentValue)
		: pythonException(
			  QString("The %1 element of the returned array from the Python script has the wrong size. "
					  "Expected value: %2. Current value: %3.").arg(
				  QString::number(elementPosition), QString::number(expectedValue), QString::number(currentValue)))
	{}
};
