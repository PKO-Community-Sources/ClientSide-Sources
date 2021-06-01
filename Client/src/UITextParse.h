#pragma once

#include "UIFont.h"

namespace GUI
{

class CGraph;

class CTextParse
{
public :
	struct node //�ڵ�
	{
	    string    str; //�ڵ�����
	    node    * next; //ָ����һ���ڵ��ָ��
	};

public:
	CTextParse(void);
	~CTextParse(void);

	void Render( string _str, int x, int y, DWORD color, ALLIGN allign=eAlignTop, int height=0);
	void RenderEx( string _str, int x, int y, DWORD color, float scale );
	
	void RenderPic ( string _str, int x, int y ) ;

	bool Init();
	int  InitEx(string _str);
	int  InitLink();


	
	bool Clear();

	void SetBoxOff( char cBoxOff )	{ _cBoxOff = cBoxOff;		}
	char GetBoxOff()				{ return _cBoxOff;			}

	void			SetCaption( const char * str ) {  _str = str;};
	const char *	GetCaption()		{              return _str.c_str();		}

	// ���ñ����Ӧ��������
	void AddFace( int nIndex, CGraph* pGraph );
	CGraph* GetFace( DWORD nIndex );
	int GetFaceCount()	{ return (int)_files.size();}

private:
	string   _str ;    
private:
	struct stFaceIndex
	{
		int		nIndex;
		CGraph  *pGraph;
	};
	typedef vector<stFaceIndex> files;

	char	_cBoxOff;		// �ָ���,Ĭ��Ϊ'#'
	files	_files;
	int     _scaleX;
	int     _scaleY;

};

extern CTextParse	g_TextParse;



//�㷨 
//��ȡһ���ַ�����ǰ��n���ַ����ߺ����n���ַ�
//flag ==true ,���ʾ��ȡǰ����ַ� 
inline string  GetSelfString(string str , int n , bool flag )
{	
	string strReturn;
	const char* s =  str.c_str() ;

	if ( (int)strlen(s) <=n )
	{
		strReturn = s ;
		return strReturn ;
	}
	
	int i ;
	if ( flag )
	{
		i=0;
		while ( i< n )
		{
			if  (s[i] & 0x80)
				i+=2;
			else
				i+=1;
		}
		strReturn  = str.substr( 0 , i );

	}
	else 
	{
		i=0;
		int len = (int) str.length() -1 ;
		while ( i< n	)
		{			
			if  (s[len-i] & 0x80)
				i+=2;
			else 
				i +=1;
		}
		strReturn  = str.substr( len +1- i   , i );
	}

	return strReturn ;
}


}
