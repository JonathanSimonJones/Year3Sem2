// File:    FutureRaceApp.cpp
// Purpose: Runs the game
// Author:  Allan C. Milne
//			Jonathan Jones

#include "dsoundUtility.h"
#include "EnvSound.h"
#include <Crowd.h>

// Forward declerations
bool Setup();
void Cleanup();
bool Process (float timeDelta);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--- Globals.
//--- Program-wide resource declarations.

// The number of environmental sounds making up the soundscape.
#define ENVSOUNDS 3

DSoundDevice* dsDevice     = NULL;    // DirectSound device.
EnvSound*     soundscape[ENVSOUNDS];  // The environment sound buffers.

Crowd * CrowdTest;

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
		"Future Race Application",  
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

	//soundscape[0]->Play();

	CrowdTest = new Crowd(dsDevice);
	CrowdTest->CreateNewSound("SoundFiles/Ship/ShipMoving.wav");
	CrowdTest->CreateNewSound("SoundFiles/Crowd/LargeCrowd1.wav");
	CrowdTest->Play();

	return true;
}


//--- Clean up and release the sound resources.
void Cleanup()
{
	for (int i=0; i<ENVSOUNDS; i++)
	{
		if (soundscape[i]) 
		{
			free (soundscape[i]);
		}
	}
}

float deleteMe = 0.0f;
//--- Do the head turning.
//--- Parameter is time since last call of this function.
//--- Return false if error detected.
//--- this function will be called through the windows message hanlding loop.
bool Process (float timeDelta)
{ 
	CrowdTest->SetPosition(0.0f, 0.0f, deleteMe+=0.1f);
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
