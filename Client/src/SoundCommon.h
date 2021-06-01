#ifndef SOUND_COMMON_H
#define SOUND_COMMON_H

#include <math.h>

#define DIRECTSOUND_VERSION 0x0900
#include <dsound.h>

#include <string>
#include <map>

using namespace std;

#define LONG_BIGE_TO_NATIVE(l) (((l >> 24) & 0xFF) | ((l >> 8) & 0xFF00) | ((l << 8) & 0xFF0000) | ((l << 24) & 0xFF000000))
#define WORD_BIGE_TO_NATIVE(w) (((w >> 8) & 0xFF) | ((w << 8) & 0xFF00))
#define LONG_LITTLEE_TO_NATIVE(l) (l)
#define WORD_LITTLEE_TO_NATIVE(w) (w)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

string			StringToLower(const string& theString);
string			GetDirectSoundString( HRESULT theResult );
string			GetFileDir(const string& thePath, bool withSlash = false);
void			MkDir(const string& theDir);

#endif