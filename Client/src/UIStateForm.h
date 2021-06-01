#pragma once
#include "UIGlobalVar.h"

namespace GUI
{
// �������
class CStateMgr : public CUIInterface
{
public:

	void			RefreshStateFrm();

protected:
	virtual bool	Init();
    virtual void	End();
	virtual void	FrameMove(DWORD dwTime);

private:
	static void _evtMainShow(CGuiData *pSender);
	static void MainMouseDown(CGuiData *pSender, int x, int y, DWORD key);

private:
	//frmState����Ŀؼ�
	CForm*			frmState;
	CLabelEx*		labName;                    //������ɫ����
	CLabelEx*		labGuildName;               //������������
	CLabelEx*		labStateLevel;              //�����ȼ�
	CLabelEx*		labStatePoint;              //�������Ե��� 
	CLabelEx*  		labSkillPoint;              //�������ܵ���
	CLabelEx*		labJobShow;					//ְҵ
	CLabelEx *      labFameShow;				//��������

	//6����������
	CLabelEx*		labStrshow;
	CLabelEx*		labDexshow;
	CLabelEx*		labAgishow;
	CLabelEx*		labConshow;
	CLabelEx*		labStashow;
	CLabelEx*		labLukshow;
	CLabelEx*		labSailLevel;
	CLabelEx*		labSailEXP;

	//8����������
	CLabelEx*		labMinAtackShow;
	CLabelEx*		labMaxAtackShow;
	CLabelEx*		labFleeShow;
	CLabelEx*		labAspeedShow;
	CLabelEx*		labMspeedShow;
	CLabelEx*		labHitShow;
	CLabelEx*		labDefenceShow;
	//CLabelEx*		labCriticalShow;
	//CLabelEx*		labMfShow;
	CLabelEx*		labPhysDefineShow;

	//6 ���������԰�ť
	CTextButton*   btnStr;						//����������
	CTextButton*   btnAgi;						//����������
	CTextButton*   btnCon;						//����������
	CTextButton*   btnSta;						//����������
	//CTextButton*   btnLuk;						//����������
	CTextButton*   btnDex;						//����רע��     

	CLabelEx*		labStateEXP;
	CLabelEx*		labStateHP;
	CLabelEx*		labStateSP;

	CLabelEx*		labFameSho;              //��������
	CLabelEx*		labStateName;
	CLabelEx*		labStateJob;



};

}

