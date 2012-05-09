// File:    TurningHeadApp.cpp
// Purpose: Turns a listner within a soundscape.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    24 April 2006
// 
// Requires: DSoundUtility.h, EnvSound.h, TurningHead.h
//
// Environmental sound files required in the folder 'SoundFiles':
//	woop.wav, car.wav, jungle.wav


#include "dsoundUtility.h"
#include "EnvSound.h"
#include "TurningHead.h"


//--- Globals.
//--- Program-wide resource declarations.

// The number of environmental sounds making up the soundscape.
#define ENVSOUNDS 3

DSoundDevice* dsDevice     = NULL;    // DirectSound device.
EnvSound*     soundscape[ENVSOUNDS];  // The environment sound buffers.
TurningHead*  head         = NULL;    // A listening head that can be turned.


//--- Framework Functions

//--- Set up soundscape 3D buffers and the turning head.
//--- Returns false if error detected.
bool Setup()
{
	//--- Sset up the soundscape and start playing it.
	bool ok = true;
	Sound* theSounds[ENVSOUNDS];
	ok = ok && dsDevice->CreateSoundBuffer (&(theSounds[0]), "SoundFiles/EventSounds/EndOfRaceSound.wav", DSBCAPS_CTRL3D);
	//ok = ok && dsDevice->CreateSoundBuffer (&(theSounds[1]), "SoundFiles\\car.wav", DSBCAPS_CTRL3D);
	//ok = ok && dsDevice->CreateSoundBuffer (&(theSounds[2]), "SoundFiles\\jungle.wav", DSBCAPS_CTRL3D);
	if (!ok) return false;            // sound buffer creation failed.
	soundscape[0] = new EnvSound (theSounds[0], 0, 0, 0.0);
	//soundscape[1] = new EnvSound (theSounds[1], 5, 0, 5);
	//soundscape[2] = new EnvSound (theSounds[2], -2, 0, 0);

	head = new TurningHead (dsDevice);

	return true;
}


//--- Clean up and release the sound resources.
void Cleanup()
{
	for (int i=0; i<ENVSOUNDS; i++)
		if (soundscape[i]) free (soundscape[i]);
	if (head) free (head);
}


//--- Do the head turning.
//--- Parameter is time since last call of this function.
//--- Return false if error detected.
//--- this function will be called through the windows message hanlding loop.
bool Process (float timeDelta)
{
	// Check if keys were pressed since last key state call - least significant bit.
	if (GetAsyncKeyState (VK_SPACE) & 0x0001 ) 
	head->SayDirection();
	if (GetAsyncKeyState (VK_LEFT) & 0x0001 ) 
		head->TurnLeft();
	if (GetAsyncKeyState (VK_RIGHT) & 0x0001 ) 
		head->TurnRight();

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
		"TurningHead DirectSound Listener Application",  
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

	//--- do the cleaning up of resources before end of app.
	Cleanup();

	//--- clean-up DirectSound device.
	free (dsDevice);   // implicitly calss destructor method.

	return 0;
}