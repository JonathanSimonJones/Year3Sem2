// File:    MoveableCar.cpp
// Purpose: Implementation of moveable car in 2D sound space.
// Author:  Allan C. Milne
// Version: 2.0
// Date:    22 January 2006
//
// In this implementation a linear pan & volume change
// is used to indicate left/right and away/towards.

#include "dsoundUtility.h"
#include "MoveableCar.h"

// constructor method - always start object at front centre.
MoveableCar::MoveableCar (Sound* soundBuff, int mx, int my) {
	// set up stereo sound space.
	maxX = mx;  maxY = my;
	x = 0;	// centre.
	y = 0;	// front.
	// pan & volume increments - assumes equal left/right split
	// provides linear changes which are not perfect!
	panInc    =   DSBPAN_RIGHT / maxX;
	volumeInc = DSBVOLUME_MIN / maxY;

	// Save sound buffer buffer and start the car.
	carSound = soundBuff;
	Start();
} // end MoveableCar constructor method

// Destructor method.
MoveableCar::~MoveableCar () {
	if (carSound) free (carSound);
	carSound = NULL;
} // end MoveableCar destructor method


// Determine if car is started.
bool MoveableCar::IsStarted () {
	if (!carSound) return false;
	return carSound->IsPlaying(); 
}  // end IsStarted method

// start the car engine, at current car position.
void MoveableCar::Start () {
	if (!carSound) return;
	if (IsStarted()) return;
	carSound->Play (DSBPLAY_LOOPING);
} // end Start method

// Stop the car, remain in current position.
void MoveableCar::Stop () {
	if (IsStarted())
		carSound->Stop();
} // end Stop method

// move the object and change the sound position.
// Only apply if the car is started.

void MoveableCar::MoveLeft() {
	if (!IsStarted()) return;
	if (x > (-maxX)) {
		x--;
		carSound->ChangePan (-panInc);
	}
} // end MoveLeft method

void MoveableCar::MoveRight() {
	if (!IsStarted()) return;
	if (x < maxX) {
		x++;
		carSound->ChangePan (panInc);
	}
} // end MoveRight method

void MoveableCar::MoveCloser() {
	if (!IsStarted()) return;
	if (y > 0) {
		y--;
		carSound->ChangeVolume (-volumeInc);
	}
} // end MoveCloser method

void MoveableCar::MoveAway() {
	if (!IsStarted()) return;
	if (y < maxY) {
		y++;
		carSound->ChangeVolume (volumeInc);
	}
} // end MoveAway method


// get the current X & Y positions of the object

int MoveableCar::GetX() {
	return x;
} // end GetX method

int MoveableCar::GetY() {
	return y;
} // end GetY method
