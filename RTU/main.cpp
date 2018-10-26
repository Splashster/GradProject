
#include <iostream>
#include <ini.h>
#include <stdexcept>
#include <wiringPi.h>
#include <Python.h>

#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/UpdateBuilder.h>
#include <asiodnp3/PrintingChannelListener.h>
#include <asiopal/UTCTimeSource.h>
#include <asiodnp3/ConsoleLogger.h>
#include <opendnp3/LogLevels.h>

#include <thread>
#include "Config.h"
#include "GPIOCommandHandler.h"

// prototypes for handling ini file reading
int cfg_handler(void* user, const char* section, const char* name, const char* value);
bool safe_handler(Config& config, const std::string& section, const std::string& name, const std::string& value);
int splitString(char* string, char delim, char** storage);

using namespace opendnp3;
using namespace asiopal;
using namespace asiodnp3;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		std::cerr << "usage: rpi-dnp3-gpio <ini file path>" << std::endl;
		return -1;
	}

	auto filename = argv[1];

	Config config;
	if(ini_parse(filename, cfg_handler, &config) != 0)
	{
		std::cerr << "error parsing ini file: " << filename << std::endl;
		return -1;
	}

	// Initialize wiringPi -- using Broadcom pin numbers
	// this call automatically terminates program with an error msg if the setup cannot be completed
	wiringPiSetupGpio();

	// setup inputs and outputs
	for(auto pin : config.inputs) {
		pinMode(pin, INPUT);
		std::cout << "pin " << static_cast<int>(pin) << " set as INPUT" << std::endl;
	}

	for(auto pin : config.outputs) {
		pinMode(pin, OUTPUT);
		std::cout << "pin " << static_cast<int>(pin) << " set as OUTPUT" << std::endl;
	}

	const auto commandHandler = std::make_shared<GPIOCommandHandler>(config.outputs);

	const auto LOG_LEVELS = levels::NORMAL | levels::ALL_APP_COMMS;

	DNP3Manager manager(1, ConsoleLogger::Create());

	auto channel = manager.AddTCPServer("server", LOG_LEVELS, ChannelRetry::Default(), "0.0.0.0", config.port, PrintingChannelListener::Create());

	OutstationStackConfig stack(
		DatabaseSizes(
			0, 0, 24, 0, 0, 0, 0, 0 //24 analog inputs 
		)
	);
	stack.link = config.link;

	stack.outstation.eventBufferConfig = EventBufferConfig(50);
	stack.outstation.params.allowUnsolicited = true;

	auto outstation = channel->AddOutstation("outstation", commandHandler, DefaultOutstationApplication::Create(), stack);

	outstation->Enable();

	std::cout << "Sample period is: " << config.sample_period_ms << std::endl;
        const auto SAMPLE_PERIOD = std::chrono::milliseconds(config.sample_period_ms);

	PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;

	Py_Initialize();
	pName = PyString_FromString("read_serial");

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if(pModule != NULL){
		pFunc = PyObject_GetAttrString(pModule, "readSerial");

		if (pFunc && PyCallable_Check(pFunc)){
			pArgs = PyTuple_New(2);
			pValue = Pystring_FromString("/dev/ttyACM0");
			PyTuple_SetItem(pArgs, 0, pValue);
			pValue = Pystring_FromString("2000000");
			PyTuple_SetItem(pArgs, 1, pValue);
		}
	}

	while(true) {

		DNPTime time(asiopal::UTCTimeSource::Instance().Now().msSinceEpoch);

		UpdateBuilder builder;

		uint16_t index = 0;
		int num_elemnts = 0;
		char** serial_info = (char*)malloc(sizeof(char*) * 1024);
		char** sensor_info = (char*)malloc(sizeof(char*) * 1024);
 		
		
		pValue = PyObject_CallObject(pFunc, pArgs);

		if(pValue != NULL){
			serial_info = PyString_FromString(pValue);
			num_elements = splitStrings(serial_info, ",", sensor_info);				
			free(serial_info);
		}else{
			printf("Failed");
		}

		Py_XDECREF(pValue);
		/*for(auto pin : config.inputs) {
			bool value = digitalRead(pin);
			builder.Update(Binary(value, 0x01, time), index);
			++index;
		}

		index = 0;
		for(auto pin : config.outputs) {
			bool value = digitalRead(pin);
			builder.Update(BinaryOutputStatus(value, 0x01, time), index);
			++index;
		}*/

		for (index = 0; index < num_elements; index++){			
			builder.Update(Analog(atof(sensor_info[index]), 0x01, time), index);
		}

		outstation->Apply(builder.Build());
		free(serial_info);
		
		// determines the sampling rate
		std::this_thread::sleep_for(SAMPLE_PERIOD);
	}
	Py_DECREF(pArgs);
	Py_XDECREF(pFunc);	
	Py_XDECREF(pModule);	
}

bool safe_handler(Config& config, const std::string& section, const std::string& name, const std::string& value)
{
	try
	{		
		if(section == "input")
		{
			return config.AddInput(std::stoi(name));
		}
		else if(section == "output")
		{
			return config.AddOutput(std::stoi(name));
		}
		else if(section == "program")
		{
			if(name == "sample-period-ms")
			{
				config.sample_period_ms = std::stoi(value);
				return true;
			}
		}
		else if(section == "link")
		{
			if(name == "remote-addr")
			{
				config.link.RemoteAddr = std::stoi(value);
				return true;
			}
			else if(name == "local-addr")
			{
				config.link.LocalAddr = std::stoi(value);
				return true;
			}
			else if (name == "port")
			{
				config.port = std::stoi(value);
				return true;
			}
		}

		std::cerr << "Unknown parameter, section: " << section << " name: " << name << " value: " << value << std::endl;
		return false;
	}
	catch(std::invalid_argument)
	{
		std::cerr << "Bad integer conversion, section: " << section << " name: " << name << " value: " << value << std::endl;
		return false;
	}
}

int cfg_handler(void* user, const char* section, const char* name, const char* value)
{
	return safe_handler(*(Config*)user, section, name, value) ? 1 : 0;
}

int splitString(char* string, char delim, char** storage){
	char* savePtr, tok;
	int i = 0;
	tok = strtok_r(string, delim, savePtr);

	while(tok != NULL){
		stroage[i] = tok;
		tok = strtok_r(string, delim, savePtr);
		i++;

	return i;

}
