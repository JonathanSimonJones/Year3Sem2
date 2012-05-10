// File:    FutureRaceApp.cpp
// Purpose: Runs the game
// Author:  Allan C. Milne
//			Jonathan Jones

#include "dsoundUtility.h"
#include "EnvSound.h"
#include <Crowd.h>
#include <Ship.h>
#include <Console.hpp>
#include <iostream>

// Forward declerations
bool Setup();
void Cleanup();
bool Process (float timeDelta);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--- Globals.
//--- Program-wide resource declarations.

DSoundDevice* dsDevice     = NULL;    // DirectSound device.

Crowd *CrowdTest;
Crowd *CrowdTest2;
Sound *SoundPointer;
Sound *Footsteps;
Sound *BigDoor;
Ship *Ship1;

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

	//Debug::displayConsole();
	//--- set up the 'process' function as the message loop handler.
	EnterMsgLoop (Process);

	//--- do the cleaning up of resources before end of app.
	Cleanup();

	//--- clean-up DirectSound device.
	free (dsDevice);   // implicitly calss destructor method.

	return 0;
}

//--- Framework Functions

//--- Set up 
//--- Returns false if error detected.
bool Setup()
{
	//--- Set up the soundscape and start playing it.
	// Create crowd 1
	CrowdTest = new Crowd(dsDevice);
	CrowdTest->CreateNewSound("SoundFiles/Crowd/SmallCrowd1.wav");
	CrowdTest->CreateNewSound("SoundFiles/Crowd/LargeCrowd1.wav");
	CrowdTest->CreateNewSound("SoundFiles/Crowd/LargeCrowd2.wav");
	CrowdTest->SetPosition(0.0f,0.0f, 17.5f);

	// Create crowd 2
	CrowdTest2 = new Crowd(dsDevice);
	CrowdTest2->CreateNewSound("SoundFiles/Crowd/SmallCrowd2.wav");
	CrowdTest2->CreateNewSound("SoundFiles/Crowd/LargeCrowd3.wav");
	CrowdTest2->CreateNewSound("SoundFiles/Crowd/LargeCrowd4.wav");
	CrowdTest2->SetPosition(0.0f,0.0f, -17.5f);
	
	// Bool to describe if creating the sound was successful
	bool ok = true;

	// Create footstep noise 
	ok = ok && dsDevice->CreateSoundBuffer(&Footsteps, "SoundFiles/Player/FootstepsTarmac.wav", DSBCAPS_CTRLVOLUME );

	// Create big door sound
	ok = ok && dsDevice->CreateSoundBuffer(&BigDoor, "SoundFiles/EventSounds/DoorToCrowdOpening.wav", 0);

	// Create the ship
	Ship1 = new Ship(dsDevice);

	//////////////////////////////////////////////////////
	// Start beginning sequence
	//////////////////////////////////////////////////////

	BigDoor->Play();
	CrowdTest->Play();
	CrowdTest2->Play();
	
	// Wait till big door sound has finished playing
	while(BigDoor->IsPlaying() )
	{
	}

	// Start playing footsteps at a decreased volume level
	Footsteps->Play(DSBPLAY_LOOPING, -500);

	// Wait 
	Sleep(5000);

	// Stop footsteps
	Footsteps->Stop();

	// Play enter ship sequence
	Ship1->EnterShipSequence();

	// Lower the volume of the crowds so it appears that ship appears to be noise cancelling 
	CrowdTest->SetPosition(0.0f,0.0f,75.f);
	CrowdTest2->SetPosition(0.0f,0.0f,-75.f);

	// Play the ship start sound sequence
	Ship1->StartShip();

	///////////////////////////////////////////////////////
	// End beginning start sequence
	///////////////////////////////////////////////////////

	return true;
}


//--- Clean up and release the sound resources.
void Cleanup()
{
	delete Footsteps;
	Footsteps = 0;

	delete BigDoor;
	BigDoor = 0;
}

float deleteMe = 0.0f;
float tempX = 0.0f, tempY = 0.0f , tempZ = 0.0f;
//--- Do the head turning.
//--- Parameter is time since last call of this function.
//--- Return false if error detected.
//--- this function will be called through the windows message hanlding loop.
bool Process (float timeDelta)
{ 
	float tempVarJJWhy = 0.0f;
	tempVarJJWhy = timeDelta;

	if(GetAsyncKeyState(VK_UP) & 0x0001)
	{
		//tempZ += tempVarJJWhy;
		tempZ++;
		dsDevice->Get3dListener()->SetPosition(tempX, tempY, tempZ, DS3D_IMMEDIATE );
	}

	if(GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		//tempZ -= tempVarJJWhy;
		tempZ--;
		dsDevice->Get3dListener()->SetPosition(tempX, tempY, tempZ, DS3D_IMMEDIATE );
	}

	if(GetAsyncKeyState(VK_LEFT) & 0x0001)
	{
		//tempX -= tempVarJJWhy;
		tempX--;
		dsDevice->Get3dListener()->SetPosition(tempX, tempY, tempZ, DS3D_IMMEDIATE );
	}

	if(GetAsyncKeyState(VK_RIGHT) & 0x0001)
	{
		//tempX += tempVarJJWhy;
		tempX++;
		dsDevice->Get3dListener()->SetPosition(tempX, tempY, tempZ, DS3D_IMMEDIATE );
	}

	/*
	if(deleteMe < -100.0f)
	{
		timeDelta *= -1;
	}

	if(deleteMe > 100.0f)
	{
		timeDelta *= -1;
	}

	deleteMe += timeDelta;

	CrowdTest->SetPosition(0.0f, 0.0f, deleteMe);
	*/

	//std::cout << RandTempVar << std::endl;

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
