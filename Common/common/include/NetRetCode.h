/**************************************************************************************************************
*
*			网络调用返回码定义文件											(Created by Andor.Zhang in 2004.12)
*
**************************************************************************************************************/
#ifndef NETRETCODE_H
#define NETRETCODE_H

#define ERR_SUCCESS	0				//所有的成功调用返回均为 0，非0表示各种不成功原因标志

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//					网络调用返回码定义(16bit)
//			返回码定义传输方向标识(如ERR_CM_XXX中的CM)：
//				CM	(C)lient		->Ga(m)eServer
//				MC	Ga(m)eServer	->(C)lient
//						......
//				(其它见"返回码基数定义"部分)
//	(注：每个返回包的返回码均为包的开始的16bit,即第一个WriteShort/ReadShort的操作)
//	(注：每个方向的返回码空间请预留500个，并且每个区间从1开始，如ERR_MC_BASE+1开始定义)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
/*=====================================================================================================
*					返回码基数定义：
*/
#define ERR_MC_BASE			   0				//GameServer/GateServer->Client返回的错误码空间1－500
#define ERR_PT_BASE			 500				//GroupServer->GateServer返回的错误码空间501－1000
#define ERR_AP_BASE			1000				//AccountServer->GroupServer返回的错误码空间1001－1500
#define ERR_MT_BASE			1500				//GameServer->GateServer返回的错误码空间1501－2000
#define ERR_TM_BASE         2000                //GateServer->GameServer返回的错误码空间2000－2500
/*=====================================================================================================
*				AccountServer->GroupServer
*/
#define ERR_AP_INVALIDUSER	ERR_AP_BASE	+ 1		//用户名不存在
#define ERR_AP_INVALIDPWD	ERR_AP_BASE	+ 2		//口令错误
#define ERR_AP_ACTIVEUSER   ERR_AP_BASE + 3     //激活用户失败
#define ERR_AP_LOGGED		ERR_AP_BASE + 4		//此用户已登录
#define ERR_AP_DISABLELOGIN ERR_AP_BASE + 5     //此用户处于被禁止Login状态
#define ERR_AP_BANUSER      ERR_AP_BASE + 6     //官方封停帐号

#define ERR_AP_GPSLOGGED    ERR_AP_BASE + 11    //此GroupServer已登录
#define ERR_AP_GPSAUTHFAIL  ERR_AP_BASE + 12    //此GroupServer认证失败

#define ERR_AP_UNKNOWN		ERR_AP_BASE + 100	//未知原因
#define ERR_AP_OFFLINE		ERR_AP_BASE + 101	//此用户正处于离线状态
#define ERR_AP_LOGIN1		ERR_AP_BASE + 102	//此用户正处于认证1阶段
#define ERR_AP_LOGIN2		ERR_AP_BASE + 103	//此用户正处于认证2状态
#define ERR_AP_ONLINE		ERR_AP_BASE + 104	//此用户已在线
#define ERR_AP_SAVING		ERR_AP_BASE + 105	//此用户账号正处于存盘状态
#define ERR_AP_LOGINTWICE	ERR_AP_BASE + 106   //此用户账号在远端再次登录
#define ERR_AP_DISCONN		ERR_AP_BASE + 107	//group已断掉
#define ERR_AP_UNKNOWNCMD	ERR_AP_BASE + 108	//未知协议
#define ERR_AP_TLSWRONG		ERR_AP_BASE + 109	//线程本地存储错误
#define ERR_AP_NOBILL		ERR_AP_BASE + 110	//账号需要充值

/*=====================================================================================================
*				GroupServer->GateServer
*/
#define	ERR_PT_LOGFAIL		ERR_PT_BASE	+ 1		//GateServer向GroupServer的登录失败
#define ERR_PT_SAMEGATENAME	ERR_PT_BASE	+ 2		//GateServer与已登录GateServer重名

#define ERR_PT_INVALIDDAT	ERR_PT_BASE	+ 20	//无效的数据格式
#define ERR_PT_INERR		ERR_PT_BASE + 21	//服务器之间的操作完整性错误
#define ERR_PT_NETEXCP		ERR_PT_BASE	+ 22	//GroupServer发现的到AccuntServer的网络故障
#define ERR_PT_DBEXCP		ERR_PT_BASE + 23	//GroupServer发现的到Database的故障
#define ERR_PT_INVALIDCHA	ERR_PT_BASE + 24	//当前帐号不拥有请求(选择/删除)的角色
#define ERR_PT_TOMAXCHA		ERR_PT_BASE	+ 25	//已经达到最多能创建的角色数了
#define ERR_PT_SAMECHANAME	ERR_PT_BASE	+ 26	//重复的角色名
#define ERR_PT_INVALIDBIRTH	ERR_PT_BASE	+ 27	//非法的职业
#define ERR_PT_TOOBIGCHANM	ERR_PT_BASE	+ 28	//角色名太长
#define ERR_PT_KICKUSER		ERR_PT_BASE	+ 29
#define ERR_PT_ISGLDLEADER	ERR_PT_BASE	+ 30	//公会不可一日无长，请先解散公会再删除角色
#define ERR_PT_ERRCHANAME	ERR_PT_BASE	+ 31	//非法的角色名称
#define ERR_PT_SERVERBUSY   ERR_PT_BASE + 32	//服务器忙碌，请稍后
#define ERR_PT_TOOBIGPW2	ERR_PT_BASE + 33	//二次密码太长
#define ERR_PT_INVALID_PW2  ERR_PT_BASE + 34	//无效得二次密码，不能进入游戏

// Add by lark.li 20080825 begin
#define ERR_PT_BANUSER      ERR_PT_BASE + 35     //1ù·?·aí￡?êo?
#define ERR_PT_PBANUSER     ERR_PT_BASE + 36     //??è??ü±￡?¨?êo?
// End

#define ERR_PT_GMISLOG     ERR_PT_BASE + 37

#define ERR_PT_BADBOY		ERR_PT_BASE + 50	//

/*=====================================================================================================
*				GameServer/GateServer->Client
*/
#define ERR_MC_NETEXCP		ERR_MC_BASE	+ 1		//GateServer发现的网络异常
#define ERR_MC_NOTSELCHA	ERR_MC_BASE	+ 2		//当前未处于选择角色状态
#define ERR_MC_NOTPLAY		ERR_MC_BASE + 3		//当前不处于玩游戏状态，不能发送ENDPLAY命令
#define ERR_MC_NOTARRIVE	ERR_MC_BASE + 4		//目标地图不可到达
#define ERR_MC_TOOMANYPLY	ERR_MC_BASE	+ 5		//目标服务器玩家过多
#define ERR_MC_NOTLOGIN		ERR_MC_BASE	+ 6		//尚未登陆。
#define ERR_MC_VER_ERROR	ERR_MC_BASE + 7		//客户端的版本号错误
#define ERR_MC_ENTER_ERROR	ERR_MC_BASE + 8		//进入地图失败
#define ERR_MC_ENTER_POS	ERR_MC_BASE + 9		//进入地图的位置非法

/*=====================================================================================================
*				GateServer->GameServer
*/
#define ERR_TM_OVERNAME     ERR_TM_BASE + 1     //GameServer名重复
#define ERR_TM_OVERMAP      ERR_TM_BASE + 2     //GameServer上地图名重复
#define ERR_TM_MAPERR       ERR_TM_BASE + 3     //GameServer地图配置语法错误

#endif
