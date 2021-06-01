//
#pragma once

#include <tchar.h>
#include "util.h"
#include "TableData.h"

// ����������ťֵ
enum ID_BTN_BOAT
{
    ID_BTN_HEADLEFT =      0,
    ID_BTN_HEADRIGHT,
    ID_BTN_POWERLEFT,
    ID_BTN_POWERRIGHT,
    ID_BTN_WEAPONLEFT,
    ID_BTN_WEAPONRIGHT,

    ID_BTN_ITEM_NUM,
};
#define BOAT_MAXSIZE_MINNAME	2   // ��ֻ���������ַ�����
#define BOAT_MAXSIZE_BOATNAME	17	// ��ֻ��������󳤶�
#define BOAT_MAXSIZE_NAME		64	// ��ֻ����������󳤶�
#define BOAT_MAXSIZE_DESP		128 // ��ֻ������Ϣ��������ֽ���
#define BOAT_MAXSIZE_PART		3   // ��ֻ��󲿼�����
#define BOAT_MAXNUM_PARTITEM	16  // ��ֻÿ���ɻ���������ѡ������
#define BOAT_MAXNUM_MOTOR		4	// ÿ������������������������
#define BOAT_MAXNUM_MODEL		8	// ÿ����ֻ���ģ������
#define MAX_CHAR_BOAT			3	// ÿ����ɫ������ӵ�еĴ�ֻ����

// ��ֻѡ���б�����
struct BOAT_BERTH_DATA
{
	BYTE byID[MAX_CHAR_BOAT];
	BYTE byState[MAX_CHAR_BOAT];
	char szName[MAX_CHAR_BOAT][BOAT_MAXSIZE_BOATNAME + BOAT_MAXSIZE_DESP];
};

enum BOAT_STATE
{
	BS_GOOD		= 0,				// ״̬����
	BS_NOSP		= 1,				// ��Ҫ����SP
	BS_NOHP		= 2,				// ��Ҫ����
	BS_DEAD		= 3,				// ��ֻ��û
};

// ��ɫ���졢����ӵ�еĴ�ֻ
struct BOAT_DATA
{
	char szName[BOAT_MAXSIZE_BOATNAME];		// ��ֻ����
	USHORT sBoat;				// ��ֻ��ϢID
	USHORT sHeader;				// ��ͷ
	USHORT sBody;				// ����
	USHORT sEngine;				// ��ֻ���
	USHORT sCannon;				// ��ֻ����
	USHORT sEquipment;			// ��ֻװ��
	USHORT sBerth;				// ��ֻͣ���ۿ�
	USHORT sCapacity;			// ��������
	DWORD  dwOwnerID;			// ��ֻӵ����ID
};

// ��ɫװ�ش�ֻ����
struct BOAT_LOAD_INFO
{
	DWORD  dwID;				// ��ֻ���ݿ�ID
	char   szName[BOAT_MAXSIZE_BOATNAME];			// ��ֻ����
	USHORT sHeader;				// ��ͷ
	USHORT sBody;				// ����
	USHORT sEngine;				// ��ֻ���
	USHORT sCannon;				// ��ֻ����
	USHORT sEquipment;			// ��ֻװ��
	USHORT sBerth;				// ��ֻͣ���ۿ�
};

struct xShipBuildInfo
{
	char szName[BOAT_MAXSIZE_NAME];			// ����
	char szDesp[BOAT_MAXSIZE_DESP];			// ���������ߵȼ���
	char szBerth[BOAT_MAXSIZE_NAME];		// ��ֻͣ���ۿ�����
	char szHeader[BOAT_MAXSIZE_NAME];		// ��ͷ����
	char szBody[BOAT_MAXSIZE_NAME];			// ��������
	char szEngine[BOAT_MAXSIZE_NAME];		// ���������
	char szCannon[BOAT_MAXSIZE_NAME];		// ��װ�ػ�������
	char szEquipment[BOAT_MAXSIZE_NAME];	// ���������

	// ��ֻ���ģ��ID	
	USHORT sPosID;				// ģ�Ͷ�����ϢID
	union
	{
		struct
		{
			DWORD dwBody;		// ����		
			DWORD dwHeader;		// ��ͷ	
			DWORD dwEngine;		// ��ֻ����
			DWORD dwFlag;		// ����־
			DWORD dwMotor[BOAT_MAXNUM_MOTOR];
		};

		DWORD dwBuf[BOAT_MAXNUM_MODEL];
	};

	BYTE  byHeader;	   // ��ͷ��ѡ��������¼
	BYTE  byEngine;	   // ��������ѡ��������¼
	BYTE  byCannon;	   // �����ڵ�ѡ��������¼
	BYTE  byEquipment; // �������ѡ��������¼
	BYTE  byIsUpdate;  // �Ƿ���Ը��´�ֻ����
	DWORD dwMoney;	   // ��ֵ
    DWORD dwMinAttack; // ��С����
    DWORD dwMaxAttack; // ��󹥻�
    DWORD dwCurEndure; // ��ǰ�;�
    DWORD dwMaxEndure; // ����;�
    DWORD dwSpeed;	   // �ƶ��ٶ�
    DWORD dwDistance;  // ��󹥻�����
    DWORD dwDefence;   // ����
	DWORD dwCurSupply; // ��ǰ����
	DWORD dwMaxSupply; // ��󲹸�
	DWORD dwConsume;   // ��ֻ��������
    DWORD dwAttackTime;// �������ʱ��
	USHORT	sCapacity; // ��������
};

struct xShipAttrInfo
{
	// ��ֻ�����������
	DWORD dwMoney;	   // ��ֵ
    DWORD dwMinAttack; // ��С����
    DWORD dwMaxAttack; // ��󹥻�
    DWORD dwCurEndure; // ��ǰ�;�
    DWORD dwMaxEndure; // ����;�
    DWORD dwSpeed;	   // �ƶ��ٶ�
    DWORD dwDistance;  // ��󹥻�����
    DWORD dwDefence;   // ����
	DWORD dwCurSupply; // ��ǰ����
	DWORD dwMaxSupply; // ��󲹸�
    DWORD dwAttackTime;// �������ʱ��
	USHORT	sCapacity; // ��������

	// ��������	
	DWORD dwResume;			// ���;ûָ�
	DWORD dwResist;	        // ��ֻ����
	DWORD dwScope;			// ��ը��Χ
	DWORD dwConsume;		// ��ֻ��������
	DWORD dwCannonSpeed;	// �ڵ������ٶ�

	// ��ֻ����ϵ��
};

struct xShipInfo: public CRawDataInfo
{
	char szName[BOAT_MAXSIZE_NAME];	// ��ֻ����
	char szDesp[BOAT_MAXSIZE_DESP];	// ���������ߵȼ���
	USHORT sItemID;					// ����֤������ID
	USHORT sCharID;					// ��ֻ��ɫ����ID
	USHORT sPosID;					// ģ�Ͷ�����ϢID
	BYTE byIsUpdate;				// �Ƿ���Ը�����������	
	USHORT sNumHeader;
	USHORT sNumEngine;
	USHORT sNumCannon;
	USHORT sNumEquipment;
	USHORT sHeader[BOAT_MAXNUM_PARTITEM];		// ��ͷ	
	USHORT sEngine[BOAT_MAXNUM_PARTITEM];		// ����
	USHORT sCannon[BOAT_MAXNUM_PARTITEM];		// ����
	USHORT sEquipment[BOAT_MAXNUM_PARTITEM];	// ���
	USHORT sBody;			// ����	
	USHORT sLvLimit;		// �ȼ�����
	USHORT sNumPfLimit;
	USHORT sPfLimit[BOAT_MAXNUM_PARTITEM];		// ְҵ����

	USHORT sEndure;			// ��ֻ�;�
	USHORT sResume;			// ���;ûָ�
	USHORT sDefence;		// ��ֻ������
	USHORT sResist;			// ��ֻ����
    USHORT sMinAttack;		// ��С����
    USHORT sMaxAttack;		// ��󹥻�
	USHORT sDistance;		// ��������
	USHORT sTime;			// ��ʱ��
	USHORT sScope;			// ��ը��Χ
	USHORT sCapacity;		// ��������
	USHORT sSupply;			// ��ֻ����
	USHORT sConsume;		// ��ֻ��������
	USHORT sCannonSpeed;	// �ڵ������ٶ�
	USHORT sSpeed;			// ���ƶ��ٶ�
	USHORT sParam;			// ��ע

	xShipInfo()
	{
		memset( szName, 0, BOAT_MAXSIZE_NAME );
		memset( szDesp, 0, BOAT_MAXSIZE_DESP );
		sItemID = 0;
		sCharID = 0;
		sPosID = 0;
		byIsUpdate = 0;
		sNumHeader = 0;
		sNumEngine = 0;
		sNumCannon = 0;
		sNumEquipment = 0;
		memset( sHeader, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sEngine, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sCannon, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sEquipment, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		sBody = 0;
		sLvLimit = 0;		// �ȼ�����
		sNumPfLimit = 0;
		memset( sPfLimit, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );	


		sEndure = 0;
		sResume = 0;
		sDefence = 0;
		sResist = 0;
		sMinAttack = 0;
		sMaxAttack = 0;
		sDistance = 0;
		sTime = 0;
		sScope = 0;
		sCapacity = 0;
		sSupply = 0;			
		sConsume = 0;		
		sCannonSpeed = 0;	
		sSpeed = 0;
		sParam = 0;
	}
};

struct xShipPartInfo: public CRawDataInfo
{
	char szName[BOAT_MAXSIZE_NAME];	// ��ֻ��������
	char szDesp[BOAT_MAXSIZE_DESP];	// ���������ߵȼ���
    DWORD dwModel;			// ����ģ��ID
	USHORT sMotor[BOAT_MAXNUM_MOTOR];	// ��ֻ�������
	DWORD  dwPrice;			// �۸�

	USHORT sEndure;			// ��ֻ�;�
	USHORT sResume;			// ���;ûָ�
	USHORT sDefence;		// ��ֻ������
	USHORT sResist;			// ��ֻ����
    USHORT sMinAttack;		// ��С����
    USHORT sMaxAttack;		// ��󹥻�
	USHORT sDistance;		// ��������
	USHORT sTime;			// ��ʱ��
	USHORT sScope;			// ��ը��Χ
	USHORT sCapacity;		// ��������
	USHORT sSupply;			// ��ֻ����
	USHORT sConsume;		// ��ֻ��������
	USHORT sCannonSpeed;	// �ڵ������ٶ�
	USHORT sSpeed;			// ���ƶ��ٶ�
	USHORT sParam;			// ��ע
	
	xShipPartInfo()
	{
		memset( szName, 0, BOAT_MAXSIZE_NAME );
		memset( szDesp, 0, BOAT_MAXSIZE_DESP );
		dwModel = 0;
		memset( sMotor, 0, sizeof(USHORT)*BOAT_MAXNUM_MOTOR );
		dwPrice = 0;
		sEndure = 0;
		sResume = 0;
		sDefence = 0;
		sResist = 0;
		sMinAttack = 0;
		sMaxAttack = 0;
		sDistance = 0;
		sTime = 0;
		sScope = 0;
		sCapacity = 0;
		sSupply = 0;			
		sConsume = 0;		
		sCannonSpeed = 0;	
		sSpeed = 0;
		sParam = 0;
	}
};

// ��ֻ�������Ա�
class xShipSet : public CRawDataSet
{
public:
	xShipSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}
	
	static xShipSet* I() { return _Instance; }

protected:

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return (CRawDataInfo*)new xShipInfo[nCnt];
	}
	
	static xShipSet* _Instance; // �൱�ڵ���, ���Լ���ס

	virtual void _DeleteRawDataArray()
	{
		delete[] (xShipInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(xShipInfo);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList);
};

// ��ֻ�����������Ա�
class xShipPartSet : public CRawDataSet
{
public:
	
	static xShipPartSet* I() { return _Instance; }

	
	xShipPartSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:

	static xShipPartSet* _Instance;
	
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return (CRawDataInfo*)new xShipPartInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (xShipPartInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(xShipPartInfo);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList);
};

inline xShipInfo* GetShipInfo( int nTypeID )
{
	return (xShipInfo*)xShipSet::I()->GetRawDataInfo(nTypeID);
}

inline xShipPartInfo* GetShipPartInfo( int nTypeID )
{
	return (xShipPartInfo*)xShipPartSet::I()->GetRawDataInfo(nTypeID);
}
