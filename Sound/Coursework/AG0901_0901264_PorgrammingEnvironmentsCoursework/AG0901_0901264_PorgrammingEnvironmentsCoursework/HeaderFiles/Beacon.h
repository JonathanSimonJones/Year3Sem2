/*
	Name	Beacon.cpp
	Author	Jonathan Jones
	Details	Contains all variables and funtions for a beacon  
*/

#ifndef BEACON_H_
#define BEACON_H_

#include <dsoundUtility.h>

class EnvSound;

class Beacon
{
public:
	Beacon(DSoundDevice* DirectSoundDevice, const float x, const float y, const float z);
	~Beacon();

	float GetX();
	float GetZ();
	void Stop();
	bool isPlaying();
	void Play();
private:
	EnvSound* Beacon_;
};

#endif	// BEACON_H_