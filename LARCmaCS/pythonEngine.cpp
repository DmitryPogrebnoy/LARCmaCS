#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

// Bug lib. Include first of all - math.h, then Python.h. Use Python 3.7
#include <math.h>
#include <Python.h>

#include <QCoreApplication>
#include <QDebug>

#include "pythonEngine.h"
#include "pythonScopedPointerDeleter.h"

#include "Exception/pythonException/attrNotFoundException.h"
#include "Exception/pythonException/callMethodException.h"
#include "Exception/pythonException/couldNotCastToDouble.h"
#include "Exception/pythonException/couldNotSetItemTupleException.h"
#include "Exception/pythonException/createTupleException.h"
#include "Exception/pythonException/decodeException.h"
#include "Exception/pythonException/encodeStreamException.h"
#include "Exception/pythonException/notArrayReturnValueException.h"
#include "Exception/pythonException/notArrayReturnValueElementException.h"
#include "Exception/pythonException/notBoolReturnValueException.h"
#include "Exception/pythonException/objectNotFoundException.h"
#include "Exception/pythonException/wrongSizeArrayReturnValueElementException.h"
#include "Exception/pythonException/wrongSizeArrayReturnValueException.h"

#include "constants.h"

// Use NumPy 1.16.2
#include <arrayobject.h>

PythonEngine::PythonEngine(SharedRes * sharedRes)
	: EngineInterface(sharedRes)
{
	QString name = QCoreApplication::applicationFilePath();
	QByteArray byteArrayName = name.toLocal8Bit();
	wchar_t* programName = Py_DecodeLocale(byteArrayName.data(), nullptr);
	if (programName == nullptr) {
		qDebug()<<"Fatal error: cannot decode application file path"<<endl;
	}
	Py_SetProgramName(programName);
	PyMem_RawFree(programName);

	Py_Initialize();

	QScopedPointer<wchar_t,ScopedPointerPy_DecodeLocaleDeleter>
			py_argv((Py_DecodeLocale("Python algorithm", nullptr)));
	wchar_t* set_argv = py_argv.data();
	PySys_SetArgv(1, &set_argv);

	PyRun_SimpleString("import sys\n"
					   "import os\n"
					   "import io\n"
					   "sys.path.append(os.path.join(os.getcwd(),'PYScripts'))\n"
					   "sys.stdout = io.StringIO()\n"
					   "sys.stderr = sys.stdout\n");

	import_array1();
}

PythonEngine::~PythonEngine(){
	Py_Finalize();
}

void PythonEngine::evaluate(){

	cleanOutputError();

	const QSharedPointer<PacketSSL> & packetSSL = loadVisionData();

	//Evaluate control signals in Python script
	try{

		QByteArray byteArrayNamePyMainScript = Constants::pythonNameMainScript.toLocal8Bit();
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				py_NameModule(PyUnicode_DecodeFSDefault(byteArrayNamePyMainScript.data()));
		if (py_NameModule.isNull()) {
			throw DecodeException(Constants::pythonNameMainScript);
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>py_Module(PyImport_GetModule(py_NameModule.data()));
		if (py_Module.isNull()) {
			PyErr_Clear( );
			py_Module.reset(PyImport_Import(py_NameModule.data()));
			if (py_Module.isNull()) {
				throw ObjectNotFoundException(Constants::pythonNameMainScript);
			}
		}

		QByteArray byteArrayNamePyMainMethod = Constants::pythonNameMainMethod.toLocal8Bit();
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				py_Method(PyObject_GetAttrString(py_Module.data(), byteArrayNamePyMainMethod.data()));
		if (py_Method.isNull() || !PyCallable_Check(py_Method.data())) {
			throw AttrNotFoundException(Constants::pythonNameMainScript,Constants::pythonNameMainMethod);
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter> py_Args(PyTuple_New(3));
		if (py_Args.isNull()){
			throw CreateTupleException(3);
		}

		npy_intp dims_Ball[1] = {Constants::ballAlgoPacketSize};
		npy_intp dims_TeamBlue[1] = {Constants::robotAlgoPacketSize};
		npy_intp dims_TeamYellow[1] = {Constants::robotAlgoPacketSize};

		PyObject* py_List_Ball =
				PyArray_SimpleNewFromData(1,dims_Ball, NPY_FLOAT64, packetSSL->balls);
		PyObject* py_List_TeamBlue =
				PyArray_SimpleNewFromData(1, dims_TeamBlue, NPY_FLOAT64, packetSSL->robots_blue);
		PyObject* py_List_TeamYellow =
				PyArray_SimpleNewFromData(1, dims_TeamYellow, NPY_FLOAT64, packetSSL->robots_yellow);


		if (PyTuple_SetItem(py_Args.data(), 0, py_List_Ball) != 0){
			throw CouldNotSetItemTupleException(QString("py_Args"),0,QString("py_List_Ball"));
		}
		if (PyTuple_SetItem(py_Args.data(), 1, py_List_TeamBlue) != 0){
			throw CouldNotSetItemTupleException(QString("py_Args"),1,QString("py_List_TeamBlue"));
		}
		if (PyTuple_SetItem(py_Args.data(), 2, py_List_TeamYellow) != 0){
			throw CouldNotSetItemTupleException(QString("py_Args"),2,QString("py_List_TeamYellow"));
		}


		//Run the Python script and return the result.
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				py_Value(PyObject_CallObject(py_Method.data(), py_Args.data()));
		if (py_Value.isNull()){
			throw CallMethodException(Constants::pythonNameMainScript, Constants::pythonNameMainMethod);
		}
		if (!PyList_Check(py_Value.data())){
			throw NotArrayReturnValueException(Constants::pythonNameMainScript, Constants::pythonNameMainMethod);
		}

		int countReturnedRule = static_cast<int>(PyList_Size(py_Value.data()));

		if ((countReturnedRule == 0) || (countReturnedRule > Constants::ruleAmount)){
			throw WrongSizeArrayReturnValueException(Constants::ruleAmount,
													 static_cast<const int>(PyList_Size(py_Value.data())));
		}

		for (int i = 0; i < countReturnedRule; i++){
			if (!PyList_Check(PyList_GetItem(py_Value.data(),i))) {
				throw NotArrayReturnValueElementException(i);
			}
			if (PyList_Size(PyList_GetItem(py_Value.data(),i)) != Constants::pythonRuleLength) {
				throw WrongSizeArrayReturnValueElementException
						(i, Constants::pythonRuleLength,
						 static_cast<const int>(PyList_Size(PyList_GetItem(py_Value.data(),i))));
			}
		}

		// Intermediate buffer for control signals.
		int** controlSignals = new int*[countReturnedRule];
		for (int i = 0; i < countReturnedRule; i++){
			controlSignals[i] = new int[Constants::pythonRuleLength];
		}

		/*
		* Convert the values returned by python into arrays of int.
		* It is important that the returned numeric values that fit into the int, otherwise
		* if it does not fit in long, then we throw an exception;
		* if it fit in long, then we lose the high bits.
		*/
		for (int i = 0; i < countReturnedRule; i++){
			for (int j = 0; j < Constants::pythonRuleLength; j++){
				controlSignals[i][j] = static_cast<int>
						(PyLong_AsLong(PyList_GetItem(PyList_GetItem(py_Value.data(),i),j)));
				//If the сast fails, then Python will set an error and we throw an exception.
				if (PyErr_Occurred()) {
					throw CouldNotCastToDouble(i,j);
				}
			}
		}

		// Sending, coming from the Python script, control signals to robots
		/*
		 * Python script returns control signals
		 * each of which is 5 int, where
		 * 0 - the number of the robot that owns the rule. number i=0..ruleAmount-1
		 * 1 - SpeedX
		 * 2 - SpeedY
		 * 3 - kickForward
		 * 4 - SpeedR
		 * 5 - kickUp
		*/
		QVector<Rule> rules(Constants::ruleAmount);

		for (int i = 0; i < countReturnedRule; i++) {
			rules[controlSignals[i][0]].mSpeedX = controlSignals[i][1];
			rules[controlSignals[i][0]].mSpeedY = controlSignals[i][2];
			rules[controlSignals[i][0]].mSpeedR = controlSignals[i][4];
			rules[controlSignals[i][0]].mKickUp = controlSignals[i][5];
			rules[controlSignals[i][0]].mKickForward = controlSignals[i][3];
		}

		emit newData(rules);

		printOutputPython();

		checkPauseState();

		for (int i = 0; i < countReturnedRule; i++){
			delete [] controlSignals[i];
		}
		delete [] controlSignals;

	} catch (pythonException& e) {
		emit consoleMessage(e.message());
		printErrorPython();
		emit algoStatus(QString("ERROR"));

		QVector<Rule> rules(Constants::ruleAmount);
		emit newData(rules);
	}
}

void PythonEngine::pauseUnpause(){
	cleanOutputError();
	try{
		QByteArray byteArrayNamePyPauseScript = Constants::pythonNamePauseScript.toLocal8Bit();
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				py_NameModule(PyUnicode_DecodeFSDefault(byteArrayNamePyPauseScript.data()));
		if (py_NameModule.isNull()) {
			throw DecodeException(byteArrayNamePyPauseScript.data());
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>py_Module(PyImport_GetModule(py_NameModule.data()));
		if (py_Module.isNull()) {
			PyErr_Clear( );
			py_Module.reset(PyImport_Import(py_NameModule.data()));
			if (py_Module.isNull()) {
				throw ObjectNotFoundException(Constants::pythonNameMainScript);
			}
		}

		QByteArray byteArrayNamePyPauseMethod = Constants::pythonNamePauseMethod.toLocal8Bit();
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				py_Method(PyObject_GetAttrString(py_Module.data(), byteArrayNamePyPauseMethod.data()));
		if (py_Method.isNull() || !PyCallable_Check(py_Method.data())) {
			throw AttrNotFoundException(Constants::pythonNamePauseScript, Constants::pythonNamePauseMethod);
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter> py_Value(PyObject_CallObject(py_Method.data(), nullptr));
		if (py_Value.isNull()){
			throw CallMethodException(byteArrayNamePyPauseScript.data(), byteArrayNamePyPauseMethod.data());
		}
		if (!PyBool_Check(py_Value.data())){
			throw NotBoolReturnValueException(Constants::pythonNamePauseScript, Constants::pythonNamePauseMethod);
		}

		bool pauseMark = PyObject_IsTrue(py_Value.data());
		emit isPause(pauseMark);

		printOutputPython();

	} catch (pythonException& e) {
		emit consoleMessage(e.message());
		if (PyErr_Occurred()) {
			printErrorPython();
		}
		emit algoStatus(QString("ERROR"));
	}
}

void PythonEngine::setDirectory(const QString &path){
	cleanOutputError();
	QString simpleString = QString("sys.path.append('%1')\n").arg(path);
	QByteArray byteArraySimpleString = simpleString.toLocal8Bit();
	PyRun_SimpleString(byteArraySimpleString.data());
	if (PyErr_Occurred()) {
		printErrorPython();
	}
}

void PythonEngine::printOutputPython(){
	try{
		PyObject* sysModule = PyImport_AddModule("sys");

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter> stdoutBuffer(PyObject_GetAttrString(sysModule,"stdout"));
		if (stdoutBuffer.isNull()) {
			throw AttrNotFoundException(QString("sys"),QString("stdout"));
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				stdoutBufferGetValue(PyObject_GetAttrString(stdoutBuffer.data(), "getvalue"));
		if (stdoutBufferGetValue.isNull()) {
			throw AttrNotFoundException(QString("sys.stdout"),QString("getvalue"));
		}
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				value(PyObject_CallObject(stdoutBufferGetValue.data(),nullptr));
		if (value.isNull()) {
			throw CallMethodException(QString("sys.stdout"),QString("getvalue"));;
		}
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter> outputUTF8String(PyUnicode_AsUTF8String(value.data()));
		if (outputUTF8String.isNull()) {
			throw EncodeStreamException(QString("stdout"));;
		}
		char* outputStr = PyBytes_AsString(outputUTF8String.data());

		QString message = QString(outputStr);
		if (!message.isEmpty()){
			emit consoleMessage(QString(outputStr));
		}

	} catch (pythonException& e){
		emit consoleMessage(QString("Could not access the output. %1").arg(e.message()));
	}
}

void PythonEngine::printErrorPython(){
	// If there are errors, output them to stderr
	if (PyErr_Occurred()) {
		PyErr_Print();
	}
	try{
		PyObject* sysModule = PyImport_AddModule("sys");

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter> stderrBuffer(PyObject_GetAttrString(sysModule,"stderr"));
		if (stderrBuffer.isNull()) {
			throw AttrNotFoundException(QString("sys"),QString("stderr"));
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				stderrBufferMethod(PyObject_GetAttrString(stderrBuffer.data(), "getvalue"));
		if (stderrBufferMethod.isNull()) {
			throw AttrNotFoundException(QString("sys.stderr"),QString("getvalue"));
		}
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				error(PyObject_CallObject(stderrBufferMethod.data(),nullptr));
		if (error.isNull()) {
			throw CallMethodException(QString("sys.stderr"),QString("getvalue"));;
		}
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter> errorUTF8String(PyUnicode_AsUTF8String(error.data()));
		if (errorUTF8String.isNull()) {
			throw EncodeStreamException(QString("sys.stderr"));;
		}
		char* errorStr = PyBytes_AsString(errorUTF8String.data());

		QString message = QString(errorStr);
		if (!message.isEmpty()){
			emit consoleMessage(QString(errorStr));
		}

	} catch (pythonException& e){
		emit consoleMessage(QString("Could not access the error. %1").arg(e.message()));
	}
}

void PythonEngine::cleanOutputError(){
	PyRun_SimpleString("sys.stderr.truncate(0)\n"
					   "sys.stderr.seek(0)\n");
}

void PythonEngine::checkPauseState(){
	try{
		QByteArray byteArrayNamePyPauseScript = Constants::pythonNamePauseScript.toLocal8Bit();
		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				py_NameModule(PyUnicode_DecodeFSDefault(byteArrayNamePyPauseScript.data()));
		if (py_NameModule.isNull()) {
			throw DecodeException(byteArrayNamePyPauseScript.data());
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter> py_Module(PyImport_Import(py_NameModule.data()));
		if (py_Module.isNull()) {
			throw ObjectNotFoundException(byteArrayNamePyPauseScript.data());
		}

		QScopedPointer<PyObject, ScopedPointerPyObjectDeleter>
				py_Value(PyObject_GetAttrString(py_Module.data(), "PAUSE"));
		if (py_Value.isNull()){
			throw CallMethodException(byteArrayNamePyPauseScript.data(), QString("PAUSE"));
		}
		if (!PyBool_Check(py_Value.data())){
			throw NotBoolReturnValueException(Constants::pythonNamePauseScript, QString("PAUSE"));
		}

		bool pauseMark = PyObject_IsTrue(py_Value.data());

		if (pauseMark) {
			emit algoStatus(QString("PAUSE"));
		} else {
			emit algoStatus(QString("WORK"));
		}
	} catch (pythonException& e) {
		emit consoleMessage(e.message());
		if (PyErr_Occurred()) {
			printErrorPython();
		}
		emit algoStatus(QString("ERROR"));
	}
}
