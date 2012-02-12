// File:    Playsound.cpp
// Purpose: Windows app allowing a sound to be played and stopped.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    16 January 2006
// Requires: dsoundUtility.h, dsoundUtility.cpp
//
// Based on MinimalDxProject.

#include "dsoundUtility.h"
#include <vector>

//--- Globals.
DSoundDevice* dsDevice = NULL;   // DirectSound device.
Sound* sound = NULL;             // a single sound sample.
//Sound* sound1 = NULL;
std::vector<Sound*> SoundContainer;

//--- Set up application specific resources.
//--- Return false if error detected.
bool Setup()
{
	bool result;
	SoundContainer.push_back(sound);
	result = dsDevice->CreateSoundBuffer (&SoundContainer[0], "pres4ya.wav", 0);

	SoundContainer.push_back(sound);	
	result = dsDevice->CreateSoundBuffer (&SoundContainer[1], "I got an itch.wav", 0);
	return result;
}

//--- Clean up and release any application specific resources.
void Cleanup()
{
	for(int i = 0; i < SoundContainer.size(); i++)
	{
		free (SoundContainer[i]);
	}
}

//--- Pressing space plays and stops the sound (if it is currentlyplaying).
//--- Parameter is time since last call of this function.
//--- Return false if error detected.
bool Process (float timeDelta)
{
	if( GetAsyncKeyState ('1') & 0x0001 )
	{
		for(int i = 0; i < SoundContainer.size(); i++)
		{
			if( !(i == 0) )
			{
				if(SoundContainer[i]->ReturnActive() == true)
				{
					//SoundContainer[i]->Stop();
					//SoundContainer[i]->Active( false );
				}
			}
		}

		if( SoundContainer[0]->ReturnActive() == false )
		{
			SoundContainer[0]->Active( true );
		}
		else if( SoundContainer[0]->ReturnActive() == true )
		{
			SoundContainer[0]->Active( false );
		}
	}

	if( GetAsyncKeyState ('2') & 0x0001 )
	{
		for(int i = 0; i < SoundContainer.size(); i++)
		{
			if( !(i == 1) )
			{
				if(SoundContainer[i]->ReturnActive() == true)
				{
					//SoundContainer[i]->Stop();
					//SoundContainer[i]->Active( false );
				}
			}
		}

		if( SoundContainer[1]->ReturnActive() == false )
		{
			SoundContainer[1]->Active( true );
		}
		else if( SoundContainer[1]->ReturnActive() == true )
		{
			SoundContainer[1]->Active( false );
		}
	}

	// Check if keys were pressed since last key state call 
	// - least significant bit.
	if (GetAsyncKeyState (VK_SPACE) & 0x0001 ) 
	{
		for(int i = 0; i < SoundContainer.size(); i++)
		{
			if(SoundContainer[i]->ReturnActive() == true)
			{
				if (SoundContainer[i]->IsPlaying())
				{
					SoundContainer[i]->Stop();
				}
				else 
				{
					SoundContainer[i]->Play ();
				}
			}
		}
	}

	// When one sound sample is selected then stop any other sample that is currently playing. 
	return true;
}

//--- WndProc
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

//--- WinMain
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	HWND hwnd;

	// Create window
	if (!InitWindow ("PlaySound.cpp", hinstance, 800, 600, false, &hwnd))
	{
		MessageBox(0, "InitWindow() - FAILED", 0, 0);
		return 0;
	}

	//--- Initialise DirecSound.
	dsDevice = new DSoundDevice();
	if (!dsDevice->Initialize (hwnd, DSSCL_NORMAL))
	{
		MessageBox(0, "DSoundDevice::Initialize() - FAILED", 0, 0);
		return 0;
	}

	// Create sound
	if(!Setup())
	{
		MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	// Check for windows messages 
	EnterMsgLoop (Process);

	// Release the sound
	Cleanup();

	// Free up the device 
	free (dsDevice);

	return 0;
}