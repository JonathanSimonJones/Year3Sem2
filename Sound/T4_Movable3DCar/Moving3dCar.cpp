// File:    Moving3dCar.cpp
// Purpose: Implementation of Moving car in 3D sound space.
// Author:  Allan C. Milne
// Version: 1.1
// Date:    27th February 2006

#include "dsoundUtility.h"
#include "Moving3dCar.h"


//--- constructor method.
//--- Default position (0,0,0) moving away on the z axis, with no Doppler shift.
Moving3dCar::Moving3dCar (
		Sound* soundBuff,             // car sound buffer.
		float speedKmH)               // speed in Km/h
{
	carSound   = soundBuff;
	car3dSound = carSound->Get3dBuffer ();
	speed      = speedKmH / 3.6f;            // convert to m/sec.
	SetPosition (0.0, 0.0, 0.0);
	SetDirection (0, 0, 1);
	dopplerOn = false;
} // end Moving3dCar constructor method


//--- Destructor method.
Moving3dCar::~Moving3dCar () {
	if (carSound)   free (carSound);
	if (car3dSound) car3dSound->Release();
	carSound   = NULL;
	car3dSound = NULL;
} // end Moving3dCar destructor method


//--- Determine if car is started.
bool Moving3dCar::IsStarted () {
	if (!carSound) return false;
	return carSound->IsPlaying(); 
}  // end IsStarted method


//--- start the car engine, at current position.
void Moving3dCar::Start () {
	if (!carSound || !car3dSound) return;
	if (IsStarted()) return;
	carSound->Play (DSBPLAY_LOOPING);
} // end Start method

//--- Stop the car.
void Moving3dCar::Stop () 
{
	if (IsStarted())
		carSound->Stop();
} // end Stop method


//--- get and set current position coordinates.
D3DVECTOR Moving3dCar::GetPosition ()
{
	D3DVECTOR pos;
	if (car3dSound)
		car3dSound->GetPosition (&pos);
	return pos;
} // end GetPosition method.

void Moving3dCar::SetPosition (float x, float y, float z) 
{
	if (!car3dSound) return;
	car3dSound->SetPosition (x, y, z, DS3D_IMMEDIATE);
} // end SetPosition method.


//--- get the current speed in Km/H.
float Moving3dCar::GetSpeed ()
{	return speed * 3.6f;   }   // convert from m/s.

//--- increase/decrease the speed by supplied Km/H.
//--- velocity bector must also be updated.
void Moving3dCar::ChangeSpeed (float kmh)
{
	float newSpeed = speed + kmh / 3.6f;
	if (newSpeed < 0) newSpeed = 0.0;

	// update velocity vector.
	// if current speed was 0 then vector was unitised, so temprarily set speed to 1.
	if (speed == 0)  speed = 1.0f;
	if (newSpeed > 0) {     // rescale velocity vector.
		velocity.x = velocity.x / speed * newSpeed;
		velocity.y = velocity.y / speed * newSpeed;
		velocity.z = velocity.z / speed * newSpeed;
	} 
	else {
		// new speed is 0 so unitise velocity vector.
		UnitVector (velocity.x, velocity.y, velocity.z, &velocity);
	} 
	speed = newSpeed;

	// update the buffer's Doppler velocity if required.
	if (IsDopplerOn())
		SetDopplerOn ();    // update the buffer's velocity.
} // end changeSpeed method.


//--- get the direction  as a unit vector.
D3DVECTOR Moving3dCar::GetDirection ()
{
	D3DVECTOR direction;
	UnitVector (velocity.x, velocity.y, velocity.z, &direction);
	return direction;
} // end GetDirection method.

//--- Set the velocity vector, does not have to be a unit vector.
void Moving3dCar::SetDirection (float x, float y, float z)
{
	if (x==0 && y==0 && z==0) return;   // not a valid direction.
	UnitVector (x, y, z, &velocity);      // first unitise the vector,
	velocity.x = velocity.x * speed;      // now make the velocity vector.
	velocity.y = velocity.y * speed;
	velocity.z = velocity.z * speed;
	if (IsDopplerOn())
		SetDopplerOn ();   // resets the velocity vector on the 3D buffer.
} // end SetDirection method.


//--- move the car in a straight line.
//--- New position calculated for supplied time in the current direction at current speed.
//--- only applies if car is started.
void Moving3dCar::MoveStraight (float timeDelta)              // time to action move on.
{
	if (!carSound || !car3dSound) return;
	if (!IsStarted()) return;

	D3DVECTOR pos = GetPosition ();
	float xNew = pos.x + timeDelta * velocity.x;
	float yNew = pos.y + timeDelta * velocity.y;
	float zNew = pos.z + timeDelta * velocity.z;
	SetPosition (xNew, yNew, zNew);
} // end MoveStraight method.


//--- Doppler shift methods.
bool Moving3dCar::IsDopplerOn ()
{	return dopplerOn;   }

void Moving3dCar::SetDopplerOn ()
{
	if (!car3dSound) return;
	car3dSound->SetVelocity (velocity.x, velocity.y, velocity.z, DS3D_IMMEDIATE);
	dopplerOn = true;
}
void Moving3dCar::SetDopplerOff ()
{
	if (!car3dSound) return;
	car3dSound->SetVelocity (0.0, 0.0, 0.0, DS3D_IMMEDIATE);
	dopplerOn = false;
}


//--- get & set the minimum distance for distance attenuation.
float Moving3dCar::GetMinDistance ()
{	
	if (!car3dSound) return 0.0;
	float min;
	car3dSound->GetMinDistance (&min);
	return min;
}
void Moving3dCar::SetMinDistance (float min) 
{
	if (!car3dSound) return;
	if (min < 0) min = 0.0;
	car3dSound->SetMinDistance (min, DS3D_IMMEDIATE);
}


