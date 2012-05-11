/*
	Name	Crowd.cpp
	Author	Jonathan Jones
	Details	Conatins all the methods and variables that make up the crowd object
	Purpose	To create a crowd sound
*/

#include <Crowd.h>

Crowd::Crowd(DSoundDevice* DirectSoundDevice)
: DirectSoundDevice_(DirectSoundDevice)
, x_(0)
, y_(0)
, z_(0)
{
}

Crowd::~Crowd()
{
	// Release all sound files 
	while(CrowdList_.empty())
	{
		delete CrowdList_.back();

		CrowdList_.pop_back();
	}
}

void Crowd::CreateNewSound(char* NameOfFile)
{
	bool ok = true;
	Sound *SoundPointer;
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPointer, NameOfFile, DSBCAPS_CTRL3D);

	CrowdList_.push_back(new EnvSound(SoundPointer, x_, y_, z_));

	SoundPointer = 0;
}

void Crowd::SetPosition(const float x, const float y, const float z)
{
	x_ = x;
	y_ = y;
	z_ = z;

	for(std::list<EnvSound*>::iterator iter = CrowdList_.begin(); iter != CrowdList_.end(); iter++)
	{
		(*iter)->SetPosition(x_, y_, z_);
	}
}

void Crowd::Play()
{
	for(std::list<EnvSound*>::iterator iter = CrowdList_.begin(); iter != CrowdList_.end(); iter++)
	{
		(*iter)->PlayLooping();
	}
}

void Crowd::Stop()
{
	for(std::list<EnvSound*>::iterator iter = CrowdList_.begin(); iter != CrowdList_.end(); iter++)
	{
		(*iter)->StopPlaying();
	}
}