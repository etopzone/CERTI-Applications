#ifndef	XP_CNT_H
#define	XP_CNT_H

extern	CPhidgetAccelerometerHandle	hAccel;				//Handle Accéléro
extern	CPhidgetInterfaceKitHandle		hInKit;				//Handle Réglette+Distance
extern	CPhidgetInterfaceKitHandle		hCircul;				//Handle Accéléro
																	
extern	float									vCircul;				//Rotation {0..359}
extern	float									vInKit[8];			//[0] Réglette	{0..100} [1] Distance					
extern	float									vAccel[3];			//Accélérateur

//General
void				PhyDgets_Init();
void				PhyDgets_Exit();

int __stdcall	AttachHandler(CPhidgetHandle IFK, void *userptr);
int __stdcall	DetachHandler(CPhidgetHandle IFK, void *userptr);
int __stdcall	ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown);

//Multi
int __stdcall	InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
int __stdcall	OutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
int __stdcall	SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value);
int				display_multi(CPhidgetInterfaceKitHandle phid);
int				interfaceKit_loop();
void				interfaceKit_exit();

//Circular
int __stdcall	InputCirculHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
int __stdcall	SensorCirculHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value);
int				display_circular(CPhidgetInterfaceKitHandle phid);
int				circular_loop();
void				circular_exit();

//Accelero
int __stdcall	AccelChangeHandler(CPhidgetAccelerometerHandle WGT, void *userptr, int Index, double Value);
int				display_accel(CPhidgetHandle phid);
int				accelerometer_loop();
void				accelerometer_exit();
#endif 