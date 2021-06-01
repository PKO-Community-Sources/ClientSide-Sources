//----------------------------------------------------------------------
// ����:����Edit�����룬��ʾ��꣬��ʾѡ��״̬��
// ����:lh 2005-05-23
// ����޸�����:
//----------------------------------------------------------------------
#pragma once

namespace GUI
{

class CEditParse;
class CEditObj;
class CEditKey
{
public:		
	CEditKey();
	~CEditKey();

	void	Init();
	bool	SetFont( DWORD dwFont );
	void	SetColor( DWORD dwColor )		{ _dwFontColor=dwColor;		}

public:		// �ⲿ�ӿ�
	bool	OnChar( char c );
	bool	OnKeyDown( int key );

	void	Render();	

private:
	void	AddChar( CEditObj* pObj );

private:
	CEditParse*		_pParse;

	char			_szEnter[16];				// ���ڼ��������buf
	int				_nEnterPos;

	bool			_IsReadyOnly;				// �Ƿ�ֻ��

	DWORD			_dwFontColor;				// ��ǰ������ɫ
	int				_dwFontIndex;				// ��ǰ����

private:
	DWORD			_dwCurosrIndex;				// ��ǰ���λ��

	DWORD			_dwCursorHeight;			// ���߶�
	DWORD			_dwCursorColor;				// �����ɫ 
	int				_nCursorX, _nCursorY;		// �����Ⱦλ��
	bool			_IsShowCursor;
	DWORD			_dwCursorTime;
	DWORD			_dwCursorSpace;

};

}
