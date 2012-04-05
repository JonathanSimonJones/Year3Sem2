// File:    Compass.h
// Purpose: Class specification for a compass with discrete bearings
//          and speech output of the bearing.
// Author:  Allan C. Milne
// Version: 1.0
// Date:    24 April 2006
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
// requires north.wav, northeast.wav, east.wav, southeast.wav,
//          south.wav  southwest.wav, west.wav, northwest.wav


#include "dsoundUtility.h"

class Compass {

private:
	int bearing;		// the current bearing.
	int compassPoints;	// number of discrete compass points.

	// For spoken feedback on the current bearing.
	Sound* directions[8];

public:
	// constructor method.
	// sets up initial bearing and loads sounds.
	// If invalid number of bearings defaults to 4.
	Compass (
		DSoundDevice* device,	// the DirectSound device object.
		int numPoints);		// number of compass points.
	~Compass ();

	// speak the current bearing.
	void SpeakBearing ();

	// Get the current bearing.
	int GetBearing ();

	// Return the current bearing as a 3D orientation vector.
	void GetOrientation (
		D3DVECTOR* orient);	// (x,y,z) orientation vector.

	// Change the current bearing to a new setting by a relative amount.
	// Returns new setting.
	int ChangeBearing (
		int change);

	// set the current bearing to a new setting based on an orientation vector.
	// The new bearing will be rounded to the nearest bearing to the orientation.
	// Returns new setting.
	int SetBBearing (
		D3DVECTOR orient);	// (x,y,z) orientation vector.
	
}; // end Compass class.
