#include <iostream>
#include <memory>
#include <string>
#include "phidget21.h"

#include "XP_Certi.h"
#include "XP_Cnt.h"

//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
int __stdcall InputCirculHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
printf("Cir_S[%i] -- %1d\n", Index, State);
return 0;
}

//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
int __stdcall SensorCirculHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
try {
vCircul= Value * 0.359f;
printf("Circu[%i] -- %5.0f\n", Index, vCircul);

upDateValues("angulaire",49,vCircul);		//Call update (length limitation)
}
catch (...)
{ }
return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
//Will also display the number of inputs, outputs, and analog inputs on the interface kit as well as the state of the ratiometric flag
//and the current analog sensor sensitivity.
int display_circular(CPhidgetInterfaceKitHandle phid)
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

for(i = 0; i < numSensors; i++)
	{
	CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);
	printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
	}

return 0;
}

//Main---------------------------------------------------------------------------------------------
int circular_loop()
{
int result, numSensors;
const char *err;

hCircul = 0;									//Declare an InterfaceKit handle
CPhidgetInterfaceKit_create(&hCircul);//create the InterfaceKit object

CPhidget_set_OnAttach_Handler((CPhidgetHandle)hCircul, AttachHandler, NULL);
CPhidget_set_OnDetach_Handler((CPhidgetHandle)hCircul, DetachHandler, NULL);
CPhidget_set_OnError_Handler((CPhidgetHandle)hCircul, ErrorHandler, NULL);
CPhidgetInterfaceKit_set_OnInputChange_Handler	(hCircul, InputCirculHandler, NULL);
CPhidgetInterfaceKit_set_OnSensorChange_Handler (hCircul, SensorCirculHandler, NULL);

CPhidget_open((CPhidgetHandle)hCircul, -1);	//open the interfacekit for device connections
printf("Waiting for interface kit to be attached....");
if(result = CPhidget_waitForAttachment((CPhidgetHandle)hCircul, 10000))
	{
	CPhidget_getErrorDescription(result, &err);
	printf("Problem waiting for attachment: %s\n", err);
	return 0;
	}

//Display the properties of the attached accelerometer device
display_circular(hCircul);

CPhidgetInterfaceKit_getSensorCount(hCircul, &numSensors);
CPhidgetInterfaceKit_setSensorChangeTrigger(hCircul, 0, 2);

printf("LOOP.....\n\n");															
return(0);
}

//Main---------------------------------------------------------------------------------------------
void circular_exit()
{
CPhidget_close((CPhidgetHandle)hCircul);
CPhidget_delete((CPhidgetHandle)hCircul);
}