/*
	Name	Crowd.h
	Author	Jonathan Jones
	Details	Conatins all the methods and variables that make up the crowd object
	Purpose	To create a crowd sound
*/

#ifndef CROWD_H_
#define CROWD_H_

#include <dsoundUtility.h>
#include <EnvSound.h>
#include <list>

class Crowd
{
public:
	Crowd(DSoundDevice* DirectSoundDevice);
	~Crowd();
	void CreateNewSound(char* NameOfFile);
	void SetPosition(const float x, const float y, const float z);
	void Play();
private:
	DSoundDevice* DirectSoundDevice_;
	std::list<EnvSound*> CrowdList_;
	float x_, y_, z_;
};

#endif	// CROWD_H_