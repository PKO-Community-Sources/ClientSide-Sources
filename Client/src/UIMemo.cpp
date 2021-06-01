#include "StdAfx.h"
#include "uimemo.h"
#include "uigraph.h"
#include "uitextparse.h"
#include <characterrecord.h>
#include "gameapp.h"
using namespace GUI;

CMemo::CMemo(CForm& frmOwn)
: CCompent(frmOwn), _IsTextCenter(true), _color(0xff000000)
,_nItemRowNum(0),_nMisRowNum(0),evtSelectChange(NULL) ,_nMaxNum(40),_nRowNum(0)
, _nFirst(0),_nLast(0),_nPageShowNum(4),_bIsHaveItem(false),_bIsHaveMis(false)
,_nLeftMargin(0),_nTopMargin(4),_nRightMargin(4),_nBottomMargin(4)
,_nRowHeight(20), _nSelectItem(-1) ,_nSelectMis(-1),_nMemoNum(0)
{
	int i=0;
	for (; i<MEMO_MAX_LINE; i++)
        _str[i]= "" ;

	for (i=0; i<MEMO_MAX_ITEM; i++)
		_strItem[i] = "" ;
	for (i=0; i<MEMO_MAX_MISSION; i++)
		_strMis[i] = "" ;

	for (i=0 ;i<3 ;i++)
		for(int j=0; j<2 ;j++)
			_nRowInfo[i][j] = 0;


	_strCaption  = "" ;
	_pScroll = new CScroll( frmOwn );	
	_SetSelf();
}

CMemo::CMemo( const CMemo& rhs )
: CCompent(rhs), _color(rhs._color), _strCaption(rhs. _strCaption), _IsTextCenter(true)
{
	_pScroll = new CScroll(*rhs._pScroll);
	_nLeftMargin = rhs._nLeftMargin;
	_nTopMargin = rhs._nTopMargin;
	_nRightMargin = rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;
	_bIsHaveItem = rhs._bIsHaveItem ;
	_bIsHaveMis  = rhs._bIsHaveMis ;
	_nItemRowNum  =rhs._nItemRowNum ;
	_nMisRowNum   = rhs._nMisRowNum;

	_nMemoNum =rhs._nMemoNum;
	evtSelectChange =rhs.evtSelectChange;

	_Copy(rhs);
}

void  CMemo::reset()
{
	int i=0;
	for (; i<MEMO_MAX_LINE; i++)
        _str[i]= "" ;

	for (i=0; i<MEMO_MAX_ITEM; i++)
		_strItem[i] = "" ;

	for (i=0; i<MEMO_MAX_MISSION; i++)
		_strMis[i] = "" ;
    
	
	for (i=0 ;i<3 ;i++)
		for(int j=0; j<2 ;j++)
			_nRowInfo[i][j] = 0;

	_strCaption  = "" ;
	_nSelectItem = -1;
	_nSelectMis  = -1;
	_nMemoNum =0;
	_bIsHaveItem = false;
	_bIsHaveMis = false;
	_nItemRowNum =0;
	_nMisRowNum = 0;

	while(!_files.empty())
	{
		_files.pop_back();
	}




}

CMemo& CMemo::operator=(const CMemo& rhs)
{
	CCompent::operator =(rhs);
	*_pScroll = *rhs._pScroll;

	_nLeftMargin   = rhs._nLeftMargin;
	_nTopMargin    = rhs._nTopMargin;
	_nRightMargin  = rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;
	_bIsHaveItem   = rhs._bIsHaveItem ;
	_bIsHaveMis    = rhs._bIsHaveMis;
	_nItemRowNum   = rhs._nItemRowNum ;
	_nMisRowNum    = rhs._nMisRowNum;

	_nMemoNum	   = rhs._nMemoNum;

	evtSelectChange =rhs.evtSelectChange;


	_Copy(rhs);
    return *this;
}

CMemo::~CMemo(void)
{
}

void CMemo::_Copy( const CMemo& rhs )
{
	_nRowHeight=rhs._nRowHeight;
	_nMaxNum=rhs._nMaxNum;
	_nRowNum=rhs._nRowNum;
	_nItemRowNum=rhs._nItemRowNum;
	_nMisRowNum=rhs._nMisRowNum;
	_nMemoNum=rhs._nMemoNum;
	_nSelectItem=rhs._nSelectItem;
	_nSelectMis=rhs._nSelectMis;
	_strCaption=rhs._strCaption;
	_nFirst=rhs._nFirst;
	_nLast=rhs._nLast;
	_nPageShowNum=rhs._nPageShowNum;

	_SetSelf();
}

void CMemo::_SetSelf()
{
	_pScroll->SetParent(this);
	_pScroll->SetIsShow(true );
	_pScroll->evtChange = _OnScrollChange;
}


void CMemo::_CheckTextAlign()
{


}

void CMemo::_CheckScroll()
{	

}
void CMemo::AddItemRowContent(int row ,const  char* szFunc, const char* szItemEx )
{
	if( !szFunc )
	{
		LG( "errormemo", "msgCMemo::AddItemRowContent szFunc is null!\n" );
		return;
	}

	if( row>=MEMO_MAX_ITEM || row<0 )
	{
		LG( "errormemo", "msgCMemo::AddItemRowContent( %d, %s ) out of range!\n", row, szFunc );
		return;
	}
	//任务处理
	_strItem[row] = szFunc;
	_strItemEx[row] = szItemEx;
}

void CMemo::AddMisRowContent(int row ,const char *szFunc)
{
	if( !szFunc )
	{
		LG( "errormemo", "msgCMemo::AddMisRowContent szFunc is null!\n" );
		return;
	}

	if( row>=MEMO_MAX_ITEM || row<0 )
	{
		LG( "errormemo", "msgCMemo::AddMisRowContent( %d, %s ) out of range!\n", row, szFunc );
		return;
	}

	_strMis[row]  = szFunc ;
}

void CMemo::ProcessCaption()
{
	//换行标志：_
	for (int nRow =0 ; nRow <=_nRowNum ; nRow++)
		_str[nRow] = "";
	_nRowNum = 0 ;

	
	//对输入的字符的处理
	const char  *s = _strCaption.c_str() ;
	string  strTemp ;
	int length =(int ) strlen( s );
	int begin = 0 ;
	int end   = 0 ;
	int i ;
	bool bNotFlag = false ;
	for (  i = 0  ; i < length ; i ++ )
	{			
		if( s[i] & 0x80 )
		{
			i++;
			continue;
		}
		else if ( s[i] == '_')
		{
			bNotFlag = true ;
			end = i ;
			strTemp =  _strCaption.substr( begin , end - begin );

			if ( (int)strlen( strTemp.c_str()) > _nMaxNum  )
			{
				while ( (int)strlen( strTemp.c_str()) > _nMaxNum )
				{
                    _str[_nRowNum] = GetSelfString( strTemp, _nMaxNum , true ) ;
					strTemp =  strTemp.substr( _str[_nRowNum].length() , strTemp.length() -_str[_nRowNum].length() ) ;
					_nRowNum +=1;
				}
			}
			else
			{
				_str[ _nRowNum] = strTemp ;
				_nRowNum +=1;
			}
			begin =  i+1;			
		}
	}
	if ( bNotFlag )		
	{
		_str[_nRowNum] = _strCaption.substr( begin , length - begin );
		_nRowNum++;
	}

	else if ( ! bNotFlag )
	{
		strTemp  = _strCaption ;
		if ( (int)strlen( strTemp.c_str()) > _nMaxNum  )
		{
			while ( (int)strlen( strTemp.c_str()) > _nMaxNum )
			{
                _str[_nRowNum] =GetSelfString( strTemp, _nMaxNum , true ) ;	
				size_t len=strTemp.length() -_str[_nRowNum].length();
				len = (len > strTemp.length())?strTemp.length():len;
				string strTmp = strTemp.substr( _str[_nRowNum].length() , len );
				strTemp =  strTmp;
				_nRowNum +=1;			
			}
			_str[_nRowNum] = strTemp ;
			_nRowNum +=1;
		}
		else
		{
			_str[ _nRowNum] = strTemp ;
			_nRowNum +=1;
		}
	}
	
	_nRowInfo[0][1] = _nRowNum;    //前面属于memo内容

	if (_strCaption.length() == 0)
	{
		_nRowInfo[0][0]=_nRowInfo[0][1] =0;
		_nRowNum = 0;		
	}

	if( _bIsHaveMis)
	{
		_str[_nRowNum] = "" ;
		_nRowNum++;
		_nRowInfo[1][0]= _nRowNum;

		for(int i = 0 ; i<_nMisRowNum ; i ++)  
		{			
			_str[_nRowNum]  =  _strMis[i];
			_nRowNum++;				
		} 	       
		_nRowInfo[1][1]= _nRowNum;
	}
	else
	{
		//_str[_nRowNum] = "" ;
		//_nRowNum++;
		//_nRowInfo[1][0]= _nRowNum;
		_nRowInfo[1][0]= _nRowNum;
		
	}

    if (_bIsHaveItem)
	{
        _str[_nRowNum]  = "";   //空行，区分说明文字与任务
		_nRowNum++;		
		_nRowInfo[2][0]= _nRowNum;
	
		for(i = 0 ; i<_nItemRowNum ; i ++)  //选择lst的行数
		{
			_str[_nRowNum]  =  _strItem[i];
			_nRowNum++;				
		}
		_nRowInfo[2][1]= _nRowNum;
	}
	else
	{
		_str[_nRowNum]  = ""; 
		_nRowNum++;
		_nRowInfo[2][0]= _nRowNum;
		_nRowInfo[2][1]= _nRowNum;
	}
	Refresh();
}

void CMemo::Init()
{
	_pScroll->Reset();
	_pScroll->SetSize( _pScroll->GetWidth(), GetHeight() );
	_pScroll->SetPos( GetWidth() - _pScroll->GetWidth(), 0 );
	_pScroll->Init();
	_OnScrollChange();

	_strCaption = "";
	int i = 0;
	for( ; i < MEMO_MAX_LINE; i++ )
	{
		_str[i] = "";
	}

	_IsTextCenter = false;
	for( i = 0; i < MEMO_MAX_ITEM; i++ )
	{
		_strItem[i] = "";
	}
	_nSelectItem = 0;
	
	for( i = 0; i < MEMO_MAX_ITEM ; i++ )
	{
		_strMis[i] = "";
	}
	_nSelectMis = 0;
	_bIsHaveItem = false;
	_bIsHaveMis = false;
}

void CMemo::AddIcon(int index ,CGraph *pGraph)
{
	stIconIndex f;
	f.nIndex = index;
	f.pGraph = pGraph;
	_files.push_back( f );
}

void CMemo::Render()
{
	int i = 0;
	int offset = 21;
	for( i =_nFirst ;i<_nLast; i++ )
	{
		if(i >=_nRowInfo[0][0] &&i<= _nRowInfo[0][1])
		{
			CGuiFont::s_Font.Render( (char*)_str[i].c_str(), GetX() +_nLeftMargin , GetY()+ _nTopMargin + _nRowHeight * (i- _nFirst ) ,0xFF000000 );// 0xFFFFFFFF ); 
		}
		else if(i >=_nRowInfo[1][0] &&i<= _nRowInfo[1][1])
		{
			if ( i == _nSelectMis + _nRowInfo[1][0])
			{
				int nPosX =  GetX() +_nLeftMargin + offset;
				int nPosY =  GetY()+ _nTopMargin + _nRowHeight * (i - _nFirst );

				if (_files[i-_nRowInfo[1][0]].pGraph && _str[i].length() > 0)
					_files[i-_nRowInfo[1][0]].pGraph->Render( nPosX ,nPosY - 3 );
				
				CGuiFont::s_Font.Render( (char*)_str[i].c_str(), nPosX + offset, nPosY, 0xFFFF00FF );
			}
			else
			{
				int nPosX =  GetX() +_nLeftMargin + offset;
				int nPosY =  GetY()+ _nTopMargin + _nRowHeight * (i - _nFirst );

				if (_files[i-_nRowInfo[1][0]].pGraph && _str[i].length() > 0)
					_files[i-_nRowInfo[1][0]].pGraph->Render( nPosX, nPosY - 2 );

				CGuiFont::s_Font.Render( (char*)_str[i].c_str(), nPosX + offset, nPosY + 1, 0xFFcF00cF );
			}
		}
		if(i >=_nRowInfo[2][0] &&i<= _nRowInfo[2][1])
		{
			if ( i == _nSelectItem + _nRowInfo[2][0])
			{
				int nPosX =  GetX() +_nLeftMargin;
				int nPosY =  GetY()+ _nTopMargin + _nRowHeight * (i- _nFirst );
				CGuiFont::s_Font.Render( (char*)_str[i].c_str(), nPosX + 24, nPosY, 0xFF0000FF );
			}
			else
			{
				int nPosX =  GetX() +_nLeftMargin;
				int nPosY =  GetY()+ _nTopMargin + _nRowHeight * (i- _nFirst );
				CGuiFont::s_Font.Render( (char*)_str[i].c_str(), nPosX + 24, nPosY + 1, 0xFF0000aF );
			}
		}
	}

	if( _pScroll->GetIsShow() ) _pScroll->Render();
}

void CMemo::Refresh()
{
	CCompent::Refresh();
	_pScroll->SetRange( (float)0.0f, (float )_nRowNum-4 );
	_pScroll->Refresh();
	_OnScrollChange();
}

void CMemo::SetMisRowNum(int num ) 
{
	if( num<0 || num>=MEMO_MAX_ITEM )
	{
		LG( "errormemo", "msgCMemo::SetMisRowNum( %d ) out of range!\n", num );
		if( num<0 ) num=0;
		if( num>=MEMO_MAX_ITEM ) num=MEMO_MAX_ITEM-1;
	}
	_nMisRowNum = num;
}

bool CMemo::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;
	_nSelectMis = -1;
	_nSelectItem= -1;
	if( InRect( x, y ) )
	{
		if( _pScroll->MouseRun( x, y, key ) ) return true;

		int  nPosY = y -GetTop() - GetForm()->GetTop();
		int  nRow = _nFirst  + (nPosY -  _nTopMargin) / _nRowHeight - _nMemoNum;
		if (_bIsHaveMis && nRow>=_nRowInfo[1][0] &&nRow<=_nRowInfo[1][1] )
		{
			_nSelectMis = nRow - _nRowInfo[1][0];
			if(_nSelectMis >=_nMisRowNum)
			{
				_nSelectMis= -1;
			}
			else if( _strMis[_nSelectMis].length() > 0 )
			{
				SetCursor( CCursor::stButtonClick );
			}
		}
		else if (_bIsHaveItem && nRow>=_nRowInfo[2][0] &&nRow<=_nRowInfo[2][1] )
		{
			_nSelectItem = nRow - _nRowInfo[2][0];
			if(_nSelectItem>= _nItemRowNum)
			{
				_nSelectItem = -1;
			}
			else if( _strItem[_nSelectItem].length() > 0 )
			{
				SetCursor( CCursor::stButtonClick );
			}
		}

		if( (key & Mouse_LDown) )
		{
			if( evtSelectChange ) evtSelectChange( this );
		}
		return true;
	}

	return _IsMouseIn;
}

bool CMemo::GetSelectItemText( string& item, string& itemex )
{
	if( _nSelectItem>=0 && _nSelectItem<MEMO_MAX_ITEM )
	{
		item = _strItem[_nSelectItem];
		itemex = _strItemEx[_nSelectItem];
		return true;
	}
	return false;
}

void CMemo::_OnScrollChange()
{
	 _nFirst = (int)_pScroll->GetStep().GetPosition() ;	
	_nLast = _nFirst + _nPageShowNum;
	if( _nLast> MEMO_MAX_LINE ) _nLast= MEMO_MAX_LINE;
}


bool CMemo::MouseScroll( int nScroll )
{
	if( !IsNormal() ) return false;
	
	if( _IsMouseIn ) _pScroll->MouseScroll( nScroll );
	return _IsMouseIn;
}


void CMemo::SetMargin( int left, int top, int right, int bottom ) 
{ 
	_nLeftMargin   = left;
	_nTopMargin    = top;
	_nRightMargin  = right;
	_nBottomMargin = bottom; 
}

void CMemo::_SetScrollRange()
{	
	_pScroll->Refresh();
}


////////////////////////////////////////////////////
//说明：该控件用于处理任务界面的显示和处理
////////////////////////////////////////////////////

CMemoEx::CMemoEx(CForm& frmOwn)
: CCompent(frmOwn), _color(0xff000000),evtSelectChange(NULL) 
,_nMaxNum(64),_nRowNum(0),_nFirst(0),_nLast(0),_nPageShowNum(4)
,_nLeftMargin(0),_nTopMargin(4),_nRightMargin(4),_nBottomMargin(4),
_nRowHeight(14),_nTitleHeight(24),_nIconHeight(18),_nPhraseNum(0), evtClickItem(NULL)
{
	_pScroll = new CScroll( frmOwn );	
	_SetSelf();

	// 初始化任务数据信息
	for( int i = 0; i < MEMO_MAX_LINE; i++ )
	{
		m_MemoInfo[i].strDesp = "";
		m_MemoInfo[i].byData = 0;
		m_MemoInfo[i].sData  = 0;
		m_MemoInfo[i].byType = 0;
		m_MemoInfo[i].dwColor = 0;
		m_MemoInfo[i].pIcon = NULL;
		m_MemoInfo[i].sxPos = 0;
		m_MemoInfo[i].syPos = 0;
	}	

	// Add by lark.li 20080721 begin
	m_SelMem = -1;
	// End
	
	memset( &m_PageInfo, 0, sizeof(NET_MISPAGE) );
	m_sNumInfo = 0;
	m_bUpdate = FALSE;
	m_pPrizePic = NULL;
	m_pPrizeSelPic = NULL;
	m_bIsSelect = FALSE;
}

CMemoEx::CMemoEx( const CMemoEx& rhs )
: CCompent(rhs), _color(rhs._color)
{
	_pScroll = new CScroll(*rhs._pScroll);
	_nLeftMargin = rhs._nLeftMargin;
	_nTopMargin = rhs._nTopMargin;
	_nRightMargin = rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;

	_nPhraseNum   = rhs._nPhraseNum ;

	
	evtSelectChange =rhs.evtSelectChange;
	evtClickItem =rhs.evtClickItem;
	m_pPrizePic = NULL;
	m_pPrizeSelPic = NULL;
	m_bIsSelect = FALSE;
	_Copy(rhs);
}

void  CMemoEx::Clear()
{
	// 初始化任务数据信息
	for( int i = 0; i < MEMO_MAX_LINE; i++ )
	{
		m_MemoInfo[i].strDesp = "";
		m_MemoInfo[i].byData = 0;
		m_MemoInfo[i].sData  = 0;
		m_MemoInfo[i].byType = 0;
		m_MemoInfo[i].dwColor = 0;
		SAFE_DELETE(m_MemoInfo[i].pIcon);
		m_MemoInfo[i].sxPos = 0;
		m_MemoInfo[i].syPos = 0;
	}	

	// Add by lark.li 20080721 begin
	m_SelMem = -1;
	// End
	
	memset( &m_PageInfo, 0, sizeof(NET_MISPAGE) );
	m_sNumInfo = 0;
	m_bUpdate = FALSE;
	m_bySelPrize = -1;
	m_bIsSelect = FALSE;
}

CMemoEx& CMemoEx::operator=(const CMemoEx& rhs)
{
	CCompent::operator =(rhs);
	*_pScroll = *rhs._pScroll;

	_nLeftMargin   = rhs._nLeftMargin;
	_nTopMargin    = rhs._nTopMargin;
	_nRightMargin  = rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;
	
	evtSelectChange =rhs.evtSelectChange;
	evtClickItem =rhs.evtClickItem;
	m_pPrizePic = NULL;
	m_pPrizeSelPic = NULL;
	m_bIsSelect = FALSE;

	_Copy(rhs);
    return *this;
}

CMemoEx::~CMemoEx(void)
{
	Clear();
	SAFE_DELETE(m_pPrizePic);
	SAFE_DELETE(m_pPrizeSelPic);
}

void CMemoEx::_Copy( const CMemoEx& rhs )
{
	_SetSelf();
}

void CMemoEx::_SetSelf()
{
	_pScroll->SetParent(this);
	_pScroll->SetIsShow(true );
	_pScroll->evtChange = _OnScrollChange;
}

void CMemoEx::_CheckScroll()
{	
}

void CMemoEx::Init()
{
	_pScroll->Reset();
	_pScroll->SetRange( 0, 16 );
	_pScroll->SetSize( _pScroll->GetWidth(), GetHeight() );
	_pScroll->SetPos( GetWidth() - _pScroll->GetWidth(), 0 );
	_pScroll->Init();
	_OnScrollChange();

	// 装载奖励选项背景图形
	if( m_pPrizePic == NULL && m_pPrizeSelPic == NULL )
	{
		SAFE_DELETE(m_pPrizePic);
		SAFE_DELETE(m_pPrizeSelPic);

		m_pPrizePic = new CGuiPic;
		m_pPrizePic->LoadImage( "./texture/ui/QQ2.tga", 156, 38, 0, 81, 0 );
		m_pPrizeSelPic = new CGuiPic;
		m_pPrizeSelPic->LoadImage( "./texture/ui/QQ2.tga", 156, 38, 0, 81, 39 );
	}

	// 初始化任务数据信息
	Clear();
}

BOOL CMemoEx::SelPrizeItem( int nxPos, int nyPos, DWORD dwKey )
{
	if( !GetIsShow() ) {
		return FALSE;
	}
	
	int nPosY = GetY() + _nTopMargin;
	int nPosX = GetX() + _nLeftMargin;

	// 字体尺寸
	int nHeight = _nRowHeight;
	int nWidth = CGuiFont::s_Font.GetWidth("a");	
	int x, y;
	int nInterval = 0;
	BYTE bySelID = 0;

	for( int i = 0; i < m_sNumInfo; i++ )
	{
		if( m_MemoInfo[i].byType != MEMO_LINE_ICON )
		{
			if( m_MemoInfo[i].syPos < _nFirst || m_MemoInfo[i].syPos > _nLast )
			{
				continue;
			}
		}

		x = m_MemoInfo[i].sxPos * nWidth + nPosX;
		y = (m_MemoInfo[i].syPos - _nFirst) * _nRowHeight + nPosY + nInterval*2;
		if( m_MemoInfo[i].byType == MEMO_LINE_TITLE )
		{
			//CGuiFont::s_Font.Render( 2, m_MemoInfo[i].strDesp.c_str(), x, y, m_MemoInfo[i].dwColor );
			nInterval++;
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_DESP )
		{
			//CGuiFont::s_Font.Render( m_MemoInfo[i].strDesp.c_str(), x, y, m_MemoInfo[i].dwColor );
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_INTERVAL )
		{
			nInterval += 2;
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_ICON )
		{
			//m_MemoInfo[i].pIcon->Render( x, y );
			if( nxPos >= x - 2 && nxPos <= x + 25 * nWidth && nyPos >= y && nyPos <= y + 32 )
			{
				if( dwKey & Mouse_LDown )
				{
					m_bySelPrize = bySelID;
				}
				return TRUE;
			}
			bySelID ++;
		}
		else
		{
			// 未知的数据需要显示
		}
	}
	return FALSE;
}

// Add by lark.li 20080721 begin
int CMemoEx::SelMemInfo(int x, int y, DWORD key)
{
	//if(Mouse_LUp != key)
	//	return -1;

	if( !GetIsShow() ) return -1;
	
	int nPosY = GetY() + _nTopMargin;
	int nPosX = GetX() + _nLeftMargin;

	// 字体尺寸
	int nHeight = _nRowHeight;
	int nWidth = CGuiFont::s_Font.GetWidth("a");	
	int startX, startY;
	
	int nInterval = 0;

	for( int i = 0; i < m_sNumInfo; i++ )
	{
		if( m_MemoInfo[i].byType != MEMO_LINE_ICON )
		{
			if( m_MemoInfo[i].syPos < _nFirst || m_MemoInfo[i].syPos > _nLast )
			{
				continue;
			}
		}

		startX = m_MemoInfo[i].sxPos * nWidth + nPosX;
		startY = (m_MemoInfo[i].syPos - _nFirst) * _nRowHeight + nPosY + nInterval*2;

		if( m_MemoInfo[i].byType == MEMO_LINE_TITLE )
		{
			nInterval++;
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_DESP )
		{
			if( startX < x && ( x < startX + nWidth * m_MemoInfo[i].strDesp.length()) && startY < y && ( y < startY + nHeight ))
			{
				return i;
			}
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_INTERVAL )
		{
			nInterval += 2;
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_ICON )
		{

		}
		else
		{
			// 未知的数据需要显示
		}
	}

	return -1;
}
// End

void CMemoEx::Render()
{	
	if( !GetIsShow() ) return;
	
	int nPosY = GetY() + _nTopMargin;
	int nPosX = GetX() + _nLeftMargin;

	// 字体尺寸
	int nHeight = _nRowHeight;
	int nWidth = CGuiFont::s_Font.GetWidth("a");	
	int x, y;
	int nInterval = 0;
	BYTE bySelID = 0;

	// 裁剪显示区
	GetRender().SetClipRect( nPosX, nPosY, 32 * nWidth, (_nPageShowNum + 1) * nHeight );

	//CGuiFont::s_Font.Begin();
	for( int i = 0; i < m_sNumInfo; i++ )
	{
		if( m_MemoInfo[i].byType != MEMO_LINE_ICON )
		{
			if( m_MemoInfo[i].syPos < _nFirst || m_MemoInfo[i].syPos > _nLast )
			{
				continue;
			}
		}

		x = m_MemoInfo[i].sxPos * nWidth + nPosX;
		y = (m_MemoInfo[i].syPos - _nFirst) * _nRowHeight + nPosY + nInterval*2;
		if( m_MemoInfo[i].byType == MEMO_LINE_TITLE )
		{
			//CGuiFont::s_Font.End();

			CGuiFont::s_Font.Render( 2, m_MemoInfo[i].strDesp.c_str(), x, y, m_MemoInfo[i].dwColor );
			nInterval++;

			//CGuiFont::s_Font.Begin();
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_DESP )
		{
			//if( this->m_SelMem == i)
			//	CGuiFont::s_Font.Render( m_MemoInfo[i].strDesp.c_str(), x, y, 0xFF0000FF );
			//else
				CGuiFont::s_Font.Render( m_MemoInfo[i].strDesp.c_str(), x, y, m_MemoInfo[i].dwColor );

			//CGuiFont::s_Font.Draw( m_MemoInfo[i].strDesp.c_str(), x, y, m_MemoInfo[i].dwColor );
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_INTERVAL )
		{
			nInterval += 2;
		}
		else if( m_MemoInfo[i].byType == MEMO_LINE_ICON )
		{
			//CGuiFont::s_Font.End();

			if( m_bySelPrize == bySelID ) 
			{
				m_pPrizeSelPic->Render( x - 4, y - 3 );
			}
			else
			{
				m_pPrizePic->Render( x - 4, y - 3 );
			}
			m_MemoInfo[i].pIcon->Render( x, y );
			bySelID++;

			//CGuiFont::s_Font.Begin();
		}
		else
		{
			// 未知的数据需要显示
		}
	}
	//CGuiFont::s_Font.End();

	GetRender().Reset();
	if( _pScroll->GetIsShow() ) _pScroll->Render();
}

void CMemoEx::SetMisPage( const NET_MISPAGE& page )
{
	m_bUpdate = TRUE;
	memcpy( &m_PageInfo, &page, sizeof(NET_MISPAGE) );
	ParseMisPage();
}

void CMemoEx::ParseMisPage()
{
	USHORT sMaxCom = 32;
	USHORT sRow = 0, sCom = 0;
	DWORD dwColor = 0xFF000000; // 黑色

	// 任务名称标题
	m_MemoInfo[m_sNumInfo].strDesp = m_PageInfo.szName;
	m_MemoInfo[m_sNumInfo].dwColor = 0xFF000000;
	m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_TITLE;
	m_MemoInfo[m_sNumInfo].sxPos   = sCom;
	m_MemoInfo[m_sNumInfo].syPos   = sRow;

	// 开始下一行	
	m_sNumInfo++;
	sRow++;
	m_MemoInfo[m_sNumInfo].strDesp = "";
	m_MemoInfo[m_sNumInfo].dwColor = dwColor;
	m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_DESP;
	m_MemoInfo[m_sNumInfo].sxPos   = sCom;
	m_MemoInfo[m_sNumInfo].syPos   = sRow;

	// 任务描述解析
	char* pszData = m_PageInfo.szDesp;
	char* pszTemp = pszData;
	ParseScript( pszData, m_sNumInfo, sRow, sCom, 0, sMaxCom, 0xFF000000, m_MemoInfo, MEMO_MAX_LINE );
	

	// 开始下一行	
	m_sNumInfo++;
	//sRow++;
	m_MemoInfo[m_sNumInfo].strDesp = "";
	m_MemoInfo[m_sNumInfo].dwColor = dwColor;
	m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_INTERVAL;
	m_MemoInfo[m_sNumInfo].sxPos   = sCom;
	m_MemoInfo[m_sNumInfo].syPos   = sRow;

	char szNumber[128] = {0};
	// 任务目标
	if( m_PageInfo.byNeedNum > 0 )
	{
		sRow++;
		sCom = 0;
		dwColor = 0xFF000000;
		m_sNumInfo++;
		m_MemoInfo[m_sNumInfo].strDesp = g_oLangRec.GetString(704);
		m_MemoInfo[m_sNumInfo].dwColor = dwColor;
		m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_TITLE;
		m_MemoInfo[m_sNumInfo].sxPos   = sCom;
		m_MemoInfo[m_sNumInfo].syPos   = sRow;
		
		for( int i = 0; i < m_PageInfo.byNeedNum; i++ )
		{
			string strTemp = "  ";
			if( m_PageInfo.MisNeed[i].byType == mission::MIS_NEED_ITEM )
			{
				strTemp += g_oLangRec.GetString(705);
				CItemRecord *pItem = GetItemRecordInfo( m_PageInfo.MisNeed[i].wParam1 );
				if( !pItem )
				{
					strTemp += g_oLangRec.GetString(706);
					sprintf( szNumber, "0x%X", m_PageInfo.MisNeed[i].wParam1 );
					strTemp += szNumber;
					strTemp += "]>";
				}
				else
				{
					strTemp += g_oLangRec.GetString(707);
					strTemp += pItem->szName;
					strTemp += g_oLangRec.GetString(708);
				}
				szNumber[0] = 0;
				sprintf( szNumber, "<r%d/%d>", m_PageInfo.MisNeed[i].wParam3, m_PageInfo.MisNeed[i].wParam2 );
				size_t nLen = strTemp.length() + strlen( szNumber ) - 6;
				if( nLen > sMaxCom )
				{
					strTemp += szNumber;
				}
				else
				{
					for( int n = 0; n < int(sMaxCom - nLen); n++ )
					{
						strTemp += " ";
					}
					strTemp += szNumber;
				}
			}
			else if( m_PageInfo.MisNeed[i].byType == mission::MIS_NEED_KILL )
			{
				strTemp += g_oLangRec.GetString(709);
				CChaRecord* pInfo = GetChaRecordInfo( m_PageInfo.MisNeed[i].wParam1 );
				if( !pInfo)
				{
					strTemp += g_oLangRec.GetString(706);
					sprintf( szNumber, "0x%X", m_PageInfo.MisNeed[i].wParam1 );
					strTemp += szNumber;
					strTemp += "]>";
				}
				else
				{
					strTemp += g_oLangRec.GetString(707);
					strTemp += pInfo->szName;
					strTemp += g_oLangRec.GetString(708);
				}
				szNumber[0] = 0;
				sprintf( szNumber, "<r%d/%d>", m_PageInfo.MisNeed[i].wParam3, m_PageInfo.MisNeed[i].wParam2 );
				size_t nLen = strTemp.length() + strlen( szNumber ) - 6;
				if( nLen > sMaxCom )
				{
					strTemp += szNumber;
				}
				else
				{
					for( int n = 0; n < int(sMaxCom - nLen); n++ )
					{
						strTemp += " ";
					}
					strTemp += szNumber;
				}
			}
			else if( m_PageInfo.MisNeed[i].byType == mission::MIS_NEED_DESP )
			{
				strTemp += m_PageInfo.MisNeed[i].szNeed;
			}
			else
			{
				// 错误的类型
				strTemp += g_oLangRec.GetString(710);
			}

			sRow++;
			sCom = 0;
			dwColor = 0xFF000000;
			m_sNumInfo++;
			m_MemoInfo[m_sNumInfo].strDesp = "";
			m_MemoInfo[m_sNumInfo].dwColor = dwColor;
			m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_DESP;
			m_MemoInfo[m_sNumInfo].sxPos   = sCom;
			m_MemoInfo[m_sNumInfo].syPos   = sRow;
			char szNeed[256] = {0};
			strncpy( szNeed, strTemp.c_str(), 255 );
			ParseScript( szNeed, m_sNumInfo, sRow, sCom, 0, sMaxCom, 0xFF000000, m_MemoInfo, MEMO_MAX_LINE );
			if( m_MemoInfo[m_sNumInfo].strDesp.length() <= 0 )
			{
				m_sNumInfo--;
			}
		}

		// 开始下一行
		m_sNumInfo++;
		//sRow++;
		m_MemoInfo[m_sNumInfo].strDesp = "";
		m_MemoInfo[m_sNumInfo].dwColor = dwColor;
		m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_INTERVAL;
		m_MemoInfo[m_sNumInfo].sxPos   = sCom;
		m_MemoInfo[m_sNumInfo].syPos   = sRow;
	}

	if( m_PageInfo.byPrizeNum > 0 )
	{
		// 任务奖励
		sRow++;
		sCom = 0;
		dwColor = 0xFF000000;
		m_sNumInfo++;
		m_MemoInfo[m_sNumInfo].strDesp = g_oLangRec.GetString(711);
		if( m_PageInfo.byPrizeSelType == mission::PRZIE_SELALL )
		{
			//m_MemoInfo[m_sNumInfo].strDesp += "(全部)";
		}
		else
		{
			m_MemoInfo[m_sNumInfo].strDesp += g_oLangRec.GetString(712);
		}
		m_MemoInfo[m_sNumInfo].dwColor = dwColor;
		m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_TITLE;
		m_MemoInfo[m_sNumInfo].sxPos   = sCom;
		m_MemoInfo[m_sNumInfo].syPos   = sRow;

		sRow++;
		sCom = 1;
		char szItem[64];
		for( int n = 0; n < m_PageInfo.byPrizeNum; n++ )
		{
			strcpy( szItem, "./texture/ui/system/error.tga" );
			if( m_PageInfo.MisPrize[n].byType == mission::MIS_PRIZE_ITEM )
			{
				m_sNumInfo++;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				m_MemoInfo[m_sNumInfo].byData  = mission::MIS_PRIZE_ITEM;
				m_MemoInfo[m_sNumInfo].sData   = m_PageInfo.MisPrize[n].wParam1;
				CItemRecord *pItem = GetItemRecordInfo( m_PageInfo.MisPrize[n].wParam1 );
				if( pItem )
				{
					strcpy( szItem, pItem->GetIconFile() );
				}

				m_MemoInfo[m_sNumInfo].byType = MEMO_LINE_ICON;
				m_MemoInfo[m_sNumInfo].pIcon = new CGraph( szItem, 32,32, 0, 0, 1 );

				// 开始下一行
				m_sNumInfo++;
				//sRow++;
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = dwColor;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_INTERVAL;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;

				sCom += 8;

				m_sNumInfo++;
				if( pItem )
				{
					strcpy( szItem, pItem->szName );
				}
				else 
				{
					sprintf( szItem, g_oLangRec.GetString(713), m_PageInfo.MisPrize[n].wParam1 );
				}

				sprintf( szNumber, g_oLangRec.GetString(714), m_PageInfo.MisPrize[n].wParam2, szItem );
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = 0xFF808080;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_DESP;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				ParseScript( szNumber, m_sNumInfo, sRow, sCom, 6, sMaxCom, 0xFF808080, m_MemoInfo, MEMO_MAX_LINE );
				sRow += 2;
				sCom  = 1;
			}
			else if( m_PageInfo.MisPrize[n].byType == mission::MIS_PRIZE_MONEY )
			{
				m_sNumInfo++;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				m_MemoInfo[m_sNumInfo].byData  = mission::MIS_PRIZE_ITEM;
				m_MemoInfo[m_sNumInfo].sData   = m_PageInfo.MisPrize[n].wParam1;
				CItemRecord *pItem = GetItemRecordInfo( 194 );
				if( pItem )
				{
					strcpy( szItem, pItem->GetIconFile() );
				}

				m_MemoInfo[m_sNumInfo].byType = MEMO_LINE_ICON;
				m_MemoInfo[m_sNumInfo].pIcon = new CGraph( szItem, 32,32, 0, 0, 1 );

				// 开始下一行
				m_sNumInfo++;
				//sRow++;
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = dwColor;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_INTERVAL;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;

				sCom += 8;

				m_sNumInfo++;
				sprintf( szNumber, g_oLangRec.GetString(715), m_PageInfo.MisPrize[n].wParam1 );
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = 0xFF808080;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_DESP;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				ParseScript( szNumber, m_sNumInfo, sRow, sCom, 6, sMaxCom, 0xFF808080, m_MemoInfo, MEMO_MAX_LINE );
				sRow += 2;
				sCom  = 1;
			}
			else if( m_PageInfo.MisPrize[n].byType == mission::MIS_PRIZE_FAME  )
			{
				m_sNumInfo++;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				m_MemoInfo[m_sNumInfo].byData  = mission::MIS_PRIZE_ITEM;
				m_MemoInfo[m_sNumInfo].sData   = m_PageInfo.MisPrize[n].wParam1;
				CItemRecord *pItem = GetItemRecordInfo( 195 );
				if( pItem )
				{
					strcpy( szItem, pItem->GetIconFile() );
				}

				m_MemoInfo[m_sNumInfo].byType = MEMO_LINE_ICON;
				m_MemoInfo[m_sNumInfo].pIcon = new CGraph( szItem, 32,32, 0, 0, 1 );

				// 开始下一行
				m_sNumInfo++;
				//sRow++;
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = dwColor;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_INTERVAL;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;

				sCom += 8;

				m_sNumInfo++;
				sprintf( szNumber, g_oLangRec.GetString(716), m_PageInfo.MisPrize[n].wParam1 );
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = 0xFF808080;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_DESP;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				ParseScript( szNumber, m_sNumInfo, sRow, sCom, 6, sMaxCom, 0xFF808080, m_MemoInfo, MEMO_MAX_LINE );
				sRow += 2;
				sCom  = 1;
			}
			else if( m_PageInfo.MisPrize[n].byType == mission::MIS_PRIZE_CESS  )
			{
				m_sNumInfo++;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				m_MemoInfo[m_sNumInfo].byData  = mission::MIS_PRIZE_ITEM;
				m_MemoInfo[m_sNumInfo].sData   = m_PageInfo.MisPrize[n].wParam1;
				CItemRecord *pItem = GetItemRecordInfo( 195 );
				if( pItem )
				{
					strcpy( szItem, pItem->GetIconFile() );
				}

				m_MemoInfo[m_sNumInfo].byType = MEMO_LINE_ICON;
				m_MemoInfo[m_sNumInfo].pIcon = new CGraph( szItem, 32,32, 0, 0, 1 );

				// 开始下一行
				m_sNumInfo++;
				//sRow++;
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = dwColor;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_INTERVAL;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;

				sCom += 8;

				m_sNumInfo++;
				sprintf( szNumber, g_oLangRec.GetString(717), m_PageInfo.MisPrize[n].wParam1 );
				m_MemoInfo[m_sNumInfo].strDesp = "";
				m_MemoInfo[m_sNumInfo].dwColor = 0xFF808080;
				m_MemoInfo[m_sNumInfo].byType  = MEMO_LINE_DESP;
				m_MemoInfo[m_sNumInfo].sxPos   = sCom;
				m_MemoInfo[m_sNumInfo].syPos   = sRow;
				ParseScript( szNumber, m_sNumInfo, sRow, sCom, 6, sMaxCom, 0xFF808080, m_MemoInfo, MEMO_MAX_LINE );
				sRow += 2;
				sCom  = 1;
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(718), m_PageInfo.MisPrize[n].byType, m_PageInfo.MisPrize[n].wParam1, m_PageInfo.MisPrize[n].wParam2);
			}
		}
	}
	m_sNumInfo++;
}

BOOL CMemoEx::ParseScript( char* pszTemp, USHORT& sNumLine, USHORT& sRow, USHORT& sCom, USHORT sStartCom, USHORT sMaxCom, DWORD dwDefColor, MEMO_INFO* pInfo, USHORT sMaxInfo )
{
	if( pszTemp == NULL || pInfo == NULL )
	{
		return FALSE;
	}

	DWORD dwColor = dwDefColor;
	char  szData[3];
	memset( szData, 0, 3 );
	
	while( pszTemp[0] )
	{
		szData[0] = 0;
		szData[1] = 0;
		
		if( pszTemp[0] == '<' )
		{
			pszTemp++;
			if( pszTemp[0] == 'p' )
			{
				dwColor = 0xFFFF00FF; // 紫色
				if( pszTemp[1] == '>' )
				{
					pszTemp++;
					continue;
				}
			}
			else if( pszTemp[0] == 'j' )
			{
				dwColor = 0xFFFE0000;
				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 'r' )
			{
				dwColor = 0xFFFF0000;
				if( pszTemp[1] == '>' )
				{
					pszTemp++;
					continue;
				}
			}
			else if( pszTemp[0] == 'y' )
			{
				dwColor = 0xFFFF8A00;
				if( pszTemp[1] == '>' )
				{
					pszTemp++;
					continue;
				}
			}
			else if( pszTemp[0] == 'b' )
			{
				dwColor = 0xFF0000FF;
				if( pszTemp[1] == '>' )
				{
					pszTemp++;
					continue;
				}
			}
			else if( pszTemp[0] == 'g' )
			{
				dwColor = 0xFF808080;
				if( pszTemp[1] == '>' )
				{
					pszTemp++;
					continue;
				}
			}
			else if( pszTemp[0] == 'n' )
			{
				sRow++;
				sCom = sStartCom;
				if( pszTemp[1] == '>' ) 
				{
					pszTemp++;
					continue;
				}
			}
			else if( pszTemp[0] == 't' )
			{
				sCom += 4;
				if( sCom >= sMaxCom )
				{
					sRow++;
					sCom = sStartCom;
				}
				if( pszTemp[1] == '>' ) 
				{
					pszTemp++;
					continue;
				}
			}
			else
			{
				// 未知类型报错！
				continue;
			}

			// 使用新的字符记录
			pszTemp++;
			sNumLine++;
			pInfo[sNumLine].strDesp = "";
			pInfo[sNumLine].dwColor = dwColor;
			pInfo[sNumLine].byType  = MEMO_LINE_DESP;
			pInfo[sNumLine].sxPos   = sCom;
			pInfo[sNumLine].syPos	= sRow;
		}
		else if( pszTemp[0] == '>' )
		{
			dwColor = dwDefColor; // 恢复字体为黑色
			pszTemp++;

			// 使用新的字符记录
			sNumLine++;
			pInfo[sNumLine].strDesp = "";
			pInfo[sNumLine].dwColor = dwColor;
			pInfo[sNumLine].byType  = MEMO_LINE_DESP;
			pInfo[sNumLine].sxPos   = sCom;
			pInfo[sNumLine].syPos	= sRow;
			continue;
		}

		// 判断是否一个GBK
		BOOL bFlag1 = 0x81 <= (BYTE)pszTemp[0] && (BYTE)pszTemp[0] <= 0xFE;
		BOOL bFlag2 = (0x40 <= (BYTE)pszTemp[1] && (BYTE)pszTemp[1] <= 0x7E) || (0x7E <= (BYTE)pszTemp[1] && (BYTE)pszTemp[1] <= 0xFE);
		if( bFlag1 && bFlag2 )
		{
			if( sMaxCom - sCom >= 2 )
			{
				szData[0] = pszTemp[0];
				szData[1] = pszTemp[1];
				pInfo[sNumLine].strDesp += szData;
				
				// 移动列和指向下一个字符
				sCom += 2;
				pszTemp += 2;
			}
			else
			{
				// 换行
				sRow++;
				sCom = sStartCom;

				sNumLine++;
				pInfo[sNumLine].strDesp = "";
				pInfo[sNumLine].dwColor = dwColor;
				pInfo[sNumLine].byType  = MEMO_LINE_DESP;
				pInfo[sNumLine].sxPos   = sCom;
				pInfo[sNumLine].syPos	= sRow;
			}
		}
		else
		{
			if( sMaxCom - sCom >= 1 )
			{				
				szData[0] = pszTemp[0];
				pInfo[sNumLine].strDesp += szData;
				
				// 移动列和指向下一个字符
				sCom++;
				pszTemp++;
			}
			else
			{
				// 换行
				sRow++;
				sCom = sStartCom;

				sNumLine++;
				pInfo[sNumLine].strDesp = "";
				pInfo[sNumLine].dwColor = dwColor;
				pInfo[sNumLine].byType  = MEMO_LINE_DESP;
				pInfo[sNumLine].sxPos   = sCom;
				pInfo[sNumLine].syPos	= sRow;
			}
		}
	}
	return TRUE;
}

void CMemoEx::Refresh()
{
	CCompent::Refresh();
	//_pScroll->SetRange( (float)0.0f, (float )_nRowNum-4 );  // _nRowNum这个值为零
	_pScroll->Refresh();
	_OnScrollChange();

}

BOOL CMemoEx::IsSelPrize()
{
	return m_PageInfo.byPrizeNum > 0 && m_PageInfo.byPrizeSelType == mission::PRIZE_SELONE;
}

bool CMemoEx::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) return false;

	if( InRect( x, y ) )
	{ 	
		if( _pScroll->MouseRun( x, y, key ) ) {
			return true;
		}
		
		// Add by lark.li 20080721 begin
		m_SelMem = SelMemInfo(x , y, key);
		// End
	
		if(m_SelMem > 0 && evtClickItem)
		{
			if( m_MemoInfo[m_SelMem].dwColor ==  0xFFFE0000)
				GUI::CGuiData::SetCursor( GUI::CCursor::stActive);
			else
				GUI::CGuiData::SetCursor( GUI::CCursor::stNormal);
		}
		else
		{
			GUI::CGuiData::SetCursor( GUI::CCursor::stNormal);
		}

		if( m_SelMem > 0 && (key & Mouse_LDown) )
		{
			if( evtClickItem ) 
			{
				if( m_MemoInfo[m_SelMem-1].dwColor ==  m_MemoInfo[m_SelMem].dwColor )
					evtClickItem( m_MemoInfo[m_SelMem-1].strDesp + m_MemoInfo[m_SelMem].strDesp );
				else if( m_MemoInfo[m_SelMem+1].dwColor ==  m_MemoInfo[m_SelMem].dwColor )
					evtClickItem( m_MemoInfo[m_SelMem].strDesp + m_MemoInfo[m_SelMem+1].strDesp );
				else
					evtClickItem( m_MemoInfo[m_SelMem].strDesp );

				
			}
		}

		if( m_bIsSelect )
		{
			// 判断是否选择了奖励物品
			if( IsSelPrize() )
			{
				if( SelPrizeItem( x, y, key ) )
				{
					SetCursor( CCursor::stButtonClick );
				}
			}
		}
		return true;
	}

	return _IsMouseIn;
}

void CMemoEx::_OnScrollChange()
{
	_nFirst = (int)_pScroll->GetStep().GetPosition();
	_nLast = _nFirst + _nPageShowNum;
	if( _nLast> MEMO_MAX_LINE ) _nLast= MEMO_MAX_LINE;

	return ;
}

bool CMemoEx::MouseScroll( int nScroll )
{
	if( !IsNormal() ) return false;
	
	if( _IsMouseIn ) _pScroll->MouseScroll( nScroll );
	return _IsMouseIn;
}

void CMemoEx::SetMargin( int left, int top, int right, int bottom ) 
{ 
	_nLeftMargin   = left;
	_nTopMargin    = top;
	_nRightMargin  = right;
	_nBottomMargin = bottom; 
}

void CMemoEx::_SetScrollRange()
{	
	_pScroll->Refresh();
}

// CRichMemo implement
CRichMemo::CRichMemo(CForm& frmOwn)
: CCompent(frmOwn), m_pScroll(NULL)
{
	m_ShowRect.left = 0;
	m_ShowRect.right = 0;
	m_ShowRect.top  = 0;
	m_ShowRect.bottom = 0;

	m_pScroll = new CScroll( frmOwn );
	m_pScroll->SetParent( this );
	m_pScroll->SetIsShow( true );
	m_pScroll->evtChange = OnScrollChange;
	m_pScroll->SetAutoHide(false);

	m_pImageList = new CFaceImage;
	m_sMaxLine = 40;
	m_dwTitleColor = 0xFF0000FF;
	m_byTitleFont = 0;
	m_byDist = 2;
	m_bAutoScroll = TRUE;
}

CRichMemo::CRichMemo( const CRichMemo& rhs )
: CCompent( rhs )
{
	m_pScroll = new CScroll( *rhs.m_pScroll );
	m_pScroll->SetParent( this );
	m_pScroll->SetIsShow( true );
	m_pScroll->evtChange = OnScrollChange;

	memcpy( &m_ShowRect, &rhs.m_ShowRect, sizeof(RECT) );
	m_sMaxLine = rhs.m_sMaxLine;
	m_dwTitleColor = rhs.m_dwTitleColor;
	m_byTitleFont = rhs.m_byTitleFont;
	m_byDist = rhs.m_byDist;
	m_pImageList = new CFaceImage;
	m_bAutoScroll = rhs.m_bAutoScroll;
}

CRichMemo& CRichMemo::operator = ( const CRichMemo& rhs )
{
	CCompent::operator = ( rhs );
	*m_pScroll = *rhs.m_pScroll;
	m_pScroll->SetParent( this );
	m_pScroll->SetIsShow( true );
	m_pScroll->evtChange = OnScrollChange;

	memcpy( &m_ShowRect, &rhs.m_ShowRect, sizeof(RECT) );
	m_sMaxLine = rhs.m_sMaxLine;
	m_dwTitleColor = rhs.m_dwTitleColor;
	m_byTitleFont = rhs.m_byTitleFont;
	m_byDist = rhs.m_byDist;
	m_pImageList = new CFaceImage;
	m_bAutoScroll = rhs.m_bAutoScroll;

	return *this;
}

CRichMemo::~CRichMemo(void)
{
	MEMO_LINEINFO_LIST::iterator linePos;
	for( linePos = m_LineList.begin(); linePos != m_LineList.end(); linePos++ )
	{
		SAFE_DELETE( (*linePos) );		
	}
	m_LineList.clear();
	SAFE_DELETE( m_pImageList );
}

void CRichMemo::Render()
{
	int nStart = (int)m_pScroll->GetStep().GetPosition() + m_ShowRect.top;
	int nEnd = nStart + _nHeight - m_ShowRect.bottom;

	int nPosY = GetY();
	int nPosX = GetX();
	DWORD dwyPos = 0;

	// 裁剪显示区
	GetRender().SetClipRect( nPosX + m_ShowRect.left, nPosY + m_ShowRect.top, 
		GetWidth() - m_ShowRect.right+2, GetHeight() - m_ShowRect.bottom );

	nPosY += m_ShowRect.top;
	MEMO_LINEINFO_LIST::iterator pos;
	for( pos = m_LineList.begin(); pos != m_LineList.end(); pos++ )
	{
		MEMO_LINE_INFO* pInfo = *pos;
		dwyPos += pInfo->sHeight + m_byDist;
		// 显示所有可见的信息
		MEMO_COLINFO_ARRAY::iterator colpos;
		for( colpos = pInfo->ColInfoArray.begin(); colpos != pInfo->ColInfoArray.end(); colpos++ )
		{
			int nxPos = (*colpos).sxPos + nPosX;			
			switch( (*colpos).byType )
			{
			case COL_TEXT:
				{
					int nyPos = dwyPos - pInfo->sFontHeight - nStart;
					if( nyPos > _nHeight || nyPos + pInfo->sHeight < 0 )
					{
						continue;
					}
					nyPos += nPosY;
					CGuiFont::s_Font.Render( pInfo->byFontType, (*colpos).strDesp.c_str(), nxPos, nyPos, (*colpos).dwColor );
				}
				break;
			case COL_ICON:
				{
					int nyPos = dwyPos - (*colpos).pIcon->GetHeight() - nStart;
					if( nyPos > _nHeight || nyPos + pInfo->sHeight < 0 )
					{
						continue;
					}
					nyPos += nPosY;
					(*colpos).pIcon->Render( nxPos, nyPos );
				}
				break;
			default:
				break;
			}
		}
	}

	GetRender().Reset();
	if( m_pScroll->GetIsShow() )  m_pScroll->Render();
}

void CRichMemo::AutoScroll()
{
	if( m_bAutoScroll )
	{
		DWORD dwyPos = 0;
		MEMO_LINEINFO_LIST::iterator pos;
		for( pos = m_LineList.begin(); pos != m_LineList.end(); pos++ )
		{
			MEMO_LINE_INFO* pInfo = *pos;		
			dwyPos += pInfo->sHeight + m_byDist;
		}

		int nHeight = _nHeight - (m_ShowRect.top + m_ShowRect.bottom);
		float fData = ( dwyPos > (DWORD)nHeight ) ? float(dwyPos - nHeight) : 0;

		m_pScroll->SetRange( 0, float(fData) );
		m_pScroll->GetStep().SetPosition( fData );
		m_pScroll->Refresh();
	}
}

void CRichMemo::Refresh()
{
	CCompent::Refresh();
	m_pScroll->Refresh();
	OnScrollChange();
}

bool CRichMemo::MouseRun( int x, int y, DWORD key )
{
	if( !IsNormal() ) {
		return false;
	}

	if( InRect( x, y ) )
	{ 	
		return m_pScroll->MouseRun( x, y, key );
	}

	return _IsMouseIn;
}

bool CRichMemo::MouseScroll( int nScroll )
{
	if( !IsNormal() ) {
		return false;
	}
	
	if( _IsMouseIn ) {
		m_pScroll->MouseScroll( nScroll );
	}

	return _IsMouseIn;
}

void CRichMemo::OnScrollChange()
{

}

void CRichMemo::Init()
{
	Clear();
}

void CRichMemo::Clear()
{
	m_pScroll->Reset();
	m_pScroll->SetSize( m_pScroll->GetWidth(), GetHeight() );
	m_pScroll->SetPos( GetWidth() - m_pScroll->GetWidth()+5 , 0 );
	m_pScroll->Init();
	OnScrollChange();

	MEMO_LINEINFO_LIST::iterator linePos;	
	for( linePos = m_LineList.begin(); linePos != m_LineList.end(); linePos++ )
	{
		SAFE_DELETE( (*linePos) );		
	}
	m_LineList.clear();

	// 自动滚屏
	AutoScroll();
}

void CRichMemo::AddText( const char szTitle[], const char szText[], BYTE byColorType, BYTE byFontType )
{
	// 判断是否超出显示范围
	if( m_LineList.size() >= m_sMaxLine )
	{
		// 删除第一个行的显示内容
		MEMO_LINEINFO_LIST::iterator pos = m_LineList.begin();
		if( pos != m_LineList.end() )
		{
			SAFE_DELETE( (*pos) );
			m_LineList.erase( pos );
		}
	}

	DWORD dwColor = GetColor( byColorType );
	ParseScript( szTitle, m_dwTitleColor, m_byTitleFont, (USHORT)m_ShowRect.left, 
		(USHORT)(_nWidth - m_ShowRect.right - m_ShowRect.left) );
	ParseScript( szText, dwColor, byFontType, (USHORT)m_ShowRect.left + 2, 
		(USHORT)(_nWidth - m_ShowRect.right - m_ShowRect.left) );
	
	// 自动滚屏
	AutoScroll();
}

DWORD CRichMemo::GetColor( BYTE byType )
{
	switch( byType )
	{
	case TEXT_COLOR_WIGHT: // 白色
		return 0xFFFFFFFF;
		break;
	case TEXT_COLOR_BLACK: // 黑色
		return 0xFF000000;
		break;
	case TEXT_COLOR_RED: // 红色
		return 0xFFFF0000;
		break;
	case TEXT_COLOR_GREEN: // 绿色
		return 0xFF00FF00;
		break;
	case TEXT_COLOR_BLUE: // 蓝色
		return 0xFF0000FF;
		break;
	case TEXT_COLOR_GRAY: // 灰色
		return 0xFF808080;
		break;
	case TEXT_COLOR_PURPLE: // 紫色
		return 0xFFFF00FF;
		break;
	default:
		return 0xFF233413; // 随机色
		break;
	}
}

CGraph* CRichMemo::GetImage( const char szImage[] )
{
	static CGraph* g_pErrorImage = NULL;
	if( !g_pErrorImage )
	{
		g_pErrorImage = new CGraph();
		g_pErrorImage->GetImage()->LoadImage( "./texture/ui/system/error.tga" );
	}
	return g_pErrorImage;
}

void CRichMemo::ParseScript( const char szInfo[], DWORD dwDefColor, BYTE byFont, USHORT sStartCom, USHORT sMaxCom )
{
	char szData[3];
	memset( szData, 0, 3 );

	// 获取字体尺寸
	int nWidth, nHeight;
	CGuiFont::s_Font.GetSize( byFont, "a", nWidth, nHeight );

	string strDesp = "";
	USHORT sCurHeight = (USHORT)nHeight;
	USHORT sCom = sStartCom;
	USHORT sxPos = sCom;
	DWORD dwColor = dwDefColor;
	const char* pszTemp = szInfo;

	MEMO_LINE_INFO* pLineInfo = new MEMO_LINE_INFO;
	if( pLineInfo == NULL ) return;

	while( pszTemp[0] )
	{
		szData[0] = 0;
		szData[1] = 0;
		
		if( pszTemp[0] == '<' )
		{
			if( strDesp.length() > 0 )
			{
				// 添加数据
				MEMO_COLINFO Info;
				Info.strDesp = strDesp;
				Info.dwColor = dwColor;
				Info.sxPos = sxPos;
				Info.byType = COL_TEXT;
				Info.pIcon = NULL;
				pLineInfo->ColInfoArray.push_back( Info );

				sxPos = sCom;
				strDesp = "";
			}

			pszTemp++;
			if( pszTemp[0] == 'p' )
			{
				dwColor = 0xFFFF00FF; // 紫色
				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 'j' )
			{
				dwColor = 0xFFFE0000;
				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 'r' )
			{
				dwColor = 0xFFFF0000;
				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 'y' )
			{
				dwColor = 0xFFFFFF00;
				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 'b' )
			{
				dwColor = 0xFF0000FF;
				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 'g' )
			{
				dwColor = 0xFF808080;
				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 'n' )
			{
				pLineInfo->byFontType = byFont;
				pLineInfo->sFontHeight = (USHORT)nHeight;
				pLineInfo->sFontWidth  = (USHORT)nWidth;
				pLineInfo->sHeight = sCurHeight;
				m_LineList.push_back( pLineInfo );

				pLineInfo = new MEMO_LINE_INFO;
				if( pLineInfo == NULL ) return;

				// 换行
				sCom = sStartCom;
				sxPos = sCom;
				sCurHeight = (USHORT)nHeight;
				strDesp = "";

				if( pszTemp[1] == '>' )
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == 't' )
			{
				sCom += 4*(USHORT)nWidth;
				sxPos = sCom;
				if( sCom >= sMaxCom )
				{
					pLineInfo->byFontType = byFont;
					pLineInfo->sFontHeight = (USHORT)nHeight;
					pLineInfo->sFontWidth  = (USHORT)nWidth;
					pLineInfo->sHeight = sCurHeight;
					m_LineList.push_back( pLineInfo );
					pLineInfo = new MEMO_LINE_INFO;
					if( pLineInfo == NULL ) return;

					// 换行
					sCom = sStartCom;
					sxPos = sCom;
					sCurHeight = (USHORT)nHeight;
					strDesp = "";
				}
				if( pszTemp[1] == '>' ) 
				{
					pszTemp += 2;
					continue;
				}
			}
			else if( pszTemp[0] == '/' )
			{
				if( pszTemp[1] == '>' )
				{
					pszTemp++;
					continue;
				}

				char* pszEnd = strstr( pszTemp, ">" );
				if( pszEnd == NULL )
				{
					continue;
				}
				char szIcon[32] = "";
				strncpy( szIcon, pszTemp + 1, pszEnd - pszTemp - 1 );
				pszTemp = pszEnd;
				// 获取图标
				CGraph* pIcon = m_pImageList->GetImage( szIcon );
				if( !pIcon )
				{
					continue;
				}
				else
				{
					// 判断是否改行能够显示的下该图，否则换行显示
					if( sCom + pIcon->GetWidth() > sMaxCom )
					{
						pLineInfo->byFontType = byFont;
						pLineInfo->sFontHeight = (USHORT)nHeight;
						pLineInfo->sFontWidth  = (USHORT)nWidth;
						pLineInfo->sHeight = sCurHeight;
						m_LineList.push_back( pLineInfo );
						pLineInfo = new MEMO_LINE_INFO;
						if( pLineInfo == NULL ) return;

						sCom = sStartCom;
						sxPos = sCom;
						sCurHeight = (USHORT)nHeight;
						strDesp = "";
					}						

					// 调整改行显示高度
					if( sCurHeight < pIcon->GetHeight() + 4 )
					{
						sCurHeight = pIcon->GetHeight() + 4;
					}

					MEMO_COLINFO Info;
					memset( &Info, 0, sizeof(MEMO_COLINFO) );
					Info.byType = COL_ICON;
					Info.strDesp = "";
					Info.pIcon = pIcon;
					Info.sxPos = sxPos;
					pLineInfo->ColInfoArray.push_back( Info );
										
					sCom += pIcon->GetWidth();
					sxPos = sCom;

					// 判断图形是否太大，下次显示是否需要换行
					if( sCom >= sMaxCom )
					{
						pLineInfo->byFontType = byFont;
						pLineInfo->sFontHeight = (USHORT)nHeight;
						pLineInfo->sFontWidth  = (USHORT)nWidth;
						pLineInfo->sHeight = sCurHeight;
						m_LineList.push_back( pLineInfo );
						pLineInfo = new MEMO_LINE_INFO;
						if( pLineInfo == NULL ) return;

						sCom = sStartCom;
						sxPos = sCom;
						sCurHeight = (USHORT)nHeight;
						strDesp = "";
					}		
				}
			}
			else
			{
				// 未知类型报错！
				continue;
			}

			// 使用新的字符记录
			pszTemp++;
			continue;
		}
		else if( pszTemp[0] == '>' )
		{
			// 使用新的字符记录
			// 添加数据
			MEMO_COLINFO Info;
			Info.strDesp = strDesp;
			Info.dwColor = dwColor;
			Info.sxPos = sxPos;
			Info.byType = COL_TEXT;
			Info.pIcon = NULL;
			pLineInfo->ColInfoArray.push_back( Info );

			dwColor = dwDefColor; // 恢复字体为黑色
			pszTemp++;
			sxPos = sCom;
			strDesp = "";
			continue;
		}

		// 判断是否一个GBK
		BOOL bFlag1 = 0x81 <= (BYTE)pszTemp[0] && (BYTE)pszTemp[0] <= 0xFE;
		BOOL bFlag2 = (0x40 <= (BYTE)pszTemp[1] && (BYTE)pszTemp[1] <= 0x7E) || (0x7E <= (BYTE)pszTemp[1] && (BYTE)pszTemp[1] <= 0xFE);
		if( bFlag1 && bFlag2 )
		{
			if( sMaxCom - sCom >= 2 )
			{
				szData[0] = pszTemp[0];
				szData[1] = pszTemp[1];
				strDesp += szData;
				
				// 移动列和指向下一个字符
				sCom += (USHORT)nWidth*2;
				pszTemp += 2;
			}
			else
			{
				MEMO_COLINFO Info;
				memset( &Info, 0, sizeof(MEMO_COLINFO) );
				Info.byType = COL_TEXT;
				Info.strDesp = strDesp;
				Info.pIcon = NULL;
				Info.sxPos = sxPos;
				Info.dwColor = dwColor;
				pLineInfo->ColInfoArray.push_back( Info );

				pLineInfo->byFontType = byFont;
				pLineInfo->sFontHeight = (USHORT)nHeight;
				pLineInfo->sFontWidth  = (USHORT)nWidth;
				pLineInfo->sHeight = sCurHeight;
				m_LineList.push_back( pLineInfo );
				pLineInfo = new MEMO_LINE_INFO;
				if( pLineInfo == NULL ) return;

				// 换行
				sCom = sStartCom;
				sxPos = sCom;
				sCurHeight = (USHORT)nHeight;
				strDesp = "";
			}
		}
		else
		{
			if( sMaxCom - sCom >= 1 )
			{
				szData[0] = pszTemp[0];
				strDesp += szData;
				
				// 移动列和指向下一个字符
				sCom += (USHORT)nWidth;
				pszTemp++;
			}
			else
			{
				MEMO_COLINFO Info;
				memset( &Info, 0, sizeof(MEMO_COLINFO) );
				Info.byType = COL_TEXT;
				Info.strDesp = strDesp;
				Info.pIcon = NULL;
				Info.sxPos = sxPos;
				Info.dwColor = dwColor;
				pLineInfo->ColInfoArray.push_back( Info );

				pLineInfo->byFontType = byFont;
				pLineInfo->sFontHeight = (USHORT)nHeight;
				pLineInfo->sFontWidth  = (USHORT)nWidth;
				pLineInfo->sHeight = sCurHeight;
				m_LineList.push_back( pLineInfo );
				pLineInfo = new MEMO_LINE_INFO;
				if( pLineInfo == NULL ) return;

				// 换行
				sCom = sStartCom;
				sxPos = sCom;
				sCurHeight = (USHORT)nHeight;
				strDesp = "";
			}
		}
	}

	if( strDesp.length() > 0 )
	{
		MEMO_COLINFO Info;
		memset( &Info, 0, sizeof(MEMO_COLINFO) );
		Info.byType = COL_TEXT;
		Info.strDesp = strDesp;
		Info.pIcon = NULL;
		Info.sxPos = sxPos;
		Info.dwColor = dwColor;
		pLineInfo->ColInfoArray.push_back( Info );

		pLineInfo->byFontType = byFont;
		pLineInfo->sFontHeight = (USHORT)nHeight;
		pLineInfo->sFontWidth  = (USHORT)nWidth;
		pLineInfo->sHeight = sCurHeight;
		m_LineList.push_back( pLineInfo );		
	}
	else if( pLineInfo )
	{
		pLineInfo->byFontType = byFont;
		pLineInfo->sFontHeight = (USHORT)nHeight;
		pLineInfo->sFontWidth  = (USHORT)nWidth;
		pLineInfo->sHeight = sCurHeight;
		m_LineList.push_back( pLineInfo );
	}
	else if( pLineInfo && pLineInfo->ColInfoArray.size() <= 0 )
	{
		//delete pLineInfo;
		SAFE_DELETE(pLineInfo); // UI当机处理
	}
}

//------------------------------------------------------------------------
//	Class CImageData
//------------------------------------------------------------------------
vector<CFaceImage::sImage*> CFaceImage::m_pData;
CFaceImage::CFaceImage()
{
}

CFaceImage::~CFaceImage()
{
	sImage* pNode;
	vector <sImage*>::iterator Iter;
	while (m_pData.size()>0)
	{
		Iter=m_pData.begin();
		pNode=*Iter;
		
		//delete pNode;
		SAFE_DELETE(pNode); // UI当机处理
		
		m_pData.erase(Iter);
	}
}

CGraph* CFaceImage::GetImage( const char* szImage )
{
	int nIndex=atoi(szImage);
	if (nIndex>=0 && nIndex<g_TextParse.GetFaceCount())
	{
		return g_TextParse.GetFace(nIndex);
	}
	vector <sImage*>::iterator Iter;
	for (Iter=m_pData.begin();Iter!=m_pData.end();Iter++)
	{
		if ((*Iter)->name==szImage)
		{
			return (*Iter)->pImage;
		}
	}
	return NULL;
}

bool CFaceImage::AddImage( const char* szImage, CGraph* pImage )
{
	if (!szImage || !pImage) return false;
	sImage *pNewImage=new sImage;
	pNewImage->name=szImage;
	pNewImage->pImage=pImage;
	m_pData.push_back(pNewImage);
	return true;
}
