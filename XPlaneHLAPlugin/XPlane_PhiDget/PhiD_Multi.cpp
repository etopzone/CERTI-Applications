#include <iostream>
#include <memory>
#include <string>
#include "phidget21.h"

#include "XP_Certi.h"
#include "XP_Cnt.h"

//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
int __stdcall InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
printf("Mul_O[%i] -- %6d\n", Index, State);
return 0;
}

//callback that will run if an output changes.
//Index - Index of the output that generated the event, State - boolean (0 or 1) representing the output state (on or off)
int __stdcall OutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
printf("Mul_I[%i] -- %6d\n", Index, State);
return 0;
}

//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
int __stdcall SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
char		sensor[50];
float		result;

vInKit[Index]= Value;
printf("Multi[%i] -- %6f\n", Index, vInKit[Index]);

switch (Index)
	{
	case 0: strncpy(sensor,"curseur",50);	result= vInKit[Index] * 0.001f; break;
	case 1: strncpy(sensor,"genIR",50);		result= vInKit[Index] * 1.000f; break;
	default: sensor[0]= 0;;
	}

if (sensor[0]) upDateValues(sensor,49,result);		//Call update (length limitation)

return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
//Will also display the number of inputs, outputs, and analog inputs on the interface kit as well as the state of the ratiometric flag
//and the current analog sensor sensitivity.
int display_multi(CPhidgetInterfaceKitHandle phid)
{
int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
const char* ptr;

CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

CPhidgetInterfaceKit_getInputCount(phid, &numInputs);
CPhidgetInterfaceKit_getOutputCount(phid, &numOutputs);
CPhidgetInterfaceKit_getSensorCount(phid, &numSensors);
CPhidgetInterfaceKit_getRatiometric(phid, &ratiometric);

printf("%s\n", ptr);
printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
printf("# Digital Inputs: %d\n# Digital Outputs: %d\n", numInputs, numOutputs);
printf("# Sensors: %d\n", numSensors);
printf("Ratiometric: %d\n", ratiometric);

for(i = 0; i < numSensors; i++)
	{
	CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);
	printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
	}

return 0;
}

//Main---------------------------------------------------------------------------------------------
int interfaceKit_loop()
{
int result, numSensors;
const char *err;

hInKit = 0;												//Declare an InterfaceKit handle
CPhidgetInterfaceKit_create(&hInKit);			//create the InterfaceKit object

CPhidget_set_OnAttach_Handler((CPhidgetHandle)hInKit, AttachHandler, NULL);
CPhidget_set_OnDetach_Handler((CPhidgetHandle)hInKit, DetachHandler, NULL);
CPhidget_set_OnError_Handler((CPhidgetHandle)hInKit, ErrorHandler, NULL);
CPhidgetInterfaceKit_set_OnInputChange_Handler	(hInKit, InputChangeHandler, NULL);
CPhidgetInterfaceKit_set_OnSensorChange_Handler (hInKit, SensorChangeHandler, NULL);
CPhidgetInterfaceKit_set_OnOutputChange_Handler (hInKit, OutputChangeHandler, NULL);

CPhidget_open((CPhidgetHandle)hInKit, -1);	//open the interfacekit for device connections
printf("Waiting for interface kit to be attached....");
if(result = CPhidget_waitForAttachment((CPhidgetHandle)hInKit, 10000))
	{
	CPhidget_getErrorDescription(result, &err);
	printf("Problem waiting for attachment: %s\n", err);
	return 0;
	}

display_multi(hInKit);								//Display the properties of the attached accelerometer device

CPhidgetInterfaceKit_getSensorCount(hInKit, &numSensors);
for(short i = 0; i < numSensors; i++)
	CPhidgetInterfaceKit_setSensorChangeTrigger(hInKit, i, 10); 

CPhidgetInterfaceKit_setRatiometric(hInKit, 0);

printf("LOOP.....\n\n");
return(0);
}

//Main---------------------------------------------------------------------------------------------
void interfaceKit_exit()
{
CPhidget_close((CPhidgetHandle)hInKit);
CPhidget_delete((CPhidgetHandle)hInKit);
}