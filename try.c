#include <Python.h>

int main(int argc, char* argv[]){

	PyObject *pName, *pModule, *pFunc, *pValue;

	//Initialize the Python Interpreter
	Py_Initialize();


	//Build the name object
	pName = PyString_FromString("read_serial");


	
	//Load the module object
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);


	//pDict is a borrowed reference
	//pDict = PyModule_GetDict(pModule);

	//pFunc is also a borrowed reference
	
	if(pModule != NULL){	
		pFunc = PyObject_GetAttrString(pModule, "readSerial");

		if(PyCallable_Check(pFunc)){
			while(1){
				pValue = PyObject_CallObject(pFunc, NULL);
				printf("%s",PyString_AsString(pValue));
				Py_DECREF(pValue);
			}
		}else{
			PyErr_Print();
		}

		//Clean up
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
		//Py_DECREF(pName);

		//Finish the Python Interpreter
		Py_Finalize();

		return 0;
	}else{
		printf("bye");

	}
}
