#pragma once

#include "Exception/pythonException/pythonException.h"

class CouldNotSetItemTupleException : public pythonException
{
public:
	explicit CouldNotSetItemTupleException(const QString & tupleName, const int & itemNumber, const QString & valueName)
		: pythonException(
			  QString("Could not set '%1' value as item %2 of '%3' tuple.").arg(
				  valueName, QString::number(itemNumber), tupleName)) {}
};
