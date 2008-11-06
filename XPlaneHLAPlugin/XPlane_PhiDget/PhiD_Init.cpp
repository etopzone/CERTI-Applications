#include <iostream>
#include <memory>
#include <string>
#include "phidget21.h"

#include "XP_Certi.h"
#include "XP_Cnt.h"

CPhidgetAccelerometerHandle	hAccel;	
CPhidgetInterfaceKitHandle		hInKit;
CPhidgetInterfaceKitHandle		hCircul;
																	
float									vCircul;							
float									vInKit[8];							
float									vAccel[3];							

int main()
{
createFedere();
PhyDgets_Init();	
getchar();
PhyDgets_Exit();
destroyFederate();
return (0);
}

//callback that will run if the Accelerometer is attached to the computer
int __stdcall AttachHandler(CPhidgetHandle IFK, void *userptr)
{
int serialNo;
CPhidget_getSerialNumber(IFK, &serialNo);
printf("PhiDgets %10d attached!\n", serialNo);
return 0;
}

//callback that will run if the Accelerometer is detached from the computer
int __stdcall DetachHandler(CPhidgetHandle IFK, void *userptr)
{
int serialNo;
CPhidget_getSerialNumber(IFK, &serialNo);
printf("PhiDgets %10d detached!\n", serialNo);
return 0;
}

//callback that will run if the Accelerometer generates an error
int __stdcall ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
printf("Error handled. %i - %s \n", ErrorCode, unknown);
return 0;
}

//Run PhiDgets-------------------------------------------------------------------------------------
void PhyDgets_Init()
{
interfaceKit_loop();
accelerometer_loop();
circular_loop();
}

void PhyDgets_Exit()
{
interfaceKit_exit();
accelerometer_exit();
circular_exit();
}