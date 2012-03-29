// File:    dsoundUtility.cpp
// Purpose: Implementation of utility class methods for simple DirectSound processing.
// Author:  Allan C. Milne
//          based on Windows processing model by Frank Luna and
//          DirectSound wrapper approach in DirectX SDK samples.
// Version: 1.2
// Date:    15th February 2006
//
// Description.
// Includes some utility functions.
// Facilitates a Windows message processing model.
// Provides simple classes for encapsulating basic DirectSound concepts.
// Use as the basis for your own encapsulations.


#include "dsoundUtility.h"


//--- Utility functions.

//--- unitise a 3D vector.
void UnitVector (
	float x, float y, float z,    // supplied vector components.
	D3DVECTOR* unit )             // unitised vector.
{
	float mod;
	mod = sqrt (x*x + y*y + z*z);
	(*unit).x = x / mod;
	(*unit).y = y / mod;
	(*unit).z = z / mod;
} // end UnitVector function.


//--- Windows processing functions.

bool InitWindow (
	char*title,                // window title bar text.
	HINSTANCE hInstance,       // [in] Application instance.
	int width, int height,     // [in] Backbuffer dimensions.
	bool windowed,             // [in] Windowed (true)or full screen (false).
	HWND *ptr_hwnd)            // returns new window handle.
{
	// Create the main application window.
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = title;

	if( !RegisterClass(&wc) ) 
	{
		MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return false;
	}
	HWND hwnd = 0;
	hwnd = CreateWindow (title, title, 
		WS_EX_TOPMOST,
		0, 0, width, height,
		0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/); 
	if( !hwnd )
	{
		MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return false;
	}
	(*ptr_hwnd) = hwnd;

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return true;
} // end InitWindow function.


//--- Encapsulates calling of the windows message processing function.
//--- function pointer provided as parameter - includes time since last call.
int EnterMsgLoop( bool (*ptr_process)(float timeDelta))
{
	// time of last call of message processing function.
	static float lastTime = (float)timeGetTime(); 

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
        {	
			// get time of the current call.
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			//=== call the loop processing funciton. ===
			ptr_process (timeDelta);

			lastTime = currTime;
        }
    }
    return msg.wParam;
} // end EnterMsgLoop function.


//---
//--- Implementation of DSoundDEvice class.
//---

//--- constructor method.
DSoundDevice::DSoundDevice ()
{
	dsound = NULL; 
} // end DSoundDevice constructor method.

//--- Destructor method.
DSoundDevice::~DSoundDevice ()
{
	if (dsound) dsound->Release();
} // end DSoundDevice destructor method.

//--- Create the DirectSound device object.
bool DSoundDevice::Initialize (
		HWND hwnd,		// current window handle.
		DWORD coopLevel) 	// cooperative level.
{
	if (dsound) dsound->Release();
	HRESULT hr;	
	hr = DirectSoundCreate8 (NULL, &dsound, NULL);
	if (hr!=DS_OK) return false;
	hr = dsound->SetCooperativeLevel (hwnd, coopLevel);
	return (hr == DS_OK);
} // end Initialize method.

//--- Get the DirectSound Device.
IDirectSound8* DSoundDevice::GetDirectSound ()
{
	return dsound;
} // end GetDirectSound method.

//--- Create and load a secondary buffer from a wave file.
bool DSoundDevice::CreateSoundBuffer (
		Sound** sound,         // Buffer to create and load into.
		char*   filename,      // Name of wave file.
		DWORD   bufferFlags)   // flags to use when creating buffer.
{
	// Get the wave file format details.
	WAVEFORMATEX wfx;
	memset (&wfx, 0, sizeof(WAVEFORMATEX));
	HMMIO wavefile;
	wavefile = mmioOpen (filename, 0, MMIO_READ | MMIO_ALLOCBUF);
	if (wavefile == NULL) return false;
	MMCKINFO parent, child;
	memset (&parent,0,sizeof(MMCKINFO));
	memset (&child,0,sizeof(MMCKINFO));
	//find wave data chunk.
	parent.fccType = mmioFOURCC ('W','A','V','E');
	mmioDescend (wavefile,&parent,0,MMIO_FINDRIFF);
	//find fmt data chunk.
	child.fccType = mmioFOURCC ('f','m','t',' ');
	mmioDescend (wavefile,&child,&parent,0);
	//read the format chunk into the structure.
	mmioRead (wavefile, (char*)&wfx, sizeof(wfx));
	if (wfx.wFormatTag != WAVE_FORMAT_PCM) return false;

	// Move to the wave sample data chunk.
	mmioAscend (wavefile,&child,0);
	child.ckid = mmioFOURCC ('d','a','t','a');
	mmioDescend (wavefile,&child,&parent,MMIO_FINDCHUNK);

	// Create the sound buffer.
	LPDIRECTSOUNDBUFFER  dsbuf;
	LPDIRECTSOUNDBUFFER8 dsbuf8;
	DSBUFFERDESC bufdesc;
	memset (&bufdesc, 0, sizeof(DSBUFFERDESC));
	bufdesc.dwSize = sizeof(DSBUFFERDESC);
	bufdesc.dwFlags = bufferFlags;
	bufdesc.dwBufferBytes = child.cksize;
	bufdesc.lpwfxFormat = &wfx;
	if (dsound->CreateSoundBuffer (&bufdesc, &dsbuf, NULL)
	    != DS_OK) return false;
	if (dsbuf->QueryInterface (IID_IDirectSoundBuffer8, (LPVOID*)&dsbuf8)
	    != DS_OK) return false;
	dsbuf->Release(); dsbuf = NULL;   // finished with this interface.

	// read in the wave sample data.
	LPVOID write1=0, write2 = 0;
	DWORD  length1, length2;
	dsbuf8->Lock (0, child.cksize, &write1, &length1, &write2, &length2, 0);
	if (write1 > 0)
	   mmioRead (wavefile, (char*)write1, length1);
	if (write2 > 0)
	   mmioRead (wavefile, (char*)write2, length2);
	dsbuf8->Unlock (write1, length1, write2, length2);
	mmioClose (wavefile, 0);

	// Create the Sound object.
	(*sound) = new Sound (dsbuf8, bufferFlags);

	return true;
} // end CreateSoundBuffer method.

//--- Get the 3D listener for the primary buffer.
LPDIRECTSOUND3DLISTENER8 DSoundDevice::Get3dListener ()
{
	LPDIRECTSOUNDBUFFER      dsPrimBuf;  // not 8.
	LPDIRECTSOUND3DLISTENER8 ds3Dlistener;
	DSBUFFERDESC dsbd;
	ZeroMemory (&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	if ( FAILED (dsound->CreateSoundBuffer (&dsbd, &dsPrimBuf, NULL)))
		return NULL;
	if (FAILED (dsPrimBuf->QueryInterface (IID_IDirectSound3DListener8,
                                               (LPVOID*)&ds3Dlistener)))
		return NULL;
	dsPrimBuf->Release(); 
	dsPrimBuf = NULL;
	return ds3Dlistener;
} // end Get3dListener method


//---
//---  The secondary buffer for a sound sample.
//---

//--- Constructor method.
Sound::Sound (LPDIRECTSOUNDBUFFER8 buff, DWORD flags)
{
	buffer = buff;
	ctrlFlags = flags;
} // end Sound constructor method.

//--- Destructor method.
Sound::~Sound ()
{
	if (buffer) buffer->Release();
} // end Sound destructor method.

//--- Return buffer pointer to allow use of native methods.
LPDIRECTSOUNDBUFFER8 Sound::GetBuffer ()
{
	return buffer;
} // end GetBuffer method.

//--- Return a 3D buffer interface to allow 3D methods to be called.
LPDIRECTSOUND3DBUFFER8 Sound::Get3dBuffer () 
{
	LPDIRECTSOUND3DBUFFER8 buf3D; 
	HRESULT hr;
	if (!buffer) return NULL;
	hr = buffer->QueryInterface (IID_IDirectSound3DBuffer8, (LPVOID*)&buf3D); 
	if (hr != DS_OK) return NULL;
	return buf3D;
} // end Get3dBuffer method

//--- Play the sound; optional parameters.
//--- use DSPLAY_LOOPING flag for looped sound.
//--- Other parameters are the settings to use in playing.
//--- : only applied if that buffer control is available.
//--- if no setting parameters supplied then full volume and middle pan are used.
void Sound::Play (DWORD flags, LONG vol,  LONG pan, DWORD freq)
{
	if (!buffer) return;
	 buffer->SetCurrentPosition (0); 
	if (ctrlFlags & DSBCAPS_CTRLVOLUME)
		buffer->SetVolume (vol); 
	if (ctrlFlags & DSBCAPS_CTRLPAN)
		buffer->SetPan (pan); 
	if ((freq != -1) && (ctrlFlags & DSBCAPS_CTRLFREQUENCY))
		buffer->SetFrequency (freq);
	 buffer->Play (0, 0, flags);
} // end Play method.

//--- Stop a currently playing sound.
void Sound::Stop ()
{
	if (!buffer) return;
	if (IsPlaying())  buffer->Stop ();
} // end Stop method.

//--- Indicate whether the sound is currently playing.
bool Sound::IsPlaying ()
{
	if (!buffer) return false;
	DWORD status; 
	buffer->GetStatus (&status); 
	return (status & DSBSTATUS_PLAYING);
} // end IsPlaying method.

void Sound::ChangeFrequency (DWORD freqChange) 
{
	if (!buffer) return;
	 DWORD frequency; 
	 buffer->GetFrequency (&frequency); 
	 buffer->SetFrequency (frequency + freqChange); 
} // end changeFrequency method

void Sound::ChangeVolume (LONG volChange)
{
	if (!buffer) return;
	 LONG  volume; 
	 buffer->GetVolume (&volume);
	if (volume+volChange > 0)
		buffer->SetVolume (0);
	else	buffer->SetVolume (volume + volChange); 
} // end ChangeVolume method

void Sound::ChangePan (LONG panChange)
{
	if (!buffer) return;
	 LONG  pan; 
	 buffer->GetPan (&pan); 
	 buffer->SetPan (pan + panChange); 
} // end ChangePan method

