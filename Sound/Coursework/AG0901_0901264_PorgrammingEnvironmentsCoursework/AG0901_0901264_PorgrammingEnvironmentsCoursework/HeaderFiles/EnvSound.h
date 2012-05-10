// File:    EnvSound.h
// Purpose: Class specification for an environmental sound placed in the 3D world.
//          It is instantiated at a specific point and immediately plays in looping mode.
// Author:  Allan C. Milne
//			Jonathan Jones
// Version: 1.0
// Date:    13 April 2006

#ifndef ENV_SOUND_H_
#define ENV_SOUND_H_

#include "dsoundUtility.h"

class EnvSound {

private:
	Sound* theSound;                    // 2D sound buffer to be played.
	LPDIRECTSOUND3DBUFFER the3dSound;   // 3D interface to the above.

public:
	// constructor method.
	// Positions 3d sound and starts playing it.
	EnvSound (
		Sound* soundBuff,             // 2D sound buffer.
		float x, float y, float z);   // Position of sound in 3D.
	~EnvSound ();

	void SetPosition(const float x, const float y, const float z);
	void PlayLooping();
	void PlayOnce();
	void StopPlaying();
	bool isPlaying();
	void changePan(int Pan);
}; // end EnvSound class.

#endif // ENV_SOUND_H_
