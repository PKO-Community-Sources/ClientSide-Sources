#pragma  once
struct SClientAttr
{
	short sTeamAngle; // ���ʱ��ͷ��Ƕ�
	float fTeamDis;   // ���ʱ��ͷ�����
	float fTeamHei;   // ���ʱ��ͷ�����

	SClientAttr()
		:sTeamAngle(0),
		fTeamDis(0),
		fTeamHei(0)
	{
	}
};

extern SClientAttr g_ClientAttr[2000];

// ����ΪCharacterInfo�����ID
inline SClientAttr* GetClientAttr(int nScriptID)
{
	return &g_ClientAttr[nScriptID];
}


struct SCameraMode
{
	float	m_fminxy ;
	float   m_fmaxxy ;
	float	m_fminHei;
	float   m_fmaxHei;
	float   m_fminfov;
	float   m_fmaxfov;
	int		m_nShowWidth;
	int		m_nShowHeight;

	BOOL	m_bRotate;
};

extern SCameraMode CameraMode[4];
