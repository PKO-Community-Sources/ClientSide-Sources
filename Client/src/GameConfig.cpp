#include "stdafx.h"
#include "GameConfig.h"
#include "GameApp.h"

CGameConfig g_Config;


CGameConfig::CGameConfig()
{
	g_bBinaryTable	= TRUE;// 此变量决定了表格使用是否二进制
	SetDefault();
	Load("scripts/kop.cfg"); // 读配置文件
}

void CGameConfig::SetDefault()   // 默认配置
{
	memset(m_ChaList, 0, sizeof(SPlaceCha) * 20);

	m_bAutoLogin	    = FALSE; 
	
	m_bFullScreen       = FALSE;
    m_bEnableMusic      = TRUE;
	
	m_nChaCnt			= 0;  // 角色个数

	m_LightColor[0]     =  1.0f; 
	m_LightColor[1]     =  1.0f;
	m_LightColor[2]     =  1.0f;
	m_LightDir[0]       =  1.0f;
	m_LightDir[1]       =  1.0f;
	m_LightDir[2]       = -1.0f;

    m_bNoObj            = FALSE; 
    m_bEditor           = FALSE; //  游戏编辑器
    m_btScreenMode      = 0;

    m_nMaxChaType       = 350;
	m_nMaxSceneObjType  = 800;
	m_nMaxEffectType    = 14000;		// 用于增加精炼道具的特效数	modify by Michael 2005-11-9
    //m_nMaxItemType      = 6000;
	m_nMaxItemType      = 20000;
	m_nMaxResourceNum	= 3000;        


	m_fCameraVel		= 0;  
	m_fCameraAccl		= 0;

	m_nCreateScene		= 1;          // 初始场景


	nLeftHand            = 0;      // 单机左右手武器设置
    nRightHand           = 0;

	m_bCheckOvermax      = TRUE; 
    m_nSendHeartbeat     = 30;
	m_nConnectTimeOut	 = 0;   // 网络连接超时

    m_bEnableLGMsg       = TRUE;     //   允许弹出LG-Box
    m_bEnableLG          = TRUE;     //   允许输出LG信息

	m_bRenderSceneObj = TRUE;  //场景物体实施
	m_bRenderCha      = TRUE;  //角色实施
	m_bRenderEffect   = TRUE;
	m_bRenderTerrain  = TRUE;
	m_bRenderUI       = TRUE;
	m_bRenderMinimap  = TRUE;
	m_bRender         = TRUE;

	m_bMThreadRes	  = TRUE;	//线程资源载入

    m_dwFullScreenAntialias = 0;

    m_fLgtFactor = 0.0f;
    m_dwLgtBkColor = 0xffc0c0c0;
    // 用于网络切换地图时的参数
	m_dwMaxCha	= 300;   
	m_dwMaxEff	= 500;
	m_dwMaxItem = 400;
	m_dwMaxObj	= 600;

	strcpy(m_szMD5Pass, "");  // 填写的MD5密码
	memset( m_szVerErrorHTTP, 0, sizeof(m_szVerErrorHTTP) ); 

	m_IsShowConsole = false; // 是否可以操作控件台
	m_IsMoveClient = true;
	m_IsBill = false; //  是否充值

	// Add by lark.li 20080429 for res
	memcpy(m_szFontName1, "", sizeof(m_szFontName1));
	memcpy(m_szFontName2, "", sizeof(m_szFontName1));
	// Res

	// Add by lark.li 20080825 begin
	m_nMovieW = -1;
	m_nMovieH = -1;
	// End
}

void CGameConfig::Load(char *pszFileName)   //  读kop.cfg
{ 
	LG("init", "Load Game Config File(Text Mode) [%s]\n", pszFileName);
	
	ifstream in(pszFileName);
	if(in.is_open()==0)  //是否能打开
	{
		return;
	}
	string strPair[2];
	string strComment;
	string strLine;
	char szLine[255];
	while(!in.eof())  //是否到达文件尾
	{
		in.getline(szLine, 255);
		strLine = szLine;
		int p = (int)(strLine.find("//"));//获取注释和代码
		if(p!=-1)  
		{
			string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		Util_TrimString(strLine);   //  丢失空格修正
		if(strLine.size()==0) continue;
		if(strLine[0]=='[') 
		{
			Log("\n%s\n", strLine.c_str());
			continue;
		}
		
		int n = Util_ResolveTextLine(strLine.c_str(), strPair, 2, '='); 
		if(n < 2) continue;
        string strKey   = strPair[0];
		string strValue = strPair[1];
		
		if(strKey=="autologin") //自动登录
		{
			m_bAutoLogin = Str2Int(strValue);
		}
		else if(strKey=="cha") // 固定位置摆放的角色, For Demo
		{
			string strCha[5];
			int nCnt = Util_ResolveTextLine(strValue.c_str(), strCha, 3, ',');
			if(nCnt==3 && m_nChaCnt < 20)
			{
				m_ChaList[m_nChaCnt].nTypeID = Str2Int(strCha[0]);
				m_ChaList[m_nChaCnt].nX      = Str2Int(strCha[1]);
				m_ChaList[m_nChaCnt].nY      = Str2Int(strCha[2]);
				m_nChaCnt++;
			}
		}
		else if(strKey=="fullscreen")  //全屏
		{ 
			m_bFullScreen = Str2Int(strValue); 
		}
		else if(strKey=="screenmode")  //  屏幕模式
		{
			m_btScreenMode = (BYTE)(Str2Int(strValue));
		}
		else if(strKey=="light_dir")  
		{
			string strLight[3];
			int nCnt = Util_ResolveTextLine(strValue.c_str(), strLight, 3, ',');
			if(nCnt==3)
			{
				m_LightDir[0] = Str2Float(strLight[0]);
				m_LightDir[1] = Str2Float(strLight[1]);
				m_LightDir[2] = Str2Float(strLight[2]);
			}
		}
   		else if(strKey=="light_color")
		{
			string strColor[3];
			int nCnt = Util_ResolveTextLine(strValue.c_str(), strColor, 3, ',');
			if(nCnt==3)
			{
				m_LightColor[0] = Str2Float(strColor[0]);
				m_LightColor[1] = Str2Float(strColor[1]);
				m_LightColor[2] = Str2Float(strColor[2]);
			}
		}else if(strKey=="fogcolorR") // 雾颜色
		{
			m_iFogR = Str2Int(strValue);
		}else if(strKey=="fogcolorG")
		{
			m_iFogG = Str2Int(strValue);
		}else if(strKey=="fogcolorB") 
		{
			m_iFogB = Str2Int(strValue);
		}else if(strKey=="fogexp2")  // 雾密度
		{
			m_fExp2 = Str2Float(strValue);
		}
        else if(strKey=="noobj")
        {
            m_bNoObj = Str2Int(strValue); 
        }
 		else if(strKey=="eyeX")   
		{
			eyeX = Str2Float(strValue);
		}
		else if(strKey=="eyeY")
		{
			eyeY = Str2Float(strValue);
		}
		else if(strKey=="eyeZ")
		{
			eyeZ = Str2Float(strValue);
		}
		else if(strKey=="refX")
		{
			refX = Str2Float(strValue);
		}
		else if(strKey=="refY")
		{
			refY = Str2Float(strValue);
		}	
		else if(strKey=="refZ")
		{
			refZ = Str2Float(strValue);
		}else if(strKey=="fov")
		{
			fov = Str2Float(strValue); //  视野
		}else if(strKey=="Aspect")
		{
			fAspect = Str2Float(strValue); // 外表
		}else if(strKey=="near")
		{
			fnear = Str2Float(strValue);
		}else if(strKey=="far")
		{
			ffar = Str2Float(strValue);
		}//
		else if( strKey=="CreateScene" )
		{
			m_nCreateScene = Str2Int(strValue);
		}
        else if(strKey=="music")
        {
            m_bEnableMusic = Str2Int(strValue);
        }
		else if(strKey=="cameraVel")
		{
			m_fCameraVel = Str2Float(strValue);
		}
		else if(strKey=="cameraAccl")
		{
			m_fCameraAccl = Str2Float(strValue);
		}
        else if( strKey=="left_hand" ) // 左手武器
        {
            nLeftHand = Str2Int(strValue);
        }
        else if( strKey=="right_hand" )// 右手武器
        {
            nRightHand = Str2Int(strValue);
        }
		else if( strKey=="check_overmax")
		{
			m_bCheckOvermax = Str2Int(strValue);
		}
        else if( strKey=="send_heartbeat" )
        {
            m_nSendHeartbeat = Str2Int(strValue);
        }
        else if( strKey=="enable_lg_msg" )
        {
            m_bEnableLGMsg = Str2Int(strValue);  // 是否允许弹出LG-Box
        }
        else if( strKey=="enable_lg" )
        { 
            m_bEnableLG = Str2Int(strValue);   // 是否允许输出LG信息
        }
		else if(strKey=="sceneobj_render")
		{
			m_bRenderSceneObj = Str2Int(strValue);
		}
		else if(strKey=="cha_render")
		{
			m_bRenderCha = Str2Int(strValue);
		}
		else if(strKey=="effect_render")
		{
			m_bRenderEffect = Str2Int(strValue);
		}
		else if(strKey=="terrain_render")
		{
			m_bRenderTerrain = Str2Int(strValue);
		}
		else if(strKey=="ui_render")
		{
			m_bRenderUI = Str2Int(strValue);
		}
		else if(strKey=="minimap_render")
		{
			m_bRenderMinimap = Str2Int(strValue);//是否实施小地图
		}
		else if(strKey=="render")
		{
			m_bRender = Str2Int(strValue);
		}
		else if(strKey=="multithreadres")
		{
			m_bMThreadRes = Str2Int(strValue);
		}
		else if(strKey=="fullscreen_antialias")
		{
			m_dwFullScreenAntialias = Str2Int(strValue);
		}
		else if(strKey=="connect_time_out")
		{
			m_nConnectTimeOut = 1000 * Str2Int(strValue);
		}		
		else if(strKey=="lgt_factor")
		{
            m_fLgtFactor = Str2Float(strValue);
		}		
		else if(strKey=="lgt_bkcolor")
		{
			string strColor[3];
			int nCnt = Util_ResolveTextLine(strValue.c_str(), strColor, 3, ',');
			if(nCnt==3)
			{
                lwDwordByte4 c;
                c.b[3] = 0xff;
				c.b[2] = Str2Int(strColor[0]);
				c.b[1] = Str2Int(strColor[1]);
				c.b[0] = Str2Int(strColor[2]);
                m_dwLgtBkColor = c.d;
            }
		}		
		else if( strKey=="MaxChaNum" )
		{
			m_dwMaxCha = Str2Int(strValue);
		}
		else if( strKey=="MaxEffNum" )
		{
			m_dwMaxEff = Str2Int(strValue);
		}
		else if( strKey=="MaxItemNum" )
		{
			m_dwMaxItem = Str2Int(strValue);
		}
		else if( strKey=="MaxObjNum" )
		{
			m_dwMaxObj = Str2Int(strValue);
		}
		else if(strKey=="md5pass") // 填写的MD5密码
		{
			strcpy(m_szMD5Pass, strValue.c_str());
		}
		else if( strKey=="console" )
		{
			m_IsShowConsole = Str2Int(strValue)!=0 ? true : false;
		}
		else if( strKey=="HTTP" ) // 版本不匹配时调用的网页
		{
			strcpy(m_szVerErrorHTTP, strValue.c_str());			 
		}
		else if( strKey=="client_move" )
		{
			// m_IsMoveClient = Str2Int(strValue)!=0 ? true : false;
		}	
		else if( strKey=="bill" )
		{
			//m_IsBill = Str2Int(strValue)!=0 ? true : false;
			// 改成宏定义
#ifdef USED_BILL
			m_IsBill = true;
#else
			m_IsBill = false;
			
#endif
		}
		// Add by lark.li 20080201
		else if( strKey=="locale" )
		{
			strcpy(m_szLocale, strValue.c_str());
		}
		else if( strKey=="path" )
		{
			strcpy(m_szResPath, strValue.c_str());
		}
		else if( strKey=="fontname1" )
		{
			strcpy(m_szFontName1, strValue.c_str());
		}
		else if( strKey=="fontname2" )
		{
			strcpy(m_szFontName2, strValue.c_str());
		}
		// End
		// Add by lark.li 20080825 begin
		else if( strKey=="movie_w" )
		{
			m_nMovieW = Str2Int(strValue);
		}
		else if( strKey=="movie_h" )
		{
			m_nMovieH = Str2Int(strValue);
		}
		// End
   	}
	in.close(); // 关闭文件
}

void CGameConfig::SetMoveClient( bool v )  // 设置是否与客户端同步
{
	m_IsMoveClient = v;
	g_pGameApp->SysInfo( g_Config.m_IsMoveClient ? g_oLangRec.GetString(142) : g_oLangRec.GetString(141) );
}
