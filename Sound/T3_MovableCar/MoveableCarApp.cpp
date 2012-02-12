// File:    MoveableCarApp.cpp
// Purpose: Windows app allowing a car to be moved audibally.
// Version: 1.0
// Author:  Allan C. Milne
// Date:    22 January 2006
// 
// Requires: MoveableCar.h, MoveableCar.cpp, dsoundUtility.h, dsoundUtility.cpp


#include "dsoundUtility.h"
#include "MoveableCar.h"

//--- Globals.

DSoundDevice* dsDevice = NULL;   // DirectSound device.
MoveableCar         *car;        // A car object to move around.
MoveableCar         *car2;        // A car object to move around.
Sound* backGround;


//--- Set up application specific resources.
//--- Return false if error detected.
bool Setup()
{
	Sound* carSound;
	if (!dsDevice->CreateSoundBuffer (&carSound, "car.wav",
		DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN))
	{
	   return false;
	}

	Sound* carSound2;
	if (!dsDevice->CreateSoundBuffer (&carSound2, "SecondEngineSound.wav",
		DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN))
	{
	   return false;
	}

	car = new MoveableCar (carSound, 100, 100);

	car2 = new MoveableCar (carSound2, 100, 100);
	
	if (!dsDevice->CreateSoundBuffer (&backGround, "Heroes and Villians 22100 16bit.wav",
		DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN))
	{
	   return false;
	}

	//backGround->Play(DSBPLAY_LOOPING);

	return true;
}

//--- Clean up and release any application specific resources.
void Cleanup()
{
	if (car) 
	{
		free (car);
	}

	if (backGround) 
	{
		free (backGround);
	}
}

//--- Control the car by pressing space and arrow keys.
//--- Parameter is time since last call of this function.
//--- Return false if error detected.
//--- this function will be called through the windows message hanlding loop.
bool Process (float timeDelta)
{
	// Check if keys were pressed since last key state call - least significant bit.
	if (GetAsyncKeyState (VK_SPACE) & 0x0001 ) {
		// toggle car start / stop.
		if (car->IsStarted())
		     car->Stop();
		else car->Start();
	}
	// First car 
	if (GetAsyncKeyState (VK_LEFT) & 0x0001)  car->MoveLeft();
	if (GetAsyncKeyState (VK_RIGHT) & 0x0001) car->MoveRight();
	if (GetAsyncKeyState (VK_UP) & 0x0001)    car->MoveAway();
	if (GetAsyncKeyState (VK_DOWN) & 0x0001)  car->MoveCloser();

	// Second car
	if (GetAsyncKeyState ('A') & 0x0001)  car2->MoveLeft();
	if (GetAsyncKeyState ('D') & 0x0001)  car2->MoveRight();
	if (GetAsyncKeyState ('W') & 0x0001)  car2->MoveAway();
	if (GetAsyncKeyState ('S') & 0x0001)  car2->MoveCloser();
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

	if (!InitWindow (
		"Moveable Car",  // window title text.
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