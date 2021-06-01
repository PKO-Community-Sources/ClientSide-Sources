#include "Stdafx.h"
#include "Character.h"
#include "Point.h"
#include "MPShadeMap.h"
#include "mpparticlectrl.h"
#include "Scene.h"
#include "SceneItem.h"
#include "CharacterAction.h"
#include "actor.h"
#include "STStateObj.h"
//add by ALLEN 2007-10-16
#include "state_reading.h"

#include "EffectObj.h"
#include "SkillRecord.h"
#include "worldscene.h"
#include "ItemRecord.h"
#include "EffectObj.h"
#include "terrainattrib.h"
#include "GameApp.h"
#include "MusicSet.h"
#include "STAttack.h"
#include "UIHeadSay.h"
#include "UICommand.h"
#include "CommFunc.h"
#include "effdelay.h"
#include "SkillStateRecord.h"
#include "uistartform.h"
#include "netprotocol.h"
#include "uiboatform.h"
#include "steadyframe.h"
#include "event.h"
#include "chastate.h"
#include "shipset.h"
#include "SceneHeight.h"
#include "stpose.h"
#include "UIBoothForm.h"
#include "stmove.h"
#include "UIEquipForm.h"
#include "UIItemCommand.h"
#include "UIMiniMapForm.h"
#include <string> 
#include <sstream>

extern CGameApp*		g_pGameApp;
extern bool				g_IsShowShop;

CSkillRecord* CCharacter::_pDefaultSkillInfo = NULL;
CSkillRecord* CCharacter::_pReadySkillInfo = NULL;
bool CCharacter::_IsShowShadow = true;
bool CCharacter::_ShowApparel = true;
bool CCharacter::_ShowEffects = true;
//static bool onmount = true;
// �������
enum eSpecial
{
	enumAlphaIn     = 1,		// ����
	enumAlphaOut    = 2,		// ����
	enumDrop        = 3,		// �³�
	enumFastRaise   = 4,		// ��������
	enumFastDrop    = 5,		// �����³�
	enumHelixPos    = 6,		// ��ת
	enumHelixYaw    = 7,		// ��ת
	enumHelixOut    = 8,		// ����������
	enumHelixIn     = 9,		// ������С����
    enumDisappear   = 10,       // ������ʧ
	enumCyclone		= 11,		// 쫷�
};

#ifdef _LOG_NAME_
bool CCharacter::IsShowLogName=false;
#endif

inline int GetWeaponBackDummy( int nWeaponType, bool isLeftHand )
{
    int nBackDummy = -1;
    if( isLeftHand )
    {
        switch( nWeaponType )
        {
        case 2:
        case 5:
        case 9:
        case 10:
            nBackDummy = 21; 
            break;
        case 1:
        case 3:
        case 11:
            nBackDummy = 22; 
            break;
        case 4:
        case 7:
        case 8:
            nBackDummy = 23; 
            break;
        }
    }
    else
    {
        switch( nWeaponType )
        {
        case 1:
        case 2:
        case 5:
        case 9:
        case 10:
            nBackDummy = 21; 
            break;
        case 3:
        case 11:
		case 18:
		case 19:
            nBackDummy = 22; 
            break;
        case 4:
        case 7:
        case 8:
            nBackDummy = 23; 
            break;
        }
    }
    return nBackDummy;
}

inline void SetPreName( int nItem, char* szName, DWORD& dwColor )
{
	DWORD COLOR_SKYBLUE = D3DCOLOR_ARGB(255, 168, 168, 255);

	switch( nItem )
	{
	case 3936:	strcpy( szName, g_oLangRec.GetString(7) );   dwColor = D3DCOLOR_ARGB(255,192,192,192); return;
	case 3937:	strcpy( szName, g_oLangRec.GetString(8) );   dwColor = D3DCOLOR_ARGB(255,000,255,012); return;
	case 3938:	strcpy( szName, g_oLangRec.GetString(9) );   dwColor = D3DCOLOR_ARGB(255,217,255,119); return;
	case 3939:	strcpy( szName, g_oLangRec.GetString(10) );  dwColor = D3DCOLOR_ARGB(255,070,182,240); return;
	case 3940:	strcpy( szName, g_oLangRec.GetString(11) );  dwColor = D3DCOLOR_ARGB(255,255,207,000); return;
	case 3941:	strcpy( szName, g_oLangRec.GetString(12) );  dwColor = D3DCOLOR_ARGB(255,255,000,000); return;
	case 3942:	strcpy( szName, g_oLangRec.GetString(13) );  dwColor = D3DCOLOR_ARGB(255,241,014,240); return;

	case 5331:	strcpy( szName, g_oLangRec.GetString(922) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,192,192,192); return;
	case 5332:	strcpy( szName, g_oLangRec.GetString(923) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,135,135,135); return;
	case 5333:	strcpy( szName, g_oLangRec.GetString(924) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,255,255,255); return;
	case 5334:	strcpy( szName, g_oLangRec.GetString(925) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,070,182,240); return;
	case 5335:	strcpy( szName, g_oLangRec.GetString(926) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,000,255,012); return;
	case 5336:	strcpy( szName, g_oLangRec.GetString(927) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,217,255,119); return;
	case 5337:	strcpy( szName, g_oLangRec.GetString(928) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,255,207,000); return;
	case 5338:	strcpy( szName, g_oLangRec.GetString(929) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,255,127,000); return;
	case 5339:	strcpy( szName, g_oLangRec.GetString(930) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,255,000,000); return;
	case 5340:	strcpy( szName, g_oLangRec.GetString(931) ); dwColor = COLOR_SKYBLUE; return;//D3DCOLOR_ARGB(255,241,014,240); return;

	// TOM版本
	case 822:	strcpy( szName, g_oLangRec.GetString(14) );  dwColor = D3DCOLOR_ARGB(255,255,000,000); return;	// 荣誉勋章
	case 823:	strcpy( szName, g_oLangRec.GetString(15) );  dwColor = D3DCOLOR_ARGB(255,241,014,240); return;	// 财富勋章
	default: szName[0] = '\0';
	}
}

static void __keyframe_proc( DWORD type, DWORD pose_id, DWORD key_id, DWORD key_frame, void* param )
{
	CCharacter* pCha = ((CCharacter*)param);
	switch( type )
	{
	case KEYFRAME_TYPE_BEGIN:
		{
            pCha->GetActor()->ActionBegin( pCha->GetCurPoseType() );

			if( pCha->GetCurPoseType()>6 &&pCha->GetCurPoseType()<15 )
			{
				CEffectObj	*pEffect;
				CSceneItem* item = pCha->GetLinkItem( LINK_ID_RIGHTHAND );		
				if( item && item->GetItemDrapID()>0 )
				{
					pEffect = pCha->GetScene()->GetFirstInvalidEffObj();
					if(pEffect)
					{
						if(pEffect->Create( item->GetItemDrapID() ) )
						{
							pEffect->setFollowObj((CSceneNode*)item,NODE_ITEM);
							pEffect->Emission( -1, &pCha->GetPos(), NULL );
							pEffect->SetValid(TRUE);
						}
					}
				}
				item = pCha->GetLinkItem( LINK_ID_LEFTHAND );		
				if( item && item->GetItemDrapID()>0 )
				{
					pEffect = pCha->GetScene()->GetFirstInvalidEffObj();
					if(pEffect)
					{
						if(pEffect->Create( item->GetItemDrapID() ) )
						{
							pEffect->setFollowObj((CSceneNode*)item,NODE_ITEM);

							pEffect->Emission( -1, &pCha->GetPos(), NULL );
							pEffect->SetValid(TRUE);
						}
					}
				}

			}
			break;
		}
	case KEYFRAME_TYPE_KEY:
		pCha->ActionKeyFrame( key_id );
		break;
	case KEYFRAME_TYPE_END:
		pCha->GetActor()->ActionEnd( pCha->GetCurPoseType() );
		break;
	}
}

// ========================
CCharacter::CCharacter()
: _bEnableAI(FALSE),
 _isArrive(true),
 _nTurnCnt(0),
 _pszFootMusic(NULL),
 _pszWhoopMusic(NULL),
 _fMaxOpacity(1.0f),
 _pNpcStateItem(NULL),
 _eChaModalType(enumMODAL_MAIN_CHA),
 _eChaCtrlType(enumCHACTRL_PLAYER),
 _IsForUI(false),
 _pDefaultChaInfo(NULL),
 _bShowSecondName(TRUE),
 _sPhotoID(0),
 _nNpcType(0),
 _eMainType(enumMainNone),
 _nLeaderID(0),
 _chGMLv(0),
 _pEvent(NULL),
 _pShipInfo(NULL),
 _dwHumanID(0),
 _fMapHeight( 0.0f ),
 _nGuildID(0),
 _nGuildPermission(0),
 _dwGuildColor(COLOR_BLACK),
 _dwNameColor(COLOR_WHITE),
 _szPreColor(COLOR_WHITE),
 _sReliveTime(60),				// Ĭ��Ϊ1����
 _nPatrolX(0),
 _nPatrolY(0),
 _pBoatFog(NULL),
 _pShopItem(NULL),
 _lSideID(0),
 _pSideShade(0),
 _nServerX(0),
 _nServerY(0),
 _nDanger(0),
 mParentNode(NULL),
 mParentBoneID(0)
{
    memset( _szName,0,sizeof(_szName) );
    memset( _szHumanName,0,sizeof(_szHumanName) );
	memset( &_vPos, 0, sizeof(_vPos) );
    memset( _szGuildName,0,sizeof(_szGuildName) );	
    memset( _szGuildMotto,0,sizeof(_szGuildMotto) );
	memset( _szPreName, 0, sizeof(_szPreName) );
    memset( _szShopName,0,sizeof(_szShopName) );
    memset( _ItemFace,0,sizeof(_ItemFace) );
	memset( _pItemFaceEff,0,sizeof(_pItemFaceEff) );

	strcpy( _szName, "Player" );
	strcpy( _szSecondName, "");
	_bUpdate = false;

#ifdef _LOG_NAME_
	strcpy( _szLogName, "LogName" );
#endif

	_pActor			= new CActor( this );
	
    _Attr.set(ATTR_MXHP,  20);
    _Attr.set(ATTR_HP,     20);
    _Attr.set(ATTR_MSPD, 400);

	_pHeadSay		= new CHeadSay( this );

    memset( _pHandItem, 0, sizeof(_pHandItem) );
	memset( _pHandItemEff, 0, sizeof(_pHandItemEff) );

	memset( &_stChaPart, 0, sizeof(_stChaPart) );

	_pChaState = new CChaStateMgr(this);
	_ChaState.AllTrue();

	_pSceneHeight = new CSceneHeight(this);
	_fMoveSpeed = 1.0f;
}

CCharacter::~CCharacter() 
{
	
	SAFE_DELETE(_pActor);       // UI��������
	SAFE_DELETE(_pChaState);    // UI��������
	SAFE_DELETE(_pSceneHeight); // UI��������
} 

int CCharacter::LoadCha( const LoadChaInfo* info )
{
    if( _Create( info->cha_id, 0 ) == 0 )
        return FALSE;

    if( info->bone[0] != 0 )
    {
        if( FAILED( LoadBone( info->bone ) ) )
        {
            LG("error", g_oLangRec.GetString(16) );
            return FALSE;
        }
    }

    for( DWORD i = 0; i < 5; i++ )
    {
        if( info->part[i][0] == 0 )
            continue;

        if( FAILED( LoadPart( i, info->part[i] ) ) )
        {
            LG("error", g_oLangRec.GetString(16) );
            return FALSE;
        }

    }

    for( DWORD i = 0; i < 4; i++ )
    {
        if( strlen(info->prop[i]) == 0 )
            continue;

        CSceneItem* p = this->GetScene()->AddSceneItem( info->prop[i] );

        if( p == NULL )
        {
            LG("error", "msgLoad Ship Power System error!\n");
            return FALSE;
        }
        p->PlayDefaultAnimation();

        lwSceneItemLinkInfo ili;
        ili.data = 99;
        ili.id = 0;
        ili.obj = p;
        ili.link_item_id = 0;
        ili.link_parent_id = i;

        this->SetSubObjItemLink( 2, &ili );
    }

    return TRUE;
}

BOOL CCharacter::_Create(int nCharTypeID,int nType)
{
	return TRUE;
}

bool CCharacter::LoadBoat( stNetChangeChaPart& stPart )
{
	memcpy( &_stChaPart, &stPart, sizeof(_stChaPart) );
	DWORD dwBuf[ 8 ] = { 0 };
	if( _pShipInfo=ConvertPartTo8DWORD( stPart, dwBuf ) )
	{
		//if(_pShipInfo->sNumHeader||_pShipInfo->sNumEngine||_pShipInfo->sNumCannon||_pShipInfo->sNumEquipment)
		//	_bUpdate = true;
		if(stPart.sBoatID > 10)
			_bUpdate = true;
		else
			_bUpdate = false;

		return LoadShip( stPart.sPosID, dwBuf ) ? true : false;
	}

	return false;
}

void CCharacter::_UpdateValid(BOOL bValid)
{
	if(bValid == FALSE)
	{
		setSideID( 0 );
		if( GetDrunkState() ) CCharacterModel::SetDrunkState( FALSE );
		_pChaState->ChaDestroy();

		DieTime();
		if( _pEvent )
		{
			_pEvent->SetIsValid( false );
			_pEvent = NULL;
		}
        _DetachAllItem();
        for( int i=0; i<enumEQUIP_NUM; i++ )
        {
            if( _pHandItem[i] ) 
            {
                _pHandItem[i]->SetValid( FALSE );
                _pHandItem[i] = NULL;
            }
		}

		for( int i=0; i<ITEM_FACE_MAX; i++ )
		{
			if( _pItemFaceEff[i] )
			{
				_pItemFaceEff[i]->SetValid( FALSE );
				_pItemFaceEff[i] = NULL;
			}
		}
	    memset( _ItemFace,0,sizeof(_ItemFace) );

		GetScene()->HandleSceneMsg(SCENEMSG_CHA_DESTROY, getID());
	}
	else
	{
		SetPoseKeyFrameProc( __keyframe_proc, (void*)this );
	}
}

void CCharacter::FrameMove(DWORD dwTimeParam)
{
	CSceneNode::FrameMove(dwTimeParam);	

	if( !_isArrive )
	{
		if( _IsMoveTimeType )
		{
			static float dis;
			static D3DXVECTOR2 vTmp;

			dis = (float)( CGameApp::GetCurTick() - _dwStartTime ) * _fMoveSpeed;
			if( dis>_fMoveLen )
			{
				setPos( (int)_vMoveEnd.x, (int)_vMoveEnd.y );

				_isArrive = true;
			}
			else
			{
				vTmp = _vMoveStart + _vMoveDir * dis;
				setPos( (int)vTmp.x, (int)vTmp.y );
			}
		}
		else if( GetChaState()->IsTrue(enumChaStateMove) )
		{
			// ���¼����µ�ProgressRate(������ǰ��λ�ú͸߶�)------------------------------------------------------------------------------
			if( _nTurnCnt>1 )
				_fStepProgressRate = _fStepProgressRate + _fStep * CWaitMoveState::GetMoveRate() / (float)_nTurnCnt;
			else
				_fStepProgressRate = _fStepProgressRate + _fStep * CWaitMoveState::GetMoveRate();

			_CalPos(_fStepProgressRate);
			if(_fStepProgressRate >= 1.0f)
			{
				_isArrive = true;				
			}
			else if( _isStopMove )
			{
				_isArrive = true;
			}
		}
	}
	else
	{
		// ÿ300msˢ��һ�θ߶�
		if( _pSceneHeight->FrameMove(dwTimeParam) )
		{
			_UpdateHeight();

			if( _pNpcStateItem )
			{
				//_pNpcStateItem->setHeightOff( (int)(GetDefaultChaInfo()->fHeight * 100.0f) );
				_pNpcStateItem->setHeightOff( (int)(GetDefaultChaInfo()->fHeight * ( GetIsFly() ? 130.0f : 100.0f ) ) );
			}
		}
	}

	if(_nTurnCnt)
    {
		// ���·���
        _nYaw+=_nAngleStep;
        SetYaw(Angle2Radian((float)_nYaw));
        UpdateYawPitchRoll();
        _nTurnCnt--;
    }

	_pActor->FrameMove(dwTimeParam);

	if( _Special.IsAny() )
	{
		static long x, y;
        static float f = 0.0f;

		if( _Special.IsTrue( enumDrop )  ) 
        {
            _nHeightOff--;
            _fMapHeight = 0.0f;
        }
		if( _Special.IsTrue( enumFastRaise ) ) 
        {
            _nHeightOff+=25;
            _fMapHeight = 0.0f;
        }
		if( _Special.IsTrue( enumFastDrop ) )
		{
			 _nHeightOff -= 25;
			 if( _nHeightOff<0 )
			 {
				 _nHeightOff = 0;
				 _Special.SetFalse( enumFastDrop );
			 }
            _fMapHeight = 0.0f;
		}
		if( _Special.IsTrue( enumAlphaOut ) )
		{
            f = GetOpacity() - 0.03f;
            if( f<0.0f ) 
            {
                f=0.0f;
                _Special.SetFalse( enumAlphaOut );
            }
			SetOpacity( f );
		}
		if( _Special.IsTrue( enumAlphaIn ) )
		{
			// �������ʱ��������Ч��ȡ��        
            f = GetOpacity() + 0.03f;
            if( f>_fMaxOpacity ) 
            {
                f=_fMaxOpacity;
                _Special.SetFalse( enumAlphaIn );
            }
            SetOpacity( f );
		}
		if( _Special.IsTrue( enumHelixOut ) )
		{
			_nHelixAngle+=13;
			_nHelixRadii+=20;
			::eddy( _nHelixCenterX, _nHelixCenterY, _nHelixAngle, _nHelixRadii, x, y );
			_nCurX = x;
			_nCurY = y;
		}
		if( _Special.IsTrue( enumHelixIn ) )
		{
			_nHelixAngle+=13;
			_nHelixRadii-=20;
			if( _nHelixRadii>0 )
			{
				::eddy( _nHelixCenterX, _nHelixCenterY, _nHelixAngle, _nHelixRadii, x, y );
				_nCurX = x;
				_nCurY = y;
			}
			else
			{
				_nCurX = _nHelixCenterX;
				_nCurY = _nHelixCenterY;
				_Special.SetFalse( enumHelixIn );
			}
		}
		if( _Special.IsTrue( enumHelixYaw ) )
		{
			setYaw( ( getYaw() + 18 ) % 360 );
		}
		if( _Special.IsTrue( enumHelixPos ) )
		{
			_nHelixAngle+=13;
			::eddy( _nHelixCenterX, _nHelixCenterY, _nHelixAngle, 200, x, y );
			_nCurX = x;
			_nCurY = y;
		}
		if( _Special.IsTrue( enumCyclone ) )
		{
			_nHelixAngle+=13;
			::eddy( _nHelixCenterX, _nHelixCenterY, _nHelixAngle, _nHelixRadii, x, y );
			_nCurX = x;
			_nCurY = y;

			if( _nHeightOff<=500 )
				_nHeightOff+=25;
		}
		_UpdateHeight();
	}

	_computeLinkedMatrix();

    // by lsh
    // ע�⣬�����FrameMove������������ã��������λ�����
    CCharacterModel::FrameMove();
	return;
}

void CCharacter::Render()
{ 
    if(!_pScene->IsPointVisible((float)_nCurX / 100.0f, (float)_nCurY / 100.0f)) return;
    
	g_Render.EnableZBuffer(TRUE);

	CSceneNode::Render();

    CCharacterModel::Render();
}

void CCharacter::RefreshLevel( int nMainLevel )
{
    if( IsMonster() )
    {
        int nDifLevel = getGameAttr()->get(ATTR_LV) - nMainLevel;
        if( nDifLevel>=7 )
        {
            _pHeadSay->SetNameColor( D3DCOLOR_ARGB(255,241,14,240) );
        }
        else if( nDifLevel>=5 )
        {
            _pHeadSay->SetNameColor( D3DCOLOR_ARGB(255,255,0,0) );
        }
        else if( nDifLevel>=3 )
        {
            _pHeadSay->SetNameColor( D3DCOLOR_ARGB(255,255,207,0) );
        }
        else if( nDifLevel>=-2 )
        {
            _pHeadSay->SetNameColor( D3DCOLOR_ARGB(255,255,255,255) );
        }
        else if( nDifLevel>=-4 )
        {
            _pHeadSay->SetNameColor( D3DCOLOR_ARGB(255,217,255,119) );
        }
        else if( nDifLevel>=-6 )
        {
            _pHeadSay->SetNameColor( D3DCOLOR_ARGB(255,0,255,12) );
        }
        else
        {
            _pHeadSay->SetNameColor( D3DCOLOR_ARGB(255,192,192,192) );
        }
    }
    else
    {
		_pHeadSay->SetNameColor(getNameColor());
    }
}

void CCharacter::RefreshUI(int nParam)
{
	_pHeadSay->SetLifeNum( _Attr.get(ATTR_HP), _Attr.get(ATTR_MXHP) );
	_pHeadSay->SetManaNum( _Attr.get(ATTR_SP), _Attr.get(ATTR_MXSP) );

	if( g_stUIBoat.GetHuman()==this )
	{
		g_stUIStart.RefreshMainLifeNum( _Attr.get(ATTR_HP), _Attr.get(ATTR_MXHP)) ;
		g_stUIStart.RefreshMainSP(_Attr.get(ATTR_SP), _Attr.get(ATTR_MXSP) );

		g_stUIStart.RefreshMainExperience(_Attr.get(ATTR_CEXP), _Attr.get(ATTR_CLEXP),_Attr.get(ATTR_NLEXP) );
		g_stUIStart.RefreshLv( _Attr.get(ATTR_LV) );
	}
}

BOOL CCharacter::WieldItem( const lwSceneItemLinkInfo* info )
{
    if( FAILED( MPCharacter::SetItemLink( info ) ) )
        return FALSE;

    CSceneItem* item = (CSceneItem*)info->obj;
    item->setAttachedCharacterID( getID() );

    return TRUE;
}

void CCharacter::DestroyLinkItem()
{
    for( DWORD i = 0; i < GetLinkItemNum(); i++ )
    {
        CSceneItem* obj = (CSceneItem*)GetLinkItem( i );
        obj->SetValid( 0 );
    }
}

int CCharacter::_GetTargetAngle(int nTargetX, int nTargetY, BOOL bBack)
{
	D3DXVECTOR2 dPosition = D3DXVECTOR2((float)nTargetX, (float)nTargetY) - D3DXVECTOR2((float)_nCurX, (float)_nCurY);
	if(bBack) dPosition*=-1;

	int nAngle = (int)(180.0f - Radian2Angle(atan2f(dPosition.x, dPosition.y)));

	return FixAngle(nAngle);
}

void CCharacter::setPos(int nX, int nY)
{
#ifdef _STATE_DEBUG
	if( IsMainCha() )
	{
		static DWORD tick = 0;
		int dis = GetDistance( nX, nY, _nCurX, _nCurY );
		if( dis > 300.0f )
			LG( "main_cha_pos", "\t\tpos:%d, %d, target:%d, %d, dis:%d, Tick:%d, FPS: %d\n", _nCurX, _nCurY, nX, nY, dis, GetTickCount() - tick, g_Render.GetFPS() );
		else
			LG( "main_cha_pos", "pos:%d, %d, target:%d, %d, dis:%d, Tick:%d, FPS: %d\n", _nCurX, _nCurY, nX, nY, dis, GetTickCount() - tick, g_Render.GetFPS() );
		tick = GetTickCount();
	}
#endif

	if( _Special.IsAny() )
	{
		_nHelixCenterX = nX;
		_nHelixCenterY = nY;
	}

	_nCurX = nX;
	_nCurY = nY;

	if( _pNpcStateItem )
	{
		_pNpcStateItem->setPos( nX, nY );
	}

    _UpdateHeight();
}

void CCharacter::setPos(int nX, int nY, int nZ)
{
	if( _Special.IsAny() )
	{
		_nHelixCenterX = nX;
		_nHelixCenterY = nY;
	}

	_nCurX = nX;
	_nCurY = nY;

    _nHeightOff = nZ;
	_nPoseHeightOff = nZ;

    //_vPos.x = (float)nX / 100.0f;
    //_vPos.y = (float)nY / 100.0f;
	//_vPos.z = (float)nZ / 100.0f;
    
    SetPos((float*)&_vPos);
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}

int CCharacter::FaceTo(int nAngle)
{
	if( !_pDefaultChaInfo->IsFace() )
		return getYaw();

	_nYaw  = FixAngle(_nYaw);

	nAngle = FixAngle(nAngle);

	if( IsBoat() )
	{
		_nAngleStep = (int)( 300.0f / (float)CGameApp::GetFrameFPS() );	
	}
	else
	{
		_nAngleStep = (int)( 600.0f / (float)CGameApp::GetFrameFPS() );	
	}
	
	if( !_isArrive && IsMainCha() && !IsBoat() )
	{
		_nAngleStep = (int)((float)_nAngleStep * 0.75f);
		if( _nAngleStep <= 0 ) _nAngleStep = 1;
	}

	int nDis = GetAngleDistance(_nYaw, nAngle);

	_nTurnCnt = (abs)(nDis) / _nAngleStep;

	int nLeft = nDis % _nAngleStep;

	if(nDis < 0) 
	{
		_nAngleStep*=-1;
	}

	if(nLeft!=0)
	{
		_nYaw+=nLeft;
		SetYaw(Angle2Radian((float)_nYaw));
        UpdateYawPitchRoll();
	}
    return nAngle;
}

bool CCharacter::ItemEffect(int nEffectID, int nItemDummy, int nAngle )
{
	if( nEffectID==0 || nItemDummy==-1 ) return false;

	CSceneItem* item = GetAttackItem();
	if ( !item ) return false;

	CEffectObj	*pEffect = GetScene()->GetFirstInvalidEffObj();
	if( !pEffect ) return false;

	if( !pEffect->Create( nEffectID ) )
		return false;

	static MPMatrix44 mat;
	if( item->GetObjDummyRunTimeMatrix( &mat, nItemDummy )>=0 )
	{
		pEffect->setFollowObj((CSceneNode*)item,NODE_ITEM, nItemDummy);
		if( nAngle!=999 ) pEffect->SetEffectDir( nAngle );
		pEffect->Emission( -1, (D3DXVECTOR3*)&mat._41, NULL );
	}
	else
	{
		pEffect->setFollowObj((CSceneNode*)item,NODE_ITEM);
		if( nAngle!=999 ) pEffect->SetEffectDir( nAngle );
		pEffect->Emission( -1, &GetPos(), NULL );
	}

	pEffect->SetValid(TRUE);
	return true;
}

void CCharacter::RefreshSelfEffects(){
	
	for(int i = 0;i<SKILL_STATE_MAXID;i++){
		CChaStateMgr*	states = GetStateMgr();
		if(states->HasSkillState(i)){
			
			CChaStateMgr::stChaState stateData = states->GetStateData(i);
			
			if(stateData.pEffect){
				stateData.pEffect->SetValid(FALSE);
				stateData.pEffect = NULL;
				if(_ShowEffects && stateData.pInfo->sEffect>0){
					stateData.pEffect = SelfEffect( stateData.pInfo->sEffect, stateData.pInfo->sDummy1, true );
				}
				
			}
			
			
			
		}		
	}
}

CEffectObj* CCharacter::SelfEffect(int nEffectID, int nDummy, bool isLoop, int nSize, int nAngle )
{
	if(!_ShowEffects){
		return NULL;
	}
	if( nEffectID<=0 ) return NULL;

	// added by Philip.Wu  2008-01-25 �Ҷ��������ﲻ������Щ��Ч
	if(g_stUIMap.IsPKSilver() && this->IsPlayer() && this->GetMainType()!=enumMainPlayer)
	{
		if(574 <= nEffectID && nEffectID <= 577) return 0;
	}
	CEffectObj	*pEffect = GetScene()->GetFirstInvalidEffObj();
	if( !pEffect ) return NULL;

	if( !pEffect->Create( nEffectID ) )
		return NULL;

    if( isLoop ) 
		pEffect->setLoop( isLoop );

	if( nSize>0 && pEffect->GetBaseSize()>0.0f )	// ���С����,��֧������
	{
		SkillCtrl ctrl;
		ctrl.fSize = (float)nSize / 100.0f / pEffect->GetBaseSize();
		pEffect->SetSkillCtrl( &ctrl );
	}
	static MPMatrix44 mat;

	if(IsBoat())
	{
		//xShipInfo* pInfo = ::GetShipInfo( GetPart().sBoatID );
		if(!_bUpdate)
		{
			if( nDummy>=0 && GetObjDummyRunTimeMatrix( &mat, nDummy,0 )>=0 )
			{
				if( nAngle!=999 ) pEffect->SetEffectDir( nAngle );
				pEffect->setFollowObj((CSceneNode*)this,NODE_CHA,nDummy);
				pEffect->Emission( -1, (D3DXVECTOR3*)&mat._41, NULL );
			}else
			{
				if( nAngle!=999 ) pEffect->SetEffectDir( nAngle );
				pEffect->setFollowObj((CSceneNode*)this,NODE_CHA);
				pEffect->Emission( -1, &GetPos(), NULL );
			}
		}else
			goto __ret;
	}else
	{
__ret:
		if( nDummy>=0 && GetObjDummyRunTimeMatrix( &mat, nDummy )>=0 )
		{
			if( nAngle!=999 ) 
				pEffect->SetEffectDir( nAngle );
			pEffect->setFollowObj((CSceneNode*)this,NODE_CHA,nDummy);
			pEffect->Emission( -1, (D3DXVECTOR3*)&mat._41, NULL );
		}
		else
		{
			if( nAngle!=999 ) 
				pEffect->SetEffectDir( nAngle );
			pEffect->setFollowObj((CSceneNode*)this,NODE_CHA);
			pEffect->Emission( -1, &GetPos(), NULL );
		}
	}

	pEffect->SetValid(TRUE);
	return pEffect;
}

CEffectObj* CCharacter::SkyEffect( int nEffectID, int nBeginDummy, int nItemDummy, int nSpeed, D3DXVECTOR3* pTarget, int nTargetChaID, CSkillRecord* pSkill )
{
	if( nEffectID<=0 ) return NULL;

	CEffectObj	*pEffect = GetScene()->GetFirstInvalidEffObj();
	if( !pEffect ) return NULL;

	if( !pEffect->Create( nEffectID ) )
		return NULL;

	static D3DXVECTOR3 pos;
	static MPMatrix44 mat;
	if( nBeginDummy>=0 && GetObjDummyRunTimeMatrix( &mat, nBeginDummy )>=0 )
	{
		pos = *(D3DXVECTOR3*)&mat._41;
	}
	else if( nItemDummy>=0 )
	{
		CSceneItem* item = GetAttackItem();		
		if( item && item->GetObjDummyRunTimeMatrix( &mat, nItemDummy )>=0 )
		{
			pos = *(D3DXVECTOR3*)&mat._41;
		}
		else
		{
			pos = GetPos();
		}
	}
	else
	{
		pos = GetPos();
	}

	if( pSkill )
	{	
		switch( pSkill->GetShape() )
		{
		case enumRANGE_TYPE_NONE:
		case enumRANGE_TYPE_SQUARE:		// ���Σ����ȣ����ȣ�
		case enumRANGE_TYPE_CIRCLE:		// Բ�Σ��뾶��
			break;
		case enumRANGE_TYPE_FAN:		// ���Σ��뾶���Ƕȣ�
			{
				pEffect->SetFanAngle( pSkill->GetParam()[1] );
				pEffect->SetMagicDist( (float)pSkill->GetParam()[0]/100.0f );
			}
		case enumRANGE_TYPE_STICK:		// ���Σ����ȣ����ȣ�
			{
				// ȡ����Ϊ�����Ч�����ϵ��߷ɳ�ʱ�����ȴ���ڵ���������֮��ʱ����Ч�ɵķ��򲻶�
				int x, y;
				GetDistancePos( GetCurX(), GetCurY(), int(pTarget->x * 100.0f), int(pTarget->y * 100.0f), 500, x, y );
				pTarget->x = (float)x / 100;
				pTarget->y = (float)y / 100;
			}
			break;
		};
	}

	pEffect->setOwerID( getID() );
	pEffect->SetVel( (float)nSpeed );
	pEffect->Emission( nTargetChaID, &pos, pTarget );
	pEffect->SetValid(TRUE);
	return pEffect;
}

bool CCharacter::ChangeReadySkill( int nSkillID )
{
	CSkillRecord *p =  GetSkillRecordInfo( nSkillID );
	
	if( !p )
	{
		g_pGameApp->AddTipText( g_oLangRec.GetString(17), p->szName );
		return false;
	}

	if( _pReadySkillInfo && _pReadySkillInfo->sID==nSkillID ) return true;

    if( !p->GetIsValid() )
    {
		g_pGameApp->SysInfo( g_oLangRec.GetString(18), p->szName );
        return false;
    }

	g_pGameApp->AddTipText( g_oLangRec.GetString(19), p->szName );
	_SetReadySkill( p );
    return true;
}

void CCharacter::_UpdateHeight()
{
    _vPos.x = (float)_nCurX / 100.0f;
    _vPos.y = (float)_nCurY / 100.0f;

    if( _nPoseHeightOff==ERROR_POSE_HEIGHT )
    {
        switch( _pDefaultChaInfo->chTerritory )
        {
        case 1:
            _fMapHeight = (float)_pDefaultChaInfo->sSeaHeight / 100.0f;
            break;
        default:
            _fMapHeight = _pScene->GetGridHeight(_vPos.x, _vPos.y);
        }

        _vPos.z = _fMapHeight + (float)( _nHeightOff ) / 100.0f;
    }
    else
    {
        _vPos.z = (float)( _nPoseHeightOff ) / 100.0f;
    }
    
    SetPos((float*)&_vPos); // ��������ø߶ȵ�ʵ��, _vPos.x��_vPos.y������
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}
//void	CCharacter::SetHieght(float fhei)
//{
//	if( _nPoseHeightOff==ERROR_POSE_HEIGHT )
//	{
//		switch( _pDefaultChaInfo->chTerritory )
//		{
//		case 1:
//			//_fMapHeight = (float)_pDefaultChaInfo->sSeaHeight / 100.0f;
//			_fMapHeight = fhei +_pScene->GetGridHeight(_vPos.x, _vPos.y);
//			break;
//		default:
//			_fMapHeight = _pScene->GetGridHeight(_vPos.x, _vPos.y);
//		}
//
//		_vPos.z = _fMapHeight + (float)( _nHeightOff ) / 100.0f;
//	}
//	else
//	{
//		_vPos.z = (float)( _nPoseHeightOff ) / 100.0f;
//	}
//
//	SetPos((float*)&_vPos); // ��������ø߶ȵ�ʵ��, _vPos.x��_vPos.y������
//	if( GetDrunkState() )
//	{
//		UpdateChaMatrix();
//	}
//}

void CCharacter::OperatorEffect( char oper, int x, int y )
{
	//switch( oper )
	//{
	//case 3:// �ϰ�
	//	if( IsBoat() )
	//	{
	//		D3DXVECTOR3 pos;
	//		pos.x = (float)x / 100.0f;
	//		pos.y = (float)y / 100.0f;
	//		pos.z = _pScene->GetGridHeight( pos.x, pos.y );

	//		CEffectObj* pEffect = SkyEffect( 1021, 2, -1, 2500, &pos );
 //           if( pEffect )
 //           {
 //               pEffect->GetEffDelay()->SetPosEffect( 157, pos );
 //           }
	//	}
	//	break;
	//case 1:// �º�
	//	if( IsPlayer() )
	//	{
	//		D3DXVECTOR3 pos;
	//		pos.x = (float)x / 100.0f;
	//		pos.y = (float)y / 100.0f;
	//		pos.z = _pScene->GetGridHeight( pos.x, pos.y );

	//		CEffectObj* pEffect = SkyEffect( 1021, 2, -1, 2500, &pos );
 //           if( pEffect )
 //           {
 //               pEffect->GetEffDelay()->SetPosEffect( 158, pos );
 //           }
	//	}
	//	break;
	//}
}

void CCharacter::setName(const char *pszName)
{
	strncpy( _szName, pszName, sizeof(_szName) );
	_pHeadSay->SetName( _szName );
}

void CCharacter::UpdateTileColor()
{
    BYTE r = 255;
    BYTE g = 255;
    BYTE b = 255;

    MPTerrain* t = _pScene->GetTerrain();
    if(t)
    {
        D3DXVECTOR3 v = this->GetPos();
        MPTile* tile = t->GetTile((int)v.x, (int)v.y);
        if(tile)
        {
            D3DXCOLOR c = tile->dwXColor;

            //if(c.r < 0.4f)
            //    c.r = 0.4f;
            //if(c.g < 0.4f)
            //    c.g = 0.4f;
            //if(c.b < 0.4f)
            //    c.b = 0.4f;

            r = (BYTE)((float)r * ((1.0f + c.r) / 2));
            g = (BYTE)((float)g * ((1.0f + c.g) / 2));
            b = (BYTE)((float)b * ((1.0f + c.b) / 2));

            //const BYTE lmt = 160;
            //if(r < lmt)
            //    r = lmt;
            //if(g  < lmt)
            //    g = lmt;
            //if(b < lmt)
            //    b = lmt;
        }
    }

    if(GetEmiColorFlag() == 0)
    {
        CCharacterModel::SetAmbColor(r, g, b);
    }
}

void CCharacter::PlayAni( char* pAni, int nMax )
{
    _Special.AllFalse();
	_nHeightOff = 0;

    _nHelixCenterX = _nCurX;
    _nHelixCenterY = _nCurY;
    for( int i=0; i<nMax; i++ )
    {
        if( pAni[i]==0 ) break;

        switch( pAni[i] )
        {
        case enumAlphaIn: 
	        _Special.SetTrue( pAni[i] );
            SetOpacity( 0.0f );
            break;
        case enumHelixOut:		
	        _Special.SetTrue( pAni[i] );
            _nHelixRadii = 0;
            _nHelixAngle = getYaw();
            break;
        case enumHelixIn:	
	        _Special.SetTrue( pAni[i] );
            _nHelixRadii = 400;
            _nHelixAngle = getYaw();
            break;
        case enumHelixPos:
	        _Special.SetTrue( pAni[i] );
			_nHelixRadii = 200;
            _nHelixAngle = getYaw();
            break;
        case enumFastDrop:
	        _Special.SetTrue( pAni[i] );
            // _nHeightOff = 200;
            break;
        case enumDisappear:
	        _Special.SetTrue( pAni[i] );
            SetHide(TRUE);
            break;
		case enumCyclone:
			if( _pDefaultChaInfo->IsCyclone() )
			{
				_Special.SetTrue( pAni[i] );
				_nHelixAngle = getYaw();
				_nHelixRadii = 200;
				_nHeightOff = 0;
			}
			break;
		default:
			_Special.SetTrue( pAni[i] );
        }
    }
}

void CCharacter::InitState()
{
	GetActor()->InitState();

	_isArrive = true;

	_Special.AllFalse();
	
	SetOpacity( _fMaxOpacity );

	_nHeightOff = 0;
    _fMapHeight = 0.0f;
	_nPoseHeightOff = ERROR_POSE_HEIGHT;
    _InFight = false;

    SetHide( FALSE );

	_dwStartTime = 0;

	_ChaState.AllTrue();
	_hits.clear();

	_pHeadSay->Reset();

	_pSceneHeight->Reset();

	_szShopName[0] = 0;
	_PK.AllFalse();
	setSideID( 0 );
}

void CCharacter::setSideID( long v )					
{ 
	_lSideID = v;

	if( _pSideShade )
	{
		_pSideShade->SetValid( FALSE );
		_pSideShade = NULL;
	}

	if( _lSideID!=0 && IsPlayer() )
	{
		LG( getLogName(), "Add SideID:%d\n", _lSideID );
		_pSideShade = _pScene->GetFirstInvalidShadeObj();
		if( _pSideShade )
		{
			LG( getLogName(), "Add SideID:%d Successed\n", _lSideID );
			_pSideShade->Create( IsBoat() ? 7 : 2 );
			_pSideShade->setColor( _lSideID==1 ? 0xff00ff00 : 0xffff0000 );
			_pSideShade->setChaID( getID() );
			_pSideShade->setUpSea( true );
			_pSideShade->Emission( 0, &GetPos(), NULL);    
			_pSideShade->SetValid(TRUE);
		}
	}
}


void CCharacter::setGuildID( int nGuildID )			
{ 

	if(nGuildID == 0){
		if( _pSideShade ){
			_pSideShade->SetValid( FALSE );
			_pSideShade = NULL;
		}
		_nGuildCircleColour = 0xFFFFFFFF;
	}
	
	_nGuildID = nGuildID;	
	_dwGuildColor=0xfffed8ef;
	//No longer need to have pirate/navy guilds seperate.
	/*
	if(	nGuildID>=0 && nGuildID<=99 ) {
		_dwGuildColor=0xffedfcca; 
	}else {
		_dwGuildColor=0xfffed8ef;
	}
	*/
}

void CCharacter::setGuildCircle( int colour ,int icon)	{

	if( _pSideShade ){
		_pSideShade->SetValid( FALSE );
		_pSideShade = NULL;
	}

	_nGuildCircleColour = colour|0xff000000;
	_chGuildIcon = icon;
	

	
	if (_chGuildIcon == 0){
		return;
	}
	
	if(IsPlayer() ){
		_pSideShade = _pScene->GetFirstInvalidShadeObj();
		if( _pSideShade ){
			//_pSideShade->Create( IsBoat() ? 7 : 2 );			
			_pSideShade->Create( _chGuildIcon);			
			_pSideShade->setColor(_nGuildCircleColour);
			_pSideShade->setChaID( getID() );
			_pSideShade->setUpSea( true );
			_pSideShade->Emission( 0, &GetPos(), NULL);    
			_pSideShade->SetValid(TRUE);
		}
	}
}

void CCharacter::ActionKeyFrame( DWORD key_id )
{
	if( _pDefaultChaInfo )
	{
		if( _pDefaultChaInfo->sEffectActionID[0]==key_id )
		{
			SelfEffect( _pDefaultChaInfo->sEffectActionID[1], _pDefaultChaInfo->sEffectActionID[2] );
		}
	}
	
	switch( GetCurPoseType() )
	{
    case POSE_RUN:
	case POSE_RUN2:
		if( _pszFootMusic && key_id==0 )
		{
			CGameScene::PlayEnvSound( _pszFootMusic, GetCurX(), GetCurY() );
		}
		break;
	case POSE_WAITING:
    case POSE_WAITING2:
		if( _pszWhoopMusic )
		{
			CGameScene::PlayEnvSound( _pszWhoopMusic, GetCurX(), GetCurY() );
		}
		break;
	}
	
	GetActor()->ActionKeyFrame( GetCurPoseType(), key_id );
	
    if( _IsShowShadow && _eChaCtrlType==enumCHACTRL_PLAYER && !IsBoat() && GetScene()->GetTileTexAttr( (int)GetPos().x,(int)GetPos().y)==1 )
    {
       GetScene()->GetPugMgr()->NewPug( &GetPos(),(float)getYaw() * 0.01745329f + D3DX_PI );
    }
}

void CCharacter::SetDefaultChaInfo( CChaRecord* pInfo )	
{  
	_pDefaultChaInfo = pInfo;

	_pszFootMusic = NULL;
	 if( _pDefaultChaInfo->sFootfall!=-1 )
	 {
		 CMusicInfo* pInfo = GetMusicInfo(_pDefaultChaInfo->sFootfall);
		 if( pInfo )
		 {
			 _pszFootMusic = pInfo->szDataName;
		 }
	 }

	 _pszWhoopMusic = NULL;
	 if( _pDefaultChaInfo->sWhoop!=-1 )
	 {
		 CMusicInfo* pInfo = GetMusicInfo(_pDefaultChaInfo->sWhoop);
		 if( pInfo )
		 {
			 _pszWhoopMusic = pInfo->szDataName;
		 }
	 }

     _fMaxOpacity=(float)_pDefaultChaInfo->chDiaphaneity/100.0f;
}

bool CCharacter::PlayPose( DWORD pose, DWORD type, int time, int fps, bool isBlend )
{
	fps = CGameApp::GetFrameFPS();
	LG( getLogName(), "Pose:%d, type:%d, time:%d\n", pose, type, time, fps );

    bool rv = GetCurPoseType()==pose;

	// 飞行用（测试代码）
	if(GetIsFly()){
		if(pose == POSE_WAITING || pose == POSE_WAITING2) pose = POSE_FLY_WAITING;	// 站立 -> 飞行
		if(pose == POSE_RUN     || pose == POSE_RUN2)     pose = POSE_FLY_RUN;	// 走动 -> 飘走
		if(pose == POSE_SHOW)  pose = POSE_FLY_SHOW;	// 摆酷 -> 空中摆酷
		if(pose == POSE_SEAT)  pose = POSE_FLY_SEAT;	// 坐下 -> 空中悬空坐 
	}
	
	else if(GetIsOnMount()){
		pose  = POSE_SEAT2;
		GetScene()->HandleSceneMsg( SCENEMSG_CHA_BEGINMOVE, getTypeID(), getID() );
	}


	

    if((GetCurPoseType()==POSE_RUN2) || (GetCurPoseType()==POSE_RUN) || GetCurPoseType() == POSE_FLY_RUN)
    {
        if( (pose!=POSE_RUN2) && (pose!=POSE_RUN) && (pose!=POSE_FLY_RUN))
        {
            GetScene()->HandleSceneMsg( SCENEMSG_CHA_ENDMOVE, getTypeID(), getID() );
        }
    }
    else
    {
        if( (pose==POSE_RUN2) || (pose==POSE_RUN) || (pose==POSE_FLY_RUN))
        {
            GetScene()->HandleSceneMsg( SCENEMSG_CHA_BEGINMOVE, getTypeID(), getID() );
        }
    }

	if( isBlend )
	{
		CCharacterModel::PlayPose( pose, type, time, fps, 1 );
	}
	else
	{
		if( enumMODAL_MAIN_CHA==_eChaModalType && pose>POSE_WAITING && pose<POSE_ATTACK )
		{
			CCharacterModel::PlayPose( pose, type, time, fps, 1 );
		}
		else
		{
			CCharacterModel::PlayPose( pose, type, time, fps, 0 );
		}
	}

	if( rv ) return false;

    switch ( pose )
    {
    case POSE_WAITING2:
        CGameScene::PlayEnvSound( _pDefaultChaInfo->sWhoop, GetCurX(), GetCurY() );
        break;
    case POSE_DIE:
        CGameScene::PlayEnvSound( _pDefaultChaInfo->sDirge, GetCurX(), GetCurY() );
        break;
    }
	return true;
}

void CCharacter::_FightSwitch( bool isFight )
{
	
    _IsFightPose = isFight;
    if( isFight )
    {
        DetachItem( 21 );
        DetachItem( 22 );
        DetachItem( 23 );

        int dummy = -1;		
        if( _pHandItem[enumEQUIP_LHAND] )
        {
            AttachItem( enumEQUIP_LHAND, _pHandItem[enumEQUIP_LHAND], -1 );

            dummy = GetWeaponBackDummy( _pHandItem[enumEQUIP_LHAND]->GetItemInfo()->sType, true );
			/*
            if( dummy>0 )
            {
               _pHandItem[enumEQUIP_LHAND]->setAlpla();
            }
			*/		
        }

        if( _pHandItem[enumEQUIP_RHAND] )
        {
            AttachItem( enumEQUIP_RHAND, _pHandItem[enumEQUIP_RHAND], -1 );

            dummy = GetWeaponBackDummy( _pHandItem[enumEQUIP_RHAND]->GetItemInfo()->sType, false );
			/*
            if( dummy>0 )
            {
                _pHandItem[enumEQUIP_RHAND]->setAlpla();
            }
			*/
        }
    }
    else
    {      
        int dummy = -1;
        if( _pHandItem[enumEQUIP_LHAND] )
        {
            dummy = GetWeaponBackDummy( _pHandItem[enumEQUIP_LHAND]->GetItemInfo()->sType, true );
            if( dummy>0 )
            {
                DetachItem( enumEQUIP_LHAND );

                AttachItem( dummy, _pHandItem[enumEQUIP_LHAND], -1, 3 );
				//_pHandItem[enumEQUIP_LHAND]->setAlpla();
            }
			else
			{
				AttachItem( enumEQUIP_LHAND, _pHandItem[enumEQUIP_LHAND], -1 );
			}
        }
        if( _pHandItem[enumEQUIP_RHAND] )
        {
            dummy = GetWeaponBackDummy( _pHandItem[enumEQUIP_RHAND]->GetItemInfo()->sType, false );
            if( dummy>0 )
            {
                DetachItem( enumEQUIP_RHAND );

                AttachItem( dummy, _pHandItem[enumEQUIP_RHAND], -1, 3 );
				//_pHandItem[enumEQUIP_RHAND]->setAlpla();
            }
			else
			{
				AttachItem( enumEQUIP_RHAND, _pHandItem[enumEQUIP_RHAND], -1 );
			}
        }
    }
}

void CCharacter::UpdataFace( stNetChangeChaPart& stPart )
{
	//look = stPart;
	if( IsPlayer() && !IsBoat() )
	{
		
	}
	else
	{
		_szPreName[0] = '\0';
	}
    memcpy( &_stChaPart, &stPart, sizeof(_stChaPart) );

    _DetachAllItem();
    int nHairID = stPart.SLink[enumEQUIP_HEAD].sID!=0 ? stPart.SLink[enumEQUIP_HEAD].sID : stPart.sHairID;
    if( !UpdataItem( nHairID, 0 ) && nHairID!=stPart.sHairID )
	{
		UpdataItem( stPart.sHairID, 0 );
	}

    for( int i=1; i<enumEQUIP_NUM; i++ )
    {
        UpdataItem( stPart.SLink[i].sID, i );
    }

	int nCharID = getTypeID();
	//if( nCharID>=1 || nCharID<=4 )
	if( _ShowEffects )
	{
		
		if( _pHandItem[enumEQUIP_RHAND] )
			_pHandItem[enumEQUIP_RHAND]->SetForgeEffect( stPart.SLink[enumEQUIP_RHAND].lDBParam[0], nCharID );

		if( _pHandItem[enumEQUIP_LHAND] )
			_pHandItem[enumEQUIP_LHAND]->SetForgeEffect( stPart.SLink[enumEQUIP_LHAND].lDBParam[0], nCharID );
	}else{
		if( _pHandItem[enumEQUIP_RHAND] )
			_pHandItem[enumEQUIP_RHAND]->SetForgeEffect( 0, nCharID );

		if( _pHandItem[enumEQUIP_LHAND] )
			_pHandItem[enumEQUIP_LHAND]->SetForgeEffect( 0, nCharID );
	}

    if( _pHandItem[enumEQUIP_RHAND] && _pHandItem[enumEQUIP_RHAND]->GetItemInfo()->sType==6 )
    {
		int id = _pHandItem[enumEQUIP_RHAND]->GetItemInfo()->lID;
		const int mitten[][2] = {
			61, 200,
			62, 201,
			63, 202,
			64, 203,
			65, 204,
			66, 205,
			67, 206,
			68, 207,
			69, 208,
			70, 209,
			71, 210,
			72, 211
		};
		int row = sizeof(mitten)/sizeof(mitten[0][0])/2;
		for( int i=0; i<row; i++ )
		{
			if( mitten[i][0]==id )
			{
				UpdataItem(mitten[i][1], enumEQUIP_LHAND);
				break;
			}
		}
	}

	if (_pHeadSay)
	{
		if (stPart.SLink[enumEQUIP_HEAD].sID == 9160)
		{
			_pHeadSay->SetIsShowEvil(1);
			_pHeadSay->SetEvilLevel(stPart.SLink[enumEQUIP_HEAD].sEnergy[1]);
		}
		else
		{
			_pHeadSay->SetIsShowEvil(0);
		}
	}
    RefreshItem( true );
}

xShipInfo* CCharacter::ConvertPartTo8DWORD( stNetChangeChaPart& stPart, DWORD* dwBuf )
{
	xShipInfo* pInfo = ::GetShipInfo( stPart.sBoatID );
	if( !pInfo ) 
	{
		LG( "boat_error", g_oLangRec.GetString(20), stPart.sBoatID );
		return NULL;
	}

	// ��ֻ�Ƿ���Ը������
	xShipPartInfo* pData = GetShipPartInfo( pInfo->sBody );
	if( pData == NULL ) 
	{
		LG( "boat_error", g_oLangRec.GetString(21), pInfo->sBody );
		return NULL;
	}
		
	dwBuf[0] = pData->dwModel;
	if( pInfo->byIsUpdate )
	{
		pData = GetShipPartInfo( stPart.sHeader );
		if( pData == NULL ) 
		{
			LG( "boat_error", g_oLangRec.GetString(22), stPart.sHeader );
			return NULL;
		}
		dwBuf[1] = pData->dwModel;

		pData = GetShipPartInfo( stPart.sEngine );
		if( pData == NULL ) 
		{
			LG( "boat_error", g_oLangRec.GetString(23), stPart.sEngine );
			return NULL;
		}
		dwBuf[2] = pData->dwModel;

		dwBuf[3] = 0;
		for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
		{
			xShipPartInfo* pMotorData = GetShipPartInfo( pData->sMotor[i] );
			if( pMotorData ) 
			{					
				dwBuf[i+4] = pMotorData->dwModel;
			}
			else
			{
				dwBuf[i+4] = 0;
			}
		}
	}
	return pInfo;
}



int CCharacter::GetSpecialAppID(SItemGrid app){
	int appID = app.sID;
	if(appID>=5287 && appID<=5297){
		CItemRecord* item = GetItemRecordInfo(app.GetFusionItemID());
		char str[ 260 ];
		sprintf( str, "%s", item->chModule[GetDefaultChaInfo()->lID] );
		if( strcmp(str , "0")!=0){
			appID = app.GetFusionItemID() ;
		}else{
			appID = 0;
		}
	}
	return appID;
}

bool CCharacter::IsSameEquip(stNetChangeChaPart part){
	for(int i = 0;i<5;i++){
		SItemGrid item1 = GetPart().SLink[i];
		SItemGrid item2 = part.SLink[i];
		
		int ID1 = item1.sID;
		int ID2 = item2.sID;
		
		if(_ShowApparel){
			int appID1 = GetSpecialAppID(GetPart().SLink[i+19]);
			int appID2 = GetSpecialAppID(part.SLink[i+19]);
			ID1 = appID1>0&&ID1>0?appID1:ID1;
			ID2 = appID2>0&&ID2>0?appID2:ID2;
		}
		if(ID1 != ID2){
			return false;
		}
	}
	return true;
}

bool CCharacter::UpdataItem( int nItem, DWORD nLink )
{
	if( enumMODAL_MAIN_CHA!=_eChaModalType ) return false;

	switch(nLink){
		
	case enumEQUIP_MOUNT:
	case enumEQUIP_GLOWAPP:
	case enumEQUIP_CLOAK:
	case enumEQUIP_REAR:
	case enumEQUIP_WING:
	case enumEQUIP_FAIRY:{
		
		if(IsHide()){
			if( _pHandItemEff[nLink] ){
				_pHandItemEff[nLink]->SetValid( FALSE );
				_pHandItemEff[nLink] = NULL;
			}
			return false;
		} 
		
		if(!GetIsForUI() && _ShowEffects){		
			int prevID = 0; 
			if( _pHandItemEff[nLink] ){
				prevID = _pHandItemEff[nLink]->getIdxID();
			}
			int ID = GetPart().SLink[nLink].sID;
			
			
			if(nLink==enumEQUIP_FAIRY){
				if(_ShowApparel && GetPart().SLink[enumEQUIP_FAIRYAPP].sID != 0 && ID != 0){
					ID=  GetPart().SLink[enumEQUIP_FAIRYAPP].sID;
				}
			}
			
			if(ID != 0 && GetPart().SLink[nLink].sEndure[0] >= 50){
				CItemRecord* pInfo = GetItemRecordInfo( ID);
				if( pInfo ){
					int effID = pInfo->sItemEffect[0];
					int boneID = pInfo->sItemEffect[1];
					
					if (nLink == enumEQUIP_CLOAK){
						effID = effID+GetDefaultChaInfo()->lID-1;
					}  else if (nLink == enumEQUIP_MOUNT){
						effID = effID+GetDefaultChaInfo()->lID-1;
						boneID = GetDefaultChaInfo()->lID==3?24:boneID;

					}
					
					if( prevID != effID){
						
					
						if( _pHandItemEff[nLink] ){
							_pHandItemEff[nLink]->SetValid( FALSE );
							_pHandItemEff[nLink] = NULL;
						}
						
						CEffectObj* pEffect = _pScene->GetFirstInvalidEffObj();
						if( pEffect ){
						
							if(pEffect->Create(effID)){
								pEffect->setLoop( true );
								pEffect->setFollowObj((CSceneNode*)this,NODE_CHA,boneID);
								pEffect->Emission( -1, NULL, NULL); 
								pEffect->SetValid(TRUE);
								_pHandItemEff[nLink] = pEffect;
								
								if(nLink==enumEQUIP_CLOAK && GetDefaultChaInfo()->lID==1){
									_pHandItemEff[nLink]->SetScale(1,0.4,1);
								}
								
							}
						}
					}
	
					if(nLink==enumEQUIP_FAIRY && _pHandItemEff[nLink]){
						SItemGrid item = GetPart().SLink[enumEQUIP_FAIRY];
						int nLevel = item.GetAttr(ITEMATTR_VAL_STR)
							+ item.GetAttr(ITEMATTR_VAL_AGI) 
							+ item.GetAttr(ITEMATTR_VAL_DEX) 
							+ item.GetAttr(ITEMATTR_VAL_CON) 
							+ item.GetAttr(ITEMATTR_VAL_STA);
						int maxLv = 50;
						if(nLevel>maxLv){
							nLevel = maxLv;
						}
						float minSize = 0.4;
						float maxSize = 1.5;
						float size = minSize + (((maxSize-minSize)/ maxLv)* nLevel);
						
						_pHandItemEff[nLink]->SetScale(size,size,size);
					}
	
					break;
				}
			}
		}	
		if( _pHandItemEff[nLink] ){
			_pHandItemEff[nLink]->SetValid( FALSE );
			_pHandItemEff[nLink] = NULL;
		}
		break;
	}
	
        
	
	case enumEQUIP_HEAD:
    case enumEQUIP_FACE:
    case enumEQUIP_BODY:
    case enumEQUIP_GLOVE:
    case enumEQUIP_SHOES:
        if( nItem==0 ) {
			nItem=_pDefaultChaInfo->sSkinInfo[nLink];
		}else if(_ShowApparel){
			
			bool checkHair = false;
			if(nLink == enumEQUIP_HEAD){
				CItemRecord* pInfoMain = GetItemRecordInfo(nItem);
				if(pInfoMain->sType != 28){
					checkHair = true;
				}
			}
			
			int appID = GetSpecialAppID(GetPart().SLink[nLink+19]);
			
			if (appID != 0 && (nLink != enumEQUIP_HEAD || checkHair)){
				nItem = appID;
			}
		}
        if( CCharacterModel::ChangePart( nLink, nItem )==0 ){
			LG( getLogName(), "msgChangePart Error, Item[%d], Link[%d]\n\n", nItem, nLink );
			return false;
		}
        return true;
		
		
    case enumEQUIP_LHAND:
    case enumEQUIP_RHAND:
	
		if(GetIsForUI()){
			break;
		}
		
        if( nItem>0 && nItem!=enumEQUIP_BOTH_HAND ){
			
			if(_ShowApparel){
				CItemRecord* pInfoMain = GetItemRecordInfo( GetPart().SLink[nLink].sID);
				int itemType = pInfoMain->sType;
				int slot = nLink;
				if (itemType == enumItemTypeSword && nLink == enumEQUIP_LHAND){
					slot = enumEQUIP_SWORD2APP;
				}else if (itemType == enumItemTypeSword && nLink == enumEQUIP_RHAND){
					slot = enumEQUIP_SWORD1APP;
				}else if (itemType == enumItemTypeGlave){
					slot = enumEQUIP_GREATSWORDAPP;
				}else if (itemType == enumItemTypeBow){
					slot = enumEQUIP_BOWAPP;
				}else if (itemType == enumItemTypeHarquebus){
					slot = enumEQUIP_GUNAPP;
				}else if (itemType == enumItemTypeStylet){
					slot = enumEQUIP_DAGGERAPP;
				}else if (itemType == enumItemTypeCosh){
					slot = enumEQUIP_STAFFAPP;
				}else if (itemType == enumItemTypeShield){
					slot = enumEQUIP_SHIELDAPP;
				}
				
				int appID = GetSpecialAppID(GetPart().SLink[slot]);
				
				if(appID != 0){
					nItem = appID;
				}
			}
			
            if( !_pHandItem[nLink] || _pHandItem[nLink]->GetItemInfo()->lID!=nItem )
            {
                if( _pHandItem[nLink] )
                {
                    _pHandItem[nLink]->SetValid(FALSE);
                    _pHandItem[nLink] = NULL;
                }

                CSceneItem* item = _pScene->AddSceneItem( nItem, getTypeID() );
                if( item ){
					_pHandItem[nLink] = item;
                }
				else
				{
					LG( getLogName(), "msgChangePart AddSceneItem Error, Item[%d], Link[%d]\n\n", nItem, nLink );
					return false;
				}
            }
        }
        else
        {
            if( _pHandItem[nLink] ) 
            {
                DetachItem( _pHandItem[nLink] );

                _pHandItem[nLink]->SetValid(FALSE);
                _pHandItem[nLink] = NULL;
            }
        }
        return false;
    case enumEQUIP_NECK:
    case enumEQUIP_HAND1:
    case enumEQUIP_HAND2:
    case enumEQUIP_Jewelry1:
    case enumEQUIP_Jewelry2:
    case enumEQUIP_Jewelry3:
    case enumEQUIP_Jewelry4:
		return true;
    }
    return false;
}

void CCharacter::setNpcState( DWORD dwState )
{
    if( _pNpcStateItem ) 
    {
        _pNpcStateItem->SetValid( FALSE );
        _pNpcStateItem = NULL;
    }
    
	if( _chGMLv )
	{
		if( _chGMLv>=21 && _chGMLv<=30 )
		{
			_pNpcStateItem = _pScene->AddSceneItem( "hd.lgo" );
		}
		else
		{
			_pNpcStateItem = _pScene->AddSceneItem( "gm.lgo" );
		}
	}
	else if( ROLE_MIS_DELIVERY & dwState )
    {
		_pNpcStateItem = _pScene->AddSceneItem( "sighyellow.lgo" );
    }
    else if( ROLE_MIS_ACCEPT & dwState )
    {
		_pNpcStateItem = _pScene->AddSceneItem( "sighgreen.lgo" );
    }
    else if( ROLE_MIS_PENDING & dwState )
    {
		_pNpcStateItem = _pScene->AddSceneItem( "sighgray.lgo" );
    }

    if( _pNpcStateItem )
    {
        _pNpcStateItem->setIsSystem( true );
        _pNpcStateItem->setHeightOff( (int)(GetDefaultChaInfo()->fHeight * 100.0f) );
        _pNpcStateItem->setPos( GetCurX(), GetCurY() );
        _pNpcStateItem->PlayDefaultAnimation();
        _pNpcStateItem->setYaw(0);
    }
}

void CCharacter::DieTime()
{
	int size = (int)GetEffectNum();
	int id;
	CEffectObj *pEff;
	for(int n = 0; n < size; n++)
	{
		id = GetEffectID(n);
		if(id < 0)
			continue;
		pEff = GetScene()->GetEffect(id);
		if(pEff && pEff->IsValid())
		{
			pEff->SetValid(FALSE);
		}
	}
	RemoveEffect();

	if( GetDrunkState() ) CCharacterModel::SetDrunkState( FALSE );

	if( _pBoatFog )
	{
		_pBoatFog->SetValid( FALSE );
		_pBoatFog = NULL;
	}
    if( _pShopItem )
    {
        _pShopItem->SetValid( FALSE );
        _pShopItem = NULL;
    }
    if( _pNpcStateItem )
    {
        _pNpcStateItem->SetValid( FALSE );
        _pNpcStateItem = NULL;
    }

	for( int i=0; i<ITEM_FACE_MAX; i++ )
		SetItemFace( i, 0 );
	// setSideID( 0 );

	for( int i=0; i < enumEQUIP_NUM; i++ )
	{
		if (_pHandItemEff[i])
		{
			_pHandItemEff[i]->SetValid( FALSE );
			_pHandItemEff[i] = NULL;
		}
	}
}

void CCharacter::UnloadHandEff()
{
	for( int i=0; i < enumEQUIP_NUM; i++ )
	{
		if (_pHandItemEff[i])
		{
			_pHandItemEff[i]->SetValid( FALSE );
			_pHandItemEff[i] = NULL;
		}
	}
}

void CCharacter::SynchroSkillState( stSkillState* pState, int nCount )
{
	static CBoolSet set;
	set = _pChaState->Synchro( pState, nCount );

	if( set.IsTrue( enumChaStateNoShop ) != _ChaState.IsTrue( enumChaStateNoShop ) )
	{
		if( _pShopItem )
		{
			_pShopItem->SetValid( FALSE );
			_pShopItem = NULL;
		}

		if( set.IsFalse(enumChaStateNoShop) )
		{
			if( g_IsShowShop )
			{
				switch( CChaStateMgr::GetLastShopLevel() )
				{
				case 1:
					// PlayPose( POSE_WAITING, PLAY_LOOP_SMOOTH );
					_pShopItem = _pScene->AddSceneItem( "baitan1.lgo" );
					break;
				default:
					// PlayPose( POSE_SEAT, PLAY_LOOP_SMOOTH );
					_pShopItem = _pScene->AddSceneItem( "baitan2.lgo" );
					break;
				}
				if( _pShopItem )
				{
					_pShopItem->setIsSystem( true );
					_pShopItem->setYaw( getYaw() );
					_pShopItem->setPos( GetCurX(), GetCurY() );
				}
			}
		}
		else
		{
			g_stUIBooth.CloseBoothByOther( getAttachID() );
		}
	}

	if( set.IsTrue( enumChaStateNoHide ) != _ChaState.IsTrue( enumChaStateNoHide ) )
	{
		if( set.IsTrue( enumChaStateNoHide ) ) SetOpacity( _fMaxOpacity );	
		else  SetOpacity( _fMaxOpacity/2.0f );
	}

	if( set.IsTrue(enumChaStateNoAni) != _ChaState.IsTrue(enumChaStateNoAni) )
	{
		if( set.IsTrue(enumChaStateNoAni) )
		{
			if( IsMainCha() )
			{
				g_pGameApp->EnableCameraFollow( TRUE );
			}

			StopAni();
		}
		else
		{
			if( IsMainCha() )
			{
				g_pGameApp->EnableCameraFollow( FALSE );
			}

			PlayAni( CChaStateMgr::GetLastActInfo()->nActBehave, CChaStateMgr::GetLastActInfo()->GetActNum() );
		}
	}

	//if( set.IsTrue(enumChaStateMove) != _ChaState.IsTrue(enumChaStateMove) )
	//{
	//	if( set.IsTrue( enumChaStateMove ) ) PlayPoseContinue();
	//	else PlayPosePause();
	//}

	if( set.IsTrue( enumChaStateNoDizzy ) != _ChaState.IsTrue( enumChaStateNoDizzy ) )
	{
		if( _pDefaultChaInfo->IsCyclone() )
		{
			if( set.IsTrue( enumChaStateNoDizzy ) )
			{
				CCharacterModel::SetDrunkState( FALSE );

				//CDizzyState* pState = dynamic_cast<CDizzyState*>(GetActor()->GetCurState());
				//if( pState ) pState->Stop();
			}
			else
			{
				CCharacterModel::SetDrunkState( TRUE );
				//GetActor()->OverAllState();
				//CDizzyState* pState = new CDizzyState(GetActor());
				//GetActor()->SwitchState( pState );

				//set.SetFalse( enumChaStateMove );
				//set.SetFalse( enumChaStateAttack );
				//set.SetFalse( enumChaStateUseSkill );
			}
		}
	}

	_ChaState = set;
}

void CCharacter::RefreshShopShop()
{
	if( _pShopItem )
	{
		_pShopItem->SetValid( FALSE );
		_pShopItem = NULL;
	}

	if( g_IsShowShop && _ChaState.IsFalse( enumChaStateNoShop ) )
	{
		switch( CChaStateMgr::GetLastShopLevel() )
		{
		case 1:
			// PlayPose( POSE_WAITING, PLAY_LOOP_SMOOTH );
			_pShopItem = _pScene->AddSceneItem( "baitan1.lgo" );
			break;
		default:
			// PlayPose( POSE_SEAT, PLAY_LOOP_SMOOTH );
			_pShopItem = _pScene->AddSceneItem( "baitan2.lgo" );
			break;
		}
		if( _pShopItem )
		{
			_pShopItem->setIsSystem( true );
			_pShopItem->setYaw( getYaw() );
			_pShopItem->setPos( GetCurX(), GetCurY() );
		}
	}
}

bool CCharacter::GetRunTimeMatrix(MPMatrix44* mat, DWORD dummy_id)
{
	return GetObjDummyRunTimeMatrix(mat,dummy_id) == 0;
}

void CCharacter::LoadingCall()
{
    RefreshItem( true );
}

void CCharacter::RefreshItem(bool isFirst)
{
    if( isFirst )  
    {
        CheckIsFightArea();

        _FightSwitch( _InFight );
        
        PlayPose( GetPose( GetCurPoseType() ), PLAY_LOOP_SMOOTH ); 
    }
    else
    {
        if( _InFight!=_IsFightPose )
        {
            _FightSwitch( _InFight );
        }
    }
}

void CCharacter::SwitchFightPose()					
{ 
	if( !_IsFightPose )
	{
		_FightSwitch( true );
	}
}

void CCharacter::ForceMove( int nTargetX, int nTargetY )
{
	if( _nCurX==nTargetX && _nCurY==nTargetY ) 
	{
		_isArrive = true;
		return;
	}

	_nTargetX = nTargetX;
	_nTargetY = nTargetY;

	_IsMoveTimeType = !IsMainCha();
	if( _IsMoveTimeType )
	{
		_fMoveSpeed = (float)getMoveSpeed() / 950.0f;
		_fMoveLen = (int)GetDistance(_nCurX, _nCurY, nTargetX, nTargetY) + 1.0f;
		if( _fMoveLen>0.0f )
		{
			_dwStartTime = CGameApp::GetCurTick();

			_vMoveStart.x = (float)_nCurX;
			_vMoveStart.y = (float)_nCurY;
			_vMoveEnd.x = (float)nTargetX;
			_vMoveEnd.y = (float)nTargetY;
			_vMoveDir = _vMoveEnd - _vMoveStart;
			D3DXVec2Normalize( &_vMoveDir, &_vMoveDir );
			_isArrive = false;
			_isStopMove = false;
		}
		else
		{
			_isArrive = true;
		}
	}
	else
	{
		float fDistance = (float)GetDistance(_nCurX, _nCurY, nTargetX, nTargetY);		
		CGameApp::GetFrame()->RefreshFPS();
		_fStep = 1.0f / (fDistance / (float)getMoveSpeed() * (float)(CGameApp::GetFrameFPS() + 2) ); 

		if( _fStep > 1.0 ) 
		{
			_isArrive = true;
			return;
		}

		_nLastPosX = _nCurX;
		_nLastPosY = _nCurY;

		_fStepProgressRate = 0.0;
		_isArrive = false;

		_isStopMove = false;
	}
}


void CCharacter::ResetReadySkill()		
{ 
	CCommandObj::Cancel();
	_SetReadySkill( NULL );	
}

void CCharacter::SetHide(BOOL bHide)
{
	if( _bHide==bHide ) return;

	_bHide = bHide;
	RefreshShadow();

	int i;
	for( i=0; i<enumEQUIP_NUM; i++ )
		if( _pHandItem[i] )
			_pHandItem[i]->SetHide( bHide );

	if( _pNpcStateItem ) _pNpcStateItem->SetHide( bHide );

	if( _pShopItem ) _pShopItem->SetHide( bHide );

	if( _pSideShade ) _pSideShade->SetHide( bHide );
	
	for (i = 0; i < GetEffectNum(); i++)
	{
		GetScene()->GetEffect(GetEffectID(i))->SetHide(bHide);
	}
	for (i = 0; i < GetShadeNum(); i++)
	{
		GetScene()->GetShadeObj(GetShadeID(i))->SetHide(bHide);
	}
	if( _pBoatFog )
	{
		_pBoatFog->SetHide( bHide );	
	}

	for( int i=0; i<ITEM_FACE_MAX; i++ )
	{
		if( _pItemFaceEff[i] )
		{
			_pItemFaceEff[i]->SetHide( bHide );	
		}
	}
	
	for( int i=0; i<enumEQUIP_NUM; i++ )
	{
		if( _pHandItemEff[i] )
		{
			_pHandItemEff[i]->SetHide( bHide );	
		}
	}
}

void CCharacter::RefreshFog()
{
	if( !_pDefaultChaInfo->HaveEffectFog() ) return;

	if( _bHide ) return;

	static float hp[] =   { 0.2f, 0.5f, 0.8f };

	int nIDX = 0;
	if( _pBoatFog )
	{
		nIDX = _pBoatFog->getIdxID();
	}

	int nEffectID = 0;
	float fHPRate = (float)getGameAttr()->get( ATTR_HP ) / (float)getGameAttr()->get( ATTR_MXHP );
	if( fHPRate<hp[0] )
	{
		nEffectID = _pDefaultChaInfo->GetEffectFog( 0 );
	}
	else if( fHPRate<hp[1] )
	{
		nEffectID = _pDefaultChaInfo->GetEffectFog( 1 );
	}
	else if( fHPRate<hp[2] )
	{
		nEffectID = _pDefaultChaInfo->GetEffectFog( 2 );
	}

	if( nEffectID!=nIDX )
	{
		if( _pBoatFog )
		{
			_pBoatFog->SetValid( FALSE );
		}
		_pBoatFog = SelfEffect( nEffectID, 4, true );
	}
}

void CCharacter::SetItemFace( unsigned int nIndex, int nItem )
{
	return;
	
	//disabled this function, as item faces are no longer required.
	
	if( nIndex>=ITEM_FACE_MAX ) return;

	if( nItem==_ItemFace[nIndex] ) return;

	_ItemFace[nIndex] = nItem;

	if( _pItemFaceEff[nIndex] )
	{
		_pItemFaceEff[nIndex]->SetValid( FALSE );
		_pItemFaceEff[nIndex] = NULL;
	}

	CItemRecord* pInfo = GetItemRecordInfo( nItem );
	if( !pInfo || pInfo->sItemEffect[0]==0 ) return;

	switch( nIndex )
	{
	case 0:				// The first column is placed with wings
		
		return;
	case 1: 
		if( pInfo->sType!=enumItemTypePet )
		{
			return;		// The second column puts the pet elf
		}
		else
		{
			CCharacter* pCha = this;
			if(g_stUIMap.IsPKSilver() && pCha->IsPlayer() && pCha->GetMainType()!=enumMainPlayer)
				return;

			if( pInfo->sNeedLv > this->_Attr.get(ATTR_LV) )
				return;
			
			if( pInfo->chBody[0] != -1 )
			{
				bool bFlag = false;
				for( int i = 0; i < defITEM_BODY; i++ )
				{
					if( pInfo->chBody[i] != -1 && pInfo->chBody[i] == GetDefaultChaInfo()->lID )
					{
						bFlag = true;
						break;
					}
				}
				if( !bFlag ) return;
			}			
		}
		break;
	default: return;
	}
	
	CEffectObj* pEffect = _pScene->GetFirstInvalidEffObj();
	if( !pEffect ) return;

	if( !pEffect->Create( pInfo->sItemEffect[0] ) )
		return;

	pEffect->setLoop( true );
	pEffect->setFollowObj((CSceneNode*)this,NODE_CHA,pInfo->sItemEffect[1]);
	pEffect->Emission( -1, NULL, NULL); 
	pEffect->SetValid(TRUE);

	_pItemFaceEff[nIndex] = pEffect;
}


bool CCharacter::GetIsPet()
{
	long nID = GetDefaultChaInfo()->lID;

	switch(nID)
	{
	case 929:	// ʥ��С����
	case 930:	// ���ë�Ļ�
	case 931:	// ��¹����
	case 932:	// ѩ�˱���
		return true;
	}

	return false;
}

bool CCharacter::GetIsOnMount(){
	if(!g_stUIBoat.GetHuman()){
		return false;
	}
	if(GetIsForUI()){
		return false;
	}
	if ( !_pHandItemEff[enumEQUIP_MOUNT] ){
		return false;
	}
	return true;
}

bool CCharacter::GetIsFly(){
	if(!g_stUIBoat.GetHuman()){
		return false;
	}
	if(GetIsForUI()){
		return false;
	}
	if ( !_pHandItemEff[enumEQUIP_WING] )
		return false;
	
	int isFly = _stChaPart.SLink[enumEQUIP_WING].sEnergy[1] / 10;
	if (isFly != 95)
		return false;

	return !GetIsOnMount();
}

void CCharacter::_computeLinkedMatrix()
{
	if( mParentNode )
	{
		MPMatrix44 mat;
		mParentNode->GetRunTimeMatrix( &mat, mParentBoneID );

		// extract translation information from parent bone.
		D3DXVECTOR3 pos;
		pos.x = mat._41;
		pos.y = mat._42;
		pos.z = mat._43;
		
		// extract scaling information from parent bone.
		D3DXVECTOR3 scale;
		scale.x = sqrt ( mat._11 * mat._11 + mat._21 * mat._21 + mat._31 * mat._31 );
		scale.y = sqrt ( mat._12 * mat._12 + mat._22 * mat._22 + mat._32 * mat._32 );
		scale.z = sqrt ( mat._13 * mat._13 + mat._23 * mat._23 + mat._33 * mat._33 );

		// extract rotation information from parent bone.
		float rotZ = D3DX_PI - atan2( mat._21, mat._11 );
		float rotY = - asin( -mat._31 / scale.x );//
		float rotX = - atan2( mat._32 / scale.y, mat._33 / scale.z );

		float yaw = Radian2Angle( rotZ );
		float pitch = Radian2Angle( rotY );
		float roll = Radian2Angle( rotX );

		//SetPos((float*)(&pos));
		setPos( int( pos.x * 100.f ), int( pos.y * 100.f ) );
		SetScale(scale);
		setYaw(int(yaw));
		setPitch(int(pitch));
		setRoll(int(roll));	
	}

}

//add by ALLEN 2007-10-16
bool CCharacter::IsReadingBook()
{
	CReadingState* pState = dynamic_cast<CReadingState*>(GetActor()->GetCurState());
	return (0 != pState);
}