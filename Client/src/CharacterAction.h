#ifndef	CHARACTERACTION_H
#define CHARACTERACTION_H

enum EActionNumber
{
    POSE_WAITING =       01,//	��ͨwaiting	
    POSE_SHOW =          02,//	�ڿ�pose	
    POSE_SLEEP =         03,//	����
    POSE_WAITING2 =      04,//	�䱸waiting	
    POSE_RUN =           05,//	��ͨ����	
    POSE_RUN2 =          06,//	�䱸����	
    POSE_ATTACK =        07,//	��ͨ����1
    POSE_ATTACK1 =       8, //	��ͨ����2
    POSE_ATTACK2 =       9, //	��ͨ����3
    POSE_POWER_ATTACK =  10,//  �ػ�
    POSE_SKILL1 =        11,//	���ܣ�
    POSE_SKILL2 =        12,//	���ܣ�
    POSE_SKILL3 =        13,//	���ܣ�
    POSE_SKILL4 =        14,//	���ܣ�	011	
    POSE_SKILL5 =        15,//	���ܣ�	012	
    POSE_SEAT =          16,//	��ͨ����	
    POSE_DIE =           17,//	����(ͨ��)	
    POSE_WAVE =          18,//	����(ͨ��)	
    POSE_CRY =           19,//	���(ͨ��)	
    POSE_JUMP =          20,//	ȸԾ(ͨ��)	
    POSE_ANGER =         21,//	����(ͨ��)	
    POSE_DARE =          22,//	����(ͨ��)	
    POSE_GAY =           23,//	����(��ͨ��)
    POSE_EMOTE7 =        24,//	���鶯��7	
    POSE_EMOTE8 =        25,//	���鶯��8	
    POSE_EMOTE9 =        26,//	���鶯��9	
    POSE_EMOTE10 =       27,//	���鶯��10	
    POSE_MINE =          28,//	�ڿ�(��������
    POSE_COLLECT =       29,//	�ɼ�(��������
    POSE_SEAT2 =         30,//	������(������
    POSE_LEAN =          31,//	��ǽ(��������
    POSE_LEAN2 =         32,//	����(��������
    POSE_HAPPY =         33,//	��ɫ����(��������)
    POSE_HAPPY7 =        34,//	������������7
    POSE_HAPPY8 =        35,//	������������8
    POSE_HAPPY9 =        36,//	������������9
    POSE_HAPPY10 =       37,//	������������10
    POSE_RESERVED1 =     38,//	���ö���1	
    POSE_FALLDOWN =      39,//	���ö���2	
    POSE_SKILL6 =        40,//	���ܶ���6	
    POSE_SKILL7 =        41,//	���ܶ���7	
    POSE_FLY_WAITING =   42,//	����				//POSE_SKILL8 =        42,//	���ܶ���8	
    POSE_FLY_RUN =       43,//	�����ƶ�			//POSE_SKILL9 =        43,//	���ܶ���9	
    POSE_FLY_SHOW =      44,//	���аڿ�			//POSE_SKILL10 =       44,//	���ܶ���10	
    POSE_FLY_SEAT =      45,//	��������			//POSE_SKILL11 =       45,//	���ܶ���11	
    POSE_SKILL12 =       46,//	���ܶ���12	
    POSE_SKILL13 =       47,//	���ܶ���13	
    POSE_SKILL14 =       48,//	���ܶ���14	
    POSE_SKILL15 =       49,//	���ܶ���15	
    POSE_SKILL16 =       50,//	���ܶ���16	
    POSE_SKILL17 =       51,//	���ܶ���17	
    POSE_SKILL18 =       52,//	���ܶ���18	
    POSE_SKILL19 =       53,//	���ܶ���19	
    POSE_SKILL20 =       54,//	���ܶ���20	
};

struct SActionInfo
{
	short		m_sActionNO;	// �������
	//short		m_sStartFrame;	// ��ʼ֡
	//short		m_sEndFrame;	// ����֡

	//short		m_sKeyFrameNum;	// �ؼ�֡����
	//short		*m_sKeyFrame;	// �ؼ�֡��
    lwPoseInfo info;
};

struct SCharacterAction
{
	short		m_iCharacterType;	// ��ɫ����
	short		m_iMaxActionNum;	// ��ɫ���������(���ļ��ж����Ľ�ɫ���������)
	short		m_iActualActionNum;	// ��ɫ����Ч������(���ļ��ж����Ľ�ɫ�Ķ�������)
	SActionInfo	*m_SActionInfo;		// ��ɫ����������Ϣ
};

class	CGameCharacterAction
{
public:
	CGameCharacterAction();
	~CGameCharacterAction();

	bool		Init(_TCHAR *ptcsFileName);
	void		Free(void);

	void		GetMaxCharType(int *iMaxType) {*iMaxType = m_iMaxCharacterType;};
	void		GetActualCharType(int *iActualType) {*iActualType = m_iActualCharacterType;};
	bool		GetCharMaxActNum(int iCharType, int *iMaxActNum);
	bool		GetCharActualActNum(int iCharType, int *iActualActNum);
	bool		GetCharFrameInfo(int iCharType, int iActionNO, int *iStartFrame, int *iEndFrame);
	bool		GetCharAction(int iCharType, SCharacterAction *SCharAct);

protected:
	short		m_iMaxCharacterType;		// ����ɫ������(���ļ��ж��������Ľ�ɫ���ͺ�)
	short		m_iActualCharacterType;		// ��Ч��ɫ������(���ļ��ж����Ľ�ɫ����)
	SCharacterAction	*m_SCharacterAction;
};

long StringGet(_TCHAR *out, long out_max, _TCHAR *in, long *in_from, _TCHAR *end_list, long end_len);
void StringSkipCompartment(_TCHAR *in, long *in_from, _TCHAR *skip_list, long skip_len);


extern CGameCharacterAction g_PoseData;
extern void InitPoseData();
extern bool GetCharacterAction(int nTypeID, SCharacterAction *SCharAct);

#endif //end CHARACTERACTION_H
