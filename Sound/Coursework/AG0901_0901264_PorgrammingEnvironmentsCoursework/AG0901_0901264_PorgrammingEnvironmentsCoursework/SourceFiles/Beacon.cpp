/*
	Name	Beacon.cpp
	Author	Jonathan Jones
	Details	Contains all variables and funtions for a beacon  
*/

#include <Beacon.h>
#include <EnvSound.h>

Beacon::Beacon(DSoundDevice* DirectSoundDevice, const float x, const float y, const float z)
{
	Sound* SoundPointer;

	bool ok = true;
	ok = ok && DirectSoundDevice->CreateSoundBuffer(&SoundPointer, "SoundFiles/Beacon/Beacon.wav", DSBCAPS_CTRL3D);

	Beacon_ = new EnvSound(SoundPointer, x, y, z);

	SoundPointer = 0;
}

Beacon::~Beacon()
{
	delete Beacon_;
	Beacon_ = 0;
}

float Beacon::GetX()
{
	return Beacon_->getX();
}

float Beacon::GetZ()
{
	return Beacon_->getZ();
}

void Beacon::Stop()
{
	Beacon_->StopPlaying();
}

bool Beacon::isPlaying()
{
	return Beacon_->isPlaying();
}

void Beacon::Play()
{
	Beacon_->PlayLooping();
}