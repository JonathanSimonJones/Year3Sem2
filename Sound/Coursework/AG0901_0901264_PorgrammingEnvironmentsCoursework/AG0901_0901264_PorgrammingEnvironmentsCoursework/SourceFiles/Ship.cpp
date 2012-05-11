/*
	Name	Ship.cpp
	Author	Jonathan Jones
	Detail	Contains all the variables and methods for a ship
	Purpose	To contain and play all the ship sounds  
*/

#include <Ship.h>
#include <EnvSound.h>
#include <Windows.h>

Ship::Ship(DSoundDevice* DirectSoundDevice)
: DirectSoundDevice_(DirectSoundDevice)
, x_(0)
, y_(0)
, z_(0)
{
	Sound *SoundPtrButton1_;
	Sound *SoundPtrButton2_;
	Sound *SoundPtrButton3_;
	Sound *SoundPtrButton4_;
	Sound *SoundPtrShipDoorOpening_;
	Sound *SoundPtrShipDoorClosing_;
	Sound *SoundPtrShipIdle_;
	Sound *SoundPtrShipPoweringDown_;
	Sound *SoundPtrShipPowerUp_;
	Sound *SoundPtrShipPowerUp2_;
	Sound *SoundPtrWub1_;
	Sound *SoundPtrSeatMovement_;
	Sound *SoundPtrShipMoving_;

	bool ok = true;
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrButton1_			, "SoundFiles/Ship/Button1.wav"			, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrButton2_			, "SoundFiles/Ship/Button2.wav"			, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrButton3_			, "SoundFiles/Ship/Button3.wav"			, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrButton4_			, "SoundFiles/Ship/Button4.wav"			, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrShipDoorOpening_	, "SoundFiles/Ship/ShipDoorOpening.wav"	, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrShipDoorClosing_	, "SoundFiles/Ship/ShipDoorClosing.wav"	, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrShipIdle_			, "SoundFiles/Ship/ShipIdle.wav"		, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrShipPoweringDown_	, "SoundFiles/Ship/ShipPoweringDown.wav", DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrShipPowerUp_		, "SoundFiles/Ship/ShipPowerUp.wav"		, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrShipPowerUp2_		, "SoundFiles/Ship/ShipPowerUp2.wav"	, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrWub1_				, "SoundFiles/Ship/Wub1.wav"			, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrSeatMovement_		, "SoundFiles/Player/OutfitRustle.wav"	, DSBCAPS_CTRL3D);
	ok = ok && DirectSoundDevice_->CreateSoundBuffer(&SoundPtrShipMoving_		, "SoundFiles/Ship/ShipMoving.wav"		, DSBCAPS_CTRL3D);

	Button1_			= new EnvSound(SoundPtrButton1_			, x_, y_, z_ + 5.0f);
	Button2_			= new EnvSound(SoundPtrButton2_			, x_, y_, z_);
	Button3_			= new EnvSound(SoundPtrButton3_			, x_ - 5.0f, y_, z_);
	Button4_			= new EnvSound(SoundPtrButton4_			, x_ + 5.0f, y_, z_);
	ShipDoorOpening_	= new EnvSound(SoundPtrShipDoorOpening_	, x_, y_, z_ + 5.0f);
	ShipDoorClosing_	= new EnvSound(SoundPtrShipDoorClosing_	, x_ + 5.0f, y_, z_);
	ShipIdle_			= new EnvSound(SoundPtrShipIdle_		, x_, y_, z_);
	ShipPoweringDown_	= new EnvSound(SoundPtrShipPoweringDown_, x_, y_, z_);
	ShipPowerUp_		= new EnvSound(SoundPtrShipPowerUp_		, x_, y_, z_);
	ShipPowerUp2_		= new EnvSound(SoundPtrShipPowerUp2_	, x_, y_, z_);
	Wub1_				= new EnvSound(SoundPtrWub1_			, x_, y_, z_);
	SeatMovement_		= new EnvSound(SoundPtrSeatMovement_	, x_, y_, z_);
	ShipMoving_			= new EnvSound(SoundPtrShipMoving_		, x_, y_, z_);

	SoundPtrButton1_ = 0;
	SoundPtrButton2_ = 0;
	SoundPtrButton3_ = 0;
	SoundPtrButton4_ = 0;
	SoundPtrShipDoorOpening_ = 0;
	SoundPtrShipDoorClosing_ = 0;
	SoundPtrShipIdle_ = 0;
	SoundPtrShipPoweringDown_ = 0;
	SoundPtrShipPowerUp_ = 0;
	SoundPtrShipPowerUp2_ = 0;
	SoundPtrWub1_ = 0;
	SoundPtrSeatMovement_ = 0;
	SoundPtrShipMoving_ = 0;
}

Ship::~Ship()
{
	delete Button1_;
	delete Button2_;
	delete Button3_;
	delete Button4_;
	delete ShipDoorOpening_;
	delete ShipDoorClosing_;
	delete ShipIdle_;
	delete ShipPoweringDown_;
	delete ShipPowerUp_;
	delete ShipPowerUp2_;
	delete Wub1_;
	delete SeatMovement_;
	delete ShipMoving_;

	Button1_ = 0;
	Button2_ = 0;
	Button3_ = 0;
	Button4_ = 0;
	ShipDoorOpening_ = 0;
	ShipDoorClosing_ = 0;
	ShipIdle_ = 0;
	ShipPoweringDown_ = 0;
	ShipPowerUp_ = 0;
	ShipPowerUp2_ = 0;
	Wub1_ = 0;
	SeatMovement_ = 0;
	ShipMoving_ = 0;

}

void Ship::EnterShipSequence()
{
	Button1_->PlayOnce();

	while(Button1_->isPlaying())
	{
	}

	Sleep(750);
	ShipDoorOpening_->PlayOnce();

	while(ShipDoorOpening_->isPlaying())
	{
	}

	Sleep(500);
	SeatMovement_->PlayOnce();

	Sleep(250);
	ShipDoorClosing_->PlayOnce();

	while(ShipDoorClosing_->isPlaying())
	{
	}

	Sleep(500);
}
	
void Ship::MoveDeltaX()
{
}

void Ship::StartShip()
{
	Button2_->PlayOnce();

	while(Button2_->isPlaying())
	{
	}

	Sleep(250);
	ShipPowerUp2_->PlayOnce();

	Sleep(1500);
	Button3_->PlayOnce();

	Sleep(1000);
	Button4_->PlayOnce();

	while(ShipPowerUp2_->isPlaying())
	{
	}
	//ShipIdle_->PlayLooping();

}

void Ship::PlayMoving()
{
	ShipMoving_->PlayLooping();
}

void Ship::SetShipMovingSoundPos(const float x, const float y, const float z)
{
	ShipMoving_->SetPosition(x - 5.0f, y, z);	// Minus 5 to make sound appear more quite
}