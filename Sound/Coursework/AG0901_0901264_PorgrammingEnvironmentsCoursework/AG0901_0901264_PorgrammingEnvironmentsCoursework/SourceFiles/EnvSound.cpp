// File:    EnvSound.cpp
// Purpose: Implementation of environmental sound in 3D space.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    13th April 2006

#include "dsoundUtility.h"
#include "EnvSound.h"


//--- constructor method.
//--- Positions 3d sound and starts playing it.
EnvSound::EnvSound (
		Sound* soundBuff,             // 2D sound buffer.
		float x, float y, float z)    // Position of sound in 3D.
{
	theSound   = soundBuff;
	the3dSound = theSound->Get3dBuffer ();
	if (!the3dSound) return;
	the3dSound->SetPosition (x, y, z, DS3D_IMMEDIATE);
} // end EnvSound constructor method


//--- Destructor method.
EnvSound::~EnvSound () {
	if (theSound)   free (theSound);
	if (the3dSound) the3dSound->Release();
	theSound   = NULL;
	the3dSound = NULL;
} // end EnvSound destructor method

void EnvSound::SetPosition(const float x, const float y, const float z)
{
	the3dSound->SetPosition(x, y, z, DS3D_IMMEDIATE);
}

void EnvSound::Play()
{
	theSound->Play(DSBPLAY_LOOPING);
}