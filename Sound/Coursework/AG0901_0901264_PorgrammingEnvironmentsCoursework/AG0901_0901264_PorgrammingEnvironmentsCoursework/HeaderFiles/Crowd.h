/*
	Name	:	Crowd.h
	Author	:	Jonathan Jones
	Details	:	Conatins all the methods and variables that make up the crowd object
	Purpose	:	To create a crowd sound
*/

#ifndef CROWD_H_
#define CROWD_H_

// Includes 
#include <dsoundUtility.h>
#include <EnvSound.h>
#include <list>

// Class definition
class Crowd
{
public:
	Crowd(DSoundDevice* DirectSoundDevice);	// Constructor
	~Crowd();	// Deconstuctor
	void CreateNewSound(char* NameOfFile);	// Function that takes a name of a file to create a 3d sound
	void SetPosition(const float x, const float y, const float z);	// Sets the position of the crowd
	void Play();	// Play the crowd sound looping
	void Stop();	// Stop the crowd sound
private:
	DSoundDevice* DirectSoundDevice_;	// Pointer to direct sound device
	std::list<EnvSound*> CrowdList_;	// A list of all the environment sounds
	float x_, y_, z_;					// The x, y, z position in 3D space of the crowd
};

#endif	// CROWD_H_