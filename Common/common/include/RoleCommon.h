// RoleCommon.h Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------
#pragma once

#ifndef _ROLECOMMON_H_
#define _ROLECOMMON_H_

//---------------------------------------------------------

// 
#define ROLE_MAXNUM_DESCPAGE				12		// ������Ϣ���ҳ��
#define ROLE_MAXNUM_FUNCPAGE				4		// ����������Ϣ���ҳ��
#define ROLE_MAXNUM_FLAGSIZE				16		// ��ǳߴ磨��λ1�ֽ�)��ִ��128��
#define ROLE_MAXNUM_RECORDSIZE				256     // ��ʷ�����Ǽ�¼�ߴ磨��λ1�ֽڣ�����1024��
#define ROLE_MAXSIZE_DBMISSION				2048 	// ���ݿⱣ�����������ֽڿռ�
#define ROLE_MAXSIZE_DBTRIGGER				2048	// ���ݿⱣ�����񴥷��������ֽڳߴ�
#define ROLE_MAXSIZE_DBMISCOUNT				512		// ���ݿⱣ��������ɼ����ֽڳߴ�
#define ROLE_MAXSIZE_DBRECORD				1024	// ���ݿⱣ��������ʷ��¼�����ֽڳߴ�
#define ROLE_MAXNUM_INDEXSIZE				8		// ÿ���ֽڽ���8�������¼����
#define ROLE_MAXNUM_FUNCITEM				8		// ÿ������ҳ�����8��������
#define ROLE_MAXNUM_DESPSIZE				1024    // ���Ի�ҳ������Ϣ�ֽڳ���
#define ROLE_MAXNUM_NEEDDESPSIZE			256     // ���Ի�ҳ����������Ϣ�ֽڳ���
#define ROLE_MAXNUM_FUNCITEMSIZE			64      // �����ҳÿ����Ŀ������Ϣ�ֽڳ���
#define ROLE_MAXNUM_CAPACITY				64		// ÿ��npc���Я���������������ߴ�
#define ROLE_MAXNUM_TRADEITEM				120     // ÿ��npc���Я������������Ʒ�����ߴ�
#define ROLE_MAXNUM_TRADEDATA				18      // ��ɫ����ʱÿ����ɫ���Է��õ���Ʒ�����λ
#define ROLE_MAXNUM_ITEMTRADE				99		// ÿ�������һ�����׶ѵ���Ʒ������
#define ROLE_MAXNUM_CHARTRIGGER				64      // ÿ����ɫ���Я�������񴥷�������
#define ROLE_MAXNUM_NPCTRIGGER				4		// ÿ��NPC���Я�������񴥷�������
#define ROLE_MAXNUM_FLAG					16		// ÿ����ɫ����Я��20������
#define ROLE_MAXNUM_MISSIONSTATE			32		// ÿ��NPC��ɫ�ɴ��������������
#define ROLE_MAXNUM_MISSION_STEP			16		// ÿ��NPC����ÿ������������
#define ROLE_MAXNUM_RANDMISSION				10		// ÿ����ɫ������Я��4���������
#define ROLE_MAXNUM_INSIDE_NPCCOUNT			24		// ��ɫͬ����Ұ���������NPCE����
#define ROLE_MAXSIZE_TRADEDATA				2000	// ÿ��������ͬʱ����������������Ľ���
#define ROLE_MAXSIZE_STALLDATA				2000	// ÿ��������ͬʱ����������������İ�̯����
#define ROLE_MAXSIZE_MSGPROC				16		// npc��Ϣ���������Ƴ���
#define ROLE_MAXVALUE_PARAM					16      // ÿ�����Я����������ֵ
#define ROLE_MAXNUM_MISNEED					8		// ����������������
#define ROLE_MAXNUM_MISPRIZE				8		// �������Ľ�������
#define ROLE_MAXSIZE_MISNAME				32		// �����������ߴ�
#define ROLE_MAXNUM_FORGE					12		// �����������Ϣ����
#define ROLE_MAXNUM_MISSION					10		// ÿ����ɫ���Խ������10������
#define ROLE_MAXNUM_RAND_DATA				4		// �����������ĸ�ִ��������Ϣ
#define ROLE_MAXNUM_MISSIONCOUNT			32		// ���������ɴ�����¼�������
#define ROLE_MAXNUM_EUDEMON					4		// ��Ϸ����������ػ�������
#define ROLE_MAXNUM_MAPNPC					512		// ÿ����ͼ���NPC����
#define ROLE_MAXNUM_STALL_GOODS				24		// ����̯���׻���
#define ROLE_MAXNUM_STALL_NUM				64		// ����̯�����ֽڳ���
#define ROLE_MAXSIZE_PASSWORD2				32		// ����������󳤶�

// npc�Ի���Ϣ
#define ROLE_CLOSE_DESC						"������û�з����������Խ��ܵ�����"

// �Ի���Ϣ����
#define ROLE_FIRSTPAGE						0		// ������ζԻ���Ϣ
#define ROLE_CLOSEPAGE						-1		// ����رնԻ�ҳ����

// npc������Ϣ
#define ROLE_TRADE_SALE						0		// ���������Ʒ
#define ROLE_TRADE_BUY						1		// ��������Ʒ
#define ROLE_TRADE_SALE_GOODS				2		// ������ۻ�����Ʒ
#define ROLE_TRADE_BUY_GOODS				3		// �����������Ʒ
#define ROLE_TRADE_SELECT_BOAT				4		// ѡ���׵Ĵ�ֻ

// ����ʱ����Ϣ����
#define ROLE_TRADE_START					1		// ���׿�ʼ
#define ROLE_MAXNUM_TRADETIME				50000	// �����ʱ��
#define ROLE_MAXSIZE_TRADEDIST				80*80	// ����׾���

//npc����״̬��Ϣ����
#define ROLE_MIS_ACCEPT						1<<0	// �з����������Խӵ�����
#define ROLE_MIS_DELIVERY					1<<1	// ����ɿɽ���������
#define ROLE_MIS_PENDING					1<<2	// δ��ɲ��ɽ���������
#define ROLE_MIS_IGNORE						1<<3	// �������������Խӵ�����

// ����Ի���Ϣ����
#define ROLE_MIS_PREV						0		// ���������б����һҳ��Ϣ
#define ROLE_MIS_NEXT						1		// ���������б����һҳ��Ϣ
#define ROLE_MIS_PREV_END					2		// �����б�û����һҳ��Ϣ
#define ROLE_MIS_NEXT_END					3		// �����б�û����һҳ��Ϣ
#define ROLE_MIS_SEL						4		// ����ѡ�������б���Ŀ
#define ROLE_MIS_TALK						5		// ��������Ի���Ϣ
#define ROLE_MIS_BTNACCEPT					6		// �����������
#define ROLE_MIS_BTNDELIVERY				7		// ���󽻸�����
#define ROLE_MIS_BTNPENDING					8		// δ���������󣨿ͻ��˽�ֹ��ť��
#define ROLE_MIS_LOG						9		// ����������־��Ϣ

#define ROLE_MIS_PENDING_FLAG				0		// �������ڽ�����
#define ROLE_MIS_COMPLETE_FLAG				1		// �����Ѿ��������״̬���
#define ROLE_MIS_FAILURE_FALG				2		// ����ʧ��

#define	EM_OK								0		// ��ȷ
#define EM_FAILER							-1		// ����

// ��ɫ������Ϣ�洢ͷ��ʶ�Ͱ汾�ţ�����ʶ������洢��Ϣ
#define ROLE_MIS_TRIGGER_HEADER				DWORD(*(LPDWORD)"trig")
#define ROLE_MIS_MISINFO_HEADER				DWORD(*(LPDWORD)"misn")

#define ROLE_MIS_RECORD_EDITION				0x0003	// ����ϵͳ��ʷ��Ǽ�¼	
#define ROLE_MIS_TRIGGER_EDITION			0x0003	// ����ϵͳ��ǰ�������洢�汾��
#define ROLE_MIS_MISINFO_EDITION			0x0003	// ����ϵͳ��ǰ������Ϣ�洢�汾��
#define ROLE_MIS_MISCOUNT_EDITION			0x0003  // ���������ɼ�����Ϣ�洢�汾��

namespace mission
{
	// ���ײ�������
	enum TRADE_OPTYPE	
	{ 
		TRADE_SALE				= 0,     // ������Ʒ����
		TRADE_BUY				= 1,     // ������Ʒ����
		TRADE_GOODS				= 2,	 // ���״��ջ���
		TRADE_DRAGTO_ITEM		= 3,     // �϶���Ʒ�ӵ�������������
		TRADE_DRAGTO_TRADE		= 4,     // �϶���Ʒ�ӽ���������Ʒ��
		TRADE_DRAGMONEY_ITEM	= 5,	 // �϶���Ǯ�ӵ�������������
		TRADE_DRAGMONEY_TRADE	= 6,	 // �϶���Ǯ�ӽ���������Ʒ��
		TRADE_SUCCESS			= 7,	 // ��ǰ���׳ɹ�
		TRADE_FAILER			= 8,	 // ��ǰ����ʧ��
	};

	// ���׵���Ʒ����
	enum TRADE_ITEMTYPE 
	{ 
		TI_WEAPON 					= 0,	// ��������
		TI_DEFENCE 					= 1,	// ��������
		TI_OTHER    				= 2,	// ��������
		TI_SYNTHESIS 				= 3,	// �ϳ���Ʒ����
		MAXTRADE_ITEMTYPE			= 4,
	};

	// ��ɫ��������
	enum TRADE_CHAR_TYPE
	{
		TRADE_CHAR					= 0,	// ��ɫ֮�佻��
		TRADE_BOAT					= 1,	// ��ֻ֮�佻��
	};

	// ʱ�䴥����ʱ������
	enum TRIGGER_TIME_TYPE
	{
		TT_CYCLETIME	= 0,			// ʱ��ѭ��
		TT_MULTITIME	= 1,			// ѭ������n��
	};

	// �������¼�����
	enum TRIGGER_EVENT 
	{
		TE_MAP_INIT		= 0,			// ��ɫ����		����Ϸ�����������������洢��
		TE_NPC			= 1,			// npcЯ��		������Ҫ�������洢��
		TE_KILL			= 2,			// �ݻ����		����ɫ�������洢������1���ݻ������� ����2��ָ���������ID, ����3���ݻ������������
		TE_GAME_TIME	= 3,			// ��Ϸʱ��		����ɫ�������洢������1��ʱ�����ͣ� ����2�����д����� ����3��ʱ����������4��ʱ�����
		TE_CHAT			= 4,			// ����ؼ���	����ɫ�������洢������1����ɫ��������ؼ��ִ�������������2������ؼ���ID�� ����3����Ч�����������
		TE_GET_ITEM		= 5,			// �����Ʒ		����ɫ�������洢������1����Ʒ������ ����2����ƷID�� ����3��ʰȡ��Ʒ����
		TE_EQUIP_ITEM	= 6,			// װ����Ʒ		����ɫ�������洢������1����ƷID�� ����2�������� ����3��������
		TE_GOTO_MAP     = 7,			// ����Ŀ���	����ɫ�������洢������1����ͼID�� ����2����ͼ����x������3, ��ͼ����y, ����4�����귶Χ������5���Ƿ������رմ�����
		TE_LEVEL_UP     = 8,			// ����			����ɫ�������洢������1���Ƿ������رգ��������ر����ͱ������еȼ�������ִ�ж����������δ�����������رգ�������2��ָ���ȼ�����
		TE_MAXNUM_TYPE,					// ����¼�������
	};

	// ��������
	enum MIS_TYPE
	{
		MIS_TYPE_NOMAL  = 0,			// ��ͨ��������
		MIS_TYPE_RAND	= 1,			// �����������
		MIS_TYPE_WORLD	= 2,			// ������������
	};

	// ������ʾ��������
	enum MIS_SHOW_TYPE
	{
		MIS_ALLWAYS_SHOW	= 0, 	 	// ������ʾ����״̬
		MIS_COMPLETE_SHOW	= 1, 		// �����������ʱ����ʾ����״̬����ֻ�н�ɫ���������������ɫ�Żῴ������
	};

	// ������������
	enum MIS_NEED_TYPE
	{
		MIS_NEED_ITEM		= 0,		// ��Ҫ�����Ʒ (����1����ƷID������2������)
		MIS_NEED_KILL		= 1,		// ��Ҫ�ݻ���� (����1���������ID������2������)
		MIS_NEED_SEND		= 2,        // ��Ҫ�͸�ĳ�� (����1��npcid)
		MIS_NEED_CONVOY		= 3, 		// ��Ҫ���͵�ĳ�� (����1����ͼID������2��x���꣬����3��y����)
		MIS_NEED_EXPLORE	= 4,		// ��Ҫ̽��ĳ�� (����1����ͼID������2��x���꣬����3��y����)
		MIS_NEED_DESP		= 5,		// ���ֱ���������Ŀ��
	};

	// ����������
	enum MIS_PRIZE_TYPE
	{
		MIS_PRIZE_ITEM		= 0,		// ������Ʒ (����1����ƷID������2������)
		MIS_PRIZE_MONEY		= 1,		// ������Ǯ (����1������)
		MIS_PRIZE_FAME		= 2,		// �������� (����1������ֵ)
		MIS_PRIZE_CESS		= 3,		// ����ó��˰�� (����1��˰��ֵ)
	};

	// ��������������
	enum MIS_PRIZE_SELTYPE
	{
		PRIZE_SELONE        = 0,		// ��ѡһ
		PRZIE_SELALL        = 1,		// ȫѡ
	};

	// �����������
	enum MIS_RAND_TYPE
	{
		MIS_RAND_KILL		  = 0,   	// �ݻ���� (����1���������ID������2������)
		MIS_RAND_GET		  = 1,		// ��ȡ��Ʒ (����1����ƷID������2������)
		MIS_RAND_SEND		  = 2,		// ����Ʒ   (����1����ƷID������2������)
		MIS_RAND_CONVOY		  = 3,		// ����NPC  (����1����ɫ����ID������2������)
		MIS_RAND_EXPLORE	  = 4,		// ̽����ͼ (����1����ͼID������2��x���꣬����3��y����)
	};

	// ��������
	enum MIS_EXP_TYPE
	{
		MIS_EXP_NOMAL 			= 0,	// ������ͨ����
		MIS_EXP_SAIL			= 1,	// ������������
		MIS_EXP_LIFE			= 2,	// ���������
	};

	// ���ְ�����Ϣ����
	enum MIS_HELP_TYPE
	{
		MIS_HELP_DESP			= 0,	// ���ְ�����Ϣ
		MIS_HELP_IMAGE			= 1,	// ͼ�ΰ�����Ϣ
		MIS_HELP_SOUND			= 2,	// ����������Ϣ
		MIS_HELP_BICKER			= 3,	// ����������Ϣ
	};

	// ���������ʾ���Ͷ���
	enum MIS_TREENODE_TYPE
	{
		MIS_TREENODE_INVALID		= 0,	// ��Ч����
		MIS_TREENODE_NORMAL			= 1,	// ��ͨ����
		MIS_TREENODE_HISTORY		= 2,	// ��ʷ����
		MIS_TREENODE_GUILD			= 3,	// ��������
	};

	enum ENTITY_TYPE
	{
		BASE_ENTITY					= 0,	// ����ʵ��
		RESOURCE_ENTITY				= 1,	// ��Դʵ��
		TRANSIT_ENTITY				= 2,	// ����ʵ��
		BERTH_ENTITY				= 3,	// ͣ��ʵ��
	};

	enum ENTITY_STATE
	{
		ENTITY_DISABLE				= 0,	// ʵ��δ����״̬
		ENTITY_ENABLE				= 1,	// ʵ�弤��״̬
	};

	enum ENTITY_ACTION
	{
		ENTITY_START_ACTION			= 0,	// ʵ�忪ʼ����
		ENTITY_END_ACTION			= 1,	// ʵ���������
		ENTITY_INTERMIT_ACTION		= 2,	// ʵ���ж϶���
	};

	enum GOODS_TYPE
	{
		RES_WOOD					= 0,	// ľ����Դ
		RES_MINE					= 1,	// ��ʯ��Դ
	};

	enum  BOAT_LIST_TYPE
	{
		BERTH_TRADE_LIST			= 0,	// ���״�ֻ�б�
		BERTH_LUANCH_LIST			= 1,	// ������ֻ�б�
		BERTH_BAG_LIST				= 2,	// �����ֻ�б�
		BERTH_REPAIR_LIST			= 3,	// ��ֻ�����б�
		BERTH_SALVAGE_LIST			= 4,	// ��ֻ�����б�
		BERTH_SUPPLY_LIST			= 5,	// ��ֻ�����б�
		BERTH_BOATLEVEL_LIST		= 6		// ��ֻ�����б�
	};

	enum VIEW_ITEM_TYPE
	{
		VIEW_CHAR_BAG				= 0,	// �鿴�����������Ʒ��Ϣ
		VIEW_CHARTRADE_SELF			= 1,	// �鿴�����׵�������Ʒ��Ϣ
		VIEW_CHARTRADE_OTHER		= 2,	// �鿴�Է����׵�������Ʒ��Ϣ
	};

	// ��̯���ｻ����Ʒ��Ϣ�ṹ
	typedef struct _NET_STALL_GOODS
	{
		DWORD dwMoney;
		BYTE  byCount;
		BYTE  byIndex;
		BYTE  byGrid;

	} NET_STALL_GOODS, *PNET_STALL_GOODS;

	typedef struct _NET_STALL_ALLDATA
	{
		BYTE byNum;
		NET_STALL_GOODS Info[ROLE_MAXNUM_STALL_GOODS];

	} NET_STALL_ALLDATA, *PNET_STALL_ALLDATA;
}

#endif // _ROLECOMMON_H_

//---------------------------------------------------------