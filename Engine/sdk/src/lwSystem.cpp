//


#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwPathInfo.h"

LW_BEGIN

lwISystem* lwSystem::__system = LW_NULL;

LW_STD_RELEASE( lwSystem )

LW_RESULT lwSystem::GetInterface( LW_VOID** i, lwGUID guid )
{
    LW_RESULT ret;

    switch( guid )
    {
    case LW_GUID_PATHINFO:
        *i = (LW_VOID*)_path_info;
        ret = LW_RET_OK;
        break;
    case LW_GUID_OPTIONMGR:
        *i = (LW_VOID*)_option_mgr;
        ret = LW_RET_OK;
        break;
    case LW_GUID_TIMER:
        *i = (LW_VOID*)_internal_timer;
        ret = LW_RET_OK;
        break;
    case LW_GUID_SYSTEMINFO:
        *i = (LW_VOID*)_system_info;
        ret = LW_RET_OK;
        break;
    default:
        ret = LW_RET_NULL;
    }

    return ret;
}

lwSystem::lwSystem()
{
    _path_info = 0;
    _option_mgr = 0;
    _system_info = 0;
    _internal_timer = 0;
}

lwSystem::~lwSystem()
{
    LW_SAFE_RELEASE(_path_info);
    LW_SAFE_RELEASE(_option_mgr);
    LW_SAFE_RELEASE(_system_info);
    LW_SAFE_RELEASE(_internal_timer);
}

LW_RESULT lwSystem::Initialize()
{
    LW_RESULT ret = LW_RET_FAILED;

    _path_info = LW_NEW(lwPathInfo);
    _option_mgr = LW_NEW(lwOptionMgr);

    lwGUIDCreateObject((LW_VOID**)&_system_info, LW_GUID_SYSTEMINFO);
    lwGUIDCreateObject((LW_VOID**)&_internal_timer, LW_GUID_TIMER);

    if(LW_FAILED(_system_info->CheckDirectXVersion()))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwSystem::CreateGraphicsSystem( lwISysGraphics** sys )
{
    *sys = LW_NEW( lwSysGraphics( this ) );

    return LW_RET_OK;
}


LW_END