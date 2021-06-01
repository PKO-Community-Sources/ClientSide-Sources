//


#include "lwxRenderCtrlVS.h"

LW_BEGIN

lwIRenderCtrlVS* __RenderCtrlVSProcVSVertexBlend_dx8()
{
    return LW_NEW(lwxRenderCtrlVSVertexBlend_dx8);
}
#if(defined LW_USE_DX9)
lwIRenderCtrlVS* __RenderCtrlVSProcVSVertexBlend_dx9()
{
    return LW_NEW(lwxRenderCtrlVSVertexBlend);
}
#endif

LW_RESULT lwInitUserRenderCtrlVSProc(lwIResourceMgr* mgr)
{
    mgr->RegisterRenderCtrlProc(RENDERCTRL_VS_VERTEXBLEND, __RenderCtrlVSProcVSVertexBlend_dx8);
#if(defined LW_USE_DX9)
    mgr->RegisterRenderCtrlProc(RENDERCTRL_VS_VERTEXBLEND_DX9, __RenderCtrlVSProcVSVertexBlend_dx9);
#endif
    return LW_RET_OK;
}

// lwxRenderCtrlVSVertexBlend_dx8
LW_STD_IMPLEMENTATION(lwxRenderCtrlVSVertexBlend_dx8);

lwxRenderCtrlVSVertexBlend_dx8::lwxRenderCtrlVSVertexBlend_dx8()
: mPixelShaderReload( false ),
  mPixelShader( -1 )
{
}


LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::Clone(lwIRenderCtrlVS** obj)
{
    this_type* o = LW_NEW(this_type);
    *o = *this;

    *obj = o;

    return LW_RET_OK;
}

LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::Initialize(lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}

LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::BeginSet(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    dev_obj->GetRenderState(D3DRS_FOGENABLE, &_rs_fog);
    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }

    lwMatrix44* mat_global = agent->GetGlobalMatrix();
    lwMatrix44 mat(*dev_obj->GetMatViewProj());

    lwMatrix44Multiply(&mat, mat_global, &mat);
    lwMatrix44Transpose(&mat, &mat);

    // set vs constant register
    // Use COLOR instead of UBYTE4 since Geforce3 does not support it
    // const_base.w should be 3, but due to COLOR/UBYTE4 issue, mul by 255 and add epsilon
    lwVector4 const_base(1.0f, 0.0f, 0.0f, 765.01f);

    lwVector4 light_dir(0.0f, 0.0f, 0.0f, 0.0f);
    
    DWORD rs_amb;
    dev_obj->GetRenderState(D3DRS_AMBIENT, &rs_amb);
    _rs_amb.a = LW_ARGB_A(rs_amb);
    _rs_amb.r = LW_ARGB_R(rs_amb);
    _rs_amb.g = LW_ARGB_G(rs_amb);
    _rs_amb.b = LW_ARGB_B(rs_amb);

    dev_obj->GetLight(0, &_lgt);
    dev_obj->GetRenderState(D3DRS_LIGHTING, &_rs_lgt);
    dev_obj->GetLightEnable(0, &_lgt_enable);

    if(_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        *(lwVector3*)&light_dir = *(lwVector3*)&_lgt.Direction;
        light_dir.x = -light_dir.x;
        light_dir.y = -light_dir.y;
        light_dir.z = -light_dir.z;

        lwMatrix44 mat_light;
        lwMatrix44InverseNoScaleFactor(&mat_light, mat_global);
        lwVec3Mat44MulNormal((lwVector3*)&light_dir, &mat_light);
    }


    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_BASE, (float*)&const_base, 1);
    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_VIEWPROJ, (float*)&mat, 4);
    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_LIGHT_DIR, (float*)&light_dir, 1);

    // set bone matrices pallette
    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();
    DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
    lwIAnimCtrlObj* animctrl_obj;
    lwAnimCtrlObjTypeInfo type_info;

    for(DWORD i = 0; i < animobj_num; i++)
    {
        animctrl_obj = anim_agent->GetAnimCtrlObj(i);
        animctrl_obj->GetTypeInfo(&type_info);

        if(type_info.type == ANIM_CTRL_TYPE_BONE)
        {
            lwIAnimCtrlObjBone* bone_ctrl = (lwIAnimCtrlObjBone*)animctrl_obj;
            DWORD bone_num = bone_ctrl->GetBoneRTTMNum();
            lwMatrix44* rtmat = (lwMatrix44*)bone_ctrl->GetBoneRTMSeq();
            if(bone_num == 0)
                goto __ret;

            static float __this_buf[50 * 12];
            for(DWORD i = 0; i < bone_num; i++)
            {
                lwMatrix44Transpose((lwMatrix44*)&__this_buf[i * 12], &rtmat[i]);
            }

            dev_obj->SetVertexShaderConstantF(VS_CONST_REG_MAT_PALETTE, __this_buf, bone_num * 3);
            break;
        }
    }

    // set vertex shader
    lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();

#if(defined LW_USE_DX8)
    DWORD shader = 0;
    if(LW_FAILED(shader_mgr->QueryVertexShader(&shader, agent->GetVertexShader())))
        goto __ret;

    dev_obj->SetVertexShader(shader);
#endif


	// added by clp
	// set pixel shader
	if( !mPixelShaderName.empty() )
	{
		IDirect3DDeviceX* device = dev_obj->GetDevice();
		if( mPixelShaderReload )
		{
			// load pixel shader
			LPD3DXBUFFER codeBuffer = 0;
			LPD3DXBUFFER errorBuffer = 0;
			HRESULT hr = D3DXAssembleShaderFromFile(
				mPixelShaderName.c_str(),
				0, NULL, &codeBuffer, &errorBuffer );    // assemble shader code
			if( SUCCEEDED( hr ) )
			{
				hr = device->CreatePixelShader( (DWORD*)codeBuffer->GetBufferPointer(), &mPixelShader );
			}
			else
			{
				mPixelShader = 0;

				const char* str = (const char*)errorBuffer->GetBufferPointer();
				MessageBox( 0, str, "D3DXAssembleShaderFromFile Failed", 0 );
			}
			if( errorBuffer )
			{
				errorBuffer->Release();
				errorBuffer = 0;
			}
			if( codeBuffer )
			{
				codeBuffer->Release();
				codeBuffer = 0;
			}

			mPixelShaderReload = false;
		}

		device->SetPixelShader( mPixelShader );
	}

#if(defined LW_USE_DX9)
    IDirect3DVertexShaderX* shader = 0;
    IDirect3DVertexDeclarationX* decl = 0;

    if(LW_FAILED(shader_mgr->QueryVertexShader(&shader, agent->GetVertexShader())))
        goto __ret;

    if(LW_FAILED(shader_mgr->QueryVertexDeclaration(&decl, agent->GetVertexDeclaration())))
        goto __ret;

    dev_obj->SetVertexDeclarationForced(decl);
    dev_obj->SetVertexShader(shader);
#endif

    ret = LW_RET_OK;
__ret:
    if(LW_FAILED(ret) && _rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }

    return ret;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::EndSet(lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }

	dev_obj->GetDevice()->SetPixelShader(0);

#if(defined LW_USE_DX9)
    dev_obj->SetVertexShader(NULL);
#endif
    return LW_RET_OK;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();
    lwIMtlTexAgent* mtltex_agent = agent->GetMtlTexAgent();

    lwMaterial* mtl = mtltex_agent->GetMaterial();

    lwColorValue4f amb_dif[2];
    lwColorValue4f* c;
    amb_dif[0];

    if(_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        c = &amb_dif[0];
        c->r = (_lgt.Ambient.r + _rs_amb.r) * mtl->amb.r;
        c->g = (_lgt.Ambient.g + _rs_amb.g) * mtl->amb.g;
        c->b = (_lgt.Ambient.b + _rs_amb.b) * mtl->amb.b;
        c->a = (_lgt.Ambient.a + _rs_amb.a) * mtl->amb.a;
            
        c = &amb_dif[1];
        c->r = _lgt.Diffuse.r * mtl->dif.r;
        c->g = _lgt.Diffuse.g * mtl->dif.g;
        c->b = _lgt.Diffuse.b * mtl->dif.b;
        c->a = _lgt.Diffuse.a * mtl->dif.a;
    }
    else
    {
        c = &amb_dif[0];
        c->r = _rs_amb.r * mtl->amb.r;
        c->g = _rs_amb.g * mtl->amb.g;
        c->b = _rs_amb.b * mtl->amb.b;
        c->a = _rs_amb.a * mtl->amb.a;

        c = &amb_dif[1];
        c->r = c->g = c->b = c->a = 0.0f;
    }
    
    //c = &amb_dif[0];
    //if(c->r > 1.0f)
    //    c->r = 1.0f;
    //if(c->g > 1.0f)
    //    c->g = 1.0f;
    //if(c->b > 1.0f)
    //    c->b = 1.0f;
    //if(c->a > 1.0f)
    //    c->a = 1.0f;

    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_LIGHT_AMB, (float*)&amb_dif, 2);

    // set texture uv data
    DWORD stage_tab[3] =
    {
        VS_CONST_REG_TS0_UVMAT,
        VS_CONST_REG_TS1_UVMAT,
        VS_CONST_REG_TS2_UVMAT,
    };

    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();
    DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
    lwIAnimCtrlObj* animctrl_obj;
    lwAnimCtrlObjTypeInfo type_info;

    for(DWORD i = 0; i < animobj_num; i++)
    {
        animctrl_obj = anim_agent->GetAnimCtrlObj(i);
        animctrl_obj->GetTypeInfo(&type_info);

        if((type_info.data[0] == subset) && (type_info.type == ANIM_CTRL_TYPE_TEXUV))
        {
            lwIAnimCtrlObjTexUV* texuv_ctrl = (lwIAnimCtrlObjTexUV*)animctrl_obj;
            DWORD stage_id = stage_tab[type_info.data[1]];
            lwMatrix44 mat, mat_src;
            texuv_ctrl->GetRTM(&mat_src);
            lwMatrix44Transpose(&mat, &mat_src);

            dev_obj->SetVertexShaderConstantF(stage_id, (float*)&mat, 4);
        }
    }

    return LW_RET_OK;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}


#if(defined LW_USE_DX9)

// lwxRenderCtrlVSVertexBlend
LW_STD_IMPLEMENTATION(lwxRenderCtrlVSVertexBlend);


LW_RESULT lwxRenderCtrlVSVertexBlend::Clone(lwIRenderCtrlVS** obj)
{
    this_type* o = LW_NEW(this_type);
    *o = *this;

    *obj = o;

    return LW_RET_OK;
}
LW_RESULT lwxRenderCtrlVSVertexBlend::Initialize(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();

    lwVertexShaderInfo* vs_info = shader_mgr->GetVertexShaderInfo(agent->GetVertexShader());
    if(FAILED(D3DXGetShaderConstantTable((DWORD*)vs_info->data, &_const_tab)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwxRenderCtrlVSVertexBlend::BeginSet(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();
    IDirect3DDeviceX* dev = dev_obj->GetDevice();
    lwIMeshAgent* mesh_agent = agent->GetMeshAgent();
    lwIMesh* mesh = mesh_agent->GetMesh();
    DWORD blend_factor = mesh->GetMeshInfo()->bone_infl_factor;

    dev_obj->GetRenderState(D3DRS_FOGENABLE, &_rs_fog);
    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }

    lwMatrix44* mat_global = agent->GetGlobalMatrix();
    lwMatrix44 mat(*dev_obj->GetMatViewProj());

    lwMatrix44Multiply(&mat, mat_global, &mat);
    //lwMatrix44Transpose(&mat, &mat);

    lwVector3 light_dir(0.0f, 0.0f, 0.0f);
    
    DWORD rs_amb;
    dev_obj->GetRenderState(D3DRS_AMBIENT, &rs_amb);
    _rs_amb.a = LW_ARGB_A(rs_amb);
    _rs_amb.r = LW_ARGB_R(rs_amb);
    _rs_amb.g = LW_ARGB_G(rs_amb);
    _rs_amb.b = LW_ARGB_B(rs_amb);

    dev_obj->GetLight(0, &_lgt);
    dev_obj->GetRenderState(D3DRS_LIGHTING, &_rs_lgt);
    dev_obj->GetLightEnable(0, &_lgt_enable);

    if(_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        light_dir = *(lwVector3*)&_lgt.Direction;
        light_dir.x = -light_dir.x;
        light_dir.y = -light_dir.y;
        light_dir.z = -light_dir.z;

        lwMatrix44 mat_light;
        lwMatrix44InverseNoScaleFactor(&mat_light, mat_global);
        lwVec3Mat44MulNormal(&light_dir, &mat_light);
    }

    if(FAILED(_const_tab->SetInt(dev, "blend_num", blend_factor)))
        goto __ret;

    if(FAILED(_const_tab->SetMatrix(dev, "mat_viewproj", &mat)))
        goto __ret;

    if(FAILED(_const_tab->SetValue(dev, "light_dir", &light_dir, sizeof(light_dir))))
        goto __ret;

    //D3DXCONSTANT_DESC desc;
    //UINT dn = 1;
    //_const_tab->GetConstantDesc("light_dir", &desc, &dn);
    //lwVector4 io;
    //dev->GetVertexShaderConstantF(desc.RegisterIndex, &io.x, 1);

    // set bone matrices pallette
    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();
    DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
    lwIAnimCtrlObj* animctrl_obj;
    for(DWORD i = 0; i < animobj_num; i++)
    {
        animctrl_obj = anim_agent->GetAnimCtrlObj(i);
        if(animctrl_obj->GetType() == ANIM_CTRL_TYPE_BONE)
        {
            lwIAnimCtrlObjBone* bone_ctrl = (lwIAnimCtrlObjBone*)animctrl_obj;
            DWORD reg_num = bone_ctrl->GetBoneRTTMNum();
            if(reg_num == 0)
                goto __ret;

            if(FAILED(_const_tab->SetMatrixArray(dev, "mat_bonepallette", (D3DXMATRIX*)bone_ctrl->GetBoneRTTMSeq(), reg_num)))
                goto __ret;

            break;
        }
    }

    // set vertex shader
    lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();

    IDirect3DVertexShaderX* shader = 0;
    IDirect3DVertexDeclarationX* decl = 0;

    if(LW_FAILED(shader_mgr->QueryVertexShader(&shader, agent->GetVertexShader())))
        goto __ret;

    if(LW_FAILED(shader_mgr->QueryVertexDeclaration(&decl, agent->GetVertexDeclaration())))
        goto __ret;

    dev_obj->SetVertexDeclarationForced(decl);
    dev_obj->SetVertexShader(shader);

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwxRenderCtrlVSVertexBlend::EndSet(lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }

    dev_obj->SetVertexShader(NULL);

    return LW_RET_OK;

}
LW_RESULT lwxRenderCtrlVSVertexBlend::BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();
    lwIMtlTexAgent* mtltex_agent = agent->GetMtlTexAgent();
    IDirect3DDeviceX* dev = dev_obj->GetDevice();

    lwMaterial* mtl = mtltex_agent->GetMaterial();

    lwColorValue4f amb_dif[2];
    lwColorValue4f* c;
    amb_dif[0];

    if(_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        c = &amb_dif[0];
        c->r = (_lgt.Ambient.r + _rs_amb.r) * mtl->amb.r;
        c->g = (_lgt.Ambient.g + _rs_amb.g) * mtl->amb.g;
        c->b = (_lgt.Ambient.b + _rs_amb.b) * mtl->amb.b;
        c->a = (_lgt.Ambient.a + _rs_amb.a) * mtl->amb.a;
            
        c = &amb_dif[1];
        c->r = _lgt.Diffuse.r * mtl->dif.r;
        c->g = _lgt.Diffuse.g * mtl->dif.g;
        c->b = _lgt.Diffuse.b * mtl->dif.b;
        c->a = _lgt.Diffuse.a * mtl->dif.a;
    }
    else
    {
        c = &amb_dif[0];
        c->r = _rs_amb.r * mtl->amb.r;
        c->g = _rs_amb.g * mtl->amb.g;
        c->b = _rs_amb.b * mtl->amb.b;
        c->a = _rs_amb.a * mtl->amb.a;

        c = &amb_dif[1];
        c->r = c->g = c->b = c->a = 0.0f;
    }
    
    //c = &amb_dif[0];
    //if(c->r > 1.0f)
    //    c->r = 1.0f;
    //if(c->g > 1.0f)
    //    c->g = 1.0f;
    //if(c->b > 1.0f)
    //    c->b = 1.0f;
    //if(c->a > 1.0f)
    //    c->a = 1.0f;

    //dev_obj->SetVertexShaderConstantF(VS_CONST_REG_LIGHT_AMB, (float*)&amb_dif, 2);
    if(FAILED(_const_tab->SetVector(dev, "mtl_amb", (D3DXVECTOR4*)&amb_dif[0])))
        goto __ret;

    if(FAILED(_const_tab->SetVector(dev, "mtl_dif", (D3DXVECTOR4*)&amb_dif[1])))
        goto __ret;

    // set texture uv data
    DWORD stage_tab[3] =
    {
        VS_CONST_REG_TS0_UVMAT,
        VS_CONST_REG_TS1_UVMAT,
        VS_CONST_REG_TS2_UVMAT,
    };

    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();
    DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
    lwIAnimCtrlObj* animctrl_obj;
    for(DWORD i = 0; i < animobj_num; i++)
    {
        animctrl_obj = anim_agent->GetAnimCtrlObj(i);
        if((animctrl_obj->GetSubset()) == subset && (animctrl_obj->GetType() == ANIM_CTRL_TYPE_TEXUV))
        {
            lwIAnimCtrlObjTexUV* texuv_ctrl = (lwIAnimCtrlObjTexUV*)animctrl_obj;
            DWORD stage_id = stage_tab[texuv_ctrl->GetStage()];
            lwMatrix44 mat;
            lwMatrix44Transpose(&mat, texuv_ctrl->GetTexRTTM());

            //dev_obj->SetVertexShaderConstantF(stage_id, (float*)&mat, 4);
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwxRenderCtrlVSVertexBlend::EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}


#if 0
// lwxRenderCtrlVSVertexBlend_fx
LW_STD_IMPLEMENTATION(lwxRenderCtrlVSVertexBlend_fx);


LW_RESULT lwxRenderCtrlVSVertexBlend_fx::Clone(lwIRenderCtrlVS** obj)
{
    this_type* o = LW_NEW(this_type);
    *o = *this;

    *obj = o;

    return LW_RET_OK;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_fx::Initialize(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();

    lwVertexShaderInfo* vs_info = shader_mgr->GetVertexShaderInfo(agent->GetVertexShader());
    if(FAILED(D3DXGetShaderConstantTable((DWORD*)vs_info->data, &_const_tab)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_fx::BeginSet(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();
    IDirect3DDeviceX* dev = dev_obj->GetDevice();
    lwIMeshAgent* mesh_agent = agent->GetMeshAgent();
    lwIMesh* mesh = mesh_agent->GetMesh();
    DWORD blend_factor = mesh->GetMeshInfo()->bone_infl_factor;

    dev_obj->GetRenderState(D3DRS_FOGENABLE, &_rs_fog);
    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }

    lwMatrix44* mat_global = agent->GetGlobalMatrix();
    lwMatrix44 mat(*dev_obj->GetMatViewProj());

    lwMatrix44Multiply(&mat, mat_global, &mat);
    //lwMatrix44Transpose(&mat, &mat);

    lwVector3 light_dir(0.0f, 0.0f, 0.0f);
    
    DWORD rs_amb;
    dev_obj->GetRenderState(D3DRS_AMBIENT, &rs_amb);
    _rs_amb.a = LW_ARGB_A(rs_amb);
    _rs_amb.r = LW_ARGB_R(rs_amb);
    _rs_amb.g = LW_ARGB_G(rs_amb);
    _rs_amb.b = LW_ARGB_B(rs_amb);

    dev_obj->GetLight(0, &_lgt);
    dev_obj->GetRenderState(D3DRS_LIGHTING, &_rs_lgt);
    dev_obj->GetLightEnable(0, &_lgt_enable);

    if(_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        light_dir = *(lwVector3*)&_lgt.Direction;
        light_dir.x = -light_dir.x;
        light_dir.y = -light_dir.y;
        light_dir.z = -light_dir.z;

        lwMatrix44 mat_light;
        lwMatrix44InverseNoScaleFactor(&mat_light, mat_global);
        lwVec3Mat44MulNormal(&light_dir, &mat_light);
    }

    if(FAILED(_const_tab->SetInt(dev, "blend_num", blend_factor)))
        goto __ret;

    if(FAILED(_const_tab->SetMatrix(dev, "mat_viewproj", &mat)))
        goto __ret;

    if(FAILED(_const_tab->SetValue(dev, "light_dir", &light_dir, sizeof(light_dir))))
        goto __ret;

    //D3DXCONSTANT_DESC desc;
    //UINT dn = 1;
    //_const_tab->GetConstantDesc("light_dir", &desc, &dn);
    //lwVector4 io;
    //dev->GetVertexShaderConstantF(desc.RegisterIndex, &io.x, 1);

    // set bone matrices pallette
    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();
    DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
    lwIAnimCtrlObj* animctrl_obj;
    for(DWORD i = 0; i < animobj_num; i++)
    {
        animctrl_obj = anim_agent->GetAnimCtrlObj(i);
        if(animctrl_obj->GetType() == ANIM_CTRL_TYPE_BONE)
        {
            lwIAnimCtrlObjBone* bone_ctrl = (lwIAnimCtrlObjBone*)animctrl_obj;
            DWORD reg_num = bone_ctrl->GetBoneRTTMNum();
            if(reg_num == 0)
                goto __ret;

            if(FAILED(_const_tab->SetMatrixArray(dev, "mat_bonepallette", (D3DXMATRIX*)bone_ctrl->GetBoneRTTMSeq(), reg_num)))
                goto __ret;

            break;
        }
    }

    // set vertex shader
    lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();

    IDirect3DVertexShaderX* shader = 0;
    IDirect3DVertexDeclarationX* decl = 0;

    if(LW_FAILED(shader_mgr->QueryVertexShader(&shader, agent->GetVertexShader())))
        goto __ret;

    if(LW_FAILED(shader_mgr->QueryVertexDeclaration(&decl, agent->GetVertexDeclaration())))
        goto __ret;

    dev_obj->SetVertexDeclarationForced(decl);
    dev_obj->SetVertexShader(shader);

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwxRenderCtrlVSVertexBlend_fx::EndSet(lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }

    dev_obj->SetVertexShader(NULL);

    return LW_RET_OK;

}
LW_RESULT lwxRenderCtrlVSVertexBlend_fx::BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();
    lwIMtlTexAgent* mtltex_agent = agent->GetMtlTexAgent();
    IDirect3DDeviceX* dev = dev_obj->GetDevice();

    lwMaterial* mtl = mtltex_agent->GetMaterial();

    lwColorValue4f amb_dif[2];
    lwColorValue4f* c;
    amb_dif[0];

    if(_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        c = &amb_dif[0];
        c->r = (_lgt.Ambient.r + _rs_amb.r) * mtl->amb.r;
        c->g = (_lgt.Ambient.g + _rs_amb.g) * mtl->amb.g;
        c->b = (_lgt.Ambient.b + _rs_amb.b) * mtl->amb.b;
        c->a = (_lgt.Ambient.a + _rs_amb.a) * mtl->amb.a;
            
        c = &amb_dif[1];
        c->r = _lgt.Diffuse.r * mtl->dif.r;
        c->g = _lgt.Diffuse.g * mtl->dif.g;
        c->b = _lgt.Diffuse.b * mtl->dif.b;
        c->a = _lgt.Diffuse.a * mtl->dif.a;
    }
    else
    {
        c = &amb_dif[0];
        c->r = _rs_amb.r * mtl->amb.r;
        c->g = _rs_amb.g * mtl->amb.g;
        c->b = _rs_amb.b * mtl->amb.b;
        c->a = _rs_amb.a * mtl->amb.a;

        c = &amb_dif[1];
        c->r = c->g = c->b = c->a = 0.0f;
    }
    
    //c = &amb_dif[0];
    //if(c->r > 1.0f)
    //    c->r = 1.0f;
    //if(c->g > 1.0f)
    //    c->g = 1.0f;
    //if(c->b > 1.0f)
    //    c->b = 1.0f;
    //if(c->a > 1.0f)
    //    c->a = 1.0f;

    //dev_obj->SetVertexShaderConstantF(VS_CONST_REG_LIGHT_AMB, (float*)&amb_dif, 2);
    if(FAILED(_const_tab->SetVector(dev, "mtl_amb", (D3DXVECTOR4*)&amb_dif[0])))
        goto __ret;

    if(FAILED(_const_tab->SetVector(dev, "mtl_dif", (D3DXVECTOR4*)&amb_dif[1])))
        goto __ret;

    // set texture uv data
    DWORD stage_tab[3] =
    {
        VS_CONST_REG_TS0_UVMAT,
        VS_CONST_REG_TS1_UVMAT,
        VS_CONST_REG_TS2_UVMAT,
    };

    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();
    DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
    lwIAnimCtrlObj* animctrl_obj;
    for(DWORD i = 0; i < animobj_num; i++)
    {
        animctrl_obj = anim_agent->GetAnimCtrlObj(i);
        if((animctrl_obj->GetSubset()) == subset && (animctrl_obj->GetType() == ANIM_CTRL_TYPE_TEXUV))
        {
            lwIAnimCtrlObjTexUV* texuv_ctrl = (lwIAnimCtrlObjTexUV*)animctrl_obj;
            DWORD stage_id = stage_tab[texuv_ctrl->GetStage()];
            lwMatrix44 mat;
            lwMatrix44Transpose(&mat, texuv_ctrl->GetTexRTTM());

            //dev_obj->SetVertexShaderConstantF(stage_id, (float*)&mat, 4);
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwxRenderCtrlVSVertexBlend_fx::EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}
#endif

#endif


LW_END
