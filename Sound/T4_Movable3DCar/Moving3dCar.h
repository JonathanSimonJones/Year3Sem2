// File:    Moving3dCar.h
// Purpose: Class specification for a car object which can
//          ve moved in a 3D sound space using DirectSound 3D.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    15th February 2006

#include "dsoundUtility.h"


class Moving3dCar {

private:
	Sound* carSound;		    // car engine as a 2D buffer.
	LPDIRECTSOUND3DBUFFER car3dSound;   // 3D interface to the above.
	float speed;                       // speed of the car in m/sec.
	D3DVECTOR velocity;                // vector velocity of travel in m/sec.
	bool dopplerOn;                    // true if Doppler shift velocity is set.

public:
	// constructor method.
	// Default position (0,0,0) moving away on the z axis, with no doppler shift.
	Moving3dCar (
		Sound* soundBuff,             // car sound buffer.
		float speedKmH);              // speed in Km/h
	~Moving3dCar ();

	// Start & stop the car, does not change position.
	bool IsStarted ();
	void Start ();
	void Stop ();

	// get and set current position coordinates.
	D3DVECTOR GetPosition ();
	void SetPosition (float x, float y, float z);

	// get and set speed - all params in Km/H.
	float GetSpeed ();
	void ChangeSpeed (float kmh);   // amount to change speed by.

	// get and set the direction vector.
	D3DVECTOR GetDirection ();                       // as a unit vector.
	void SetDirection (float x, float y, float z);   // needd not be a unit vector.

	// move the car in a straight line.
	// New position calculated for supplied time in the current direction at current speed.
	// only applies if car is started.
	void MoveStraight (float timeDelta);              // time to action move on.

	// Doppler shift methods.
	bool IsDopplerOn ();
	void SetDopplerOn ();
	void SetDopplerOff ();

	// get ans set the minimum distance for distance attenuation.
	float GetMinDistance ();
	void SetMinDistance (float min);

}; // end Moving3dCar class
