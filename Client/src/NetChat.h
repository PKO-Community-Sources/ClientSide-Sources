#pragma once

#include "NetIF.h"
#include "compcommand.h"

// Add by lark.li 20080807 begin
struct stPersonInfo;
struct stQueryPersonInfo;
// End

/****************************************************************
		发送Packet打包函数声明段
		添加:服务器开发人员
		修改:服务器开发人员
		调用:客户端开发人员
		代码体请填入文件"NetPkXXX.cpp"(注意:有Pk,意为网络包处理的cpp文件)
*****************************************************************/
extern void CS_GM1Say(const char *pszContent);
extern void CS_GM1Say1(const char *pszContent);//Add by sunny.sun20080804
extern void CS_Say2Trade(const char *pszContent);
extern void CS_Say2All(const char *pszContent);
extern void CS_Say2You(const char *you,const char *pszContent);
extern void CS_Say2Team(const char *pszContent);
extern void CS_Say2Guild( const char* pszContent);
extern void CS_Say2VIP( const char* pszContent);
extern void CS_Say2Dis( const char* pszContent);
extern void CP_RefuseToMe(bool refusetome);

extern void	CS_Sess_Create(const char *chaname[],unsigned char chanum);
extern void	CS_Sess_Say(unsigned long sessid,const char *word);
extern void	CS_Sess_Add(unsigned long sessid,const char *chaname);
extern void	CS_Sess_Leave(unsigned long sessid);

extern void	CS_Team_Invite(const char *chaname);
extern void	CS_Team_Refuse(unsigned long chaid);
extern void	CS_Team_Confirm(unsigned long chaid);
extern void	CS_Team_Leave();
extern void CS_Team_Kick( DWORD dwKickedID );

extern void	CS_Frnd_Invite(const char *chaname);
extern void	CS_Frnd_Refuse(unsigned long chaid);
extern void	CS_Frnd_Confirm(unsigned long chaid);
extern void	CS_Frnd_Delete(unsigned long chaid);

extern void	CP_Frnd_Refresh_Info(unsigned long chaid);

extern void	CP_Change_PersonInfo(const char *motto,unsigned short icon,bool refuse_sess);//refuse_sess =true,服务器自动拒绝会话 =false,接收会话.

/****************************************************************
		接收Packet后调用的本地接口声明段
		添加:服务器开发人员
		修改:客户端开发人员
		调用:“接收的Packet解析函数声明段”对应函数
		代码体请填入文件"NetXXX.cpp"(注意:没有Pk,相对于NetPkXXX,没有包处理)
*****************************************************************/

struct stNetSay2You			// 向你说话
{
	const char *m_src;		// 说话人
	const char *m_dst;		// 接收人
	const char *m_content ;	// 内容
};

struct stNetSay2All		// 向所有人说话
{
	const char *m_src;	//说话人
	const char *m_content;//内容
};

struct stNetScrollSay//向说有人说话 滚动显示
{
	const char *m_content;//显示内容
	int		setnum;//显示次数
};

struct stNetTeamChaPart // 组队成员的外观
{
    struct SItem
    {
	    short	sID;		// 道具表中的ID（0表示没有道具）
	    short	sNum;		// 道具个数
	    char	chForgeLv;	// 精炼等级
		long lFuseID;
    };

	short		sTypeID;
	SItem	    SLink[enumEQUIP_NUM];
    short       sHairID;        // 默认的头发和脸型

	void		Convert( const stNetChangeChaPart& stPart );
};

struct stNetTeamState
{
	unsigned long	ulID;			// 角色唯一ID
	BYTE			byWork;			// 职业
	long			lHP;
	long			lSP;
	long			lLV;
    long            lMaxHP;
    long            lMaxSP;

    stNetTeamChaPart    SFace;       // 外观
};

struct stNetPCTeam      // from group
{
	unsigned char kind;	// 参考Common:netcommand.h - TEAM_MSG 开始宏
	unsigned char count;//数组元素的个数;
	unsigned long cha_dbid[10];
	char		  cha_name[10][33];
	char		  motto[10][33];
	short		  cha_icon[10];
};

//struct stMemberInfo
//{
//	DWORD dwMessage;
//	DWORD dwID;
//	string sName;
//	string sMottoName;
//	DWORD dwIcon;
//	DWORD dwLv;
//	string sJobName;
//	BYTE reason;
//}S_MEMBERINFO;
extern void NetSay2You(stNetSay2You &say2you,DWORD dwColour);
extern void	NetSay2Team(unsigned long chaid,const char *word,DWORD dwColour);
extern void NetSay2Gud(const char* src, const char* word,DWORD dwColour);
extern void NetSay2VIP(const char* src, const char* word,DWORD dwColour);
extern void NetSay2Dis(const char* src, const char* word,DWORD dwColour);
extern void NetSay2All(stNetSay2All &say2all,DWORD dwColour);
extern void NetSay2Trade(stNetSay2All &say2all,DWORD dwColour);

extern void NetGM1Say(stNetSay2All &say2all);
extern void NetGM1Say1(stNetScrollSay &say2all);//Add by sunny.sun20080804

extern void NetTeamInvite(const char *inviter_name, unsigned long inviter_chaid,unsigned short icon);
extern void NetTeamCancel(unsigned long  inviter_chaid,char reason);
extern void NetPCTeam(stNetPCTeam	&pcteam);
extern void NetSynTeam(stNetTeamState *pSTeamState);

struct stNetFrndStart
{
	const char *	szGroup;	//好友所属分组
	unsigned long	lChaid;
	const char *	szChaname;
	const char *	szMotto;
	unsigned short	sIconID;
	unsigned char	cStatus;	//0-离线；非0-在线
};

extern void NetFrndInvite(const char *inviter_name, unsigned long inviter_chaid,unsigned short icon);
extern void NetFrndCancel(unsigned long  inviter_chaid,char reason);
extern void NetFrndOnline(unsigned long cha_id);
extern void NetFrndOffline(unsigned long cha_id);
extern void NetFrndDel(unsigned long cha_id);
extern void NetFrndAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);

extern void NetFrndStart(stNetFrndStart &self,stNetFrndStart arrCha[],unsigned short arrnum);

extern void NetGMStart(stNetFrndStart arrCha[],unsigned short arrnum);
extern void NetGMOnline(unsigned long cha_id);
extern void NetGMOffline(unsigned long cha_id);
extern void NetGMDel(unsigned long cha_id);
extern void NetGMAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);
extern void NetFrndRefreshInfo(unsigned long cha_id,const char * motto,unsigned short icon,unsigned short degree,const char * job,const char *guildname);

// 师傅
extern void NetMasterOnline(unsigned long cha_id);
extern void NetMasterOffline(unsigned long cha_id);
extern void NetMasterDel(unsigned long cha_id);
extern void NetMasterAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);
extern void NetMasterStart(stNetFrndStart &self,stNetFrndStart arrCha[],unsigned short arrnum);
extern void NetMasterCancel(unsigned long  inviter_chaid,char reason);
extern void NetMasterRefreshInfo(unsigned long cha_id,const char * motto,unsigned short icon,unsigned short degree,const char * job,const char *guildname);

// 徒弟
extern void NetPrenticeOnline(unsigned long cha_id);
extern void NetPrenticeOffline(unsigned long cha_id);
extern void NetPrenticeDel(unsigned long cha_id);
extern void NetPrenticeAdd(unsigned long cha_id,const char * cha_name,const char * motto,unsigned short icon_id,const char *grp);
extern void NetPrenticeStart(stNetFrndStart &self,stNetFrndStart arrCha[],unsigned short arrnum);
extern void NetPrenticeCancel(unsigned long  inviter_chaid,char reason);
extern void NetPrenticeRefreshInfo(unsigned long cha_id,const char * motto,unsigned short icon,unsigned short degree,const char * job,const char *guildname);

extern void	NetChangePersonInfo(const char *motto,unsigned short icon,bool refuse_sess);


struct stNetSessCreate
{
	unsigned long	lChaID;
	const char *	szChaName;
	const char *	szMotto;
	unsigned short	sIconID;
};
extern void NetSessCreate(const char *chaname);
extern void NetSessCreate(unsigned long newsessid,stNetSessCreate *cha,short chanum);
extern void NetSessAdd(unsigned long sessid,stNetSessCreate *cha);
extern void NetSessLeave(unsigned long sessid,unsigned long chaid);
extern void NetSessSay(unsigned long sessid,unsigned long chaid,const char *word);
/****************************************************************
		接收的Packet解析函数声明段
		添加:服务器开发人员
		修改:服务器开发人员
		调用:NetIF::HandlePacketMessage
		代码体请填入文件"NetPkXXX.cpp"(注意:有Pk,意为网络包处理的cpp文件)
*****************************************************************/

//私聊
extern BOOL	PC_Say2VIP(LPRPACKET pk);
extern BOOL	PC_Say2Dis(LPRPACKET pk);
extern BOOL	PC_Say2You(LPRPACKET pk);
extern BOOL	PC_GM1SAY(LPRPACKET pk);
extern BOOL	PC_GM1SAY1(LPRPACKET pk);//Add by sunny.sun20080804
extern BOOL	PC_SAY2TRADE(LPRPACKET pk);
extern BOOL	PC_Say2All(LPRPACKET pk);
extern BOOL	PC_Say2Team(LPRPACKET pk);
extern BOOL	PC_Say2Gud(LPRPACKET pk);

//组队成员变化信息刷新
extern BOOL	PC_SESS_CREATE(LPRPACKET pk);
extern BOOL	PC_SESS_ADD(LPRPACKET pk);
extern BOOL	PC_SESS_LEAVE(LPRPACKET pk);
extern BOOL	PC_SESS_SAY(LPRPACKET pk);

extern BOOL	PC_TEAM_INVITE(LPRPACKET pk);
extern BOOL	PC_TEAM_CANCEL(LPRPACKET pk);
extern BOOL	PC_TEAM_REFRESH(LPRPACKET pk);

extern BOOL	PC_FRND_INVITE(LPRPACKET pk);
extern BOOL	PC_FRND_CANCEL(LPRPACKET pk);
extern BOOL	PC_FRND_REFRESH(LPRPACKET pk);
extern BOOL	PC_FRND_REFRESH_INFO(LPRPACKET pk);
extern BOOL	PC_GM_INFO(LPRPACKET pk);

extern BOOL	PC_CHANGE_PERSONINFO(LPRPACKET pk);
extern BOOL	PC_UpdateGuildAttr(LPRPACKET pk);
extern BOOL	PC_UpdateGuildCircle(LPRPACKET pk);



