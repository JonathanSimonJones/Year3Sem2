// File:    TurningHead.h
// Purpose: Class specification for a listener head that can turn.
//          The listener is positioned at the origin and initially faces North (z=0).
//          The head turns in 45 degree increments.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    16 April 2006

#include "dsoundUtility.h"
#include "Compass.h"


class TurningHead {

private:
	LPDIRECTSOUND3DLISTENER8 listener;	// The 3D listener.

	Compass* compass;			// Maintains current bearing.

public:
	// constructor method.
	TurningHead (DSoundDevice* device);
	~TurningHead ();

	void TurnLeft ();
	void TurnRight ();

	void SayDirection ();

}; // end TurningHead class.
