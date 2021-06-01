#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include "SoundCommon.h"

class SoundInstance;

#define MAX_SOURCE_SOUNDS	256
#define MAX_CHANNELS		32

class SoundManager
{
public:
	SoundManager() {}
	virtual ~SoundManager() {}

	virtual bool			Initialized() = 0;

	virtual bool			LoadSound(int theSfxID, const string& theFilename) = 0;
	virtual int				LoadSound(const string& theFilename) = 0;
	virtual void			ReleaseSound(unsigned int theSfxID) = 0;

	virtual void			SetVolume(double theVolume) = 0;
	virtual bool			SetBaseVolume(unsigned int theSfxID, double theBaseVolume) = 0;
	virtual bool			SetBasePan(unsigned int theSfxID, int theBasePan) = 0;	

	virtual SoundInstance*	GetSoundInstance(unsigned int theSfxID) = 0;

	virtual void			ReleaseSounds() = 0;
	virtual void			ReleaseChannels() = 0;

	virtual double			GetMasterVolume() = 0;
	virtual void			SetMasterVolume(double theVolume) = 0;

	virtual void			Flush() = 0;
	virtual void			SetCooperativeWindow(HWND theHWnd, bool isWindowed) = 0;
	virtual void			StopAllSounds() = 0;
};

#endif //__SOUNDMANAGER_H__