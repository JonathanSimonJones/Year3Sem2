// File:    FutureRaceApp.cpp
// Purpose: Runs the game
// Author:  Allan C. Milne
//			Jonathan Jones

// Includes
#include "dsoundUtility.h"
#include "EnvSound.h"
#include <Crowd.h>
#include <Ship.h>
#include <Beacon.h>
#include <list>

// Forward declerations
bool Setup();						// Plays intro  
void Cleanup();						// Clean up anything that will not be explicitly deleted
bool Process (float timeDelta);		// Where the game is run from
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//--- Globals.
//--- Program-wide resource declarations.

DSoundDevice* dsDevice     = NULL;    // DirectSound device.
Crowd *Crowd1;				// Pointer to the crowd sound object
Crowd *Crowd2;				// Pointer to the second crowd sound object
Sound *Footsteps;			// Pointer to footstep sound object
Sound *BigDoor;				// Pointer to big door sound object
Ship *Ship1;				// Pointer to the ship object
float listenerPosX = 0.0f, listenerPosY = 0.0f , listenerPosZ = 0.0f;	// X, Y, and Z of listener
std::list<Beacon*> Beacons;	// A list of pointers to all the beacons
Beacon *currentBeacon;		// A pointer to the current active beacon
Sound *ReadyGo;				// Pointer to the ReadyGo sound
Sound *Boost;				// Pointer to the boost sound
Sound *Bump;				// Pointer to the bump sound
Sound *EndTheme;			// Pointer to the end theme sound
bool GameOver = false;		// Bool representing if the game has finished

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

	/////////////////////
	// Create event sounds
	/////////////////////
	// Create bool to describe if the sounds have been created correctly
	bool ok = true;
	
	// Create Boost sound
	ok = ok && dsDevice->CreateSoundBuffer(&ReadyGo, "SoundFiles/EventSounds/ReadyGo.wav", 0 );

	// Create Boost sound
	ok = ok && dsDevice->CreateSoundBuffer(&Boost, "SoundFiles/EventSounds/Boost.wav", 0 );

	// Create Bump sound
	ok = ok && dsDevice->CreateSoundBuffer(&Bump, "SoundFiles/EventSounds/Bump.wav", 0 );

	// Create end theme sound
	ok = ok && dsDevice->CreateSoundBuffer(&EndTheme, "SoundFiles/EventSounds/EndOfRaceSound.wav", 0 );

	/////////////////////
	// End creating event sounds
	/////////////////////

	// Set the listener so that it is facing the positive x axis as the beacons are laid out along the positive x axis
	dsDevice->Get3dListener()->SetOrientation(1.0f,0.0f,0.0f,0.0f,1.0f,0.0f, DS3D_IMMEDIATE);

	////////////////////////////////////
	// Create beacons
	////////////////////////////////////

	// Create the first beacon
	Beacons.push_back(new Beacon(dsDevice, 100.0f, 0.0f, 5.0f));

	// Set the first beacon to playing
	Beacon* TempBeacon = Beacons.front();
	TempBeacon->Play();
	TempBeacon = 0;

	// Create rest of beacons
	Beacons.push_back(new Beacon(dsDevice, 200.0f, 0.0f, -5.0f));
	Beacons.push_back(new Beacon(dsDevice, 300.0f, 0.0f, 5.0f));
	Beacons.push_back(new Beacon(dsDevice, 400.0f, 0.0f, -5.0f));
	Beacons.push_back(new Beacon(dsDevice, 500.0f, 0.0f, -5.0f));
	Beacons.push_back(new Beacon(dsDevice, 600.0f, 0.0f, 5.0f));
	Beacons.push_back(new Beacon(dsDevice, 675.0f, 0.0f, -5.0f));
	Beacons.push_back(new Beacon(dsDevice, 750.0f, 0.0f, 5.0f));
	Beacons.push_back(new Beacon(dsDevice, 825.0f, 0.0f, 5.0f));
	Beacons.push_back(new Beacon(dsDevice, 875.0f, 0.0f, -5.0f));
	Beacons.push_back(new Beacon(dsDevice, 925.0f, 0.0f, -5.0f));
	Beacons.push_back(new Beacon(dsDevice, 975.0f, 0.0f, -5.0f));

	// Set the current beacon 
	currentBeacon = Beacons.front();

	////////////////////////////////////
	// End creating beacons
	////////////////////////////////////

	// Play ReadyGo
	ReadyGo->Play();

	// Wait for ReadyGo sound to finish before progressing
	while(ReadyGo->IsPlaying())
	{
	}

	// Set the ship moving event sound pos
	Ship1->SetShipMovingSoundPos(listenerPosX, listenerPosY, listenerPosZ);

	// Playing ship moving sound
	Ship1->PlayMoving();

	// Stop the crowd from playing, as ship has started now 
	Crowd1->Stop();
	Crowd2->Stop();

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
	
	//////////////////////////
	// Create crowds
	//////////////////////////
	// Create crowd 1
	Crowd1 = new Crowd(dsDevice);
	Crowd1->CreateNewSound("SoundFiles/Crowd/SmallCrowd1.wav");
	Crowd1->CreateNewSound("SoundFiles/Crowd/LargeCrowd1.wav");
	Crowd1->CreateNewSound("SoundFiles/Crowd/LargeCrowd2.wav");
	Crowd1->SetPosition(0.0f,0.0f, 17.5f);

	// Create crowd 2
	Crowd2 = new Crowd(dsDevice);
	Crowd2->CreateNewSound("SoundFiles/Crowd/SmallCrowd2.wav");
	Crowd2->CreateNewSound("SoundFiles/Crowd/LargeCrowd3.wav");
	Crowd2->CreateNewSound("SoundFiles/Crowd/LargeCrowd4.wav");
	Crowd2->SetPosition(0.0f,0.0f, -17.5f);
	
	//////////////////////////
	// End creation of crowds
	//////////////////////////
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
	Crowd1->Play();
	Crowd2->Play();
	
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
	Crowd1->SetPosition(0.0f,0.0f,75.f);
	Crowd2->SetPosition(0.0f,0.0f,-75.f);

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
	/////////////////////////
	// Delete all sounds
	/////////////////////////
	delete Footsteps;
	Footsteps = 0;

	delete BigDoor;
	BigDoor = 0;

	delete Boost;
	Boost = 0;

	delete Bump;
	Bump = 0;

	delete Crowd1;
	Crowd1 = 0;

	delete Crowd2;
	Crowd2 = 0;

	delete ReadyGo;
	ReadyGo = 0;

	delete Ship1;
	Ship1 = 0;

	delete EndTheme;
	EndTheme = 0;
}

//--- Process game
//--- Parameter is time since last call of this function.
//--- Return false if error detected.
//--- this function will be called through the windows message handling loop.
bool Process (float timeDelta)
{ 
	// Move the listener along the track
	listenerPosX += (10.0f*timeDelta);

	// Set the listner position
	dsDevice->Get3dListener()->SetPosition(listenerPosX, listenerPosY, listenerPosZ, DS3D_IMMEDIATE );

	// Set the ship moving event sound pos
	Ship1->SetShipMovingSoundPos(listenerPosX, listenerPosY, listenerPosZ);

	// If the player presses the left arrow or A key
	if(GetAsyncKeyState(VK_LEFT) & 0x0001 || GetAsyncKeyState('A') & 0x0001)
	{
		// If the listener is in range of a beacon and is on the correct side and has not already been passed(already been passeed is represented by if the beacon is playing or not)
		if( (listenerPosX > ( (currentBeacon)->GetX() - 50.0f) && listenerPosX < ( (currentBeacon)->GetX() + 50.0f) ) && ( ( (currentBeacon)->GetZ() > 0 ) && ( (currentBeacon)->isPlaying() ) ) ) 
		{
			// Play boost sound
			Boost->Play();

			// Supply boost
			listenerPosX += 15.0f;

			// Stop the beacon from playing now that it is passed
			(currentBeacon)->Stop();
		}
		else // Player made error
		{
			// Set back
			listenerPosX -= 1.0f;
			
			// Play bump sound 
			Bump->Play();
		}
	}

	// If the right arrow key or D is pressed
	if(GetAsyncKeyState(VK_RIGHT) & 0x0001 || GetAsyncKeyState('D') & 0x0001)
	{			
		// If the listener is in range of a beacon and is on the correct side and has not already been passed(already been passeed is represented by if the beacon is playing or not)
		if( (listenerPosX > ( (currentBeacon)->GetX() - 30.0f) && listenerPosX < ( (currentBeacon)->GetX() + 30.0f) ) && ( ( (currentBeacon)->GetZ() < 0 ) && ( (currentBeacon)->isPlaying() ) ) ) 
		{
			// Play boost sound
			Boost->Play();

			// Supply boost
			listenerPosX += 15.0f;

			// Stop the beacon from playing now that it is passed
			(currentBeacon)->Stop();
		}
		else	// Player made error
		{
			// Set back
			listenerPosX -= 1.0f;
			
			// Play bump sound 
			Bump->Play();
		}
	}

	// If the listener is beyond the beacon and out of the players reach
	if( listenerPosX > ( currentBeacon->GetX() + 35.0f ) )
	{
		// Set back
		listenerPosX -= 1.0f;

		// Play bump as player has missed the turning
		Bump->Play();

		// Stop the beacon from playering
		currentBeacon->Stop(); 
	}

	// If the beacon at the front of the list is off (meaning it has been passed by the player)
	if( Beacons.front()->isPlaying() == false)
	{
		// Delete the beacon as it is no longer in use
		delete Beacons.front();

		// Pop it from the list
		Beacons.pop_front();

		// If the beacon list is not empty
		if( !Beacons.empty() )
		{
			// Turn the new beacon on
			// Create a temp beacon pointer to the front beacon
			Beacon* TempBeacon = Beacons.front();

			// Set the beacon to play using the pointer
			TempBeacon->Play();

			// To stop errors set the temp beacon pointer to zero
			TempBeacon = 0;

			// Set the current beacon pointer to the new beacon
			currentBeacon = Beacons.front(); 
		}

		if(	Beacons.empty() )// If the list is empty
		{
			// Game over is true
			GameOver = true;

			// Set the current 
			currentBeacon = 0;
			 
			// Give time for other sounds to finish
			Sleep(2000);
			
			// Play end theme
			EndTheme->Play();
			
			// Wait for ending theme to finish before exiting
			while(EndTheme->IsPlaying() )
			{
			}

			// Close app
			exit(0);
		}
	}

	// Slow down process
	Sleep(500);

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
