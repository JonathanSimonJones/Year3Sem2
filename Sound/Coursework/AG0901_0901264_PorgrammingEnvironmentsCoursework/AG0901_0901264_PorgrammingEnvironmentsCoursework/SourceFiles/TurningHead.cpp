// File:    TurningHead.cpp
// Purpose: Class implementation of a turnable listener head in DirectSound 3D.
//          The listener is positioned at the origin and initially faces North.
//          The head turns in increments of 45 degrees with spoken feedback on the direction.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    24 April 2006

#include "TurningHead.h"


//--- constructor method.
//--- Get listner interface and set up initial compass bearing.
TurningHead::TurningHead (DSoundDevice* device)
{
	listener = device->Get3dListener();
	compass = new Compass (device, 8);
} // end TurningHead constructor method.

TurningHead::~TurningHead ()
{
	if (listener) listener->Release();
	free (compass);  
} // end ~TalkingHead destructor method.

//--- Turn the listener head anti-clockwise by 45 degrees.
void TurningHead::TurnLeft () 
{
	D3DVECTOR front, top;
	listener->GetOrientation (&front, &top);
	compass->ChangeBearing (-1);
	compass->GetOrientation (&front);
	listener-> SetOrientation (front.x, front.y, front.z, 
			top.x, top.y, top.z, DS3D_IMMEDIATE);
	compass->SpeakBearing();
} // end turnLeft method.

//--- Turn the listener head clockwise 45 degrees.
void TurningHead::TurnRight ()
{
	D3DVECTOR front, top;
	listener->GetOrientation (&front, &top);
	compass->ChangeBearing (1);
	compass->GetOrientation (&front);
	listener-> SetOrientation (front.x, front.y, front.z, 
			top.x, top.y, top.z, DS3D_IMMEDIATE);
	compass->SpeakBearing();
} // end TurnRight method.

//--- Speak the current direction of the listener head.
void TurningHead::SayDirection ()
{
	compass->SpeakBearing();
} // end SayDirection method.
