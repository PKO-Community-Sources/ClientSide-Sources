#include "Stdafx.h"

#include "GameApp.h"

#include "SceneObjSet.h"
#include "EffectSet.h"
#include "MusicSet.h"
#include "CharacterPoseSet.h"
#include "MapSet.h"
#include "ChaCreateSet.h"
#include "EventSoundSet.h"
#include "AreaRecord.h"
#include "ServerSet.h"

#include "MPEditor.h"
#include "FindPath.h"
#include "MPFont.h"

#include "EffectObj.h"
#include "notifyset.h"
#include "ChatIconSet.h"
#include "ItemTypeSet.h"
#include "InputBox.h"
#include "ItemPreSet.h"
#include "ItemRefineSet.h"
#include "ItemRefineEffectSet.h"
#include "StoneSet.h"
#include "ElfSkillSet.h"
#include "GameWG.h"
#include "GameMovie.h"

#ifndef USE_DSOUND
#include "AudioThread.h"
CAudioThread	g_AudioThread;
#endif

// 多语言库须先初始化
CLanguageRecord g_oLangRec("./scripts/table/StringSet.bin", "./scripts/table/StringSet.txt");

bool	volatile	g_bLoadRes				  = FALSE;
CGameApp*	        g_pGameApp	              = NULL;

CEffectSet*	        CEffectSet::_Instance     = NULL;
CShadeSet*          CShadeSet::_Instance      = NULL;
CMusicSet*          CMusicSet::_Instance      = NULL;
CPoseSet*           CPoseSet::_Instance       = NULL;
CMapSet*            CMapSet::_Instance        = NULL;
CChaCreateSet*      CChaCreateSet::_Instance  = NULL;
CEventSoundSet*     CEventSoundSet::_Instance = NULL;
CAreaSet*           CAreaSet::_Instance       = NULL;
CServerSet*         CServerSet::_Instance     = NULL;
CNotifySet*         CNotifySet::_Instance     = NULL;
CChatIconSet*		CChatIconSet::_Instance   = NULL;
CItemTypeSet*		CItemTypeSet::_Instance   = NULL;
CItemPreSet*		CItemPreSet::_Instance	  = NULL;
CItemRefineSet*		CItemRefineSet::_Instance = NULL;
CItemRefineEffectSet* CItemRefineEffectSet::_Instance	= NULL;
CStoneSet*			CStoneSet::_Instance				= NULL;
CElfSkillSet*		CElfSkillSet::_Instance				= NULL;

MPEditor	        g_Editor;
CFindPath			g_cFindPath(128,38);
CInputBox			g_InputBox;

CGameWG             g_oGameWG;
CGameMovie			g_GameMovie;


// 客户端版本号, 在GateServer有验证
short g_sClientVer = 136;
short g_sKeyData = short(g_sClientVer * g_sClientVer * 0x1232222);
char g_szSendKey[4];
char g_szRecvKey[4];

DWORD g_dwCurMusicID = 0;

// CLightEff plight;
