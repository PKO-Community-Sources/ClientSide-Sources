#pragma once
#include "UIGlobalVar.h"
#include "uiform.h"
#include "uigoodsgrid.h"

class CCharacter;

namespace GUI
{
#define defMaxBoat 3 // �ɻ�����ֻ��

class CBoat // ����Ϣ��
{
public:
	CBoat();
	bool Init( int n, CForm* ship, GuiDragInGridEvent evt );

	void		Reset(); // �������ô�
	void		Link( CCharacter* pBoat )	{ _chtBoat = pBoat;			}  // ������ͽ�ɫ�����Ӧ
	bool		GetIsValid()				{ return _chtBoat!=NULL;	}  // ��ý�ɫ��Ϣ
	void		UnLink()					{ _chtBoat=NULL;			}  

	CCharacter* GetCha()			{ return _chtBoat;		}  // ������
	CGoodsGrid*	GetGoodsGrid()		{ return _grdHold;		}  // ��ý�ɫ����
	CForm*		GetForm()			{ return _frmShipRoom;	}  // �Ҵ���������Ϣ

private:
	static void _evtHoldGridRMouse(CGuiData *pSender,CCommandObj* pItem,int nGridID);
	static void _evtEscClose( CForm* pForm );	// �� ESC �رմ����¼�  add by Philip.Wu  2006-06-22

private:
	int				_nIndex;

	CCharacter*		_chtBoat;
	CForm*			_frmShipRoom;
	CGoodsGrid*		_grdHold;

};

class CBoatMgr : public CUIInterface
{
public:
	void	Clear();

	bool	AddBoat( CCharacter* pBoat );  // �û���Ӵ�ֻ
	CBoat*	GetBoat( unsigned int n )		{ return &_cBoats[n];	} // ��ȡ��ֻ
	CBoat*	FindBoat( unsigned int ulWorldID );  // �ҵ��û�ȫ����ֻ

	void	SetHuman( CCharacter* p )		{ _pHuman = p;			} 
	CCharacter*		GetHuman()				{ return _pHuman;		} // ���ҵ�ǰ��ɫ

	CGoodsGrid*		FindGoodsGrid( unsigned int ulWorldID );	// ����ID���Ҷ�Ӧ�ĵ�����
	CCharacter*		FindCha( unsigned int ulWorldID );         // ���Ҵ�����
	CCharacter*		FindCha( CGoodsGrid* pGoods );   // ��װ��ƥ����Ҵ�ֻ������

	CCharacter*		ChangeMainCha( unsigned int ulWorldID );	// �л�����

	CBoat*	GetOtherBoat()					{ return &_cOther;		}  // �鿴���˴�

protected:
	virtual bool Init(); //�û�����Ϣ��ʼ��
    virtual void End(); 
	virtual void SwitchMap()				{	Clear();			} // ��ת��ͼʱ�������Ϣ

private:
	CBoat*		GetFreeBoat(); // �����Ѵ�ֻ

private:
	CCharacter*	_pHuman;
	CBoat		_cBoats[defMaxBoat];        // ��ǰ��ɫ��ӵ�еĴ�����
	CBoat		_cOther;					// һ�����ڲ鿴���˵Ĵ��գ�����ʱ����ʱ������

};

}
