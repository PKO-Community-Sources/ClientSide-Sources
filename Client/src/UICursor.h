#pragma once

namespace GUI
{
class CCursor 
{
public:
	enum eState
	{
		stNormal	= 0,	// ����״̬
		stActive	= 1,	// ������״̬
		stDrag		= 2,	// �϶�ʱ
		stSize		= 3,	// �ı��С
		stVertical	= 4,	// ��ֱ
		stLevel		= 5,	// ˮƽ
		stWait		= 6,	// �ȴ�	
		stAttack	= 7,	// ��������ƶ�ʱ��ʾ�Ĺ������
		stSkillAttack = 8,	// �����ܹ���
		stUpBank	= 9,	// �ϰ�
		stUpBoat	= 10,	// �ϴ�
		stStop		= 11,	// ���ɲ���
		stHover		= 12,	// ��ͣ��Ʒ
		stPick		= 13,	// ʰȡ��Ʒ
        stCamera	= 14,   // ��ͷ��ת
        stChat		= 15,   // �Ի�
        stMouse		= 16,   // һ�����
        stButtonClick = 17, // ��ť����
        stHide		= 18,   // ����ʾ���
		stSearch	= 19,	// ��ѯ
		stBlock		= 20,	// ������
		stRepair	= 21,	// ����
		stFeed		= 22,	// ιʳ
		stEnd,				// �������������ж��������ֵ
	};

public:
	CCursor();
	~CCursor();

	void	Init();
	void	SetCursor( eState v );			// ���ù��״̬��״̬����
	eState  GetCursor()			{ return _eActive;				}

	bool	SetFrame( eState v );			// ���ù��״̬��ÿ֡�ĵ��ú���
    void    Restore()           { _IsShowFrame=false;           }

	int		GetMax()			{ return stEnd;					}
	int		GetIndex()			{ return _eState;				}

    void    Render();

public:
	static	CCursor* I()		{ return &s_Cursor;				}

private:
	void	_ShowCursor();

private:
	static CCursor	s_Cursor;

	bool		_IsInit;
    bool        _IsShowFrame;

	eState		_eFrame;					// ֡�����Ĺ��״̬
	eState		_eState;					// ״̬�����Ĺ��״̬

	eState		_eActive;					// ��ǰ����ʹ�õ�״̬
	HCURSOR		_hCursor[stEnd];

};

class CWaitCursor
{
public:
	CWaitCursor()					{ CCursor::I()->SetCursor( CCursor::stWait );	}
	CWaitCursor(CCursor::eState c)	{ CCursor::I()->SetCursor( c );					}
	~CWaitCursor()					{ CCursor::I()->SetCursor( CCursor::stNormal );	}
};

// ��������
inline void CCursor::SetCursor( eState v )
{
    if( v>=stNormal && v<stEnd )
    {
        _eState = v;
    }
}

inline bool CCursor::SetFrame( eState v )
{
    if( v>=stNormal && v<stEnd )
    {
        _IsShowFrame = true;
        _eFrame = v;
        return true;
    }
    return false;
}

inline void CCursor::Render()
{
    if( _IsShowFrame )
    {
        if( _eActive!=_eFrame )  
        {
            _eActive = _eFrame;
            _ShowCursor();
        }
    }
    else if( _eActive!=_eState )  
    {
        _eActive = _eState;
        _ShowCursor();
    }
}

}
