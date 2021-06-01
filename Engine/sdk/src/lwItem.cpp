//


#include "lwItem.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwResourceMgr.h"
#include "lwAnimCtrl.h"
#include "lwRenderImp.h"
#include "lwPathInfo.h"
#include "lwD3D.h"
#include "lwShaderMgr.h"

LW_BEGIN


//lwItem
LW_STD_IMPLEMENTATION( lwItem )

lwItem::lwItem( lwIResourceMgr* res_mgr )
: _id( LW_INVALID_INDEX ), _res_mgr( res_mgr ), _obj( 0 ), _link_ctrl( 0 )
{
    _opacity = 1.0f;
    lwMatrix44Identity( &_mat_base );
}
lwItem::~lwItem()
{
    Destroy();
}

LW_RESULT lwItem::Load(lwIGeomObjInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_obj)
        goto __ret;


    lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
    lwISystem* sys = sys_graphics->GetSystem();
    lwIPathInfo* path_info = 0;
    sys->GetInterface( (LW_VOID**)&path_info, LW_GUID_PATHINFO );

    // create new item object
    lwPrimitive* imp = LW_NEW( lwPrimitive( _res_mgr ) );


    if( LW_FAILED( imp->Load(info, path_info->GetPath( PATH_TYPE_TEXTURE_ITEM ), NULL ) ) )
        goto __ret;

    _obj = imp;

    _res_mgr->RegisterObject( &_id, this, OBJ_TYPE_ITEM );

    RegisterSceneMgr(_res_mgr->GetSysGraphics()->GetSceneMgr());

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwItem::Load( const char* file, int arbitrary_flag )
{
    if( _obj )
    {
        return LW_RET_FAILED;
    }

    LW_RESULT ret = LW_RET_OK;


    lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
    lwISystem* sys = sys_graphics->GetSystem();
    lwIPathInfo* path_info = 0;
    sys->GetInterface( (LW_VOID**)&path_info, LW_GUID_PATHINFO );

    if(arbitrary_flag == 1)
        goto __a;

    // query model pool
    lwItem* obj_model = 0;
    _res_mgr->QueryObject( (void**)&obj_model, OBJ_TYPE_ITEM, file );

    if( obj_model )
    {
        // copy
        obj_model->Copy(this);
    }
    else
    {
__a:
        char path[ LW_MAX_PATH ];
        sprintf( path, "%s%s", path_info->GetPath( PATH_TYPE_MODEL_ITEM ), file );

        lwResFile res;
        res.obj_id = 0;
        res.res_type = RES_FILE_TYPE_GEOMETRY;
        _tcscpy( res.file_name, path );

        // create new item object
        lwPrimitive* imp = LW_NEW( lwPrimitive( _res_mgr ) );


        // begin load mesh
        lwGeomObjInfo info;

        if( LW_FAILED( info.Load( path ) ) )
            return LW_RET_FAILED;

        if( LW_FAILED( imp->Load( &info, path_info->GetPath( PATH_TYPE_TEXTURE_ITEM ), &res ) ) )
            return LW_RET_FAILED;

        _obj = imp;

        SetFileName( file );
    }

    _res_mgr->RegisterObject( &_id, this, OBJ_TYPE_ITEM );

    RegisterSceneMgr(_res_mgr->GetSysGraphics()->GetSceneMgr());

    return ret;

} 

LW_RESULT lwItem::Destroy()
{
    if( _obj == 0 )
        return LW_RET_FAILED;
    
    _obj->Destroy();
    LW_DELETE( _obj );
    _obj = 0;

    _res_mgr->UnregisterObject( NULL, _id, OBJ_TYPE_ITEM );
    _id = LW_INVALID_INDEX;

    return LW_RET_OK;
}

LW_RESULT lwItem::Copy( lwIItem* src_obj )
{
    lwItem* o = (lwItem*)src_obj;

    _tcscpy( _file_name, o->_file_name );
    
    o->_obj->Clone( &_obj );

    _id = o->_id;
    _item_type = o->_item_type;
    _mat_base = o->_mat_base;
    _link_ctrl = o->_link_ctrl;
    _link_item_id = o->_link_item_id;
    _link_parent_id = o->_link_parent_id;

    return LW_RET_OK;

}

LW_RESULT lwItem::Clone( lwIItem** ret_obj )
{
    lwItem* o;
    _res_mgr->CreateItem( (lwIItem**)&o );

    o->Copy( this );

    *ret_obj = o;
    //_tcscpy( o->_file_name, _file_name );
    //

    //lwIPrimitive* p;
    //_obj->Clone( &p );

    //o->_obj = p;

    //o->_id = _id;
    //o->_item_type = _item_type;
    //o->_mat_base = _mat_base;
    //o->_link_ctrl = _link_ctrl;
    //o->_link_item_id = _link_item_id;
    //o->_link_parent_id = _link_parent_id;


    return LW_RET_OK;
}

LW_RESULT lwItem::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    if( _obj )
    {
        //_link_ctrl = 0;
        if( _link_ctrl )
        {
            lwMatrix44 mat_parent;

            if( LW_FAILED( _link_ctrl->GetLinkCtrlMatrix( &mat_parent, _link_parent_id ) ) )
                goto __ret;

            lwMatrix44 mat_dummy;

            if( LW_FAILED( GetDummyMatrix( &mat_dummy, _link_item_id ) ) )
                goto __ret;


            lwMatrix44InverseNoScaleFactor( &mat_dummy, &mat_dummy );

            lwMatrix44Multiply( &_mat_base, &mat_dummy, &mat_parent );

        }

        _obj->SetMatrixParent( &_mat_base );

        if( LW_FAILED( _obj->Update() ) )
            goto __ret;
    }

    ret = LW_RET_OK;

__ret:

    return ret;
}

LW_RESULT lwItem::Render()
{    
    LW_RESULT ret = LW_RET_FAILED;

    if( _state_ctrl.GetState( STATE_VISIBLE ) == 0 )
        goto __addr_ok;


    if(_obj)
    {     
        if(_scene_mgr && _obj->GetState(STATE_TRANSPARENT))
        {
            if(LW_FAILED(_scene_mgr->AddTransparentPrimitive(_obj)))
                goto __ret;
        }
        else
        {
            if(LW_FAILED(_obj->Render()))
                goto __ret;
        }
    }

__addr_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

void lwItem::SetMaterial( const lwMaterial* mtl )
{
    if( _obj )
    {
        _obj->SetMaterial(mtl);
    }
}

LW_RESULT lwItem::HitTestPrimitive( lwPickInfo* info, const lwVector3* org, const lwVector3* ray )
{
    lwPickInfo p, x;
    x.dis = -1.0f;

    if( LW_SUCCEEDED( _obj->HitTest( &p, org, ray ) ) )
    {
        if( (x.dis == -1.0f) || (x.dis > p.dis) )
        {
            x = p;
        }
    }

    if( x.dis == -1.0f )
    {
        return LW_RET_FAILED;
    }
    else
    {
        *info = x;
        return LW_RET_OK;
    }

}

void lwItem::ShowBoundingObject( int show )
{
    if( _obj )
    {
        if(_obj->GetHelperObject())
        {
            _obj->GetHelperObject()->SetVisible(show);
        }
    }
}

const lwMatrix44* lwItem::GetObjDummyMatrix( DWORD id )
{
    lwMatrix44* ret = 0;

    if( _obj == 0 )
        goto __ret;

    lwIHelperObject* h = _obj->GetHelperObject();
    if(h == 0)
        goto __ret;

    lwIHelperDummy* hd = h->GetHelperDummy();
    if(hd == 0)
        goto __ret;

    lwHelperDummyInfo* info = hd->GetDataInfoWithID( id );

    if( info == 0 )
        goto __ret;

    ret = &info->mat;
__ret:
    return ret;
}

const lwMatrix44* lwItem::GetObjBoneDummyMatrix( DWORD id )
{
    lwIAnimCtrlAgent* anim_agent = _obj->GetAnimAgent();
    
    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_BONE;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    lwIAnimCtrlObj* aca_bone = (lwIAnimCtrlObj*)anim_agent->GetAnimCtrlObj(&type_info);
    lwIAnimCtrlBone* c = (lwIAnimCtrlBone*)aca_bone->GetAnimCtrl();
    if( c == 0 )
        return NULL;

    DWORD rtmat_num = c->GetDummyNum();
    lwIndexMatrix44* rtmat_seq = c->GetDummyRTMSeq();

    for( DWORD i = 0; i < rtmat_num; i++ )
    {
        if( rtmat_seq[ i ].id == id )
        {
            return &rtmat_seq[ id ].mat;
        }
    }

    return NULL;
}

LW_RESULT lwItem::GetObjDummyRunTimeMatrix( lwMatrix44* mat, DWORD id )
{
    LW_RESULT ret = LW_RET_FAILED;

    lwMatrix44 mat_dummy;

    if( _obj == 0 )
        goto __ret;

    
    if( LW_FAILED( GetDummyMatrix( &mat_dummy, id ) ) )
        goto __ret;

    //lwMatrix44Multiply( mat, &mat_dummy, &_mat_base );
    lwMatrix44Multiply(mat, &mat_dummy, _obj->GetMatrixGlobal());
    // ���ﶨ�����û�й�������
    //lwIHelperObject* ho = _obj->GetHelperObject();
    //if(ho == 0)
    //    goto __ret;
    //lwIHelperDummy* hd = ho->GetHelperDummy();
    //if(hd == 0)
    //    goto __ret;
    //lwMatrix44Multiply(mat, &mat_dummy, hd->GetMatrixParent());

    ret = LW_RET_OK;

__ret:
    return ret;
}



LW_RESULT lwItem::GetDummyMatrix( lwMatrix44* mat, DWORD id )
{
    LW_RESULT ret = LW_RET_FAILED;

    const lwMatrix44* mat_dummy;

    lwIAnimCtrlAgent* anim_agent = _obj->GetAnimAgent();   
    if(anim_agent == 0)
        goto __addr_obj_dummy;

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_BONE;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
    if(ctrl_obj == 0)
        goto __addr_obj_dummy;

    if(ctrl_obj->GetAnimCtrl() == 0)
        goto __addr_obj_dummy;


    mat_dummy = GetObjBoneDummyMatrix(id);
    goto __addr_1;

__addr_obj_dummy:
    mat_dummy = GetObjDummyMatrix( id );

__addr_1:
    if( mat_dummy == 0 )
        goto __ret;

    if( mat )
    {
        *mat = *mat_dummy;
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwItem::SetLinkCtrl( lwLinkCtrl* ctrl, DWORD link_parent_id, DWORD link_item_id )
{
    if( LW_FAILED( GetDummyMatrix( NULL, link_item_id ) ) )
        return LW_RET_FAILED;

    _link_ctrl = ctrl;
    _link_parent_id = link_parent_id;
    _link_item_id = link_item_id;

    return LW_RET_OK;
}

LW_RESULT lwItem::ClearLinkCtrl()
{
    _link_ctrl = 0;
    _link_parent_id = LW_INVALID_INDEX;
    _link_item_id = LW_INVALID_INDEX;

    return LW_RET_OK;
}

LW_RESULT lwItem::PlayDefaultAnimation()
{
    return lwPrimitivePlayDefaultAnimation( _obj );
}
void lwItem::SetOpacity(float opacity)
{
    if(_obj)
    {
        _opacity = opacity;
        _obj->SetOpacity(opacity);
    }
}

LW_RESULT lwItem::SetTextureLOD(DWORD level)
{
    if(_obj)
    {
        _obj->SetTextureLOD(level);
    }

    return LW_RET_OK;
}

LW_END