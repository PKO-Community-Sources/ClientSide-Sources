//
#pragma once

#include "lwHeader.h"
#include "lwITypes.h"
#include "lwInterfaceExt.h"

LW_BEGIN

class lwPathInfo : public lwIPathInfo
{
public:
    char _path_buf[ PATH_TYPE_NUM ][ LW_MAX_PATH ];

LW_STD_DECLARATION()

public:
    lwPathInfo();

    char* SetPath( DWORD type, const char* path );
    char* GetPath( DWORD type );

};

class lwOptionMgr : public lwIOptionMgr
{
LW_STD_DECLARATION()
private:
    BYTE _ignore_model_tex_flag;
    BYTE _byte_flag_seq[MAX_OPTION_BYTE_FLAG];

public:
    lwOptionMgr();
    ~lwOptionMgr();
    void SetIgnoreModelTexFlag(BYTE flag) { _ignore_model_tex_flag = flag; }
    BYTE GetIgnoreModelTexFlag() const { return _ignore_model_tex_flag; }
    void SetByteFlag(DWORD type, BYTE value) { _byte_flag_seq[type] = value; }
    BYTE GetByteFlag(DWORD type) const { return _byte_flag_seq[type]; }
};


LW_END