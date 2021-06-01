#include "stdafx.h"
#include "script.h"
#include <iostream>
#include "caLua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "GameConfig.h"
#include "GameApp.h"

#include "lua_platform.h"	//Add by lark.li 200804111
#include "UISystemForm.h"
//#pragma comment (lib, "lua50.lib")
//#pragma comment (lib, "lualib.lib")

#define DEFAULT_SCRIPT_NUM		 1024

DWORD CScript::_dwCount		= DEFAULT_SCRIPT_NUM;
DWORD CScript::_dwFreeCount	= DEFAULT_SCRIPT_NUM;
DWORD CScript::_dwLastFree	= 0;

CScript** CScript::_AllObj = NULL;



//---------------------------------------------------------------------------
// CScript
//---------------------------------------------------------------------------
bool CScript::Init()
{
	 _dwCount		= DEFAULT_SCRIPT_NUM;
	 _dwFreeCount	= DEFAULT_SCRIPT_NUM;
	 _dwLastFree		= 0;

	 _AllObj = new CScript*[_dwCount];
	 memset( CScript::_AllObj, 0, _dwCount * sizeof(CScript*) );
	 return true;
}

bool CScript::Clear()
{
	delete [] _AllObj;

	_AllObj		= NULL;
	_dwCount		= 0;
	_dwFreeCount	= 0;
	_dwLastFree	= 0;
	return true;
}

CScript::CScript()
{
	if( _dwFreeCount<=0 )
	{
		_dwLastFree = _dwCount + 1;
		_dwCount += DEFAULT_SCRIPT_NUM;
		_dwFreeCount = DEFAULT_SCRIPT_NUM;

		CScript** tmp = _AllObj;

		_AllObj = new CScript*[_dwCount];
		memset( _AllObj, 0, _dwCount * sizeof(CScript*) );
		memcpy( _AllObj, tmp, (_dwCount - DEFAULT_SCRIPT_NUM) * sizeof(CScript*) );
		delete [] tmp;
	}

	if( !_AllObj[_dwLastFree] )
	{
		_AllObj[_dwLastFree] = this;
		_dwScriptID = _dwLastFree;

		--_dwFreeCount;
		++_dwLastFree;
		if( _dwLastFree>=_dwCount ) 
			_dwLastFree = 0;
		return;
	}

	// Ѱ�ҵ�ǰ�����Ƿ��п�λ,���û�п�λ,���ӵ�ĩβ	
	for( DWORD i=_dwLastFree+1; i<_dwCount; ++i )
	{
		if( !_AllObj[i] )
		{
			_AllObj[i] = this;
			_dwScriptID = i;

			--_dwFreeCount;
			return;
		}
	}

	for( int i=_dwLastFree-1; i>=0; --i )
	{
		if( !_AllObj[i] )
		{
			_AllObj[i] = this;
			_dwScriptID = i;

			--_dwFreeCount;
			return;
		}
	}

	LG( "error", "msgCScript::CScript Error, dwCount: %d, dwFreeCount: %d, dwLastFree: %d", _dwCount, _dwFreeCount, _dwLastFree );
}

CScript::~CScript()
{
	if( _dwScriptID>_dwCount ) 
		LG( "error", "msgCScript::~CScript Error, dwCount: %d, dwFreeCount: %d, dwLastFree: %d", _dwCount, _dwFreeCount, _dwLastFree );

	_AllObj[_dwScriptID] = NULL;

	_dwLastFree = _dwScriptID;
	++_dwFreeCount;
}

//---------------------------------------------------------------------------
// CScriptMgr
//---------------------------------------------------------------------------
lua_State*			_pLuaState	= NULL;
static FILE*		_pStdErr	= NULL;

CScriptMgr::CScriptMgr()
{
}

CScriptMgr::~CScriptMgr()
{
	Clear();
}


bool CScriptMgr::Init()
{
	if( !CScript::Init() ) return false;
	
	_pStdErr = freopen( "lua_err.txt", "w", stderr );

	int CLU_State = CLU_Init(); 
	CLU_LoadState(CLU_State);

	extern void MPInitLua_Scene();
	extern void MPInitLua_Scene1024();
	extern void MPInitLua_Gui();
	extern void MPInitLua_Cha();
	extern void MPInitLua_App();
	MPInitLua_Gui();
	MPInitLua_App();

	if(g_stUISystem.m_sysProp.m_videoProp.bPixel1024)
	{
		MPInitLua_Scene1024();	
		CLU_LoadScript("scripts/lua/CameraConf1024.clu");
		g_pGameApp->GetMainCam()->MAX_SCALE = 1.30f;
	}
	else
	{
		MPInitLua_Scene();
		CLU_LoadScript("scripts/lua/CameraConf.clu");
		g_pGameApp->GetMainCam()->MAX_SCALE = 1.0f;
	}

	MPInitLua_Scene();
	MPInitLua_Cha();

	CLU_LoadScript("scripts/lua/scene.clu");
	CLU_LoadScript("scripts/lua/scene/face.clu");

	CLU_LoadScript("scripts/lua/CharacterConf.clu");
	//CLU_LoadScript("scripts/lua/CameraConf.clu");
	
	// Modify by lark.li 20080411 begin
	//char type[6] = "char*";
	//int ret = CLU_RegisterFunction("GetResString", type, "char*", CLU_CDECL, CLU_CAST(Lua_GetResString));
	CLU_LoadScript("scripts/lua/mission/mission.lua");
	CLU_LoadScript("scripts/lua/mission/missioninfo.lua");
	// End

	return true;
}

bool CScriptMgr::LoadScript()
{
	//_pLuaState = lua_open();
	//if( !_pLuaState ) 
	//{
	//	LG( "lua", "msglua_open error!" );
	//	return false;
	//}
 //   lua_baselibopen (_pLuaState);
 //   lua_iolibopen (_pLuaState);
 //   lua_strlibopen (_pLuaState);
 //   lua_tablibopen(_pLuaState);
 //   lua_mathlibopen (_pLuaState);

	extern  lua_State *L;
	_pLuaState = L;

	lua_dofile( _pLuaState, "scripts/lua/table/scripts.lua" );
	return true;
}

bool CScriptMgr::Clear()
{
	//if( _pLuaState )
	//{
	//	lua_close(_pLuaState);
	//	_pLuaState = NULL;
	//}

	//if( _pStdErr )
	//{
	//	fclose( _pStdErr );
	//	_pStdErr = NULL;
	//}

	if( !CScript::Clear() ) return false;

	return true;
}

bool CScriptMgr::DoFile( const char* szLuaFile )
{
	LG( "lua", "DoFile(%s)\n", szLuaFile );
	return lua_dofile( _pLuaState, szLuaFile )!=0;
}

bool CScriptMgr::DoString( const char* szLuaString )
{
	LG( "lua", "DoString(%s)\n", szLuaString );
	FILE *fp = fopen("luaexec.txt", "wt");
	if(fp==NULL) return false;
	fwrite(szLuaString, strlen(szLuaString), 1, fp); 
	fclose(fp);
	return lua_dofile(_pLuaState, "luaexec.tmp")!=0;
}

bool CScriptMgr::DoString( const char* szFunc, const char* szFormat, ... )
{
	const double value = 1081000000.0;
	double dd = value / 1000.0 * 1000.0;
	if( dd!=value ) 
	{
		_control87( _CW_DEFAULT, 0xfffff );
		LG( "float", g_oLangRec.GetString(380), szFunc, szFormat );
	}

	int narg, nres;		// ����������ֵ����

	va_list vl;
	va_start( vl, szFormat );
	lua_getglobal( _pLuaState, szFunc );
	if (!lua_isfunction(_pLuaState, -1)) // ���Ǻ�����
	{
		lua_settop(_pLuaState, 0);
		LG( "luaerror", "Func is Error, Func:%s, Fromat:%s\n", szFunc, szFormat );
		return false;
	}

	narg = 0;
	while( *szFormat )
	{
		switch( *szFormat++ )
		{
		case 'f':
			lua_pushnumber( _pLuaState, va_arg(vl,double) );
			break;
		case 'd':
			lua_pushnumber( _pLuaState, va_arg(vl,int) );
			break;
		case 'u':
			lua_pushnumber( _pLuaState, va_arg(vl,unsigned int) );
			break;
		case 's':
			lua_pushstring( _pLuaState, va_arg(vl,char*));
			break;
		case '-':
			goto endwhile;
		default: 			
			lua_settop(_pLuaState, 0);
			LG( "luaerror", "Param Error, Func:%s, Fromat:%s\n", szFunc, szFormat );
			return false;
		}
		narg++;
		luaL_checkstack( _pLuaState, 1, "too many arguments" );
	}

endwhile:

	nres = (int)strlen(szFormat);
	if( lua_pcall( _pLuaState, narg, nres, 0 )!=0 )
	{
		lua_settop(_pLuaState, 0);
		LG( "luaerror", "Func call is error, Func:%s, Fromat:%s\n", szFunc, szFormat );
		return false;
	}

	nres = -nres;
	while( *szFormat )
	{
		switch( *szFormat++ )
		{
		case 'f':
			if( !lua_isnumber( _pLuaState, nres ) )
			{
				lua_settop(_pLuaState, 0);
				LG( "luaerror", "return value(f) is error, Func:%s, Fromat:%s\n", szFunc, szFormat );
				return false;		
			}

			*va_arg( vl, double* ) = (double)lua_tonumber( _pLuaState, nres );
			break;
		case 'd':
			if( !lua_isnumber( _pLuaState, nres ) )
			{
				lua_settop(_pLuaState, 0);
				LG( "luaerror", "return value(d) is error, Func:%s, Fromat:%s\n", szFunc, szFormat );
				return false;		
			}

			*va_arg( vl, int* ) = (int)lua_tonumber( _pLuaState, nres );
			break;
		case 'u':
			if( !lua_isnumber( _pLuaState, nres ) )
			{
				lua_settop(_pLuaState, 0);
				LG( "luaerror", "return value(u) is error, Func:%s, Fromat:%s\n", szFunc, szFormat );
				return false;		
			}

			*va_arg( vl, unsigned int* ) = (unsigned int)lua_tonumber( _pLuaState, nres );
			break;
		case 's':
			if( !lua_isstring( _pLuaState, nres ) )
			{
				lua_settop(_pLuaState, 0);
				LG( "luaerror", "return value(s) is error, Func:%s, Fromat:%s\n", szFunc, szFormat );
				return false;		
			}
	
			*va_arg( vl, string* ) = lua_tostring( _pLuaState, nres );
			break;
		default:
			lua_settop(_pLuaState, 0);
			LG( "luaerror", "return value(?) is error, Func:%s, Fromat:%s\n", szFunc, szFormat );
			return false;		
		}
		nres++;
	}
	va_end(vl);
	lua_settop(_pLuaState, 0);
	return true;
}

string	CScriptMgr::GetStoneHint( const char* szHintFun, int Lv )
{
	const double value = 1081000000.0;
	double dd = value / 1000.0 * 1000.0;
	if( dd!=value ) 
	{
		_control87( _CW_DEFAULT, 0xfffff );
		LG( "float", g_oLangRec.GetString(381), szHintFun, Lv );
	}

	lua_getglobal(_pLuaState, szHintFun);
	if (!lua_isfunction(_pLuaState, -1)) // ���Ǻ�����
	{
		lua_pop(_pLuaState, 1);
		return g_oLangRec.GetString(382);
	}

	int nParamNum = 0;
	lua_pushnumber( _pLuaState, Lv );
	nParamNum = 1;
	int nState = lua_pcall(_pLuaState, nParamNum, LUA_MULTRET, 0);
	if (nState != 0)
	{
		LG("lua_err", "DoString %s\n", szHintFun);
		lua_pop( _pLuaState, 2 );
		return "lua_pcall error";
	}

	string hint;
	int nRetNum = 1;
	if (!lua_isstring(_pLuaState, -1))
	{
		LG("error", g_oLangRec.GetString(383));
	}
	else
	{
		hint = lua_tostring(_pLuaState, -1);
	}
	lua_pop(_pLuaState, nRetNum);
	return hint;
}
