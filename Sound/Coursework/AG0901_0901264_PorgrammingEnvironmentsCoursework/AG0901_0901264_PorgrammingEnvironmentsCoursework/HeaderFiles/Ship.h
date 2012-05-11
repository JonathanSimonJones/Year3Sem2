/*
	Name	Ship.h
	Author	Jonathan Jones
	Detail	Contains all the variables and methods for a ship
	Purpose	To contain and play all the ship sounds  
*/

#ifndef SHIP_H_
#define SHIP_H_

#include <dsoundUtility.h>
#include <list>

class EnvSound;

class Ship
{
public:
	Ship(DSoundDevice* DirectSoundDevice);
	~Ship();
	void EnterShipSequence();
	void StartShip();
	void MoveDeltaX();
	void PlayMoving();
	void SetShipMovingSoundPos(const float x, const float y, const float z);

private:

	DSoundDevice* DirectSoundDevice_;
	float x_, y_, z_;
	EnvSound *Button1_;
	EnvSound *Button2_;
	EnvSound *Button3_;
	EnvSound *Button4_;
	EnvSound *ShipDoorOpening_;
	EnvSound *ShipDoorClosing_;
	EnvSound *ShipIdle_;
	EnvSound *ShipPoweringDown_;
	EnvSound *ShipPowerUp_;
	EnvSound *ShipPowerUp2_;
	EnvSound *Wub1_;
	EnvSound *SeatMovement_;
	EnvSound *ShipMoving_;
	
};
#endif	// SHIP_H_