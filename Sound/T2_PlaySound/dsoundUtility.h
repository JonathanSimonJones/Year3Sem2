// File:    dsoundUtility.h
// Purpose: Utility class methods for simple DirectSound processing.
// Author:  Allan C. Milne
//          based on Windows processing model by Frank Luna and
//          DirecSound wrapper approach in DirectX SDK samples.
// Version: 1.0
// Date:    22 January 2006
//
// Description.
// Facilitates a Windows message processing model.
// Provides simple classes for encapsulating basic DirectSound concepts.
// Use as the basis for your own encapsulations.


//--- libraries required.
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dxguid.lib")

#ifndef DSOUNDUTILITY_H_v1_0
#define DSOUNDUTILITY_H_v1_0

//--- required header files.
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>


//--- Windows processing functions.

//-- Create application window.
bool InitWindow (
	char* title,               // window title bar text.
	HINSTANCE hInstance,       // [in] Application instance.
	int width, int height,     // [in] Backbuffer dimensions.
	bool windowed,             // [in] Windowed (true)or full screen (false).
	HWND *ptr_hwnd);           // returns new window handle.

//--- Encapsulates calling of the windows message processing function.
//--- function pointer provided as parameter - includes time since last call.
int EnterMsgLoop( 
	bool (*ptr_process)(float timeDelta));

//--- Windows message procedure.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


//--- DirectSound classes.
class DSoundDevice;
class Sound;

//--- The DirectSound base device.
class DSoundDevice {
protected:
	IDirectSound8* dsound;

public:
	DSoundDevice ();	//--- constructor.
	~DSoundDevice ();	//--- destructor.

	//--- set up the DirectSound device.
	bool Initialize (
		HWND hwnd,		// current window handle.
		DWORD coopLevel);	// cooperative level.

	//--- access to the DirectSound device to allow other methods to be called.
	IDirectSound8* GetDirectSound ();

	//--- Create a secondary sound buffer filled from a wave file.
	bool CreateSoundBuffer (
		Sound** sound,         // Buffer to create and load into.
		char*   filename,      // Name of wave file.
		DWORD   bufferFlags);  // capability flags to use when creating buffer.

}; // end DSoundDevice class.


//---  The secondary buffer for a sound sample.

class Sound {
protected:
	LPDIRECTSOUNDBUFFER8 buffer;
	DWORD ctrlFlags;

public:
	Sound (LPDIRECTSOUNDBUFFER8 buff, DWORD flags);	//--- constructor.
	virtual ~Sound ();				//--- destructor.

	//--- access to the buffer to allow other methods to be called.
	LPDIRECTSOUNDBUFFER8 GetBuffer ();

	//--- Play the sound from the start of the buffer.
	//--- optional parameters:
	//--- use DSPLAY_LOOPING flag for looped sound.
	//--- Other parameters are the settings to use in playing.
	//--- : only applied if that buffer control is available.
	//--- if no setting parameters supplied then full volume and middle pan are used.
	void Play (
		DWORD flags=0, 	// playing flags, eg DSPLAY_LOOPING.
		LONG vol=0, 	// volume to play at, default is maximum.
		LONG pan=0, 	// pan setting for this play, default is middle.
		DWORD freq=-1);	// sample frequency to play at, default is no change.

	//--- stop the buffer playing.
	void Stop ();

	//--- pause the buffer at current time
	void Pause ();

	//--- returns true if the buffer is currently playing.
	bool IsPlaying ();

	void Active(bool Active_);
	bool ReturnActive();
private:
	DWORD  CurrentPlayCursor, CurrentWriteCursor;
	bool m_Active;
}; // end Sound class.

#endif