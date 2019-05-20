#pragma once

// Bug lib. Include first of all - math.h, then Python.h. Use Python 3.7
#include <math.h>
#include <Python.h>

#include "engineInterface.h"

class PythonEngine : public EngineInterface
{

public:
	PythonEngine(SharedRes * sharedRes);
	~PythonEngine() override;

	void evaluate() override;

	void pauseUnpause() override;

	void setDirectory(const QString & path) override;

private:
	void printOutputPython();
	void printErrorPython();
	void cleanOutputError();
	void checkPauseState();
};

