/**
*  @file playsound.h
*
*  playsound.h, v 2.0 2004/8/22 13:14:05 shanghai China
*
*  @author claude Fan <fanyf2002@hotmail.com>
*
*/


#ifndef _PLAYSOUND_H_
#define _PLAYSOUND_H_

// ����ϵͳ��ʼ������Ӧ�ó�����ڵ����
bool mus_mgr_init(bool enable, DWORD_PTR dwValue = 0);
// ����ϵͳReset�����л�����ʱ����
void mus_mgr_reset(int op = 0); // opΪ-1�����������, 0������������������������
// ����ϵͳ�˳�����Ӧ�ó����˳������
void mus_mgr_exit();


// ���Ż�����Ч
bool env_snd_is_full(); // ������Чͨ�����Ƿ��Ѿ�ȫ��ռ��
int env_snd_add(char const* fname); // ����-1��ʾ������Чͨ������
bool env_snd_vol(int id, int vol); // vol >= 0����������false������128��128����
bool env_snd_play(int id);
inline bool env_snd_del(int id) {return env_snd_vol(id, 0);}

// ������ͨ��Ч
void cmn_snd_set_cache(int n = 20); // n�ǻ���WAVE�ļ���������
bool cmn_snd_play(char const* fname, int vol = 128); // vol >= 0����������false������128��128����

// ʹ����Ч
void snd_enable(bool enable = true);

// ���ű�������
void bkg_snd_set_notify(HWND hwnd, UINT msg_id);
bool bkg_snd_play(char const* fname, bool loop = false);
void bkg_snd_vol(int vol);
void bkg_snd_stop();


//////////////////////////////////////////////////////////////////////////
//
// ���µ��Ͻӿ�ȫ�����½ӿ�ʵ�֣���Щ�ӿڻ��𲽷��� by claude Fan at 2004-10-20
//
//////////////////////////////////////////////////////////////////////////

// ������������
inline void SetBkMusicNotify(HWND hWnd, UINT uMsgId)
    {bkg_snd_set_notify(hWnd, uMsgId);}
inline int PlayBkMusic(char const* filename, bool loop = false)
    {return int(bkg_snd_play(filename, loop));}
inline int StopBkMusic() {bkg_snd_stop(); return 0;}

// ������Ч
inline void SetWaveQueueLen(int n = 20) {cmn_snd_set_cache(n);}
inline int PlayMusic(char const* filename, int reserved = 0)
    {return int(cmn_snd_play(filename));}

#endif // _PLAYSOUND_H_

