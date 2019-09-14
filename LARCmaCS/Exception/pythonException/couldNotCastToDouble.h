#pragma once

#include "Exception/pythonException/pythonException.h"

class CouldNotCastToDouble : public pythonException
{
public:
	explicit CouldNotCastToDouble(const int numberRule, const int numberPosition)
		: pythonException(QString("Could not cast (%1,%2) element of returned array to double type."
							  ).arg(QString::number(numberRule), QString::number(numberPosition))) {}
};
