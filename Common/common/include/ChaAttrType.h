//=============================================================================
// FileName: ChaAttrType.h
// Creater: ZhangXuedong
// Date: 2004.12.13
// Comment: Character Attribute type
//=============================================================================

#ifndef ATTRTYPE_H
#define ATTRTYPE_H

const long	ATTR_COUNT_BASE0    = 0;
const long	ATTR_LV             = ATTR_COUNT_BASE0 + 0; // ��ɫ�ȼ�
const long	ATTR_HP             = ATTR_COUNT_BASE0 + 1; // ��ǰHP�����ڴ�ֻ����ʾ�;ã�
const long	ATTR_SP             = ATTR_COUNT_BASE0 + 2; // ��ǰSP�����ڴ�ֻ����ʾ������
const long	ATTR_TITLE          = ATTR_COUNT_BASE0 + 3; // ��ɫ�ƺ�
const long	ATTR_JOB            = ATTR_COUNT_BASE0 + 4; // ��ɫְҵ
const long	ATTR_FAME           = ATTR_COUNT_BASE0 + 5; // ����
const long	ATTR_AP             = ATTR_COUNT_BASE0 + 6; // ���Ե�
const long	ATTR_TP             = ATTR_COUNT_BASE0 + 7; // ���ܵ�
const long	ATTR_GD             = ATTR_COUNT_BASE0 + 8; // ��Ǯ
const long	ATTR_SPRI           = ATTR_COUNT_BASE0 + 9; // �ڵ������ٶ�
const long	ATTR_CHATYPE        = ATTR_COUNT_BASE0 + 10; // ��ɫ�������ͣ���ң�NPC������ȣ�
const long	ATTR_SAILLV         = ATTR_COUNT_BASE0 + 11; // �����ȼ�
const long	ATTR_LIFELV         = ATTR_COUNT_BASE0 + 12; // ����ȼ�
const long	ATTR_LIFETP         = ATTR_COUNT_BASE0 + 13; // ����ܵ�
const long	ATTR_BOAT_BERTH     = ATTR_COUNT_BASE0 + 14; // ��ֻͣ���ۿ�
//			// ���·�Χ�ڵĸ����Ե�˳�򲻿ɸı�
const long	ATTR_CEXP           = ATTR_COUNT_BASE0 + 15; // ��ǰ���飨���ڴ�ֻ����ʾ�����ȣ�
const long	ATTR_NLEXP          = ATTR_COUNT_BASE0 + 16; // ��һ�����辭��
const long	ATTR_CLEXP          = ATTR_COUNT_BASE0 + 17; // ��ǰ�ȼ��ľ���
const long	ATTR_CLEFT_SAILEXP  = ATTR_COUNT_BASE0 + 18; // ��ǰʣ�ຽ������
const long	ATTR_CSAILEXP       = ATTR_COUNT_BASE0 + 19; // �ۻ��������飨��ǰ�������飩
const long	ATTR_CLV_SAILEXP    = ATTR_COUNT_BASE0 + 20; // ��ǰ�ȼ���������
const long	ATTR_NLV_SAILEXP    = ATTR_COUNT_BASE0 + 21; // ��һ�ȼ��ȼ���������
const long	ATTR_CLIFEEXP       = ATTR_COUNT_BASE0 + 22; // ��ǰ�����
const long	ATTR_CLV_LIFEEXP    = ATTR_COUNT_BASE0 + 23; // ��ǰ�ȼ������
const long	ATTR_NLV_LIFEEXP    = ATTR_COUNT_BASE0 + 24; // ��һ�ȼ������
//			//

const long	ATTR_COUNT_BASE1    = 25;
const long	ATTR_STR            = ATTR_COUNT_BASE1 + 0; // ����
const long	ATTR_DEX            = ATTR_COUNT_BASE1 + 1; // רע
const long	ATTR_AGI            = ATTR_COUNT_BASE1 + 2; // ����
const long	ATTR_CON            = ATTR_COUNT_BASE1 + 3; // ����
const long	ATTR_STA            = ATTR_COUNT_BASE1 + 4; // ����
const long	ATTR_LUK            = ATTR_COUNT_BASE1 + 5; // ����
const long	ATTR_MXHP           = ATTR_COUNT_BASE1 + 6; // ���HP
const long	ATTR_MXSP           = ATTR_COUNT_BASE1 + 7; // ���SP
const long	ATTR_MNATK          = ATTR_COUNT_BASE1 + 8; // ��С������
const long	ATTR_MXATK          = ATTR_COUNT_BASE1 + 9; // ��󹥻���
const long	ATTR_DEF            = ATTR_COUNT_BASE1 + 10; // ������
const long	ATTR_HIT            = ATTR_COUNT_BASE1 + 11; // ������
const long	ATTR_FLEE           = ATTR_COUNT_BASE1 + 12; // ������
const long	ATTR_MF             = ATTR_COUNT_BASE1 + 13; // Ѱ����
const long	ATTR_CRT            = ATTR_COUNT_BASE1 + 14; // ������
const long	ATTR_HREC           = ATTR_COUNT_BASE1 + 15; // hp�ָ��ٶ�
const long	ATTR_SREC           = ATTR_COUNT_BASE1 + 16; // sp�ָ��ٶ�
const long	ATTR_ASPD           = ATTR_COUNT_BASE1 + 17; // �������
const long	ATTR_ADIS           = ATTR_COUNT_BASE1 + 18; // ��������
const long	ATTR_MSPD           = ATTR_COUNT_BASE1 + 19; // �ƶ��ٶ�
const long	ATTR_COL            = ATTR_COUNT_BASE1 + 20; // ��Դ�ɼ��ٶ�
const long	ATTR_PDEF           = ATTR_COUNT_BASE1 + 21; // ����ֿ�
const long  ATTR_BOAT_CRANGE    = ATTR_COUNT_BASE1 + 22; // ��ֻ�ڵ���ը��Χ
const long  ATTR_BOAT_CSPD      = ATTR_COUNT_BASE1 + 23; // ��ֻ�ڵ������ٶ�
const long  ATTR_BOAT_PRICE     = ATTR_COUNT_BASE1 + 24; // ���ļ�ֵ

const long	ATTR_COUNT_BASE2    = 50;
const long	ATTR_BSTR           = ATTR_COUNT_BASE2 + 0; // ��������
const long	ATTR_BDEX           = ATTR_COUNT_BASE2 + 1; // ����רע
const long	ATTR_BAGI           = ATTR_COUNT_BASE2 + 2; // ��������
const long	ATTR_BCON           = ATTR_COUNT_BASE2 + 3; // ��������
const long	ATTR_BSTA           = ATTR_COUNT_BASE2 + 4; // ��������
const long	ATTR_BLUK           = ATTR_COUNT_BASE2 + 5; // ��������
const long	ATTR_BMXHP          = ATTR_COUNT_BASE2 + 6; // �������HP
const long	ATTR_BMXSP          = ATTR_COUNT_BASE2 + 7; // �������SP
const long	ATTR_BMNATK         = ATTR_COUNT_BASE2 + 8; // ������С������
const long	ATTR_BMXATK         = ATTR_COUNT_BASE2 + 9; // ������󹥻���
const long	ATTR_BDEF           = ATTR_COUNT_BASE2 + 10; // ����������
const long	ATTR_BHIT           = ATTR_COUNT_BASE2 + 11; // ����������
const long	ATTR_BFLEE          = ATTR_COUNT_BASE2 + 12; // ����������
const long	ATTR_BMF            = ATTR_COUNT_BASE2 + 13; // ����Ѱ����
const long	ATTR_BCRT           = ATTR_COUNT_BASE2 + 14; // ����������
const long	ATTR_BHREC          = ATTR_COUNT_BASE2 + 15; // ����hp�ָ��ٶ�
const long	ATTR_BSREC          = ATTR_COUNT_BASE2 + 16; // ����(��ɫsp�ָ��ٶ�)(��ֻΪ�����ٶ�)
const long	ATTR_BASPD          = ATTR_COUNT_BASE2 + 17; // �����������
const long	ATTR_BADIS          = ATTR_COUNT_BASE2 + 18; // ������������
const long	ATTR_BMSPD          = ATTR_COUNT_BASE2 + 19; // �����ƶ��ٶ�
const long	ATTR_BCOL           = ATTR_COUNT_BASE2 + 20; // ������Դ�ɼ��ٶ�
const long	ATTR_BPDEF          = ATTR_COUNT_BASE2 + 21; // ��������ֿ�
const long  ATTR_BOAT_BCRANGE   = ATTR_COUNT_BASE2 + 22; // ��ֻ�����ڵ���ը��Χ
const long  ATTR_BOAT_BCSPD     = ATTR_COUNT_BASE2 + 23; // ��ֻ�����ڵ������ٶ�

const long	ATTR_COUNT_BASE3    = 74;
const long	ATTR_ITEMC_STR      = ATTR_COUNT_BASE3 + 0; // ��������ϵ����item coefficient���ӳ�
const long	ATTR_ITEMC_AGI      = ATTR_COUNT_BASE3 + 1; // ���ݵ���ϵ���ӳ�
const long	ATTR_ITEMC_DEX      = ATTR_COUNT_BASE3 + 2; // רע����ϵ���ӳ�
const long	ATTR_ITEMC_CON      = ATTR_COUNT_BASE3 + 3; // ���ʵ���ϵ���ӳ�
const long	ATTR_ITEMC_STA      = ATTR_COUNT_BASE3 + 4; // ��������ϵ���ӳ�
const long	ATTR_ITEMC_LUK      = ATTR_COUNT_BASE3 + 5; // ���˵���ϵ���ӳ�
const long	ATTR_ITEMC_ASPD     = ATTR_COUNT_BASE3 + 6; // �����������ϵ���ӳ�
const long	ATTR_ITEMC_ADIS     = ATTR_COUNT_BASE3 + 7; // �����������ϵ���ӳ�
const long	ATTR_ITEMC_MNATK    = ATTR_COUNT_BASE3 + 8; // ��С����������ϵ���ӳ�
const long	ATTR_ITEMC_MXATK    = ATTR_COUNT_BASE3 + 9; // ��󹥻�������ϵ���ӳ�
const long	ATTR_ITEMC_DEF      = ATTR_COUNT_BASE3 + 10; // ����������ϵ���ӳ�
const long	ATTR_ITEMC_MXHP     = ATTR_COUNT_BASE3 + 11; // ���HP����ϵ���ӳ�
const long	ATTR_ITEMC_MXSP     = ATTR_COUNT_BASE3 + 12; // ���SP����ϵ���ӳ�
const long	ATTR_ITEMC_FLEE     = ATTR_COUNT_BASE3 + 13; // �����ʵ���ϵ���ӳ�
const long	ATTR_ITEMC_HIT      = ATTR_COUNT_BASE3 + 14; // �����ʵ���ϵ���ӳ�
const long	ATTR_ITEMC_CRT      = ATTR_COUNT_BASE3 + 15; // �����ʵ���ϵ���ӳ�
const long	ATTR_ITEMC_MF       = ATTR_COUNT_BASE3 + 16; // Ѱ���ʵ���ϵ���ӳ�
const long	ATTR_ITEMC_HREC     = ATTR_COUNT_BASE3 + 17; // hp�ָ��ٶȵ���ϵ���ӳ�
const long	ATTR_ITEMC_SREC     = ATTR_COUNT_BASE3 + 18; // sp�ָ��ٶȵ���ϵ���ӳ�
const long	ATTR_ITEMC_MSPD     = ATTR_COUNT_BASE3 + 19; // �ƶ��ٶȵ���ϵ���ӳ�
const long	ATTR_ITEMC_COL      = ATTR_COUNT_BASE3 + 20; // ��Դ�ɼ��ٶȵ���ϵ���ӳ�
const long	ATTR_ITEMC_PDEF     = ATTR_COUNT_BASE3 + 21; // ����ֿ�����ϵ���ӳ�

const long	ATTR_COUNT_BASE4    = 96;
const long	ATTR_ITEMV_STR      = ATTR_COUNT_BASE4 + 0; // ����������ֵ��item value���ӳ�
const long	ATTR_ITEMV_AGI      = ATTR_COUNT_BASE4 + 1; // ���ݵ�����ֵ�ӳ�
const long	ATTR_ITEMV_DEX      = ATTR_COUNT_BASE4 + 2; // רע������ֵ�ӳ�
const long	ATTR_ITEMV_CON      = ATTR_COUNT_BASE4 + 3; // ���ʵ�����ֵ�ӳ�
const long	ATTR_ITEMV_STA      = ATTR_COUNT_BASE4 + 4; // ����������ֵ�ӳ�
const long	ATTR_ITEMV_LUK      = ATTR_COUNT_BASE4 + 5; // ���˵�����ֵ�ӳ�
const long	ATTR_ITEMV_ASPD     = ATTR_COUNT_BASE4 + 6; // �������������ֵ�ӳ�
const long	ATTR_ITEMV_ADIS     = ATTR_COUNT_BASE4 + 7; // �������������ֵ�ӳ�
const long	ATTR_ITEMV_MNATK    = ATTR_COUNT_BASE4 + 8; // ��С������������ֵ�ӳ�
const long	ATTR_ITEMV_MXATK    = ATTR_COUNT_BASE4 + 9; // ��󹥻���������ֵ�ӳ�
const long	ATTR_ITEMV_DEF      = ATTR_COUNT_BASE4 + 10; // ������������ֵ�ӳ�
const long	ATTR_ITEMV_MXHP     = ATTR_COUNT_BASE4 + 11; // ���HP������ֵ�ӳ�
const long	ATTR_ITEMV_MXSP     = ATTR_COUNT_BASE4 + 12; // ���SP������ֵ�ӳ�
const long	ATTR_ITEMV_FLEE     = ATTR_COUNT_BASE4 + 13; // �����ʵ�����ֵ�ӳ�
const long	ATTR_ITEMV_HIT      = ATTR_COUNT_BASE4 + 14; // �����ʵ�����ֵ�ӳ�
const long	ATTR_ITEMV_CRT      = ATTR_COUNT_BASE4 + 15; // �����ʵ�����ֵ�ӳ�
const long	ATTR_ITEMV_MF       = ATTR_COUNT_BASE4 + 16; // Ѱ���ʵ�����ֵ�ӳ�
const long	ATTR_ITEMV_HREC     = ATTR_COUNT_BASE4 + 17; // hp�ָ��ٶȵ�����ֵ�ӳ�
const long	ATTR_ITEMV_SREC     = ATTR_COUNT_BASE4 + 18; // sp�ָ��ٶȵ�����ֵ�ӳ�
const long	ATTR_ITEMV_MSPD     = ATTR_COUNT_BASE4 + 19; // �ƶ��ٶȵ�����ֵ�ӳ�
const long	ATTR_ITEMV_COL      = ATTR_COUNT_BASE4 + 20; // ��Դ�ɼ��ٶȵ�����ֵ�ӳ�
const long	ATTR_ITEMV_PDEF     = ATTR_COUNT_BASE4 + 21; // ����ֿ�������ֵ�ӳ�

const long	ATTR_COUNT_BASE5    = 118;
const long	ATTR_STATEC_STR     = ATTR_COUNT_BASE5 + 0; // ����״̬ϵ����state coefficient���ӳ�
const long	ATTR_STATEC_AGI     = ATTR_COUNT_BASE5 + 1; // ����״̬ϵ���ӳ�
const long	ATTR_STATEC_DEX     = ATTR_COUNT_BASE5 + 2; // רע״̬ϵ���ӳ�
const long	ATTR_STATEC_CON     = ATTR_COUNT_BASE5 + 3; // ����״̬ϵ���ӳ�
const long	ATTR_STATEC_STA     = ATTR_COUNT_BASE5 + 4; // ����״̬ϵ���ӳ�
const long	ATTR_STATEC_LUK     = ATTR_COUNT_BASE5 + 5; // ����״̬ϵ���ӳ�
const long	ATTR_STATEC_ASPD    = ATTR_COUNT_BASE5 + 6; // �������״̬ϵ���ӳ�
const long	ATTR_STATEC_ADIS    = ATTR_COUNT_BASE5 + 7; // ��������״̬ϵ���ӳ�
const long	ATTR_STATEC_MNATK   = ATTR_COUNT_BASE5 + 8; // ��С������״̬ϵ���ӳ�
const long	ATTR_STATEC_MXATK   = ATTR_COUNT_BASE5 + 9; // ��󹥻���״̬ϵ���ӳ�
const long	ATTR_STATEC_DEF     = ATTR_COUNT_BASE5 + 10; // ������״̬ϵ���ӳ�
const long	ATTR_STATEC_MXHP    = ATTR_COUNT_BASE5 + 11; // ���HP״̬ϵ���ӳ�
const long	ATTR_STATEC_MXSP    = ATTR_COUNT_BASE5 + 12; // ���SP״̬ϵ���ӳ�
const long	ATTR_STATEC_FLEE    = ATTR_COUNT_BASE5 + 13; // ������״̬ϵ���ӳ�
const long	ATTR_STATEC_HIT     = ATTR_COUNT_BASE5 + 14; // ������״̬ϵ���ӳ�
const long	ATTR_STATEC_CRT     = ATTR_COUNT_BASE5 + 15; // ������״̬ϵ���ӳ�
const long	ATTR_STATEC_MF      = ATTR_COUNT_BASE5 + 16; // Ѱ����״̬ϵ���ӳ�
const long	ATTR_STATEC_HREC    = ATTR_COUNT_BASE5 + 17; // hp�ָ��ٶ�״̬ϵ���ӳ�
const long	ATTR_STATEC_SREC    = ATTR_COUNT_BASE5 + 18; // sp�ָ��ٶ�״̬ϵ���ӳ�
const long	ATTR_STATEC_MSPD    = ATTR_COUNT_BASE5 + 19; // �ƶ��ٶ�״̬ϵ���ӳ�
const long	ATTR_STATEC_COL     = ATTR_COUNT_BASE5 + 20; // ��Դ�ɼ��ٶ�״̬ϵ���ӳ�
const long	ATTR_STATEC_PDEF    = ATTR_COUNT_BASE5 + 21; // ����ֿ�״̬ϵ���ӳ�

const long	ATTR_COUNT_BASE6    = 140;
const long	ATTR_STATEV_STR     = ATTR_COUNT_BASE6 + 0; // ����״̬��ֵ��state value���ӳ�
const long	ATTR_STATEV_AGI     = ATTR_COUNT_BASE6 + 1; // ����״̬��ֵ�ӳ�
const long	ATTR_STATEV_DEX     = ATTR_COUNT_BASE6 + 2; // רע״̬��ֵ�ӳ�
const long	ATTR_STATEV_CON     = ATTR_COUNT_BASE6 + 3; // ����״̬��ֵ�ӳ�
const long	ATTR_STATEV_STA     = ATTR_COUNT_BASE6 + 4; // ����״̬��ֵ�ӳ�
const long	ATTR_STATEV_LUK     = ATTR_COUNT_BASE6 + 5; // ����״̬��ֵ�ӳ�
const long	ATTR_STATEV_ASPD    = ATTR_COUNT_BASE6 + 6; // �������״̬��ֵ�ӳ�
const long	ATTR_STATEV_ADIS    = ATTR_COUNT_BASE6 + 7; // ��������״̬��ֵ�ӳ�
const long	ATTR_STATEV_MNATK   = ATTR_COUNT_BASE6 + 8; // ��С������״̬��ֵ�ӳ�
const long	ATTR_STATEV_MXATK   = ATTR_COUNT_BASE6 + 9; // ��󹥻���״̬��ֵ�ӳ�
const long	ATTR_STATEV_DEF     = ATTR_COUNT_BASE6 + 10; // ������״̬��ֵ�ӳ�
const long	ATTR_STATEV_MXHP    = ATTR_COUNT_BASE6 + 11; // ���HP״̬��ֵ�ӳ�
const long	ATTR_STATEV_MXSP    = ATTR_COUNT_BASE6 + 12; // ���SP״̬��ֵ�ӳ�
const long	ATTR_STATEV_FLEE    = ATTR_COUNT_BASE6 + 13; // ������״̬��ֵ�ӳ�
const long	ATTR_STATEV_HIT     = ATTR_COUNT_BASE6 + 14; // ������״̬��ֵ�ӳ�
const long	ATTR_STATEV_CRT     = ATTR_COUNT_BASE6 + 15; // ������״̬��ֵ�ӳ�
const long	ATTR_STATEV_MF      = ATTR_COUNT_BASE6 + 16; // Ѱ����״̬��ֵ�ӳ�
const long	ATTR_STATEV_HREC    = ATTR_COUNT_BASE6 + 17; // hp�ָ��ٶ�״̬��ֵ�ӳ�
const long	ATTR_STATEV_SREC    = ATTR_COUNT_BASE6 + 18; // sp�ָ��ٶ�״̬��ֵ�ӳ�
const long	ATTR_STATEV_MSPD    = ATTR_COUNT_BASE6 + 19; // �ƶ��ٶ�״̬��ֵ�ӳ�
const long	ATTR_STATEV_COL     = ATTR_COUNT_BASE6 + 20; // ��Դ�ɼ��ٶ�״̬��ֵ�ӳ�
const long	ATTR_STATEV_PDEF    = ATTR_COUNT_BASE6 + 21; // ����ֿ�״̬��ֵ�ӳ�

const long	ATTR_LHAND_ITEMV    = ATTR_COUNT_BASE6 + 22; // ���ֵ��߼ӳ�

const long	ATTR_COUNT_BASE7        = 163;
const long	ATTR_BOAT_SHIP          = ATTR_COUNT_BASE7 + 0; // ��ֻID
const long	ATTR_BOAT_HEADER        = ATTR_COUNT_BASE7 + 1; // ��ͷ����
const long  ATTR_BOAT_BODY          = ATTR_COUNT_BASE7 + 2; // ��������
const long  ATTR_BOAT_ENGINE        = ATTR_COUNT_BASE7 + 3; // ����������
const long  ATTR_BOAT_CANNON        = ATTR_COUNT_BASE7 + 4; // ����������
const long	ATTR_BOAT_PART          = ATTR_COUNT_BASE7 + 5; // ���������
const long  ATTR_BOAT_DBID          = ATTR_COUNT_BASE7 + 6; // �������ݿ�洢ID
const long  ATTR_BOAT_DIECOUNT      = ATTR_COUNT_BASE7 + 7; // �ô���������
const long  ATTR_BOAT_ISDEAD	    = ATTR_COUNT_BASE7 + 8; // �ô�Ŀǰ�Ƿ�Ϊ����״̬

const long	ATTR_COUNT_BASE8        = 172;
const long	ATTR_BOAT_SKILLC_MNATK  = ATTR_COUNT_BASE8 + 0; // ��ֻMNATK ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_MXATK  = ATTR_COUNT_BASE8 + 1; // ��ֻMXATK ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_ADIS   = ATTR_COUNT_BASE8 + 2; // ��ֻ��������atkrange ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_MSPD   = ATTR_COUNT_BASE8 + 3; // ��ֻ�ƶ��ٶȼ���ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_CSPD   = ATTR_COUNT_BASE8 + 4; // �ڵ������ٶ� ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_ASPD   = ATTR_COUNT_BASE8 + 5; // ��ֻASPD ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_CRANGE = ATTR_COUNT_BASE8 + 6; // �ڵ���ը��Χ ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_DEF    = ATTR_COUNT_BASE8 + 7; // ��ֻDEF ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_RESIST = ATTR_COUNT_BASE8 + 8; // ��ֻRESIST ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_MXUSE  = ATTR_COUNT_BASE8 + 9; // ��ֻ����;� ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_USEREC = ATTR_COUNT_BASE8 + 10; // ��ֻ�;ûظ��ٶ� ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_EXP    = ATTR_COUNT_BASE8 + 11; // ��ֻ������ ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_CPT    = ATTR_COUNT_BASE8 + 12; // �������� ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_SPD    = ATTR_COUNT_BASE8 + 13; // ���� ����ϵ��Ӱ��
const long	ATTR_BOAT_SKILLC_MXSPLY = ATTR_COUNT_BASE8 + 14; // ��󲹸�ֵ ����ϵ��Ӱ��

const long	ATTR_COUNT_BASE9        = 187;
const long	ATTR_BOAT_SKILLV_MNATK  = ATTR_COUNT_BASE9 + 0; // ��ֻMNATK ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_MXATK  = ATTR_COUNT_BASE9 + 1; // ��ֻMXATK ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_ADIS   = ATTR_COUNT_BASE9 + 2; // ��ֻ��������atkrange ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_MSPD   = ATTR_COUNT_BASE9 + 3; // ��ֻ�ƶ��ٶȼ��ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_CSPD   = ATTR_COUNT_BASE9 + 4; // �ڵ������ٶ� ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_ASPD   = ATTR_COUNT_BASE9 + 5; // ��ֻASPD ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_CRANGE = ATTR_COUNT_BASE9 + 6; // �ڵ���ը��Χ ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_DEF    = ATTR_COUNT_BASE9 + 7; // ��ֻDEF ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_RESIST = ATTR_COUNT_BASE9 + 8; // ��ֻRESIST ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_MXUSE  = ATTR_COUNT_BASE9 + 9; // ��ֻ����;� ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_USEREC = ATTR_COUNT_BASE9 + 10; // ��ֻ�;ûظ��ٶ� ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_EXP    = ATTR_COUNT_BASE9 + 11; // ��ֻ������ ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_CPT    = ATTR_COUNT_BASE9 + 12; // �������� ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_SPD    = ATTR_COUNT_BASE9 + 13; // ���� ���ܳ���Ӱ��
const long	ATTR_BOAT_SKILLV_MXSPLY = ATTR_COUNT_BASE9 + 14; // ��󲹸�ֵ ���ܳ���Ӱ��

// Modify by lark.li 20080723
// ��չ����
const long	ATTR_COUNT_BASE10        = 202;
const long	ATTR_EXTEND0  = ATTR_COUNT_BASE10 + 0;
const long	ATTR_EXTEND1  = ATTR_COUNT_BASE10 + 1;
const long	ATTR_EXTEND2  = ATTR_COUNT_BASE10 + 2;
const long	ATTR_EXTEND3  = ATTR_COUNT_BASE10 + 3;
const long	ATTR_EXTEND4  = ATTR_COUNT_BASE10 + 4;
const long	ATTR_EXTEND5  = ATTR_COUNT_BASE10 + 5;
const long	ATTR_EXTEND6  = ATTR_COUNT_BASE10 + 6;
const long	ATTR_EXTEND7  = ATTR_COUNT_BASE10 + 7;
const long	ATTR_EXTEND8  = ATTR_COUNT_BASE10 + 8;
const long	ATTR_EXTEND9  = ATTR_COUNT_BASE10 + 9;

const long	ATTR_MAX_NUM                = 202;
//const long	ATTR_MAX_NUM                = 213;

// End

const long	ATTR_CLIENT_MAX             = ATTR_ITEMC_STR;	
const long  MAX_ATTR_CLIENT				= ATTR_ITEMC_STR;				// �����������ֵ,���ͻ�����
const long	ATTR_CLIENT_SIGN_BYTE_NUM   = (ATTR_CLIENT_MAX + sizeof(char) * 8 - 1) / (sizeof(char) * 8);
const long	ATTR_SIGN_BYTE_NUM          = (ATTR_MAX_NUM + sizeof(char) * 8 - 1) / (sizeof(char) * 8);

inline bool	g_IsNosignChaAttr(long lAttrNo)
{
	if (lAttrNo >= ATTR_CEXP && lAttrNo <= ATTR_NLV_LIFEEXP)
		return true;

	return false;
}

#endif // ATTRTYPE_H