// File:    Compass.cpp
// Purpose: Class implementation of a compass with discrete bearings
//          and speech output of the bearing.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    25 April 2006
//
// This class represents a compass with discrete bearings;
// e.g. North, North-East, etc.
// Instantiated with the number of discrete bearing points (4 or 8), 
// assumes to initially face North.
// It maintains the current bearing.
// Methods are available to convert from DirectSound D3DVECTOR orientation vectors;
// this is done in the x-z plane with z=0 representing North.
//
// The bearing can be spoken by playing relevant wave files;
// these wave files should be in the folder 'SoundFiles'.
// requires north.wav, northeast.wav, east.wav, southeast.wav,
//          south.wav, southwest.wav, west.wav, northwest.wav.


#include "Compass.h"

#define PI 3.141592

//--- constructor method.
//--- sets up initial bearing and loads sounds.
//--- If invalid number of bearings defaults to 4.
Compass::Compass (
	DSoundDevice* device,	// the DirectSound device object.
	int numPoints)		// number of compass points.
{
	bearing = 0;		// initially face North.
	if (numPoints!=4 && numPoints!=8)
		compassPoints = 4;
	else	compassPoints = numPoints;
	device->CreateSoundBuffer (&(directions[0]), "SoundFiles\\north.wav", 0);
	device->CreateSoundBuffer (&(directions[2]), "SoundFiles\\east.wav", 0);
	device->CreateSoundBuffer (&(directions[4]), "SoundFiles\\south.wav", 0);
	device->CreateSoundBuffer (&(directions[6]), "SoundFiles\\west.wav", 0);
	if (compassPoints == 8) {
		device->CreateSoundBuffer (&(directions[1]), "SoundFiles\\northeast.wav", 0);
		device->CreateSoundBuffer (&(directions[3]), "SoundFiles\\southeast.wav", 0);
		device->CreateSoundBuffer (&(directions[5]), "SoundFiles\\southwest.wav", 0);
		device->CreateSoundBuffer (&(directions[7]), "SoundFiles\\northwest.wav", 0);
	}
	else
		for (int i=1; i<8; i+=2)
			directions[i] = NULL;
} // end Compass constructor method

//--- destructor method.
Compass::~Compass ()
{
	for (int i=0; i<8; i++)
		if (directions[i]) free (directions[i]);
} // end Compass destructor method.

//--- Speak the current bearing.
void Compass::SpeakBearing ()
{
	int index;
	if (compassPoints == 4)
		index = bearing * 2;
	else	index = bearing;
	directions[index]->Play ();
} // end SpeakBearingmethod.

//--- Get the current bearing.
int Compass::GetBearing ()
{  return bearing;  }

//--- Return the current bearing as a 3D orientation vector.
void Compass::GetOrientation (
	D3DVECTOR* orient)	// (x,y,z) orientation vector.
{
	double bearingInc, angle;
	orient->y = 0.0;
	bearingInc = 2.0 * PI / compassPoints;
	angle = bearing * bearingInc;
	// Process angle depending on which quadrant it is in.
	if (angle <= PI * 2.0)
		{ orient->x = (float)sin(angle);              orient->z = (float)cos(angle); }
	else if (angle <= PI)
		{ orient->x = (float)cos(angle-PI*2.0);       orient->z = -(float)sin(angle-PI*2.0); }
	else if (angle <= 3.0*PI/2.0)
		{ orient->x = -(float)cos(3.0*PI/2.0-angle);  orient->z = -(float)sin(3.0*PI/2.0-angle); }
	else	
		{ orient->x = -(float)sin(2.0*PI-angle);      orient->z = (float)cos(2.0*PI-angle); }
} // end GetOrientation method.

//--- Change the current bearing to a new setting by a relative amount.
//--- Returns new setting.
int Compass::ChangeBearing (
		int change)
{
	bearing = (bearing + change) % compassPoints;
	if (bearing < 0)
		bearing = compassPoints + bearing;
	return bearing;
} // end ChangeBearing method.

//--- set the current bearing to a new setting based on an orientation vector.
//--- The new bearing will be rounded to the nearest bearing to the orientation.
//--- Returns new setting.
int Compass::SetBBearing (
	D3DVECTOR orient)	// (x,y,z) orientation vector.
{
	double angle, bearingInc;
	bearingInc = 2.0 * PI / compassPoints;
	if (orient.x == 0)
		if (orient.z >= 0.0)
			angle = 0.0;
		else	angle = PI;
	else if (orient.x > 0)
		if (orient.z >= 0)
			angle = PI / 2.0 - atan2 (orient.z, orient.x);
		else	angle = PI / 2.0 + atan2 (-orient.z, orient.x);
	else // x < 0
		if (orient.z >= 0)
			angle = 3.0 * PI / 2.0 + atan2 (orient.z, -orient.x);
		else	angle = 3.0 * PI / 2.0 - atan2 (-orient.z, -orient.x);

	bearing = (int) (angle / bearingInc + 0.5);
	if (bearing >= compassPoints)
		bearing = 0;   // faces North.
	return bearing;
} // end SetBearing method.
