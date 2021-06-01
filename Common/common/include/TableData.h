#pragma once

// Raw Data : ԭʼ����
// Raw Data Set : ������ʵ������ֻ��һ��, ʹ�ø����ݵ�ʵ��ȴ���Դ��������Ӧ�ó���
// ���� : Meshģ������, ��ͼ����, �Ǽ����ݵȵ�,  Ҳ�������ڷ���Ϸ�ĸ��������

// RawDataSet�����Ҫ����
//1. ԭʼ���ݵ���Դ��������(�ı�,������)
//2. ͨ��ID��������
//3. ��̬�ͷ�
//4  ��Դ����Ͱ����ݴ�ȡ

// Ŀǰ��Ӧ�û�ֻ��ΪһЩ��Դ������ʽ��Ϊ�򵥵�����������ӿ�
// ����:  ID  ������Դ(�ļ���) �򵥲������� ����ĸ�ʽ

// ������ʽ:
// ����ID = �����±�
// ͨ������ID������ԭʼ����


// ������ʹ��ǰ, ����̳����µķ���
// virtual int				_GetRawDataInfoSize()										      // ÿ�������RawDataInfo���в�ͬ, ȡ��RawDataInfo�����ݳߴ�
// virtual void*			_CreateNewRawData(CRawDataInfo *pRawInfo)		    		      // ȡ���µ�RawData����, ����ģ������ָ�룬Ҳ��������ͼ����ָ��
// virtual void				_ReadRawDataInfo(CRawDataInfo *pRawInfo, list<string> &ParamList) // ��Դ�ļ�ÿ�����µ�һ��, ������Եõ��Ļص�����
// virtual void				_DeleteRawData(void *pData);								      // ɾ����Դ, ��Դ��ɾ����ʽ����������в�ͬ	

// ����, ����������Լ��Ĺ��������_Init()����


#include <fstream>


class CRawDataInfo
{
public:	
	
	CRawDataInfo()
	:bExist(FALSE),
	dwLastUseTick(0),
	bEnable(TRUE),
	pData(NULL),
	dwPackOffset(0),
	dwDataSize(0),
	nIndex(0),
    dwLoadCnt(0)
	{
		strcpy(szDataName, "");	
	}
	
    BOOL	bExist;				// ��Դ�Ƿ����
	int		nIndex;				// ��Array�е�λ��				
	char	szDataName[72];		// ������Դ(ͨ���������ļ���)
	DWORD	dwLastUseTick;		// �ϴ�ʹ�õ�ʱ��
	BOOL	bEnable;			// �Ƿ���Ч, ���Զ�̬����
	void*   pData;				// ʵ������
	DWORD   dwPackOffset;		// �ڰ��ļ��е�����ƫ��
	DWORD   dwDataSize;			// ԭʼ���ݳߴ�(�ļ��ߴ�)
	int     nID;				// ID
    DWORD   dwLoadCnt;          // ��Դ��ȡ����
};



class CRawDataSet
{

protected:

	CRawDataSet(int nIDStart, int nIDCnt, int nFieldCnt = DEFAULT_FIELD_CNT) // һ��Ҫ�̳�ʹ��
	:_nIDStart(nIDStart),
	_nIDCnt(nIDCnt),
	_bEnablePack(FALSE),
	_bBinary(FALSE),
	_nUnusedIndex(0),
	_RawDataArray(NULL),
    _nIDLast(nIDCnt)
	{
		_dwReleaseInterval  = 1000 * 60 * 1;		// Ĭ��1����û��ʹ�õ��Ļᱻ���
		// _dwReleaseInterval  = 1000 * 30;			// Ĭ��30��û��ʹ�õ��Ļᱻ���
		_nMaxRawDataCnt          = 50;				// Ĭ���ڴ��г���50��RawDataʱ, �ᰴʱ���ͷ�û��ʹ�õ�RawData
		_nLoadedRawDataCnt       = 0;
        _dwMaxFrameRawDataSize   = 0;                // ����Frame��������ߴ�, 0��ʾ������
        _bEnableRequest          = FALSE;

		// ����
		_nMaxFieldCnt = (nFieldCnt > DEFAULT_FIELD_CNT) ?
				nFieldCnt : DEFAULT_FIELD_CNT;
   	}

public:

    void*			GetRawData(int nID, BOOL bRequest = FALSE);
	void*			GetRawData(const char* pszDataName, int *pnID);
	CRawDataInfo*	GetRawDataInfo(int nID);
    CRawDataInfo*   GetRawDataInfo(const char *pszDataName);
    int             GetRawDataID(const char *pszDataName);
	
	BOOL			LoadRawDataInfo(const char *pszFileName, BOOL bBinary = FALSE);
	BOOL			LoadRawDataInfoEx(const char *pszFileName, BOOL bBinary = FALSE);
	void			WriteBinFile(const char *pszFileName);
	
	BOOL			IsValidID(int nID);
    int             GetLastID() const {return _nIDLast;}
	
	// ���ڶ�̬�ͷŵĲ�������
    void			SetReleaseInterval(DWORD dwInterval)	{ _dwReleaseInterval = dwInterval;	}
	void			SetMaxRawData(int nDataCnt)				{ _nMaxRawDataCnt	 = nDataCnt;	}
	
	int				GetLoadedRawDataCnt()					{ return _nLoadedRawDataCnt;		}
	void			DynamicRelease(BOOL bClearAll = FALSE);
	void			Release();
    void            FrameLoad(int nFrameLoad = 2);

	// ����й�
    void			EnablePack(const char *pszPackName);	// ���ڶ����������Դ�����ļ�����Ч
	void            Pack(const char *pszPackName, const char *pszBinName);
    void			PackFromDirectory(list<string> &DirList, const char *pszPackName, const char *pszBinName);
    BOOL            IsEnablePack()              { return _bEnablePack; } 
	
    // ��Դ��ȡ
    LPBYTE			LoadRawFileData(CRawDataInfo *pInfo);
	
    void            EnableRequest(BOOL bEnable)   { _bEnableRequest = bEnable; }
protected:

	virtual CRawDataInfo*	_CreateRawDataArray(int nIDCnt)								    = 0;	
	virtual void			_DeleteRawDataArray()										    = 0;	
	virtual int             _GetRawDataInfoSize()										    = 0;
	virtual void*			_CreateNewRawData(CRawDataInfo *pInfo)					  	    = 0;
	virtual BOOL			_ReadRawDataInfo(CRawDataInfo *pInfo, vector<string> &ParamList)= 0;
	virtual void			_ProcessRawDataInfo(CRawDataInfo *pInfo)                        {}
	virtual void			_DeleteRawData(CRawDataInfo *pInfo)							    = 0;
	virtual BOOL			_IsFull()
	{
		if(_nLoadedRawDataCnt <= _nMaxRawDataCnt) return FALSE;
		return TRUE;
	}
	virtual void            _AfterLoad(){}

	BOOL		_LoadRawDataInfo_Bin(const char *pszFileName);
	BOOL		_LoadRawDataInfo_Txt(const char *pszFileName, int nSep = '\t');
	void		_WriteRawDataInfo_Bin(const char *pszFileName);
    void        _Init();
    CRawDataInfo*	_GetRawDataInfo(int nID); // ����������Χ���

protected:
	
	int						    _nIDStart;
	int						    _nIDCnt;
	int						    _nUnusedIndex;
	DWORD					    _dwReleaseInterval;
	int						    _nMaxRawDataCnt;
	int						    _nLoadedRawDataCnt;
    DWORD                       _dwMaxFrameRawDataSize;
	BOOL				    	_bEnablePack;
	char                        _szPackName[64];
	BOOL						_bBinary;
    CRawDataInfo*			    _RawDataArray;
    map<string, CRawDataInfo*>	_IDIdx;
    list<int>                   _RequestList;
    BOOL                        _bEnableRequest;
    int                         _nIDLast;

	// add by claude
	enum {DEFAULT_FIELD_CNT = 80};
	int _nMaxFieldCnt;
};

inline void CRawDataSet::_Init()
{
    _DeleteRawDataArray();

	_RawDataArray = _CreateRawDataArray(_nIDCnt);

	LPBYTE pbtData = (LPBYTE)_RawDataArray;
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * i);
		pInfo->nIndex = i;
		pInfo->nID    = _nIDStart + i;
	}
}

inline CRawDataInfo* CRawDataSet::GetRawDataInfo(int nID)
{
	if (!IsValidID(nID)) return NULL;

    CRawDataInfo* pInfo = _GetRawDataInfo(nID);
	if (pInfo->bExist) return pInfo;
    else return NULL;
}

// ����������Χ��⣬��Ҫ��������
inline CRawDataInfo* CRawDataSet::_GetRawDataInfo(int nID)
{
    LPBYTE pbtData = (LPBYTE)_RawDataArray;

    CRawDataInfo *pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * (nID - _nIDStart));
    return pInfo;
}

inline CRawDataInfo* CRawDataSet::GetRawDataInfo(const char *pszDataName)
{
	map<string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // ��ID�Ѿ�����
	{
		return (*it).second;
    }
    return NULL;
}

inline void* CRawDataSet::GetRawData(int nID, BOOL bRequest)
{
	CRawDataInfo *pInfo =  GetRawDataInfo(nID);
	if(pInfo==NULL) return NULL;
	
	pInfo->dwLastUseTick = GetTickCount();
	if(!pInfo->pData)
	{
	    if(bRequest && _bEnableRequest)
        {
            LG2("debug", "Push Request RawData!\n");
            _RequestList.push_back(nID);
            return NULL;
        }
        
        pInfo->pData = _CreateNewRawData(pInfo);
        pInfo->dwLoadCnt++;
		if(pInfo->pData==NULL)
		{
			LG2("error", "Load Raw Data [%s] Failed! (ID = %d)\n", pInfo->szDataName, nID);
		}
		else
		{
			_nLoadedRawDataCnt++;
		}
	}
	return pInfo->pData;
}

inline int CRawDataSet::GetRawDataID(const char *pszDataName) // �����ֻ�ȡID, ���û�������һ��
{
	CRawDataInfo *pInfo;

    map<string, CRawDataInfo*>::iterator it = _IDIdx.find(pszDataName);

	if(it!=_IDIdx.end()) // ��ID�Ѿ�����
	{
		pInfo = (*it).second;
	}
	else
	{
		if(_nUnusedIndex >= _nIDCnt)
		{
			LG2("error", "RawDataSet OverMax Dynamic ID, MaxIDCnt = %d, Index = %d\n", _nIDCnt, _nUnusedIndex);
			return -1;
		}
		
		LPBYTE pbtData = (LPBYTE)_RawDataArray;
		pInfo = (CRawDataInfo*)(pbtData + _GetRawDataInfoSize() * _nUnusedIndex);
		strcpy(pInfo->szDataName, pszDataName);

		_IDIdx[pInfo->szDataName] = pInfo;
		_nUnusedIndex++;
	}
	return pInfo->nIndex + _nIDStart;
}


inline void* CRawDataSet::GetRawData(const char* pszDataName, int *pnID)
{
	int nID = GetRawDataID(pszDataName);
    if(pnID) *pnID = nID;
    if(nID==-1)
    {
        return NULL;
    }
    return GetRawData(nID);
}

inline BOOL CRawDataSet::IsValidID(int nID)
{
	if(nID < _nIDStart || nID >= (_nIDStart + _nIDCnt)) return FALSE;
	return TRUE;
}

extern BOOL  g_bBinaryTable;

inline BOOL CRawDataSet::LoadRawDataInfo(const char *pszFile, BOOL bBinary)
{
	char szTxtName[255], szBinName[255];

	if(g_bBinaryTable) bBinary = TRUE;
	
	_bBinary  = bBinary;

	sprintf(szTxtName, "%s.txt", pszFile);
	sprintf(szBinName, "%s.bin", pszFile);

    BOOL bRet = FALSE;
	if(bBinary) 
	{
		bRet = _LoadRawDataInfo_Bin(szBinName);
	}
	else
	{
		bRet = _LoadRawDataInfo_Txt(szTxtName);
		if(bRet)
		{
			_WriteRawDataInfo_Bin(szBinName);
		}
	}

    try {
        _AfterLoad();
    } catch (...) {}

    return bRet;
}

inline BOOL CRawDataSet::LoadRawDataInfoEx(const char *pszFile, BOOL bBinary)
{
	char szTxtName[255], szBinName[255];

	_bBinary  = bBinary;

	sprintf(szTxtName, "%s.txt", pszFile);
	sprintf(szBinName, "%s.bin", pszFile);
	if(bBinary) 
	{
		return _LoadRawDataInfo_Bin(szBinName);
	}
	else
	{
		BOOL bLoad = _LoadRawDataInfo_Txt(szTxtName);
		if(bLoad)
		{
			_WriteRawDataInfo_Bin(szBinName);
		}
		return bLoad;
	}
	return TRUE;
}

inline void CRawDataSet::FrameLoad(int nFrameLoad)
{
    int nMaxLoadPerFrame = nFrameLoad;

    list<int>::iterator it;
    list<int> FinishList;
    int n = 0;
    for(it = _RequestList.begin(); it!=_RequestList.end(); it++)
    {
        int nID = (*it);
        GetRawData(nID, FALSE);
        FinishList.push_back(nID);
        n++;
        if(n > nFrameLoad) break;
    }
    
    for(it = FinishList.begin(); it!=FinishList.end(); it++)
    {
        int nID = (*it);
        _RequestList.remove(nID);
    }
}

inline void CRawDataSet::DynamicRelease(BOOL bClearAll)
{
	if(bClearAll)
    {
        for(int i = 0; i < _nIDCnt; i++)
	    {
		    CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		    if(pInfo->pData==NULL)  continue;
            _DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
			    LG2("error", "LoadedRawDataCnt = %d , < 0 ?\n", _nLoadedRawDataCnt);
			}
      	}
        return;
    }
    
    if(_IsFull()==FALSE) return;
	
	DWORD dwCurTick = GetTickCount();

	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
		if(pInfo->pData==NULL)  continue;
		
		if((dwCurTick - pInfo->dwLastUseTick) > _dwReleaseInterval)
		{
			_DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
				LG2("error", "LoadedRawDataCnt = %d , < 0 ?\n", _nLoadedRawDataCnt);
			}
			// LG2("debug", "Dynamic Release Raw Data [%s]\n", pInfo->szDataName);
		}
	}

    
}

inline void CRawDataSet::Release()
{
	if( _nLoadedRawDataCnt > 0 ) //��ȫ�ͷ��ڴ� by Waiting 2009-06-18
	{
		for(int i = 0; i < _nIDCnt; i++)
		{
			CRawDataInfo *pInfo = GetRawDataInfo(_nIDStart + i);
			if( NULL==pInfo || NULL==pInfo->pData )  //��ȫ�ͷ��ڴ� by Waiting 2009-06-18
				continue;

			_DeleteRawData(pInfo);
			pInfo->pData = NULL;
			_nLoadedRawDataCnt--;
			if(_nLoadedRawDataCnt < 0)
			{
				LG2("error", "LoadedRawDataCnt = %d , < 0 ?\n", _nLoadedRawDataCnt);
			}
		}
	}
    //��ȫ�ͷ��ڴ� by Waiting 2009-06-18
	if( _RawDataArray )
	{
		_DeleteRawDataArray();
		_RawDataArray = NULL;
	}
	_nUnusedIndex = 0;
}

inline BOOL CRawDataSet::_LoadRawDataInfo_Bin(const char *pszFileName)
{
	FILE* fp = fopen(pszFileName, "rb");
	char szMsg[MAX_PATH] = {0};

	if(fp==NULL) 
	{
		LG2("error", "Load Raw Data Info Bin File [%s] Failed!\n", pszFileName);
		//sprintf(szMsg, "�򿪱���ļ�ʧ�ܣ�%s\n���򼴽��˳�!\n", pszFileName);
		//MessageBox(NULL, szMsg, "����", MB_OK | MB_ICONERROR);
		sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
		MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
    int nSize     = Util_GetFileSize(fp);
    int nInfoSize = _GetRawDataInfoSize();
    int nResCnt   = nSize / nInfoSize;

    LPBYTE pbtResInfo = new BYTE[nSize];
    
	DWORD dwInfoSize = 0; fread(&dwInfoSize, 4, 1, fp);

	if(dwInfoSize!=_GetRawDataInfoSize())
	{
		//sprintf(szMsg, "dwInfoSize: %d\n_GetRawDataInfoSize: %d!\n", dwInfoSize, _GetRawDataInfoSize());
		//MessageBox(NULL, szMsg, "Error2", MB_OK | MB_ICONERROR);
		//LG2("table", "msg��ȡ����ļ�[%s]ʱ, ���ְ汾��һ��!\n", pszFileName);
		LG2("table", "msg read table file [%s], version can't match!\n", pszFileName);
		fclose(fp);
		//sprintf(szMsg, "��ȡ����ļ�����%s\n���򼴽��˳�!\n", pszFileName);
		//MessageBox(NULL, szMsg, "����", MB_OK | MB_ICONERROR);
		sprintf(szMsg, "Open table file failed:%s\nProgram will exit!\n", pszFileName);
		MessageBox(NULL, szMsg, "Error", MB_OK | MB_ICONERROR);
		exit(0);
		return FALSE;
	}
	
	fread(pbtResInfo, nSize, 1, fp);
	
    for(int i = 0; i < nResCnt; i++)
    {
        CRawDataInfo *pInfo = (CRawDataInfo*)(pbtResInfo + i * _GetRawDataInfoSize());

		// modify by lark.li 20080424 begin
		//strcpy(pInfo->szDataName, ConvertResString(pInfo->szDataName));
		// End

        if(!pInfo->bExist) continue;
		if(IsValidID(i)==FALSE) continue;
		CRawDataInfo* pCurInfo = _GetRawDataInfo(pInfo->nID);
		memcpy(pCurInfo, pInfo, nInfoSize); // ���ԭ�е���Ϣ
         _IDIdx[pCurInfo->szDataName] = pCurInfo;
        //vector<string> ParamList; _ReadRawDataInfo(pCurInfo, ParamList);
        _ProcessRawDataInfo(pCurInfo);
		LG2("debug", "Load Bin RawData [%s] = %d\n", pCurInfo->szDataName, pCurInfo->nID);
    }
    
    delete pbtResInfo;
    
    fclose(fp);
	return TRUE;
}

inline void CRawDataSet::_WriteRawDataInfo_Bin(const char *pszFileName)
{
	FILE* fp = fopen(pszFileName, "wb");
	if(fp==NULL) return;
	
	DWORD dwInfoSize = _GetRawDataInfoSize();

	fwrite(&dwInfoSize, 4, 1, fp);
	for(int i = 0; i < _nIDCnt; i++)
    {
        CRawDataInfo *pInfo = (CRawDataInfo*)((LPBYTE)_RawDataArray + i * _GetRawDataInfoSize());
        if(pInfo->bExist)
        {
            fwrite(pInfo, _GetRawDataInfoSize(), 1, fp);
        }
    }
	fclose(fp);
}


inline BOOL CRawDataSet::_LoadRawDataInfo_Txt(const char *pszFileName, int nSep)
{
	BOOL bRet = TRUE;
	ifstream in(pszFileName);
    if(in.is_open()==0)
    {
       // LG2("error", "msgLoad Raw Data Info Txt File [%s] Fail!\n", pszFileName);
        return FALSE;
    }
	
const int LINE_SIZE = 2048;
	char szLine[LINE_SIZE];
	string* pstrList = new string[_nMaxFieldCnt + 1];
	string strComment;

	vector<string> ParamList;

	// add by claude at 2004-9-1
	BOOL bSaveFieldCnt = FALSE;
	int nFieldCnt = 0;

    
    while(!in.eof())
    {
		in.getline(szLine, LINE_SIZE);
		string strLine = szLine;
		
		int p = (int)strLine.find("//");
		if(p!=-1)
		{
			string strLeft = strLine.substr(0, p);
			strComment = strLine.substr(p + 2, strLine.size() - p - 2);
			strLine = strLeft;
		}
		else
		{
			strComment = "";
		}
		
		int n = Util_ResolveTextLine(strLine.c_str(), pstrList, _nMaxFieldCnt + 1, nSep);
		if (n < 2) continue;
		if (n > _nMaxFieldCnt)
			{
			//LG2("error", "msg����Դ�ļ�[%s]�У�ʵ���ֶ�������Ԥ�����ֶ���\n", pszFileName);
				LG2("error", "msg in resource [%s], the field num is greater than predefine count \n", pszFileName);
			bRet = FALSE;
            break;}

		// ��¼�������ֶ���Ŀ
		if (!bSaveFieldCnt)
			{
			nFieldCnt = n;
            bSaveFieldCnt = TRUE;}
		else {
			// �Ƚϴ����ֶ���Ŀ���һ���ֶ���Ŀ�Ƿ���ͬ
			if (nFieldCnt != n)
				{
				// �����ͬ��˵������Դ�ļ��������ݴ���
				//LG2("error", "msg������Դ�ļ�[%s]ʧ��,���[%s], �����ʽ�Ͱ汾!\n",
					LG2("error", "msg parse resource file [%s] failed ,No [%s], please chech format and version!\n",
                    pszFileName, pstrList[0].c_str());

				bRet = FALSE;
                break;}
			}

        int	nID = Str2Int(pstrList[0]);
        if (!IsValidID(nID))
            {
            //LG2("error", "msg����[%d]����Ԥ����Χ��������Դ�ļ�[%s]\n", nID, pszFileName);
				LG2("error", "msg index [%d] overflow,please check resource file [%s]\n", nID, pszFileName);
            bRet = FALSE;
            break;}

        _nIDLast = nID;

        CRawDataInfo *pInfo = _GetRawDataInfo(nID);
		pInfo->bExist = TRUE;
		
        ParamList.clear();
		int i;
        for(i = 0; i < n - 2; i++)
        {
			ParamList.push_back(pstrList[i + 2]);
		}
		for(i = 0; i < 15; i++)
			ParamList.push_back(""); // ���ÿմ�,���������������, ��ʾ��ʽ����

		//Util_TrimString(pstrList[1]);
		Util_TrimTabString(pstrList[1]); // ����Ӣ�� MAKEBIN �ո�ʧ����  modify by Philip.Wu  2006-07-31

		strcpy(pInfo->szDataName, pstrList[1].c_str());
		// char *pszDataName = _strupr( _strdup( pInfo->szDataName ) );
		
		char *pszDataName = ( _strdup( pInfo->szDataName ) );
		strcpy(pInfo->szDataName, pszDataName);
        free(pszDataName);

		_IDIdx[pInfo->szDataName] = pInfo;

        BOOL bRet = FALSE;
        // try 
		{
			bRet = _ReadRawDataInfo(pInfo, ParamList);
			_ProcessRawDataInfo(pInfo);
		}
        //catch (...)
        {
		//	LG2("error", "msg������Դ�ļ�[%s]����δ֪���쳣,����ʧ�ܣ����[%s], �����ʽ!\n", pszFileName, pstrList[0].c_str());
		//	bRet = FALSE; break;
    	}        
        if(!bRet)
        {
            //LG2("error", "msg������Դ�ļ�[%s]ʧ��,���[%s], �����ʽ�Ͱ汾!\n",
			LG2("error", "msg parse resource file [%s] failed ,No [%s], please chech format and version!\n",
                pszFileName, pstrList[0].c_str());
			bRet = FALSE; break;
        }
	}

	delete[] pstrList;
	in.close();
	return bRet;
}


//----------------------------------------------------------------------------------------------------------
//												�����ش���
//----------------------------------------------------------------------------------------------------------
inline LPBYTE Util_LoadFile(const char *pszFileName, DWORD* pdwFileSize)
{
	if(strlen(pszFileName)==0) return NULL;
	FILE *fp = fopen(pszFileName, "rb");
	if(fp==NULL)
	{
		pdwFileSize = 0;
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	LPBYTE pbtBuf = new BYTE[lSize];
	fread(pbtBuf, lSize, 1, fp);
	fclose(fp);
	*pdwFileSize = lSize;
	return pbtBuf;
}

inline LPBYTE Util_LoadFilePart(const char *pszFileName, DWORD dwStart, DWORD dwSize)
{
	FILE *fp = fopen(pszFileName, "rb");
	if(fp==NULL)
	{
		return NULL;
	}
	LPBYTE pbtBuf = new BYTE[dwSize];
	fseek(fp, dwStart, SEEK_SET);
	fread(pbtBuf, dwSize, 1, fp);
	fclose(fp);
	return pbtBuf;
}


inline void CRawDataSet::Pack(const char *pszPackName, const char *pszBinName)
{
	FILE *fp = fopen(pszPackName, "wb");
	for(int i = 0; i < _nIDCnt; i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(i);
		if(!pInfo->bExist)  continue;
		DWORD  dwFileSize = 0;
		LPBYTE pbtFileContent = Util_LoadFile(pInfo->szDataName, &dwFileSize);
		if(pbtFileContent)
		{
            pInfo->dwPackOffset = ftell(fp);
			pInfo->dwDataSize   = dwFileSize;
			fwrite(pbtFileContent, dwFileSize, 1, fp);
			delete pbtFileContent;
		}
	}
	fclose(fp);
	
	_WriteRawDataInfo_Bin(pszBinName); // ���֮����дRawDataSet Bin�ļ�
}


//--------------------------------------------
//  ��Ŀ¼�ж�ȡ�ļ�, ÿ���ļ���Ϊһ����Դ, ��
//  ������Դ������Ϣ�ļ� xxx.bin
//--------------------------------------------
inline void	CRawDataSet::PackFromDirectory(list<string> &DirList, const char *pszPackName, const char *pszBinName)
{
	list<string> FileList;
	for(list<string>::iterator itD = DirList.begin(); itD!=DirList.end(); itD++)
	{
		string strDirName = (*itD);
		ProcessDirectory(strDirName.c_str(), &FileList, DIRECTORY_OP_QUERY);
	}
	
	int nFileCnt = (int)(FileList.size());
	
	FILE *fp = fopen(pszPackName, "wb");
		
	int i = 0;
	for(list<string>::iterator it = FileList.begin(); it!=FileList.end(); it++,i++)
	{
		CRawDataInfo *pInfo = GetRawDataInfo(i + _nIDStart);
		
		strcpy(pInfo->szDataName, (*it).c_str());
		
		char *pszDataName = _strlwr( _strdup( pInfo->szDataName ) );
		strcpy(pInfo->szDataName, pszDataName);
		free(pszDataName);
		
		DWORD  dwFileSize = 0;
		LPBYTE pbtFileContent = Util_LoadFile(pInfo->szDataName, &dwFileSize);
		if(pbtFileContent)
		{
		    pInfo->bExist       = TRUE;
            pInfo->dwPackOffset = ftell(fp);
			pInfo->dwDataSize   = dwFileSize;
			fwrite(pbtFileContent, dwFileSize, 1, fp);
			delete pbtFileContent;
		}
        LG2("debug", "Pack File (index = %d) ID = %d [%s]\n", pInfo->nIndex, pInfo->nID, pInfo->szDataName);
	}
	
	fclose(fp);
	
	_WriteRawDataInfo_Bin(pszBinName); // ���֮����дRawDataSet Bin�ļ�
}	

inline void CRawDataSet::EnablePack(const char *pszPackName)
{
	if(pszPackName)
    {
        _bEnablePack = TRUE;
        strcpy(_szPackName, pszPackName);
    }
    else
    {
        _bEnablePack = FALSE;
    }
}
	


//-----------------------------------------------------------------------------
// ��ȡRawData���ݵ��ڴ�(һ�����ڰ������ļ�����, ��Ӱ���ȡ�������ļ����ݵĳ���
// ������������Ҫ��ʹ�ô˺���, Ҳ������ȫ����)
//-----------------------------------------------------------------------------
inline LPBYTE CRawDataSet::LoadRawFileData(CRawDataInfo *pInfo)
{
	LPBYTE pbtBuf    = NULL;
	DWORD  dwBufSize = 0;
	if(_bEnablePack) // �Ӱ��ж�ȡ
	{
		pbtBuf    = Util_LoadFilePart(_szPackName, pInfo->dwPackOffset, pInfo->dwDataSize);
		dwBufSize = pInfo->dwDataSize;
	}
	else
	{
		pbtBuf    = Util_LoadFile(pInfo->szDataName, &dwBufSize);
		pInfo->dwDataSize = dwBufSize;
	}
	return pbtBuf;
}