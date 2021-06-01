#include "stdafx.h"

#include "createchascene.h"

#include "CameraCtrl.h"
#include "GameApp.h"
#include "SceneObj.h"
#include "GameConfig.h"
#include "GlobalVar.h"
#include "UIFormMgr.h"
#include "UITextButton.h"
#include "Character.h"
#include "UiTextButton.h"
#include "uiformmgr.h"
#include "UiFormMgr.h"
#include "UiForm.h"
#include "UIBoxForm.h"
#include "GameApp.h"
#include "Character.h"
#include "SceneObj.h"
#include "UiFormMgr.h"
#include "UiTextButton.h"
#include "CharacterAction.h"
#include "SceneItem.h"
#include "ItemRecord.h"
#include "PacketCmd.h"
#include "GameConfig.h"
#include "ITEMRECORD.h"
#include "Character.h"
#include "caLua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "UIRender.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "uiformmgr.h"
#include "uitextbutton.h"
#include "uilabel.h"
#include "uiprogressbar.h"
#include "uiscroll.h"
#include "uilist.h"
#include "uicombo.h"
#include "uiimage.h"
#include "UICheckBox.h"
#include "uiimeinput.h"
#include "uigrid.h"
#include "uilistview.h"
#include "uipage.h"
#include "uitreeview.h"
#include "uiimage.h"
#include "UILabel.h"
#include "RenderStateMgr.h"
#include "uicompent.h"

#include "UIMemo.h"
#include "caLua.h"

#include "Connection.h"
#include "ServerSet.h"
#include "GameAppMsg.h"


#include "UI3DCompent.h"
#include "UIForm.h"
#include "UITemplete.h"
#include "commfunc.h"
#include "uiboxform.h"
#include "SelectChaScene.h"
#include "commfunc.h"
#include "uiTextButton.h"

#define ARRAY_SIZE( array )	( sizeof( array ) / sizeof( ( array )[0] ) )

inline bool Error(const char* strInfo, const char* strFormName, const char* strCompentName) {
	LG("gui", strInfo, strFormName, strCompentName );
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Class: LoginScene_CreateCha (Copy from Jack)
//////////////////////////////////////////////////////////////////////////

struct PoseConfigInfo
{
    DWORD pose_id;
    DWORD subset;
    DWORD stage;
    DWORD anim_type;
    DWORD play_type;
    float velocity;
};

int LoginScene_CreateCha::_CreateArrowItem(const char* file)
{
    LW_IF_RELEASE(_arrow);

    _res_mgr->CreateItem(&_arrow);
    if(LW_FAILED(_arrow->Load(file)))
        return 0;

    _arrow->GetPrimitive()->SetState(STATE_VISIBLE, 0);
    _arrow->PlayDefaultAnimation();

    return 1;
}

bool LoginScene_CreateCha::LoadArrowMark( const char* Filename, const lwVector3* Offsets )
{
	memcpy( _ArrowMarksOffset, Offsets, sizeof( _ArrowMarksOffset ) );

	for( int Index = 0; Index < 4; Index ++ )
	{
		LW_IF_RELEASE( _ArrowMarks[Index] );

		assert( _cha_obj[Index] );

		_res_mgr->CreateItem( &( _ArrowMarks[Index] ) );
		if( LW_FAILED( _ArrowMarks[Index]->Load( Filename ) ) )
			return false;

		_ArrowMarks[Index]->GetPrimitive()->SetState( STATE_VISIBLE, TRUE );
		_ArrowMarks[Index]->PlayDefaultAnimation();

		lwMatrix44 matWorld = *( _cha_obj[Index]->GetWorldMatrix() );
		lwMatrix44 matTranslate = lwMatrix44Translate( _ArrowMarksOffset[Index] );
		lwMatrix44Multiply( &matWorld, &matTranslate, &matWorld );
		_ArrowMarks[Index]->SetMatrix( &matWorld );
	}

	return true;
}

int LoginScene_CreateCha::_InitChaObjSeq()
{
    const DWORD cha_num = 5;

    // init pose
    int x[5][5] =
    {
        {1, 0, 125, 500, 600},
        {2, 0, 200, 500, 600},
        {3, 0, 360, 500, 600},
        {4, 0, 500, 500, 600},
        {0, 0, 360, 500, 600},
    };

    lwModelNodeQueryInfo mnqi;
    memset(&mnqi, 0, sizeof(mnqi));
    mnqi.mask = MNQI_TYPE | MNQI_ID;
    mnqi.type = NODE_PRIMITIVE;

    for(DWORD i = 0; i < cha_num; i++)
    {
        mnqi.node = 0;
        mnqi.id = x[i][0];

        if(LW_FAILED(_model_lxo->QueryTreeNode(&mnqi)))
            return 0;

        _cha_obj[i] = (lwINodePrimitive*)mnqi.node->GetData();
    }

    _cha_num = cha_num;


    lwPoseInfo pi[2];
    memset(pi, 0, sizeof(pi));

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_BONE;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    lwIPoseCtrl* pose_ctrl;
    lwIAnimCtrlObjBone* ctrl_obj;

    lwPlayPoseInfo play_info;
    lwPlayPoseInfo_Construct(&play_info);
    play_info.bit_mask = PPI_MASK_POSE | PPI_MASK_TYPE | PPI_MASK_VELOCITY;
    play_info.pose = 1;
    play_info.type = PLAY_LOOP;
    play_info.velocity = 1.0f;

    for(DWORD i = 0; i < _cha_num; i++)
    {
        lwINodeBoneCtrl* bone_ctrl = (lwINodeBoneCtrl*)_cha_obj[i]->GetParent();
        if(bone_ctrl == 0)
            return 0;

        ctrl_obj = bone_ctrl->GetAnimCtrlObj();
        if(ctrl_obj == 0)
            return 0;

        pose_ctrl = ctrl_obj->GetAnimCtrl()->GetPoseCtrl();

        pi[0].start = x[i][1];
        pi[0].end = x[i][2];
        pi[1].start = x[i][3];
        pi[1].end = x[i][4];

        pose_ctrl->InsertPose(1, &pi[0], 2);

        ctrl_obj->PlayPose(&play_info);
    }

    _act_obj[0] = 0;
    _act_obj[1] = 1;
    _act_obj[2] = 2;
    _act_obj[3] = 3;
    _act_num = 4;

    return 1;
}

void LoginScene_CreateCha::Destroy()
{
    LW_SAFE_RELEASE(_model_lxo);
    LW_SAFE_RELEASE(_model_lmo);
}

void LoginScene_CreateCha::Update()
{
    if(_model_lxo)
    {
        _model_lxo->Update();
    }
    if(_model_lmo)
    {
        _model_lmo->Update();
    }
    if(_arrow)
    {
        _arrow->Update();
    }

	for( int Index = 0; Index < 4; Index ++ )
	{
		if( _ArrowMarks[Index] )
			_ArrowMarks[Index]->Update();
	}

	if( _ArrowMarks[3] )
	{
		lwMatrix44 matWorld = *( _cha_obj[_act_obj[3]]->GetWorldMatrix() );
		lwMatrix44 matTranslate = lwMatrix44Translate( _ArrowMarksOffset[3] );
		lwMatrix44Multiply( &matWorld, &matTranslate, &matWorld );
		_ArrowMarks[3]->SetMatrix( &matWorld );
	}
}

void LoginScene_CreateCha::Render()
{
	if(_model_lxo)
	{
		_model_lxo->IgnoreNodesRender(&ignoreStruct);
	}
	if(_model_lmo)
    {
        _model_lmo->Render();
    }
    if(_arrow)
    {
        _arrow->Render();
    }

	for( int Index = 0; Index < 4; Index ++ )
	{
		if( _ArrowMarks[Index] )
			_ArrowMarks[Index]->Render();
	}
}

int LoginScene_CreateCha::LoadModelLXO(const char* file)
{
    _res_mgr->CreateNodeObject(&_model_lxo);

    lwAssObjInfo assinfo;
    assinfo.size = lwVector3(1.0f, 1.0f, 1.0f);
    _res_mgr->SetAssObjInfo(ASSOBJ_MASK_SIZE, &assinfo);

    _res_mgr->SetTexturePath(".\\texture\\scene\\");

    char path[260];
    sprintf(path, ".\\model\\scene\\%s",file);
    _model_lxo->Load(path, MODELOBJECT_LOAD_RESET, 0);

    lwINodeObjectA::PlayDefaultPose(_model_lxo);
    //lwINodeObjectA::ShowBoundingObject(_model_lxo, 1);

    //lwINodeObjectA::DumpObjectTree(_model_lxo, "test.txt");

    lwITreeNode* tn_bonectrl = 0;
    lwITreeNode* tn_dummy = 0;
    lwINodeBoneCtrl* node_bonectrl = 0;
    lwINodeDummy* node_dummy = 0;

    lwModelNodeQueryInfo mnqi;
    memset(&mnqi, 0, sizeof(mnqi));
    mnqi.mask = MNQI_TYPE | MNQI_ID;
    mnqi.type = NODE_BONECTRL;
    mnqi.id = 4;
    mnqi.node = 0;

    if(LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi)))
    {
        tn_bonectrl = mnqi.node;
        node_bonectrl = (lwINodeBoneCtrl*)mnqi.node->GetData();
    }

    mnqi.type = NODE_DUMMY;
    mnqi.id = 99;
    mnqi.node = 0;

    if(LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi)))
    {
        tn_dummy = mnqi.node;
        node_dummy = (lwINodeDummy*)mnqi.node->GetData();
    }

    if(node_bonectrl && node_dummy)
    {
        _model_lxo->RemoveTreeNode(tn_bonectrl);
        _model_lxo->InsertTreeNode(tn_dummy, 0, tn_bonectrl);
    }

    if(_InitChaObjSeq() == 0)
        return 0;
    //
    _model_type = 1;

    // Init object pose

    {
        PoseConfigInfo pci[32];
        DWORD pci_num = 13;

        //char end_pose[64];

        pci[0].pose_id=41, pci[0].subset=-1, pci[0].stage=-1, pci[0].anim_type=0, pci[0].play_type=1, pci[0].velocity=1.0f;
        pci[1].pose_id=41, pci[1].subset=0,  pci[1].stage=-1, pci[1].anim_type=4, pci[1].play_type=1, pci[1].velocity=1.0f;
        pci[2].pose_id=54, pci[2].subset=-1, pci[2].stage=-1, pci[2].anim_type=0, pci[2].play_type=1, pci[2].velocity=1.0f;
        pci[3].pose_id=54, pci[3].subset=0,  pci[3].stage=-1, pci[3].anim_type=4, pci[3].play_type=1, pci[3].velocity=1.0f;
        pci[4].pose_id=27, pci[4].subset=0,  pci[4].stage=0,  pci[4].anim_type=2, pci[4].play_type=2, pci[4].velocity=0.8f;
        pci[5].pose_id=28, pci[5].subset=0,  pci[5].stage=0,  pci[5].anim_type=2, pci[5].play_type=2, pci[5].velocity=0.5f;
        pci[6].pose_id=106,pci[6].subset=-1, pci[6].stage=-1, pci[6].anim_type=1, pci[6].play_type=2, pci[6].velocity=0.3f;
        pci[7].pose_id=60, pci[7].subset=-1, pci[7].stage=-1, pci[7].anim_type=1, pci[7].play_type=2, pci[7].velocity=0.8f;
        pci[8].pose_id=61, pci[8].subset=-1, pci[8].stage=-1, pci[8].anim_type=1, pci[8].play_type=2, pci[8].velocity=0.8f;
        pci[9].pose_id=62, pci[9].subset=-1, pci[9].stage=-1, pci[9].anim_type=1, pci[9].play_type=2, pci[9].velocity=0.8f;
        pci[10].pose_id=63, pci[10].subset=-1, pci[10].stage=-1, pci[10].anim_type=1, pci[10].play_type=2, pci[10].velocity=0.8f;
        pci[11].pose_id=64, pci[11].subset=-1, pci[11].stage=-1, pci[11].anim_type=1, pci[11].play_type=2, pci[11].velocity=0.8f;
        pci[12].pose_id=110, pci[12].subset=-1, pci[12].stage=-1, pci[12].anim_type=0, pci[12].play_type=2, pci[12].velocity=1.5f;
        pci[13].pose_id=111, pci[13].subset=-1, pci[13].stage=-1, pci[13].anim_type=0, pci[13].play_type=2, pci[13].velocity=1.5f;

        lwModelNodeQueryInfo mnqi;
        memset(&mnqi, 0, sizeof(mnqi));
        mnqi.mask = MNQI_ID | MNQI_TYPE;
        mnqi.type = NODE_PRIMITIVE;
        mnqi.node = 0;

        lwINodePrimitive* np;
        lwAnimCtrlObjTypeInfo type_info;
        lwPlayPoseInfo ppi;
        memset(&ppi, 0, sizeof(ppi));
        ppi.bit_mask = PPI_MASK_DEFAULT;
        ppi.pose = 0;
        ppi.frame = 0.0f;
        ppi.type = PLAY_ONCE;
        ppi.velocity = 1.0f;


        for(DWORD i = 0; i < pci_num; i++)
        {
            mnqi.id = pci[i].pose_id;

            if(pci[i].anim_type == ANIM_CTRL_TYPE_BONE)
            {
                mnqi.type = NODE_BONECTRL;

                if(LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi)))
                {
                    lwINodeBoneCtrl* nb = (lwINodeBoneCtrl*)mnqi.node->GetData();

                    lwIAnimCtrlObj* ctrl_obj = nb->GetAnimCtrlObj();
                    if(ctrl_obj == 0)
                        continue;

                    ppi.type = pci[i].play_type;
                    ppi.velocity = pci[i].velocity;
                    ctrl_obj->PlayPose(&ppi);
                }

            }
            else
            {
                mnqi.type = NODE_PRIMITIVE;

                if(LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi)))
                {
                    np = (lwINodePrimitive*)mnqi.node->GetData();

                    lwIAnimCtrlAgent* anim_agent = np->GetAnimCtrlAgent();
                    if(anim_agent == 0)
                        continue;

                    type_info.type = pci[i].anim_type;
                    type_info.data[0] = pci[i].subset;
                    type_info.data[1] = pci[i].stage;
                    lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
                    if(ctrl_obj == 0)
                        continue;

                    ppi.type = pci[i].play_type;
                    ppi.velocity = pci[i].velocity;
                    ctrl_obj->PlayPose(&ppi);
                }
            }

        }
    }
    return 1;

}
int LoginScene_CreateCha::LoadModelLMO(const char* file)
{
    _res_mgr->CreateModel(&_model_lmo);

    _model_lmo->Load(file);
    lwIPrimitive* pri_sea = 0;
    DWORD sea_id = 15;

    DWORD num = _model_lmo->GetPrimitiveNum();
    for(DWORD i = 0; i < num; i++)
    {
        _model_lmo->GetPrimitive(i)->SetState(STATE_TRANSPARENT, 0);
        _model_lmo->GetPrimitive(i)->SetState(STATE_UPDATETRANSPSTATE, 0);

        if(_model_lmo->GetPrimitive(i)->GetID() == sea_id)
            pri_sea = _model_lmo->GetPrimitive(i);

    }

    {
        _model_lmo->PlayDefaultAnimation();

        int pose_enable = 1;
        if(pose_enable)
        {

            PoseConfigInfo pci[32];
            DWORD pci_num = 6;

            pci[0].pose_id=0, pci[0].subset=-1, pci[0].stage=-1, pci[0].anim_type=1, pci[0].play_type=1, pci[0].velocity=0.5;
            pci[1].pose_id=1, pci[1].subset=-1, pci[1].stage=-1, pci[1].anim_type=1, pci[1].play_type=1, pci[1].velocity=0.5;
            pci[2].pose_id=2, pci[2].subset=-1, pci[2].stage=-1, pci[2].anim_type=1, pci[2].play_type=1, pci[2].velocity=0.5;
            pci[3].pose_id=3, pci[3].subset=-1, pci[3].stage=-1, pci[3].anim_type=1, pci[3].play_type=1, pci[3].velocity=0.5;
            pci[4].pose_id=4, pci[4].subset=-1, pci[4].stage=-1, pci[4].anim_type=1, pci[4].play_type=1, pci[4].velocity=0.5;
            pci[5].pose_id=26,pci[5].subset=-1, pci[5].stage=-1, pci[5].anim_type=1, pci[5].play_type=1, pci[5].velocity=0.5;

            lwIPrimitive* p = 0;

            lwPlayPoseInfo ppi;
            memset(&ppi, 0, sizeof(ppi));
            ppi.bit_mask = PPI_MASK_DEFAULT;
            ppi.pose = 0;
            ppi.frame = 0.0f;
            ppi.type = PLAY_ONCE;
            ppi.velocity = 1.0f;

            lwAnimCtrlObjTypeInfo type_info;

            for(DWORD i = 0; i < num; i++)
            {
                p = _model_lmo->GetPrimitive(i);

                for(DWORD j = 0; j < pci_num; j++)
                {
                    if(p->GetID() == pci[j].pose_id)
                    {
                        lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
                        if(anim_agent == 0)
                            continue;

                        type_info.type = pci[j].anim_type;
                        type_info.data[0] = pci[j].subset;
                        type_info.data[1] = pci[j].stage;
                        lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
                        if(ctrl_obj == 0)
                            continue;

                        ppi.type = pci[j].play_type;
                        ppi.velocity = pci[j].velocity;
                        ctrl_obj->PlayPose(&ppi);
                    }
                }
            }


        }
    }

    //
    _model_type = 2;

    return 1;
}

int LoginScene_CreateCha::LoadArrowItem(const char* file, const lwVector3* offset_pos4)
{
    if(_CreateArrowItem(file) == 0)
        return 0;

    memcpy(_arrow_offset_pos, offset_pos4, sizeof(_arrow_offset_pos));

    return 1;
}

void LoginScene_CreateCha::OnMouseMove(int flag, int x, int y)
{
    if(_model_type != 1)
        return;

    lwINodePrimitive* pri;
    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

    lwPickInfo info;
    lwVector3 org, ray;
    dev_obj->ScreenToWorld(&org, &ray, x, y);

    lwPlayPoseInfo play_info;
    lwPlayPoseInfo_Construct(&play_info);
    play_info.bit_mask = PPI_MASK_POSE | PPI_MASK_TYPE | PPI_MASK_VELOCITY | PPI_MASK_FRAME;
    play_info.type = PLAY_LOOP_SMOOTH;
    play_info.velocity = 1.0f;
    play_info.frame = 0.0f;

    BOOL octopus_flag = 0;
    BOOL dummy_flag = 0;

    if(_arrow)
    {
        _arrow->GetPrimitive()->SetState(STATE_VISIBLE, 0);
    }

    for(DWORD i = 0; i < _act_num; i++)
    {        
        if((pri = _cha_obj[_act_obj[i]]) == 0)
            continue;

        if(LW_SUCCEEDED(pri->HitTest(&info, &org, &ray)))
        {
            play_info.pose = 2;

            if(i == 3)
            {
                dummy_flag = 1;
            }
            if(i == 2)
            {
                octopus_flag = 1;
            }

            /*if(_arrow)
            {
                _arrow->GetPrimitive()->SetState(STATE_VISIBLE, 1);
                lwMatrix44 mat, m;
                mat = *pri->GetWorldMatrix();
                m = lwMatrix44Translate(_arrow_offset_pos[i]);
                lwMatrix44Multiply(&mat, &m, &mat);
                _arrow->SetMatrix(&mat);
            }*/
        }
        else
        {
            play_info.pose = 1;
        }

        lwINodeBoneCtrl* bone_ctrl = (lwINodeBoneCtrl*)pri->GetParent();
        bone_ctrl->GetAnimCtrlObj()->PlayPose(&play_info);

        if(i == 2) // octopus
        {
            lwINodeBoneCtrl* bone_ctrl = (lwINodeBoneCtrl*)_cha_obj[4]->GetParent();
            bone_ctrl->GetAnimCtrlObj()->PlayPose(&play_info);
        }
    }

    { // dummy 99 obj
        lwINodeDummy* node_dummy = (lwINodeDummy*)_cha_obj[_act_obj[3]]->GetParent()->GetParent();
        lwIAnimCtrlObjMat* ctrl_mat = (lwIAnimCtrlObjMat*)node_dummy->GetAnimCtrlObj();

        if(dummy_flag == 1)
        {
            play_info.bit_mask = PPI_MASK_TYPE | PPI_MASK_FRAME;
            play_info.frame = ctrl_mat->GetPlayPoseInfo()->frame;
            play_info.type = PLAY_FRAME;
        }
        else
        {
            play_info.bit_mask = PPI_MASK_TYPE;
            play_info.type = PLAY_LOOP_SMOOTH;
        }

        ctrl_mat->PlayPose(&play_info);
    }
}
int LoginScene_CreateCha::OnLButtonDown(int flag, int x, int y)
{
    if(_model_type != 1 )
        return -1;

	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());
	if(pCreateChaScene)
	{
		if( pCreateChaScene->frmRoleInfo->GetIsShow() )
			return -1;
	}

    lwINodePrimitive* pri;
    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

    lwPickInfo info;
    lwVector3 org, ray;
    dev_obj->ScreenToWorld(&org, &ray, x, y);

    lwPlayPoseInfo play_info;
    lwPlayPoseInfo_Construct(&play_info);
    play_info.bit_mask = PPI_MASK_POSE | PPI_MASK_TYPE | PPI_MASK_VELOCITY | PPI_MASK_FRAME;
    play_info.type = PLAY_LOOP_SMOOTH;
    play_info.velocity = 1.0f;
    play_info.frame = 0.0f;

    BOOL octopus_flag = 0;
    BOOL dummy_flag = 0;

    if(_arrow)
    {
        _arrow->GetPrimitive()->SetState(STATE_VISIBLE, 0);
    }

    for(DWORD i = 0; i < _act_num; i++)
    {        
        if((pri = _cha_obj[_act_obj[i]]) == 0)
            continue;

        if(LW_SUCCEEDED(pri->HitTest(&info, &org, &ray)))
        {
			return i;
        }
    }

    return -1;
    
}


//////////////////////////////////////////////////////////////////////////
// Class: CCreateChaScene
//////////////////////////////////////////////////////////////////////////

//~ static ====================================================================

CForm* CCreateChaScene::frmChaFound = 0; 
CEdit* CCreateChaScene::edtName = 0;
CMemo* CCreateChaScene::memChaDescribe = 0;
CLabel* CCreateChaScene::labHair = 0;
CLabel* CCreateChaScene::labFace = 0;


CForm* CCreateChaScene::frmChaCity = 0;
CMemo* CCreateChaScene::memChaDescribe2 = 0;
CLabel* CCreateChaScene::labCity = 0;

CForm* CCreateChaScene::frmQuit = 0;

CImage* CCreateChaScene::imgCities[MAX_CITY_NUM + 1][CITY_PICTURE_NUM] = {
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
};

CTextButton* CCreateChaScene::imgCitiesBlock[MAX_CITY_NUM] = {0,0,0};

int	CCreateChaScene::iCurrCity = 0;
int	CCreateChaScene::m_nCurCityIndex = 0;                    
bool CCreateChaScene::bShowDialog = false;




const char* CCreateChaScene::szDescripts[MAX_CHA_NUM] = 
{
    g_oLangRec.GetString(35),
    g_oLangRec.GetString(36),
    g_oLangRec.GetString(37),
    g_oLangRec.GetString(38),
};

const char* CCreateChaScene::szCityNames[MAX_CITY_NUM] = 
{
    g_oLangRec.GetString(39),
    g_oLangRec.GetString(40),
    g_oLangRec.GetString(41),
};

const char* CCreateChaScene::szCitiesDescripts[MAX_CITY_NUM] = 
{
	g_oLangRec.GetString(42),
	g_oLangRec.GetString(43),
	g_oLangRec.GetString(44),
};


int  CCreateChaScene::nHairTestCnt[MAX_HAIR_NUM] = { 2000, 2062, 2124, 2291};
int  CCreateChaScene::nFaceTestCnt[MAX_FACE_NUM] = { 2554, 2554, 2554, 2554};

//int CCreateChaScene::nSelHairNum[MAX_CHA_NUM] = { 61, 61, 62, 4, };
int CCreateChaScene::nSelHairNum[MAX_CHA_NUM] = { 8, 8, 8, 4, };
int CCreateChaScene::nSelFaceNum[MAX_CHA_NUM] = { 8, 8, 8, 8, };


//~ Constructors ==============================================================
CCreateChaScene::CCreateChaScene(stSceneInitParam &param)
: CGameScene(param), m_bInitOnce(false), m_bSameNameError(false)
{
	LG( "scene memory", "CCreateChaScene Create\n" );
}

//~ Destructors ===============================================================
CCreateChaScene::~CCreateChaScene()
{
	LG( "scene memory", "CCreateChaScene Destroy\n" );
}

//~ ������صĺ��� ==========================================================

//-----------------------------------------------------------------------
bool CCreateChaScene::_Init()
{
    bool bResult = CGameScene::_Init();
    
	if (!m_bInitOnce)
	{
		m_bInitOnce = true;

		m_LoginSceneCreateCha.Init(g_Render.GetInterfaceMgr()->res_mgr);
		if (m_LoginSceneCreateCha.LoadModelLXO("login03.lxo") != 1)
		{
			return false;
		}

		lwVector3 offsetPos4[4];
		offsetPos4[0].x = 3.276f;
		offsetPos4[0].y = 25.196f;
		offsetPos4[0].z = 4.8f;

		offsetPos4[1].x = -5.013f;
		offsetPos4[1].y = 17.272f;
		offsetPos4[1].z = 5.0f;

		offsetPos4[2].x = -21.163f;
		offsetPos4[2].y = 30.951f;
		offsetPos4[2].z = 5.254f;

		offsetPos4[3].x = 0.0f;
		offsetPos4[3].y = 0.0f;
		offsetPos4[3].z = 3.0f;

		lwVector3 OffsetPos4[4];
		OffsetPos4[0].x = 0.676f;
		OffsetPos4[0].y = 28.696f;
		OffsetPos4[0].z = 2.f;

		OffsetPos4[1].x = -7.013f;
		OffsetPos4[1].y = 21.072f;
		OffsetPos4[1].z = 2.5f;

		OffsetPos4[2].x = -20.663f;
		OffsetPos4[2].y = 30.951f;
		OffsetPos4[2].z = 2.f;

		OffsetPos4[3].x = 2.f;
		OffsetPos4[3].y = 0.5f;
		OffsetPos4[3].z = 0.1f;

		m_LoginSceneCreateCha.LoadArrowItem("target.lgo", offsetPos4);
	}

    //�趨����ṹ��
    CCameraCtrl *pCam = g_pGameApp->GetMainCam();
    if(pCam)
    {
        g_pGameApp->EnableCameraFollow(TRUE);	
        pCam->m_EyePos.x = -32.713f;
        pCam->m_EyePos.y = 71.002f;
        pCam->m_EyePos.z = 7.006f;

        pCam->m_RefPos.x = -0.259f;
        pCam->m_RefPos.y = 0.565f;
        pCam->m_RefPos.z = 6.366f;

    }
    g_Render.SetWorldViewFOV(Angle2Radian(42.0f));
    g_Render.SetWorldViewAspect(1.0f);
    g_Render.SetClip(1.0f, 2000.0f);

    g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
    g_Render.SetCurrentView(MPRender::VIEW_WORLD);

    //�����ĸ���ɫģ��
    int i=0;
    for (; i<4; i++)
    {
        m_pChaForUI[i] = AddCharacter(i + 1);
        if(m_pChaForUI[i])
        {
	        m_pChaForUI[i]->SetIsForUI( 1 );
        }
        else
        {
            return false;
        }
    }

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	m_oldTexFlag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
	m_oldMeshFlag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);

    _InitUI(); 

	srand( (unsigned)time( NULL ) );

	m_bSameNameError = false;

    return bResult;
}

//-----------------------------------------------------------------------
bool CCreateChaScene::_Clear()
{
	bool bResult = CGameScene::_Clear();

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, m_oldTexFlag);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, m_oldMeshFlag);

	return bResult;
}

//-----------------------------------------------------------------------
void CCreateChaScene::_FrameMove(DWORD dwTimeParam)
{
    CGameScene::_FrameMove(dwTimeParam);

    m_LoginSceneCreateCha.Update();

	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert( iMouseX, iMouseY );

	if (frmChaCity->GetIsShow() && !bShowDialog && CFormMgr::s_Mgr.ModalFormNum() == 1)
	{
		if (m_bFirstShow && iMouseX != m_iFirstMourseX && iMouseY != m_iFirstMourseY)
		{
			m_bFirstShow = false;
		}

		if (iMouseX != m_iFirstMourseX && iMouseY != m_iFirstMourseY)
		{
			if (m_bFirstShow)
				m_bFirstShow = false;
			else
			{
				int index = GetCityZone(iMouseX, iMouseY);
				ShowCityZone(index);
			}
		}
	}

}

//-----------------------------------------------------------------------
void CCreateChaScene::_Render()
{
    CGameScene::_Render();

    MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;

    RenderStateMgr* rsm = g_pGameApp->GetRenderStateMgr();

    rsm->BeginScene();
    rsm->BeginSceneObject();

    dev_obj->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dev_obj->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    dev_obj->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    dev_obj->SetRenderState(D3DRS_COLORVERTEX, TRUE);

    dev_obj->SetRenderState(D3DRS_LIGHTING, 0);
    dev_obj->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    m_LoginSceneCreateCha.Render();

    rsm->EndSceneObject();

    rsm->BeginTranspObject();
    lwUpdateSceneTransparentObject();
    rsm->EndTranspObject();


    rsm->EndScene();
}


//-----------------------------------------------------------------------
bool CCreateChaScene::_MouseMove(int nOffsetX, int nOffsetY)
{
	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();
    m_LoginSceneCreateCha.OnMouseMove(0, iMouseX, iMouseY);

    return true;
}

//-----------------------------------------------------------------------
bool CCreateChaScene::_MouseButtonDown(int nButton)	
{
	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();

    int nIndex = m_LoginSceneCreateCha.OnLButtonDown(0, iMouseX, iMouseY);

    if (nIndex < 0 || nIndex > 3)
        return false;

    if (nIndex == 1)
        m_nSelChaIndex = 2;
    else if (nIndex == 2)
        m_nSelChaIndex = 1;
    else
        m_nSelChaIndex = nIndex;


	if(frmLanchInfo && frmAimiInfo && frmFelierInfo && frmCaxiusInfo && frmRoleAllInfo)
	{
		DarkScene();

		switch(m_nSelChaIndex)
		{
		case 0:	// ����
			frmLanchInfo->ShowModal();
			break;

		case 1:	// ����˹
			frmCaxiusInfo->ShowModal();
			break;

		case 2:	// �����
			frmFelierInfo->ShowModal();
			break;

		case 3:	// ����
			frmAimiInfo->ShowModal();
			break;
		}

		return true;
	}

	ShowChaFoundForm();

    return true ;
}


//~ UI��صĺ��� =============================================================
bool CCreateChaScene::_InitUI()
{
    // ��ɫ��������
    {
        frmChaFound = CFormMgr::s_Mgr.Find("frmFound", GetInitParam()->nUITemplete);
        if (!frmChaFound)
        {
            LG( "xXx", "msgInit frmFound UI error\n" );
            return false;
        }
        CTextButton* btnLeftHair = (CTextButton*)frmChaFound->Find("btnLeftHair");
        if( !btnLeftHair ) 
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "btnLeftHair");
            return false;
        }
        btnLeftHair->evtMouseClick = __gui_event_left_hair;
        CTextButton* btnRightHair = (CTextButton*)frmChaFound->Find("btnRightHair");
        if( !btnRightHair ) 
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "btnRightHair");
            return false;
        }
        btnRightHair->evtMouseClick = __gui_event_right_hair;
        CTextButton* btnLeftFace = (CTextButton*)frmChaFound->Find("btnLeftFace");
        if( !btnLeftFace ) 
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "btnLeftFace");
            return false;
        }
        btnLeftFace->evtMouseClick = __gui_event_left_face;
        CTextButton* btnRightFace = (CTextButton*)frmChaFound->Find("btnRightFace");
        if( !btnRightFace ) 
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "btnRightFace");
            return false;
        }
        btnRightFace->evtMouseClick = __gui_event_right_face;

		// ���ͣ����� ���Ұ�ť��˸
		btnLeftHair->SetFlashCycle();
		btnRightHair->SetFlashCycle();
		btnLeftFace->SetFlashCycle();
		btnRightFace->SetFlashCycle();


        CTextButton* btnLeft3d = (CTextButton*)frmChaFound->Find("btnLeft3d");
        if (!btnLeft3d)
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "btnLeft3d");
            return false;
        }
		btnLeft3d->evtMouseClick = __gui_event_left_rotate;
        btnLeft3d->evtMouseDownContinue = __gui_event_left_continue_rotate;

        CTextButton* btnRight3d = (CTextButton*)frmChaFound->Find("btnRight3d");
        if (!btnRight3d)
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "btnRight3d");
            return false;
        }
		btnRight3d->evtMouseClick = __gui_event_right_rotate;
		btnRight3d->evtMouseDownContinue = __gui_event_right_continue_rotate;

        labHair = (CLabel*) frmChaFound->Find("labHairShow");
        if( !labHair ) 
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "labHairShow");
            return false;
        }

        labFace = (CLabel*) frmChaFound->Find("labFaceShow");
        if( !labFace ) 
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "labFaceShow");
            return false;
        }

        edtName = (CEdit*) frmChaFound->Find("edtName");
        if (!edtName)
        {
            Error(g_oLangRec.GetString(45), 
                frmChaFound->GetName(), "edtName");
            return false;
        }
        //edtName->evtChange = __gui_event_edit_change;

        memChaDescribe = (CMemo*)frmChaFound->Find("memChaDescribe");
        if (!memChaDescribe)
        {
            return Error(g_oLangRec.GetString(45), 
				frmChaFound->GetName(), "memChaDescribe");
        }
        C3DCompent* ui3dCreateCha = (C3DCompent*)frmChaFound->Find( "ui3dCreateCha" );
        if( ui3dCreateCha )
        {
            ui3dCreateCha->SetRenderEvent( __cha_render_event );
        }


        frmChaFound->evtEntrustMouseEvent = _ChaFoundFrmMouseEvent;
    }

    // ��ɫ����ѡ�����
    {
        frmChaCity = CFormMgr::s_Mgr.Find( "frmCity", GetInitParam()->nUITemplete );
        if (!frmChaFound)
        {
            LG( "xXx", "msgInit frmCity UI error\n" );
            return false;
        }

		frmChaCity->evtEntrustMouseEvent = _ChaCityFrmMouseEvent;
		//����ͼƬ
		char szPicNameBase[] = "imgCity%d%d";
		char szPicName[20];

		for (int i(0); i<MAX_CITY_NUM + 1; ++i)
		{
			for (int j(0); j<CITY_PICTURE_NUM; ++j)
			{
				sprintf(szPicName, szPicNameBase, i, j);
				imgCities[i][j] = (CImage*)frmChaCity->Find(szPicName);
				if (!imgCities[i][j])
				{
					return Error(g_oLangRec.GetString(46),
						frmChaCity->GetName(), szPicName);
				}
				if (i==0)
					imgCities[i][j]->SetIsShow(true);
				else
					imgCities[i][j]->SetIsShow(false);
			} //end of for 
		}
		iCurrCity = 0;

		//�������п�
		char szCityBlockName[] = "imgCity%d";

		for (int i(0); i<MAX_CITY_NUM; ++i)
		{
			sprintf(szPicName, szCityBlockName, i+1);
			imgCitiesBlock[i] = (CTextButton*)frmChaCity->Find(szPicName);
			if (!imgCitiesBlock[i])
			{
				return Error(g_oLangRec.GetString(46),
					frmChaCity->GetName(), szPicName);
			}
			imgCitiesBlock[i]->SetIsShow(true);	// for debug.
		}
    }

    // �˳��˵�����
    {
        frmQuit = CFormMgr::s_Mgr.Find( "frmQuit", GetInitParam()->nUITemplete );
        if (!frmQuit)
        {
            LG( "xXx", "msgInit frmQuit UI error\n" );
            return false;
        }
        frmQuit->evtEntrustMouseEvent = _QuitFrmMouseEvent;

        frmQuit->Find("imgBack")->SetIsShow(false); //��ʼʱͼƬ����ʾ
        frmQuit->SetPos(
            (g_pGameApp->GetWindowWidth() - frmQuit->GetWidth())/2,
            g_pGameApp->GetWindowHeight() - frmQuit->GetHeight() - 40 );
        frmQuit->Refresh();
        frmQuit->Show();
    }


	// �������̽���
	{
		frmRoleInfo = CFormMgr::s_Mgr.Find("frmRoleInfo");
		if(! frmRoleInfo)
		{
			LG( "xXx", "msgInit frmRoleInfo UI error\n" );
			return false;
		}
		frmRoleInfo->evtEntrustMouseEvent = _evtRoleInfoFormMouseEvent;
		frmRoleInfo->SetIsShow(true);
		DarkScene( true );

		frmLanchInfo = CFormMgr::s_Mgr.Find("frmLanchInfo");
		if(! frmLanchInfo)
		{
			LG( "xXx", "msgInit frmLanchInfo UI error\n" );
			return false;
		}
		frmLanchInfo->evtEntrustMouseEvent = _evtLanchInfoFormMouseEvent;

		frmAimiInfo = CFormMgr::s_Mgr.Find("frmAimiInfo");
		if(! frmAimiInfo)
		{
			LG( "xXx", "msgInit frmAimiInfo UI error\n" );
			return false;
		}
		frmAimiInfo->evtEntrustMouseEvent = _evtAimiInfoFormMouseEvent;

		frmFelierInfo = CFormMgr::s_Mgr.Find("frmFelierInfo");
		if(! frmFelierInfo)
		{
			LG( "xXx", "msgInit frmFelierInfo UI error\n" );
			return false;
		}
		frmFelierInfo->evtEntrustMouseEvent = _evtFelierInfoFormMouseEvent;

		frmCaxiusInfo = CFormMgr::s_Mgr.Find("frmCaxiusInfo");
		if(! frmCaxiusInfo)
		{
			LG( "xXx", "msgInit frmCaxiusInfo UI error\n" );
			return false;
		}
		frmCaxiusInfo->evtEntrustMouseEvent = _evtCaxiusInfoFormMouseEvent;

		frmRoleAllInfo = CFormMgr::s_Mgr.Find("frmRoleAllInfo");
		if(! frmRoleAllInfo)
		{
			LG( "xXx", "msgInit frmRoleAllInfo UI error\n" );
			return false;
		}
		frmRoleAllInfo->evtEntrustMouseEvent = _evtRoleAllInfoFormMouseEvent;

		// ���ְ��� MEMO
		memChaDescribeUp = dynamic_cast<CMemo*>(frmRoleAllInfo->Find("memChaDescribeUp"));
		if(! memChaDescribeUp)
		{
			LG( "xXx", "msgInit memChaDescribeUp UI error\n" );
			return false;
		}
		memChaDescribeDown = dynamic_cast<CMemo*>(frmRoleAllInfo->Find("memChaDescribeDown"));
		if(! memChaDescribeDown)
		{
			LG( "xXx", "msgInit memChaDescribeDown UI error\n" );
			return false;
		}

		char szChaView[64] = {0};
		memset(imgChaView, 0, sizeof(CImage*) * ROLE_ALL_INFO_COUNT);
		
		// ��ɫְҵ����ͼ
		for(int i = 0; i < ROLE_ALL_INFO_COUNT; ++i)
		{
			sprintf(szChaView, "imgChaView%d", i + 1);
			imgChaView[i] = dynamic_cast<CImage*>(frmRoleAllInfo->Find(szChaView));

			if(! imgChaView[i])
			{
				LG( "xXx", "msgInit %s UI error\n", szChaView );
				return false;
			}
		}
	}

    return true;
}


//~ �ص����� =================================================================

void CCreateChaScene::_SelectCity(CCompent *pSender, int nMsgType, 
								int x, int y, DWORD dwKey)
{
	bShowDialog = false;
	if( nMsgType == CForm::mrYes ) 
	{
		//֪ͨ������������ɫ
		GetCurrScene().SendChaToServ();

//		CGameApp::Waiting();

	} else if (nMsgType == CForm::mrNo)
	{
	}

}

//-----------------------------------------------------------------------
void CCreateChaScene::_ChaFoundFrmMouseEvent(CCompent *pSender, int nMsgType, 
                                             int x, int y, DWORD dwKey)
{
    string strName = pSender->GetName();

    if ( stricmp ("frmFound", pSender->GetForm()->GetName()) != 0 )
    {
        return ;
    }

    if(strName=="btnYes")
    {
        CCreateChaScene& rkScene = GetCurrScene();

        //ȷ����ť�¼�

        //����û���
        if (!rkScene.IsValidCheckChaName( edtName->GetCaption() ))
            return;

		if (rkScene.m_bSameNameError)
		{
			//ֱ�ӷ��ʹ�����ɫ
			
			GetCurrScene().SendChaToServ();//֪ͨ������������ɫ
			CGameApp::Waiting();
		}
		else
		{
			//��ѡ����н���

			frmChaFound->Close();	
			rkScene.InitChaCityFrm();//ͬ��ѡ����б�
			frmChaCity->ShowModal();//��ʾ��һ����
		}


    }
    else if ( strName == "btnNo" )
    {
        //ȡ����ť�¼�

        //�رոñ�,���س���
        frmChaFound->Close();

        //��������
        GetCurrScene().DarkScene(false);
		frmQuit->SetIsShow(true);
    }
}

//-----------------------------------------------------------------------
void CCreateChaScene::_ChaCityFrmMouseEvent(CCompent *pSender, int nMsgType, 
                                            int x, int y, DWORD dwKey)
{
    string strName = pSender->GetName();


    if ( stricmp ("frmCity", pSender->GetForm()->GetName()) != 0 )
    {
        return ;
    }

	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert( iMouseX, iMouseY );

	if (frmChaCity->GetIsShow())
	{
		int index = GetCurrScene().GetCityZone(iMouseX, iMouseY);

		if (CITY_BY_INDEX<=index && index<=CITY_BL_INDEX)
		{
			bShowDialog = true;
			GetCurrScene().m_nCurCityIndex = index - 1;
			g_stUIBox.ShowSelectBox(_SelectCity, szCitiesDescripts[index-1], true);
		}
	}

    //if(strName=="btnYes")
    //{
    //    //֪ͨ������������ɫ
    //    GetCurrScene().SendChaToServ();

    //    CGameApp::Waiting();

    //}
    if ( strName == "btnNo" )
    {
        //�رոñ�
        frmChaCity->Close();

        //������һ����
        frmChaFound->ShowModal();
    }

}

//-----------------------------------------------------------------------
void CCreateChaScene::_QuitFrmMouseEvent(CCompent *pSender, int nMsgType, 
                                         int x, int y, DWORD dwKey)
{
    string strName = pSender->GetName();

    if ( stricmp ("frmQuit", pSender->GetForm()->GetName()) != 0 )
    {
        return ;
    }

    if(strName=="btnNo")
    {
        //�˳���ť�¼�
        //g_pGameApp->MsgBox("�˳���ť�¼�");
        GetCurrScene().GotoSelChaScene();
    }
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_face(CGuiData *sender, 
                                            int x, int y, DWORD key )
{
    GetCurrScene().ChangeFace(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_face(CGuiData *sender, 
                                             int x, int y, DWORD key )
{
    GetCurrScene().ChangeFace(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_hair(CGuiData *sender, 
                                             int x, int y, DWORD key )
{
    GetCurrScene().ChangeHair(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_hair(CGuiData *sender,
                                             int x, int y, DWORD key )
{
    GetCurrScene().ChangeHair(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_city(CGuiData *sender, 
                                             int x, int y, DWORD key )
{
    GetCurrScene().ChangeCity(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_city(CGuiData *sender, 
                                             int x, int y, DWORD key )
{
    GetCurrScene().ChangeCity(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_rotate(CGuiData *sender, 
											  int x, int y, DWORD key )
{
	GetCurrScene().RotateChar(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_rotate(CGuiData *sender, 
											   int x, int y, DWORD key )
{
	GetCurrScene().RotateChar(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_continue_rotate(CGuiData *sender)
{
	GetCurrScene().RotateChar(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_continue_rotate(CGuiData *sender)
{
	GetCurrScene().RotateChar(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__cha_render_event( C3DCompent *pSender, int x, int y)
{
    GetCurrScene().RenderCha(x, y);
}


//~ �߼����� =================================================================

//-----------------------------------------------------------------------
void CCreateChaScene::ChangeFace(eDirectType enumDirect)
{
    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
        return;

    if (NULL == m_pChaForUI[m_nSelChaIndex]) return;

    //ȡ�õ�ǰ����ͷ���Ŀ�ʼ���
    const long nBeginIndex = nFaceTestCnt[m_nSelChaIndex];

    // ѭ���ƶ�
    m_nCurFaceIndex -= nBeginIndex;
    m_nCurFaceIndex += ((int)(enumDirect));
    m_nCurFaceIndex = (m_nCurFaceIndex + nSelFaceNum[m_nSelChaIndex]) % nSelFaceNum[m_nSelChaIndex];
    m_nCurFaceIndex += nBeginIndex;

    // �ı�����
    BOOL bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_FACE, m_nCurFaceIndex);
    if(bOK)
    {
        //����ԭ����ʾ"����+���"�ĳ���ʾ"����"
        CItemRecord* pItem = GetItemRecordInfo(m_nCurFaceIndex);
        if( pItem )
        {
            labFace->SetCaption(pItem->szName);
        }
    }				

}

//-----------------------------------------------------------------------
void CCreateChaScene::ChangeHair(eDirectType enumDirect)
{
    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
        return;

    if (NULL == m_pChaForUI[m_nSelChaIndex]) return;

    //ȡ�õ�ǰ����ͷ���Ŀ�ʼ���
    const long nBeginIndex = nHairTestCnt[m_nSelChaIndex];


    // ѭ���ƶ�
    m_nCurHairIndex -= nBeginIndex;
    m_nCurHairIndex += ((int)(enumDirect));
    m_nCurHairIndex = (m_nCurHairIndex + nSelHairNum[m_nSelChaIndex]) % nSelHairNum[m_nSelChaIndex];
    m_nCurHairIndex += nBeginIndex;

    // �ı�ͷ��
    BOOL bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_HEAD, m_nCurHairIndex);
    if(bOK)
    {
        CItemRecord* pItem = GetItemRecordInfo( m_nCurHairIndex );
        if( pItem )
        {
            labHair->SetCaption(pItem->szName);
        }
    }	
    else
    {
        LG( "error", g_oLangRec.GetString(47),  m_nCurHairIndex);
    }

}

//-----------------------------------------------------------------------
void CCreateChaScene::ChangeCity(eDirectType enumDirect)
{
    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
        return;

    CLabelEx  *labCityShow = ( CLabelEx *) frmChaCity ->Find("labCityShow");
    if (!labCityShow)    return ;

    //�ж�Ŀǰ�����


    m_nCurCityIndex += ((int)(enumDirect));

    // ѭ���ƶ�
    m_nCurCityIndex = (m_nCurCityIndex + MAX_CITY_NUM) % MAX_CITY_NUM;
    //labCityShow->SetCaption( szCities[m_nCurCityIndex]);
	memChaDescribe2->SetCaption(szCitiesDescripts[m_nCurCityIndex]);
	memChaDescribe2->ProcessCaption();
}



//-----------------------------------------------------------------------
void CCreateChaScene::RenderCha(int x,int y)
{
	g_Render.GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	g_Render.GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3) return;


    if( !m_pChaForUI[m_nSelChaIndex] ) return;

    g_Render.LookAt( D3DXVECTOR3( 11.0f, 36.0f, 10.0f ), D3DXVECTOR3( 8.70f, 12.0f, 8.0f ), MPRender::VIEW_3DUI );
    y +=100;

    MPMatrix44 old_mat = *m_pChaForUI[m_nSelChaIndex]->GetMatrix();
    m_pChaForUI[m_nSelChaIndex]->SetUIYaw(180 + m_nChaRotate);
    m_pChaForUI[m_nSelChaIndex]->SetUIScaleDis(9.0f * g_Render.GetScrWidth()/SMALL_RES_X );
    m_pChaForUI[m_nSelChaIndex]->RenderForUI(x, y);
    m_pChaForUI[m_nSelChaIndex]->SetMatrix(&old_mat);

    g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());

}

//-----------------------------------------------------------------------
void CCreateChaScene::RotateChar(eDirectType enumDirect)
{
    m_nChaRotate += 180;
    m_nChaRotate += -((int)(enumDirect)) * 15;
    m_nChaRotate = (m_nChaRotate + 360) % 360;
    m_nChaRotate -= 180;
}



//-----------------------------------------------------------------------
void CCreateChaScene::InitChaFoundFrm()
{
    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
        return;

	if (NULL == m_pChaForUI[m_nSelChaIndex]) return;


    CTextButton* btnLeftHair = (CTextButton*)frmChaFound->Find("btnLeftHair");
    if( !btnLeftHair ) return;
    CTextButton* btnRightHair = (CTextButton*)frmChaFound->Find("btnRightHair");
    if( !btnRightHair ) return;

    memChaDescribe->SetCaption(szDescripts[m_nSelChaIndex]);
    memChaDescribe->ProcessCaption();
    edtName->SetCaption(m_sName.c_str());


	BOOL bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_HEAD, m_nCurHairIndex);
	if(bOK)
	{
		CItemRecord* pItem = GetItemRecordInfo( m_nCurHairIndex );
		if( pItem )
		{
			labHair->SetCaption(pItem->szName);
		}
	}	
	else
	{
		LG( "error", g_oLangRec.GetString(47),  m_nCurHairIndex);
	}
	bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_FACE, m_nCurFaceIndex);
	if(bOK)
	{
		//����ԭ����ʾ"����+���"�ĳ���ʾ"����"
		CItemRecord* pItem = GetItemRecordInfo(m_nCurFaceIndex);
		if( pItem )
		{
			labFace->SetCaption(pItem->szName);
		}
	}				
	else
	{
		LG( "error", g_oLangRec.GetString(48),  m_nCurHairIndex);
	}

}

//-----------------------------------------------------------------------
void CCreateChaScene::InitChaCityFrm()
{
    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
        return;

	m_iFirstMourseX = g_pGameApp->GetMouseX();
	m_iFirstMourseY = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert( m_iFirstMourseX, m_iFirstMourseY );
	m_bFirstShow = true;

	iCurrCity = 0;

    //memChaDescribe2->SetCaption(szCitiesDescripts[m_nCurCityIndex]);
    //memChaDescribe2->ProcessCaption();
    //labCity->SetCaption(szCities[m_nCurCityIndex]);
}

//-----------------------------------------------------------------------
void CCreateChaScene::InitChaData()
{
    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
        return;

    m_sName = "";									//��ɫ��
    m_nCurHairIndex = nHairTestCnt[m_nSelChaIndex]; //��ǰ�ķ��ͱ��
    m_nCurFaceIndex = nFaceTestCnt[m_nSelChaIndex]; //��ǰ�����ͱ��

	//������������س��б��
	int i = rand();
	if (i<RAND_MAX/3)
		m_nCurCityIndex = 0;
	else if (i<RAND_MAX*2/3)
		m_nCurCityIndex = 1;
	else
		m_nCurCityIndex = 2;

    m_nChaRotate = 0;                               //�ϴ������ƫ�ƽǶ�(-180~+180)

}


//-----------------------------------------------------------------------
bool CCreateChaScene::IsValidCheckChaName(const char *name)
{
    /* Copy from LoginScene.cpp */
    if (stricmp(name, g_oLangRec.GetString(49)) == 0)
    {
        g_pGameApp->MsgBox(g_oLangRec.GetString(49));
        return false;
    }

    if (strlen(name) <= 0)
    {
        g_pGameApp->MsgBox(g_oLangRec.GetString(50));
        return false;
    }

    if( !::IsValidName( name, (unsigned short)strlen(name) ) )
    {
        g_pGameApp->MsgBox( g_oLangRec.GetString(51));
        return false;
    }
    //return true;

    const char* s = name ;
    int len = (int)strlen(s) ;
    bool bOk  = true;

    for ( int i = 0; i< len ; i++)
    {
        if ( s[i]&0x80 )
        {
            if (!(s[i]==-93) )  //���ڴ����Ƿ���˫�ֽڵ���ĸ
            {
                i++;
            }
            else
            {
                bOk = false;
                i++;
                break;
            }
        }
        else
        {
            if (  !( isdigit(s[i])||isalpha(s[i]) ) )
            {
                bOk = false;			
                break;
            }
        }	
    }

    if (!bOk )
        g_pGameApp->MsgBox( g_oLangRec.GetString(52));

    //����Ƿ��в�����Ϊ��ɫ���Ĵ�
    string sName(name);
    if (!CTextFilter::IsLegalText(CTextFilter::NAME_TABLE, sName))
    {
        g_pGameApp->MsgBox(g_oLangRec.GetString(53));
        return false;
    }

    return bOk;
}

//-----------------------------------------------------------------------
void CCreateChaScene::DarkScene(bool isDark)
{
    frmQuit->Find("imgBack")->SetIsShow(isDark);
}

//-----------------------------------------------------------------------
void CCreateChaScene::SendChaToServ()
{
    if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)   return;

    if (NULL == m_pChaForUI[m_nSelChaIndex])   return;
    
    //stNetChangeChaPart part;
    //memset( &part, 0, sizeof(part) );
    //part.sTypeID = (short)m_pChaForUI[m_nSelChaIndex]->getTypeID();
    //part.sHairID = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(0);
    //part.SLink[enumEQUIP_FACE] = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(1);
	//char szLookbuf[defLOOK_DATA_STRING_LEN];
	//if (LookData2String(&part, szLookbuf, defLOOK_DATA_STRING_LEN))
	//{
	//	CS_NewCha( edtName->GetCaption(), szCityNames[m_nCurCityIndex], part );
	//	CGameApp::Waiting();
	//}
    //
	//else
	//	g_pGameApp->MsgBox(g_oLangRec.GetString(54));


	int sTypeID = (short)m_pChaForUI[m_nSelChaIndex]->getTypeID();
	int sHairID = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(0);
	int faceID  =  (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(1);
	CS_NewCha2( edtName->GetCaption(), szCityNames[m_nCurCityIndex], sTypeID,sHairID,faceID );

}

//-----------------------------------------------------------------------
void CCreateChaScene::CreateNewCha()
{
    CGameApp::Waiting(false);

	m_bSameNameError = false;

    //�رոñ�
	frmChaFound->Close();
    frmChaCity->Close();

	if (!m_pChaForUI[m_nSelChaIndex])
		return;

    string  sName = edtName->GetCaption();

    stNetChangeChaPart part;
    memset( &part, 0, sizeof(part) );
    part.sTypeID = (short)m_pChaForUI[m_nSelChaIndex]->getTypeID();
    part.sHairID = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(0);
    part.SLink[enumEQUIP_FACE] = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(1);
    int nChaIndex = m_nSelChaIndex;

    //�л���ѡ�˳�������
    GotoSelChaScene();

    CSelectChaScene& rkScene = CSelectChaScene::GetCurrScene();
    rkScene.CreateCha(sName, nChaIndex, &part);
}

//-----------------------------------------------------------------------
CCreateChaScene& CCreateChaScene::GetCurrScene()
{
    CCreateChaScene* pScene = 
        dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

    assert(NULL != pScene);

    return *pScene;
}

//-----------------------------------------------------------------------
void CCreateChaScene::NewChaError( int error_no, const char* error_info )
{

	if (ERR_PT_SAMECHANAME == error_no)
	{
		//�ر�ѡ����б�
		frmChaCity->Close();

		m_bSameNameError = true;
		frmChaFound->ShowModal();

		g_pGameApp->MsgBox( "%s", g_GetServerError(error_no) );
		LG( "error", "%s Error, Code:%d, Info: %s", error_info, error_no, g_GetServerError(error_no) );
		CGameApp::Waiting( false );

	}
	else
	{
		g_pGameApp->MsgBox( "%s", g_GetServerError(error_no) );
		LG( "error", "%s Error, Code:%d, Info: %s", error_info, error_no, g_GetServerError(error_no) );
		CGameApp::Waiting( false );

		if (!m_bSameNameError)
		{
			m_bSameNameError = false;
		}
	}

}

//-----------------------------------------------------------------------
void CCreateChaScene::GotoSelChaScene()
{
    g_pGameApp->GotoScene(m_pkLastScene, true);
}

//-----------------------------------------------------------------------
int	CCreateChaScene::GetCityZone(int x, int y)
{
	if (!frmChaCity->GetIsShow())
		return 0;

	CGuiData* pSize = NULL;
	for (int i(0); i<MAX_CITY_NUM; ++i)
	{
		pSize = imgCitiesBlock[i];
		if( x >= pSize->GetX() && x <= pSize->GetX2() && y >= pSize->GetY() && y <= pSize->GetY2() )
		{
			return i+1;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------
void CCreateChaScene::ShowCityZone(int index)
{
	if (!frmChaCity->GetIsShow())
		return;

	else if (iCurrCity != index)
	{
		if (iCurrCity == 0)
		{
			for (int j(0); j<CITY_PICTURE_NUM; ++j)
			{
				imgCities[index][j]->SetIsShow(true);
			}
			iCurrCity = index;
		}
		else
		{
			for (int j(0); j<CITY_PICTURE_NUM; ++j)
			{
				imgCities[iCurrCity][j]->SetIsShow(false);
				imgCities[index][j]->SetIsShow(true);
			}
			iCurrCity = index;
		}
	}
}


// ��װ��loading��,ˢ��
void CCreateChaScene::LoadingCall()
{
    CGameScene::LoadingCall();

	// Hide the role info page
	/*if(frmRoleInfo)
	{
		DarkScene();
		frmRoleInfo->ShowModal();
	}*/
}


void CCreateChaScene::ShowChaFoundForm()
{
    //��ʼ����ɫ����
    InitChaData();

    //ͬ�����ﴴ����
    InitChaFoundFrm();

    //�����䰵
    this->DarkScene(true);
	frmQuit->SetIsShow(false);

	//��ʾ��
    frmChaFound->ShowModal();
}


void CCreateChaScene::ShowAllRoleInfo(int nRoleInfo)
{
	if(0 < nRoleInfo && nRoleInfo <= ROLE_ALL_INFO_COUNT)
	{
		for(int i = 0; i < ROLE_ALL_INFO_COUNT; ++i)
		{
			if(imgChaView[i] && (i + 1) == nRoleInfo && memChaDescribeUp && memChaDescribeDown)
			{
				imgChaView[i]->SetIsShow(true);

				switch(nRoleInfo)
				{
				case 2:  // ��ʿ
				case 9:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(803));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(804));
					break;

				case 5:  // ˫��ʿ
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(806));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(807));
					break;

				case 10: // �޽�ʿ
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(808));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(809));
					break;

				case 13: // ҩʦ
				case 20:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(810));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(811));
					break;

				case 16: // ʥְ��
				case 22:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(812));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(813));
					break;

				case 17: // ��ӡʦ
				case 23:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(814));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(815));
					break;

				case 4:  // ð����
				case 14:
				case 21:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(816));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(817));
					break;

				case 7:  // ����ʿ
				case 18:
				case 24:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(818));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(819));
					break;

				case 3:  // ����
				case 12:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(820));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(821));
					break;

				case 6:  // �ѻ���
				case 15:
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(822));
					memChaDescribeDown->SetCaption(g_oLangRec.GetString(823));
					break;

				case 1:  // ����
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(35));
					memChaDescribeDown->SetCaption("");
					break;

				case 8:  // ����˹
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(36));
					memChaDescribeDown->SetCaption("");
					break;

				case 11:  // ������
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(37));
					memChaDescribeDown->SetCaption("");
					break;

				case 19:  // ����
					memChaDescribeUp->SetCaption(g_oLangRec.GetString(38));
					memChaDescribeDown->SetCaption("");
					break;

				default:
					memChaDescribeUp->SetCaption("");
					memChaDescribeDown->SetCaption("");
					break;
				}

				memChaDescribeUp->ProcessCaption();
				memChaDescribeDown->ProcessCaption();
			}
			else
			{
				imgChaView[i]->SetIsShow(false);
			}
		}

		frmRoleAllInfo->ShowModal();
	}
}


void CCreateChaScene::_evtRoleInfoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();

	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());
	if(pCreateChaScene)
	{
		lwVector3 OffsetPos4[4];
		OffsetPos4[0].x = 0.676f;
		OffsetPos4[0].y = 28.696f;
		OffsetPos4[0].z = 2.f;

		OffsetPos4[1].x = -7.013f;
		OffsetPos4[1].y = 21.072f;
		OffsetPos4[1].z = 2.5f;

		OffsetPos4[2].x = -20.663f;
		OffsetPos4[2].y = 30.951f;
		OffsetPos4[2].z = 2.f;

		OffsetPos4[3].x = 2.f;
		OffsetPos4[3].y = 0.5f;
		OffsetPos4[3].z = 0.1f;

		//pCreateChaScene->m_LoginSceneCreateCha.LoadArrowMark( "hand.lgo", OffsetPos4 );

		pCreateChaScene->DarkScene(false);
		
		pCreateChaScene->frmRoleInfo->Close();
	}
}

// ������� ��1 ~ 7��
void CCreateChaScene::_evtLanchInfoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if(pCreateChaScene)
	{
		pCreateChaScene->frmLanchInfo->Close();

		if(strName == "btnNextStep")
		{
			pCreateChaScene->ShowChaFoundForm();
		}
		else if(strName == "btnClose")
		{
			pCreateChaScene->DarkScene(false);
		}
		else if(strName == "btnViewPlayer_1")
		{
			pCreateChaScene->ShowAllRoleInfo(1);	// �����ʼ
		}
		else if(strName == "btnViewPlayer_2")
		{
			pCreateChaScene->ShowAllRoleInfo(2);	// ��ʿ
		}
		else if(strName == "btnViewPlayer_3")
		{
			pCreateChaScene->ShowAllRoleInfo(3);	// ����
		}
		else if(strName == "btnViewPlayer_4")
		{
			pCreateChaScene->ShowAllRoleInfo(4);	// ð����
		}
		else if(strName == "btnViewPlayer_5")
		{
			pCreateChaScene->ShowAllRoleInfo(5);	// ˫��ʿ
		}
		else if(strName == "btnViewPlayer_6")
		{
			pCreateChaScene->ShowAllRoleInfo(6);	// �ѻ���
		}
		else if(strName == "btnViewPlayer_7")
		{
			pCreateChaScene->ShowAllRoleInfo(7);	// ����ʿ
		}
	}
}

// ���׽��� ��19 ~ 24��
void CCreateChaScene::_evtAimiInfoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if(pCreateChaScene)
	{
		pCreateChaScene->frmAimiInfo->Close();

		if(strName == "btnNextStep")
		{
			pCreateChaScene->ShowChaFoundForm();
		}
		else if(strName == "btnClose")
		{
			pCreateChaScene->DarkScene(false);
		}
		else if(strName == "btnViewPlayer_19")
		{
			pCreateChaScene->ShowAllRoleInfo(19);	// ���׳�ʼ
		}
		else if(strName == "btnViewPlayer_20")
		{
			pCreateChaScene->ShowAllRoleInfo(20);	// ҩʦ
		}
		else if(strName == "btnViewPlayer_21")
		{
			pCreateChaScene->ShowAllRoleInfo(21);	// ð����
		}
		else if(strName == "btnViewPlayer_22")
		{
			pCreateChaScene->ShowAllRoleInfo(22);	// ʥְ��
		}
		else if(strName == "btnViewPlayer_23")
		{
			pCreateChaScene->ShowAllRoleInfo(23);	// ��ӡʦ
		}
		else if(strName == "btnViewPlayer_24")
		{
			pCreateChaScene->ShowAllRoleInfo(24);	// ����ʿ
		}
	}
}

// ���������� ��11 ~ 18��
void CCreateChaScene::_evtFelierInfoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if(pCreateChaScene)
	{
		pCreateChaScene->frmFelierInfo->Close();

		if(strName == "btnNextStep")
		{
			pCreateChaScene->ShowChaFoundForm();
		}
		else if(strName == "btnClose")
		{
			pCreateChaScene->DarkScene(false);
		}
		else if(strName == "btnViewPlayer_11")
		{
			pCreateChaScene->ShowAllRoleInfo(11);	// ��������ʼ
		}
		else if(strName == "btnViewPlayer_12")
		{
			pCreateChaScene->ShowAllRoleInfo(12);	// ����
		}
		else if(strName == "btnViewPlayer_13")
		{
			pCreateChaScene->ShowAllRoleInfo(13);	// ҩʦ
		}
		else if(strName == "btnViewPlayer_14")
		{
			pCreateChaScene->ShowAllRoleInfo(14);	// ð����
		}
		else if(strName == "btnViewPlayer_15")
		{
			pCreateChaScene->ShowAllRoleInfo(15);	// �ѻ���
		}
		else if(strName == "btnViewPlayer_16")
		{
			pCreateChaScene->ShowAllRoleInfo(16);	// ʥְ��
		}
		else if(strName == "btnViewPlayer_17")
		{
			pCreateChaScene->ShowAllRoleInfo(17);	// ��ӡʦ
		}
		else if(strName == "btnViewPlayer_18")
		{
			pCreateChaScene->ShowAllRoleInfo(18);	// ����ʿ
		}
	}
}

// ����˹���� ��8 ~ 10��
void CCreateChaScene::_evtCaxiusInfoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if(pCreateChaScene)
	{
		pCreateChaScene->frmCaxiusInfo->Close();

		if(strName == "btnNextStep")
		{
			pCreateChaScene->ShowChaFoundForm();
		}
		else if(strName == "btnClose")
		{
			pCreateChaScene->DarkScene(false);
		}
		else if(strName == "btnViewPlayer_8")
		{
			pCreateChaScene->ShowAllRoleInfo(8);	// ����˹��ʼ
		}
		else if(strName == "btnViewPlayer_9")
		{
			pCreateChaScene->ShowAllRoleInfo(9);	// ��ʿ
		}
		else if(strName == "btnViewPlayer_10")
		{
			pCreateChaScene->ShowAllRoleInfo(10);	// �޽�ʿ
		}
	}
}

void CCreateChaScene::_evtRoleAllInfoFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
{
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if(strName == "btnPrevStep")
	{
		if(pCreateChaScene)
		{
			pCreateChaScene->frmRoleAllInfo->Close();

			switch(pCreateChaScene->m_nSelChaIndex)
			{
			case 0:	// ����
				pCreateChaScene->frmLanchInfo->ShowModal();
				break;

			case 1:	// ����˹
				pCreateChaScene->frmCaxiusInfo->ShowModal();
				break;

			case 2:	// ������
				pCreateChaScene->frmFelierInfo->ShowModal();
				break;

			case 3:	// ����
				pCreateChaScene->frmAimiInfo->ShowModal();
				break;
			}
		}
	}
}

