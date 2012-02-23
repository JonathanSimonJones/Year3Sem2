// File:    Moving3dCarApp.cpp
// Purpose: Windows app demonstrating a car moving audibally in 3D space.
// Version: 1.1
// Author:  Allan C. Milne
// Date:    27th February 2006
// 
// Requires: Moving3dCar.h, Moving3dCar.cpp, dsoundUtility.h, dsoundUtility.cpp
//
// Description.
// the car moves in a straight line from left to right at a fixed speed (initially 30Km/H)
// and from 60m  on the left to 60m on the right, 1m in front.
// controls allow comparison of different settings.
//
// Controls are single key presses as follows
// Space      : start / stop the car                      (initially stopped).
// D          : toglle Doppler shift on/off               (initially off).
// Up/Dow     : increase/decrease speed by 10Km/H         (initially 30Km/H).
// Left/right : increase/decrease minimum distance by 2m  (initially 0).

#include "dsoundUtility.h"
#include "Moving3dCar.h"
#include <math.h>
#define M_PI       3.14159265358979323846
//--- Globals.

DSoundDevice* dsDevice = NULL;   // DirectSound device.
Moving3dCar*  car;        // A car object to move around.


//--- Set up application specific resources.
//--- Return false if error detected.
bool Setup()
{
	Sound* carSound;
	if (!dsDevice->CreateSoundBuffer (&carSound, "Ambulence2.wav", DSBCAPS_CTRL3D))
	   return false;

	// create a 3D car.
	// speed is initially 30Km/H.
	car = new Moving3dCar (carSound, 30.0);
	car->SetDirection (1.0, 0.0, 0.0);      // moving from left to right.
	return true;
}

//--- Clean up and release any application specific resources.
void Cleanup()
{
	if (car) 
	{
		free (car);
	}
}

//--- Handle user controls and car movement.
//--- Parameter is time since last call of this function.
bool Process (float timeDelta)
{
	// Check if keys were pressed since last key state call - least significant bit.
	if (GetAsyncKeyState (VK_SPACE) & 0x0001 ) {
		// toggle car start / stop.
		if (car->IsStarted())
		{
		 	car->Stop();
		}
		else 
		{
			car->SetPosition (-60.0, 0.0, 1.0);   // 60m on right, 1m in front.
			car->Start();
		}
	}
	// check for the 'D' key.
	if (GetAsyncKeyState (44) & 0x0001 ) {
		// Toggle Doppler on/off.
		if (car->IsDopplerOn())
		     car->SetDopplerOff ();
		else car->SetDopplerOn ();
	}
	if (GetAsyncKeyState (VK_UP) & 0x0001 ) 
		car->ChangeSpeed (10.0);
	if (GetAsyncKeyState (VK_DOWN) & 0x0001 ) 
		car->ChangeSpeed (-10.0);
	if (GetAsyncKeyState (VK_LEFT) & 0x0001 ) {
		float min = car->GetMinDistance ();
		car->SetMinDistance (min-2);
	}
	if (GetAsyncKeyState (VK_RIGHT) & 0x0001 ) {
		float min = car->GetMinDistance ();
		car->SetMinDistance (min+2);
	}

	// Move the car on.
	if (car->IsStarted()) {
		D3DVECTOR pos = car->GetPosition();
		if (pos.x > 60.0)
			car->Stop();     // end of car's run.
		else	car->MoveStraight (timeDelta);
	}
	return true;
}


//--- The Windows message handler.
//--- Escape key terminates application.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			DestroyWindow(hwnd);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//--- Application entry point.
//--- calls the framework functions.
//--- Sets up Directsound.
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	HWND hwnd;   // handle to window  created.

	float a = 0;
	while( a < 360)
	{
		float radVal = a * ( M_PI / 180);
		float check = sinf(radVal);
		a++;
		int x  = 0;
	}
	if (!InitWindow (
		"Allan Milne's Moving Car In A Straight Line",  // window title text.
		hinstance,
		800, 600, true, 
		&hwnd))       // returns window handle.
	{
		MessageBox(0, "InitWindow() - FAILED", 0, 0);
		return 0;
	}

	//--- Initialise DirecSound.
	dsDevice = new DSoundDevice();
	if (!dsDevice->Initialize (hwnd, DSSCL_PRIORITY))
	{
		MessageBox(0, "DSoundDevice::Initialize() - FAILED", 0, 0);
		return 0;
	}

	//--- call the setup function and test if it worked.
	if(!Setup())
	{
		MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	//--- set up the 'process' function as the message loop handler.
	EnterMsgLoop (Process);

	//--- do any cleaning up of resources before end of app.
	Cleanup();

	//--- clean-up DirectSound device.
	free (dsDevice);   // implicitly calss destructor method.

	return 0;
}