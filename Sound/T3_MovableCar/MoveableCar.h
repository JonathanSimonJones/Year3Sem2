// File:    MoveableCar.h
// Purpose: Class specification for a car object which can
//          ve moved in a 2-D stereo sound space
// Author:  Allan C. Milne
// Version: 2.0
// Date:    22 January 2006

#include "dsoundUtility.h"


class MoveableCar {

private:
	// defines the stereo sound space
	int maxX;		// max distance to left or right.
	int maxY;		// max distance away in front.

	// pan and volume change increments
	LONG panInc, volumeInc;

	Sound* carSound;		// sound of car engine.

	int x, y;		// current position in sound space

public:
	// constructor method - always start close centre .
	// nx and ny are # of increments to use in X and & space.
	MoveableCar (Sound* soundBuff, int mx, int my);

	// destructor method.
	~MoveableCar ();

	// Start & stop the car in current position.
	bool IsStarted ();
	void Start ();
	void Stop ();

	// move the object & change the sound position.
	// only applies if car is started.
	void MoveLeft();
	void MoveRight();
	void MoveCloser();
	void MoveAway();

	// get the current X & Y positions of the object
	int GetX();
	int GetY();

}; // end MoveableCar class
