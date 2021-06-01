#pragma once
#include "uicompent.h"
#include "uitextbutton.h"
#include "uigraph.h"

#include "uiscroll.h"
#include "netprotocol.h"
namespace GUI
{
	#define MEMO_MAX_LINE			128
	#define MEMO_MAX_ITEM           64 
	#define MEMO_MAX_MISSION        8 
	#define MEMO_MAX_MISNEED        6 
	#define MEMOEX_MAX_PHRASE       16

	// 
	enum MEMO_LINE_TYPE
	{
		MEMO_LINE_TITLE,		// ���ֱ���
		MEMO_LINE_DESP,			// ��������
		MEMO_LINE_INTERVAL,		// �����
		MEMO_LINE_ICON,			// ͼ��	
	};

	class CMemo : public CCompent
	{
	public:
		CMemo(CForm& frmOwn);
		CMemo( const CMemo& rhs );
		CMemo& operator=(const CMemo& rhs);
		virtual ~CMemo(void);
		GUI_CLONE(CMemo)

		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun( int x, int y, DWORD key );
		virtual bool IsHandleMouse()					{ return true;								}

		const char*	GetCaption() { return _strCaption.c_str(); }
		void		SetCaption( const char * str ) { _strCaption = str; }
		void		ProcessCaption();

		virtual void	SetAlpha( BYTE alpha ){ _color = (_color & 0x00ffffff) & (alpha << 24); }
		virtual void	SetTextColor( DWORD color ) { _color = color; }
		DWORD			GetTextColor() { return _color; }

		CScroll*	GetScroll()							{ return _pScroll;							}
		void	Init();
		void	AddIcon(int index ,CGraph *pGraph);
		bool	MouseScroll( int nScroll ) ;

	public:
		GuiEvent	evtSelectChange;				// ѡ�����˱仯
		void	SetMaxNumPerRow(int n) { _nMaxNum = n; }
		int		GetMaxNumPerRow() {  return _nMaxNum; } 
		void	SetPageShowNum(int n) {  _nPageShowNum = n; }
		void	SetRowHeight( int v ) { if( v>0) _nRowHeight = v;					}
		int		GetRowHeight() { return _nRowHeight;						}
		void	SetMargin( int left, int top, int right, int bottom );
		void	SetIsHaveItem(bool v ) { _bIsHaveItem = v ;}
		bool	GetIsHaveItem()        {return _bIsHaveItem ;}
		void	SetItemRowNum(int num ) { _nItemRowNum = num  ;}	 
		void	AddItemRowContent(int  row, const char* szFunc, const char* szItemEx="") ;
		void	SetIsHaveMis(bool v ) { _bIsHaveMis = v ;}
		bool	GetIsHaveMis()        {return _bIsHaveMis ;}
		void	SetMisRowNum(int num );
		void	AddMisRowContent(int row, const char* szFunc ) ;
		int		GetSelectItem() {  return  _bIsHaveItem ? _nSelectItem : -1 ; } 
		bool	GetSelectItemText( string& item, string& itemex );
		int		GetSelectMis()  {  return  _bIsHaveMis?_nSelectMis:-1;      }
		void	reset();

	protected:
		static void	_OnScrollChange(CGuiData *pSender) 
		{
			((CMemo*)(pSender->GetParent()))->_OnScrollChange();
		}
		void	_OnScrollChange();

	protected:
		void	_SetScrollRange();
		void	_Copy( const CMemo& rhs );
		void	_SetSelf();
		void	_CheckTextAlign();			// ����ı����з�ʽ
		void	_CheckScroll();			// ����Ƿ���ʾ������

		string  _strCaption;
		string  _str[MEMO_MAX_LINE];
		bool	_IsTextCenter;

		int		_nLeftMargin;	// ����Χ�߿�ľ���
		int		_nTopMargin;
		int		_nRightMargin;
		int		_nBottomMargin;
		int		_nRowHeight;
		int		_nMaxNum;
		int		_nRowNum;

		bool	_bIsHaveItem;
		bool	_bIsHaveMis;

		int		_nItemRowNum;
		int		_nMisRowNum;
		int		_nMemoNum;
		string	_strItem[MEMO_MAX_ITEM];
		string	_strItemEx[MEMO_MAX_ITEM];		// ��������,����ʾ
		string	_strMis[MEMO_MAX_ITEM];
		int		_nSelectItem;
		int		_nSelectMis;
		int		_nRowInfo[3][2];

	private:
		DWORD	_color;
		CScroll*	_pScroll;
		int		_nFirst, _nLast;
		int		_nPageShowNum;

		struct stIconIndex
		{
			int		nIndex;
			CGraph  *pGraph;
		};

		typedef vector<stIconIndex> files;
			files _files;
	};

	class CGraph;
	class CMemoEx : public CCompent
	{
	public:
		CMemoEx(CForm& frmOwn);
		CMemoEx( const CMemoEx& rhs );
		CMemoEx& operator=(const CMemoEx& rhs);
		virtual ~CMemoEx(void);
		GUI_CLONE(CMemoEx)

		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun( int x, int y, DWORD key );
		virtual bool IsHandleMouse() { return true; }

		virtual void SetAlpha( BYTE alpha ){ _color = (_color & 0x00ffffff) & (alpha << 24); }
		virtual void SetTextColor( DWORD color ) { _color = color; }
		DWORD		 GetTextColor() { return _color; }

		CScroll*	GetScroll()	{ return _pScroll; }
		void        Init();

		bool		MouseScroll( int nScroll ) ;
		GuiEvent	evtSelectChange; // ѡ�����˱仯
		GuiItemClickEvent evtClickItem;	// �����Ī���ַ���
		void		SetMaxNumPerRow(int n) { _nMaxNum  = n; }
		int			GetMaxNumPerRow() { return _nMaxNum; } 
		void		SetRowNum(int n) { _nRowNum = n; }
		int			GetRowNum() const { return _nRowNum; }
		void		SetPageShowNum(int n) { _nPageShowNum = n; }
		void		SetRowHeight( int v ) { if( v > 0 ) _nRowHeight = v; }
		int			GetRowHeight() { return _nRowHeight; }
		void		SetMargin( int left, int top, int right, int bottom );
		void		Clear(); 
		void		SetMisPage( const NET_MISPAGE& page );
		BYTE		GetSelPrize() { return m_bySelPrize; }
		void		SetIsSelect( BOOL bSel ) { m_bIsSelect = bSel; }
		BOOL		IsSelPrize();
	protected:
		struct MEMO_DESP_SECTION
		{
			char* pszStart;
			char* pszEnd;
			DWORD dwColor;
			BYTE  byType;
			USHORT sData;
		};

		struct MEMO_INFO
		{
			string	strDesp;
			DWORD	dwColor;
			CGraph* pIcon;
			USHORT	sData;
			BYTE	byData;
			BYTE	byType;		  // ��Ϣ����
			USHORT	sxPos, syPos; // �к���
		};

		// ��Ҫ��ʾ������������Ϣ
		USHORT		m_sNumInfo;
		MEMO_INFO	m_MemoInfo[MEMO_MAX_LINE];	// ���������ʾ����������
		NET_MISPAGE m_PageInfo;					// ��������ԭʼ��Ϣ
		BOOL		m_bUpdate;					// ���������Ƿ��ѱ�����

		// Add by lark.li 20080721 begin
		int	m_SelMem;
		// End
		static void		_OnScrollChange(CGuiData *pSender)
		{
			((CMemoEx*)(pSender->GetParent()))->_OnScrollChange();
		}

		void	_OnScrollChange();
		void	_SetScrollRange();
		void	_Copy( const CMemoEx& rhs );
		void	_SetSelf();
		void	_CheckScroll();			// ����Ƿ���ʾ������
		void	ParseMisPage();
		BOOL	ParseScript( char* pszTemp, USHORT& sNumLine, USHORT& sRow, USHORT& sCom, USHORT sStartCom, USHORT sMaxCom, DWORD dwDefColor, MEMO_INFO* pInfo, USHORT sMaxInfo );
		BOOL	SelPrizeItem( int nxPos, int nyPos, DWORD dwKey );
		
		// Add by lark.li 20080721 begin
		int SelMemInfo(int x, int y, DWORD key);
		// End
	private:
		DWORD	_color;				// ������ɫ
		CScroll*	_pScroll;
		int		_nFirst, _nLast;	// ��ʾ�ĵ�һ�к����һ��
		int		_nPageShowNum ;		// һҳ��������ʾ������
		int		_nLeftMargin;	// ����Χ�߿�ľ���
		int		_nTopMargin;	// ����Χ�߿�ľ���
		int		_nRightMargin;	// ����Χ�߿�ľ���
		int		_nBottomMargin;	// ����Χ�߿�ľ���
		int		_nRowHeight;
		int     _nTitleHeight;
		int     _nIconHeight;
		int     _nMaxNum;
		int     _nRowNum; 
		int     _nPhraseNum;

		// ��¼������Ʒѡ����
		BYTE	m_bySelPrize;
		CGuiPic* m_pPrizePic;
		CGuiPic* m_pPrizeSelPic;
		BOOL	m_bIsSelect;
	};

	class CImageList
	{
	public:
		CImageList(){};
		virtual ~CImageList(){};
		
		virtual CGraph* GetImage( const char* szImage )						= 0;
		virtual bool	AddImage( const char* szImage, CGraph* pImage )		= 0;
	};

	enum MEMO_COL_TYPE
	{
		COL_ICON			= 0, // ͼ��
		COL_TEXT			= 1, // �ı�
	};

	enum MEMO_COLOR_TYPE
	{
		TEXT_COLOR_WIGHT	= 0, // ��ɫ
		TEXT_COLOR_BLACK	= 1, // ��ɫ
		TEXT_COLOR_RED		= 2, // ��ɫ
		TEXT_COLOR_GREEN	= 3, // ��ɫ
		TEXT_COLOR_BLUE		= 4, // ��ɫ
		TEXT_COLOR_GRAY		= 5, // ��ɫ
		TEXT_COLOR_PURPLE	= 6, // ��ɫ
	};
	
	enum MEMO_FONT_TYPE
	{
		TEXT_FONT_9			= 0, // 9����
		TEXT_FONT_10		= 1, // 10����
	};

	class CRichMemo : public CCompent
	{
	public:
		CRichMemo(CForm& frmOwn);
		CRichMemo( const CRichMemo& rhs );
		CRichMemo& operator=(const CRichMemo& rhs);
		GUI_CLONE(CRichMemo)
		virtual ~CRichMemo();

		virtual void Init();
		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun( int x, int y, DWORD key );
		virtual bool IsHandleMouse() { return true; }
	
		// ����������
		CScroll* GetScroll() { return m_pScroll; }
		bool	MouseScroll( int nScroll );

		// �����������¼�����
		void	OnScrollChange();

		// ���ÿؼ���ʾ��Ϣ�ӿ�		
		void	Clear();
		void	AddText( const char szTitle[], const char szText[], BYTE byColType, BYTE byFontType );
		void	SetClipRect( const RECT& rect ) { m_ShowRect = rect; }
		void	SetMaxLine( USHORT sNum ) { m_sMaxLine = sNum; }
		void	SetImageList( CImageList* pList ) { m_pImageList = pList; }
		void	SetTitleInfo( DWORD dwColor, BYTE byFont ) { m_dwTitleColor = dwColor; m_byTitleFont = byFont; }
		void	SetIntervalDist( BYTE byDist ) { m_byDist = byDist; }
		void	SetAutoScroll( BOOL bAuto ) { m_bAutoScroll = bAuto; }
		void	AutoScroll();		

	protected:
		// ��ʾ����Ϣ�ṹ
		struct MEMO_COLINFO
		{
			string  strDesp;
			CGraph* pIcon;
			BYTE	byType;
			DWORD	dwColor;	// ������ɫ
			USHORT  sxPos;
		};

		// ÿ������Ϣ����
		typedef vector<MEMO_COLINFO> MEMO_COLINFO_ARRAY;

		// ����Ϣ��ʾ�ṹ
		struct MEMO_LINE_INFO
		{
			MEMO_COLINFO_ARRAY ColInfoArray;
			BYTE	byFontType; // ��������
			USHORT  sFontWidth;	// ��ʾ������
			USHORT	sFontHeight;
			USHORT  sHeight;	// ��ʾ�и߶�
		};

		// ������Ҫ��ʾ������Ϣ�б�
		typedef list<MEMO_LINE_INFO*> MEMO_LINEINFO_LIST;
			MEMO_LINEINFO_LIST	m_LineList;

		// ÿ���������Ϣ�ṹ
		struct MEMO_SECTION_INFO
		{
			BYTE	byFontType; // ��������
			USHORT  sFontWidth;	// ��ʾ������
			USHORT	sFontHeight;
			DWORD	dwColor;
			string  strTitle;
			string	strDesp;
		};

		
		static void	OnScrollChange( CGuiData *pSender )
		{
			((CRichMemo*)(pSender->GetParent()))->OnScrollChange();
		}
		
		void	ParseScript( const char szInfo[], DWORD dwDefColor, BYTE byFont, USHORT sStartCom, USHORT sMaxCom );
		DWORD	GetColor( BYTE byType );
		CGraph* GetImage( const char szImage[] );

	private:
		CImageList*	m_pImageList;   // ��ʾͼ���б�
		CScroll*	m_pScroll;		// �������ؼ�
		RECT		m_ShowRect;		// �ؼ���Ч��ʾ����ü�
		USHORT		m_sMaxLine;		// ��������ʾ�Ķ���Ϣ����
		DWORD		m_dwTitleColor;	// �����������ɫ
		BYTE		m_byTitleFont;	// ��������Ĵ�С
		BYTE		m_byDist;		// ����ÿ����ʾ�������
		BOOL		m_bAutoScroll;  // �Ƿ��Զ�����
	};


	class CFaceImage : public CImageList
	{
		struct sImage
		{
			string name;
			CGraph* pImage;
		};
	public:
		CFaceImage();
		virtual ~CFaceImage();

		virtual CGraph* GetImage( const char* szImage );
		virtual bool	AddImage( const char* szImage, CGraph* pImage );
	private:
		static vector<sImage*> m_pData;
	};

}
