#include <iostream>
#include <memory>
#include <string>
#include "phidget21.h"

#include "XP_Certi.h"
#include "XP_Cnt.h"

//callback that will run if the acceleration changes by more than the Acceleration trigger.
//Index - Index of the axis that is generating the event, Value - the value read by the accelerometer axis
int __stdcall AccelChangeHandler(CPhidgetAccelerometerHandle WGT, void *userptr, int Index, double Value)
{
vAccel[Index]= (vAccel[Index] * 0.90) + (Value * 0.10);
printf("Accel[%i] -- %6f\n", Index, vAccel[Index]); 
return 0;
}

//Display the properties of the attached phidget to the screen.  
//We will be displaying the name, serial number, version of the attached device and number of Axes on the accelerometer.
int display_accel(CPhidgetHandle phid)
{
int serialNo, version;
const char* ptr;
int numAxes;

CPhidget_getDeviceType(phid, &ptr);
CPhidget_getSerialNumber(phid, &serialNo);
CPhidget_getDeviceVersion(phid, &version);
CPhidgetAccelerometer_getAxisCount((CPhidgetAccelerometerHandle)phid, &numAxes);

printf("%s\n", ptr);
printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
printf("Number of Axes: %i\n", numAxes);
return 0;
}

//Main---------------------------------------------------------------------------------------------
int accelerometer_loop()
{
int result, numAxes;
const char *err;

hAccel = 0;																								//Handle
CPhidgetAccelerometer_create(&hAccel);															//Create the accelerometer object

CPhidget_set_OnAttach_Handler((CPhidgetHandle)hAccel,	AttachHandler, NULL);		//CallBack Contrôle
CPhidget_set_OnDetach_Handler((CPhidgetHandle)hAccel,	DetachHandler, NULL);
CPhidget_set_OnError_Handler((CPhidgetHandle)hAccel,	ErrorHandler, NULL);								
CPhidgetAccelerometer_set_OnAccelerationChange_Handler(hAccel, AccelChangeHandler, NULL);	//CallBack Test

CPhidget_open((CPhidgetHandle)hAccel, -1);												//open the acclerometer for device connections
printf("Waiting for accelerometer to be attached.... \n");							//wait for an accelerometer device to be attached
if(result = CPhidget_waitForAttachment((CPhidgetHandle)hAccel, 10000))
	{
	CPhidget_getErrorDescription(result, &err);
	printf("Problem waiting for attachment: %s\n", err);
	return 0;
	}

display_accel((CPhidgetHandle)hAccel);														//Display the properties of the attached accelerometer device

CPhidgetAccelerometer_getAxisCount(hAccel, &numAxes);									//Get the number of available axes on the attached accelerometer
for (short i= 0; i < numAxes; i++)
	CPhidgetAccelerometer_setAccelerationChangeTrigger(hAccel, i, 0.500);		//Pre-set their sensitivity to make the data more readable

printf("LOOP.....\n\n");																			//Read accelerometer event data
return 0;
}

//Main---------------------------------------------------------------------------------------------
void accelerometer_exit()
{
CPhidget_close((CPhidgetHandle)hAccel);
CPhidget_delete((CPhidgetHandle)hAccel);
}
