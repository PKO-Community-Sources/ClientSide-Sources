//=============================================================================
// FileName: JobType.h
// Creater: ZhangXuedong
// Date: 2005.03.18
// Comment: Job Type
//=============================================================================

#ifndef JOBTYPE_H
#define JOBTYPE_H

const long	JOB_TYPE_XINSHOU		= 0;	// "����"
const long	JOB_TYPE_JIANSHI		= 1;	// "��ʿ"
const long	JOB_TYPE_LIEREN			= 2;	// "����"
const long	JOB_TYPE_SHUISHOU		= 3;	// "ˮ��"
const long	JOB_TYPE_MAOXIANZHE		= 4;	// "ð����"
const long	JOB_TYPE_QIYUANSHI		= 5;	// "��Ըʹ"
const long	JOB_TYPE_JISHI			= 6;	// "��ʦ"
const long	JOB_TYPE_SHANGREN		= 7;	// "����"
const long	JOB_TYPE_JUJS			= 8;	// "�޽�ʿ"
const long	JOB_TYPE_SHUANGJS		= 9;	// "˫��ʿ"
const long	JOB_TYPE_JIANDUNSHI		= 10;	// "����ʿ"
const long	JOB_TYPE_XUNSHOUSHI		= 11;	// "ѱ��ʦ"
const long	JOB_TYPE_JUJISHOU		= 12;	// "�ѻ���"
const long	JOB_TYPE_SHENGZHIZHE	= 13;	// "ʥְ��"
const long	JOB_TYPE_FENGYINSHI		= 14;	// "��ӡʦ"
const long	JOB_TYPE_CHUANZHANG		= 15;	// "����"
const long	JOB_TYPE_HANGHAISHI		= 16;	// "����ʿ"
const long	JOB_TYPE_BAOFAHU		= 17;	// "������"
const long	JOB_TYPE_GONGCHENGSHI	= 18;	// "����ʦ"

const long	MAX_JOB_TYPE			= 19;	// ���ְҵ����

extern const char	*g_szJobName[MAX_JOB_TYPE];

#endif // JOBTYPE_H