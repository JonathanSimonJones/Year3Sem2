/*
	Name	:	Crowd.h
	Author	:	Jonathan Jones
	Details	:	Conatins all the methods and variables that make up the crowd object
	Purpose	:	To create a crowd sound
*/

// Includes
#include <Crowd.h>

// Constructor
// Set initial variables to zero
// Sets direct sound point to the direct sound pointer
Crowd::Crowd(DSoundDevice* DirectSoundDevice)
: DirectSoundDevice_(DirectSoundDevice)
, x_(0)
, y_(0)
, z_(0)
{
}

// Deconstuctor
// Deletes all the sounds
Crowd::~Crowd()
{
	// Release all sound files
	// Check to see if the crowd list is empty, if it isn't enter body
	while(CrowdList_.empty())
	{
		// Delete the back object
		delete CrowdList_.back();

		// Pop the back oject
		CrowdList_.pop_back();
	}
}

// Function that takes a name of a file to create a 3d sound
void Crowd::CreateNewSound(char* NameOfFile)
{
	// Create a variable to describe if sound buffer is created corretly
	bool ok = true;

	// Create a temporary sound pointer
	Sound *SoundPointer;

	// Create the new sound to check to see if it was created correctly
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPointer, NameOfFile, DSBCAPS_CTRL3D);

	// Create a new 3d sound and add it to the list
	CrowdList_.push_back(new EnvSound(SoundPointer, x_, y_, z_));

	// Set the sound pointer to zero to clean up
	SoundPointer = 0;
}

// Sets the position of the crowd
// Takes const values for x, y and z so that they cannot be changed accidently
void Crowd::SetPosition(const float x, const float y, const float z)
{
	// Set the x, y and z position with passed variables
	x_ = x;
	y_ = y;
	z_ = z;

	// Loop through all the sounds in the list and set there new position
	// For all items in the list
	for(std::list<EnvSound*>::iterator iter = CrowdList_.begin(); iter != CrowdList_.end(); iter++)
	{
		// Set the new position
		(*iter)->SetPosition(x_, y_, z_);
	}
}

// Play the crowd sound looping
void Crowd::Play()
{
	// Set all the sounds in the list to play continuously
	for(std::list<EnvSound*>::iterator iter = CrowdList_.begin(); iter != CrowdList_.end(); iter++)
	{
		(*iter)->PlayLooping();
	}
}

// Stop the crowd sound
void Crowd::Stop()
{
	// Sets all the sounds in the list to stop playing
	for(std::list<EnvSound*>::iterator iter = CrowdList_.begin(); iter != CrowdList_.end(); iter++)
	{
		(*iter)->StopPlaying();
	}
}