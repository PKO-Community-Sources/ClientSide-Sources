#include "Packet.h"
#include "Comm.h"

int count = 0;

_DBC_USING

extern PreAllocHeap<rbuf>	__bufheap;
//=====WPacket=========================================================================================
WPacket::WPacket(const RPacket &rpk)
	:rptr<rbuf>(rpk),__tca(rpk.__tca)
	,m_offset(rpk.m_offset),m_head(rpk.m_head)
	,m_wpos(rpk.GetDataLen() -em_cmdsize),m_cpos(0)//,m_appendant(false)
{}
WPacket::WPacket(const TcpCommApp *tca)
:__tca(tca),m_offset(0),m_wpos(0),m_cpos(0)//,m_appendant(false)
,m_head(tca?tca->GetPktHead():0)
{}
//--------------------------------------------------------------------
WPacket &WPacket::operator=(rbuf *bptr){
	if(__tca)
	{
		rptr<rbuf>::operator=(bptr);
		m_offset	=0;
		m_wpos		=0;
		m_cpos		=0;
		//m_appendant	=true;
	}
	return	*this;
}
//--------------------------------------------------------------------
WPacket &WPacket::operator=(const WPacket &wpk)
{
	rptr<rbuf>::operator=(wpk);
	*(const_cast<const TcpCommApp **>(&__tca))	=wpk.__tca;
	*(const_cast<uLong *>(&m_head))				=wpk.m_head;
	m_offset	=wpk.m_offset;
	m_wpos		=wpk.m_wpos;
	m_cpos		=wpk.m_cpos;
	//m_appendant	=wpk.m_appendant;
	return *this;
}
//--------------------------------------------------------------------
WPacket &WPacket::operator=(const RPacket &rpk)
{
	if(__tca){
		if((__tca == rpk.__tca)||(m_head == rpk.m_head))
		{
			rptr<rbuf>::operator=(rpk);
			m_offset	=rpk.m_offset;
			m_wpos		=rpk.GetDataLen() -em_cmdsize;
			m_cpos		=0;
			//m_appendant	=false;
		}else
		{
			m_offset	=0;
			rptr<rbuf>::operator=(__bufheap.Get(m_offset +m_head +rpk.GetDataLen()));
			MemCpy(const_cast<char*>(GetDataAddr()),rpk.GetDataAddr(),rpk.GetDataLen());
			m_wpos		=rpk.GetDataLen() -em_cmdsize;
			m_cpos		=0;
			//m_appendant	=true;
		}
	}else
	{
		rptr<rbuf>::operator=(rpk);
		*(const_cast<const TcpCommApp **>(&__tca))	=rpk.__tca;
		*(const_cast<uLong *>(&m_head))				=rpk.m_head;
		m_offset	=rpk.m_offset;
		m_wpos		=rpk.GetDataLen() -em_cmdsize;
		m_cpos		=0;
		//m_appendant	=false;
	}
	return	*this;
}
//--------------------------------------------------------------------
WPacket WPacket::Duplicate()const
{
	WPacket l_ret		=__tca;
	if(__tca)
	{
		l_ret			=__bufheap.Get(GetPktLen());
	}

	l_ret.m_offset		=0;
	l_ret.m_wpos		=m_wpos;
	l_ret.m_cpos		=0;
	//l_ret.m_appendant	=__tca?true:false;
	if(bool(*this))
	{
		MemCpy(const_cast<char*>(l_ret.GetPktAddr()),GetPktAddr(),GetPktLen());
	}
	return l_ret;
}
//--------------------------------------------------------------------
void WPacket::WritePktLen()const
{
	uLong		l_lenl;
	uShort		l_lens;
	uChar		l_lenc;
	if(!bool(*this))	return;
	if(GetPktLen() <(__tca->__len_offset +__tca->__len_size))	return;
	switch(__tca->__len_size)
	{
	case sizeof(uShort):
		{
			l_lens	=uShort(GetPktLen());
			l_lens	=htons(l_lens);
			MemCpy(const_cast<char*>(GetPktAddr())+__tca->__len_offset,(char*)&l_lens,sizeof(uShort));
			break;
		}
	case sizeof(uLong):
		{
			l_lenl	=GetPktLen();
			l_lenl	=htonl(l_lenl);
			MemCpy(const_cast<char*>(GetPktAddr())+__tca->__len_offset,(char*)&l_lenl,sizeof(uLong));
			break;
		}
	case sizeof(uChar):
		{
			l_lenc	=uChar(GetPktLen());
			MemCpy(const_cast<char*>(GetPktAddr())+__tca->__len_offset,(char*)&l_lenc,sizeof(uChar));
			break;
		}
	}
}
//--------------------------------------------------------------------
uLong WPacket::BackupSESS()const
{
	uLong l_ses;
	if(!bool(*this))	return 0;
	if(!__tca->__rpc)	return 0;
	if(GetPktLen() <Head())	return 0;
	MemCpy((char*)&l_ses,GetDataAddr()-sizeof(uLong),sizeof(uLong));
	return ntohl(l_ses);
}
//--------------------------------------------------------------------
void WPacket::WriteSESS(uLong ses)const
{
	if(!bool(*this))	return;
	if(!__tca->__rpc)	return;
	if(GetPktLen() <Head())	return;
	ses	=htonl(ses);
	MemCpy(const_cast<char*>(GetDataAddr())-sizeof(uLong),(char*)&ses,sizeof(uLong));
}
//--------------------------------------------------------------------
bool WPacket::WriteCmd(uShort cmd)
{
	
	if (cmd == 431 || cmd == 438){
		count = 0;
	}
	
	if(!bool(*this))					return false;
	if(GetPktLen() <Head()+em_cmdsize)	return false;
	cmd	=htons(cmd);
	MemCpy(const_cast<char*>(GetDataAddr()),(char *)&cmd,sizeof(uShort));
	
	this->WriteLong(count);
	count++;
	return true;
}
//--------------------------------------------------------------------
bool WPacket::WriteChar(uChar ch)
{
	if(!bool(*this))	return false;
	if(m_wpos >=SIGN32)	return false;
	if(m_offset +GetPktLen() +sizeof(uChar) >Size())
	{
		uLong	l_newlen	=( GetPktLen() + sizeof(uChar) ) +( Size() - m_offset ) + ( __bufheap.GetUnitSize() - 1);
		l_newlen	/=__bufheap.GetUnitSize();
		for(uLong i=0;l_newlen>i;i+=32);
		l_newlen			=i*__bufheap.GetUnitSize();

		rbuf * l_buf	=__bufheap.Get(l_newlen);
		if(*this)
		{
			MemCpy(l_buf->getbuf() ,GetPktAddr(),GetPktLen());
		}
		rptr<rbuf>::operator=(l_buf);
		m_offset	=0;
	}

	MemCpy(const_cast<char*>(GetDataAddr()) +GetDataLen(),(cChar *)&ch,sizeof(uChar));
	m_wpos +=sizeof(uChar);
	return true;
}
//--------------------------------------------------------------------
bool WPacket::WriteShort(uShort sh)
{
	if(!bool(*this))	return false;
	if(m_wpos >=SIGN32)	return false;
	if(m_offset +GetPktLen() +sizeof(uShort) >Size())
	{
		uLong	l_newlen	=( GetPktLen() + sizeof(uShort) ) +( Size() - m_offset ) + ( __bufheap.GetUnitSize() - 1);
		l_newlen			/=__bufheap.GetUnitSize();
		for(uLong i=0;l_newlen>i;i+=32);
		l_newlen			=i*__bufheap.GetUnitSize();

		rbuf * l_buf		=__bufheap.Get(l_newlen);
		if(*this)
		{
			MemCpy(l_buf->getbuf() ,GetPktAddr(),GetPktLen());
		}
		rptr<rbuf>::operator=(l_buf);
		m_offset	=0;
	}

	sh	=htons(sh);
	MemCpy(const_cast<char*>(GetDataAddr()) +GetDataLen(),(cChar *)&sh,sizeof(uShort));
	m_wpos +=sizeof(uShort);
	return true;
}
//--------------------------------------------------------------------
bool WPacket::WriteLong(uLong lo)
{
	if(!bool(*this))	return false;
	if(m_wpos >=SIGN32)	return false;
	if(m_offset +GetPktLen() +sizeof(uLong) >Size())
	{
		uLong	l_newlen	=( GetPktLen() + sizeof(uLong) ) +( Size() - m_offset ) + ( __bufheap.GetUnitSize() - 1);
		l_newlen			/=__bufheap.GetUnitSize();
		for(uLong i=0;l_newlen>i;i+=32);
		l_newlen			=i*__bufheap.GetUnitSize();

		rbuf * l_buf		=__bufheap.Get(l_newlen);
		if(*this)
		{
			MemCpy(l_buf->getbuf() ,GetPktAddr(),GetPktLen());
		}
		rptr<rbuf>::operator=(l_buf);
		m_offset	=0;
	}

	lo	=htonl(lo);
	MemCpy(const_cast<char*>(GetDataAddr()) +GetDataLen(),(cChar *)&lo,sizeof(uLong));
	m_wpos +=sizeof(uLong);
	return true;
}
//--------------------------------------------------------------------
bool WPacket::WriteLongLong(LONG64 ll)
{
	struct _str_hl
	{
			uLong	high;
			uLong	low;
	};
	union  UnLong64
	{
		LONG64  llval;
		_str_hl	unit;		
	};

	UnLong64 ul64;
	ul64.llval = ll;

	if(!WriteLong(ul64.unit.high))
		return false;

	return WriteLong(ul64.unit.low);
}
//--------------------------------------------------------------------
bool WPacket::WriteSequence(cChar *seq,uShort len)
{
	if(!bool(*this))	return false;
	if(m_wpos >=SIGN32)	return false;
	if(!WriteShort(len))
	{
		return false;
	}
	if(m_offset +GetPktLen() +len >Size())
	{
		uLong	l_newlen	=( GetPktLen() + len ) +( Size() - m_offset ) + ( __bufheap.GetUnitSize() - 1);
		l_newlen			/=__bufheap.GetUnitSize();
		for(uLong i=0;l_newlen>i;i+=32);
		l_newlen			=i*__bufheap.GetUnitSize();

		rbuf * l_buf		=__bufheap.Get(l_newlen);
		if(*this)
		{
			MemCpy(l_buf->getbuf() ,GetPktAddr(),GetPktLen());
		}
		rptr<rbuf>::operator=(l_buf);
		m_offset	=0;
	}

	MemCpy(const_cast<char*>(GetDataAddr()) +GetDataLen(),seq,len);
	m_wpos +=len;
	return true;
}
bool WPacket::WriteString(cChar *str)
{
	return WriteSequence(str,uShort(strlen(str)+1));
}

bool WPacket::WriteFloat(float fVal)
{
	if(!bool(*this))	return false;
	if(m_wpos >=SIGN32)	return false;
	if(m_offset +GetPktLen() +sizeof(float) >Size())
	{
		uLong	l_newlen	=( GetPktLen() + sizeof(float) ) +( Size() - m_offset ) + ( __bufheap.GetUnitSize() - 1);
		l_newlen			/=__bufheap.GetUnitSize();
		for(uLong i=0;l_newlen>i;i+=32);
		l_newlen			=i*__bufheap.GetUnitSize();

		rbuf * l_buf		=__bufheap.Get(l_newlen);
		if(*this)
		{
			MemCpy(l_buf->getbuf() ,GetPktAddr(),GetPktLen());
		}
		rptr<rbuf>::operator=(l_buf);
		m_offset	=0;
	}

	MemCpy(const_cast<char*>(GetDataAddr()) +GetDataLen(),(cChar *)&fVal,sizeof(float));
	m_wpos +=sizeof(float);
	return true;
}

//====RPacket==========================================================================================
RPacket::RPacket(const WPacket &wpk)
	:rptr<rbuf>(wpk),__tca(wpk.__tca),m_head(wpk.m_head)
	,m_offset(wpk.m_offset),m_rpos(0),m_revrpos(0),m_tickcount(GetTickCount())
	,m_len(wpk.GetDataLen() +wpk.m_head)
{}
RPacket::RPacket(const TcpCommApp *tca)
:__tca(tca),m_offset(0),m_rpos(0),m_revrpos(0),m_len(0),m_tickcount(GetTickCount())
,m_head(tca?tca->GetPktHead():0)
{}
//--------------------------------------------------------------------
RPacket &RPacket::operator=(rbuf *bptr)
{
	if(__tca){
		rptr<rbuf>::operator =(bptr);
		m_offset	=0;
		m_rpos		=0;
		m_len		=0;
		m_revrpos	=0;
		m_tickcount	=GetTickCount();
	}
	return *this;
}
RPacket	&RPacket::operator=(const RPacket &rpk)
{
	rptr<rbuf>::operator=(rpk);
	*(const_cast<const TcpCommApp **>(&__tca))	=rpk.__tca;
	*(const_cast<uLong *>(&m_head))				=rpk.m_head;
	m_offset	=rpk.m_offset;
	m_rpos		=rpk.m_rpos;
	m_len		=rpk.m_len;
	m_tickcount=rpk.m_tickcount;
	m_revrpos	=0;
	return *this;
}
RPacket &RPacket::operator=(const WPacket &wpk)
{
	if(__tca){
		if((__tca	==wpk.__tca)||(m_head == wpk.m_head))
		{
			rptr<rbuf>::operator=(wpk);
			m_offset	=wpk.m_offset;
			m_rpos		=0;
			m_len		=wpk.GetDataLen() +m_head;
			m_revrpos	=0;
		}else
		{
			m_offset	=0;
			rptr<rbuf>::operator=(__bufheap.Get(m_offset +m_head +wpk.GetDataLen()));
			MemCpy(const_cast<char*>(GetDataAddr()),wpk.GetDataAddr(),wpk.GetDataLen());
			m_rpos		=0;
			m_len		=wpk.GetDataLen() +m_head;
			m_revrpos	=0;
		}
	}else
	{
		rptr<rbuf>::operator=(wpk);
		*(const_cast<const TcpCommApp **>(&__tca))	=wpk.__tca;
		*(const_cast<uLong *>(&m_head))				=wpk.m_head;
		m_offset	=wpk.m_offset;
		m_rpos		=0;
		m_len		=wpk.GetDataLen() +m_head;
		m_revrpos	=0;
	}
	m_tickcount	=GetTickCount();
	return	*this;
}
//--------------------------------------------------------------------
void RPacket::ReadPktLen()
{
	if(!bool(*this))	return;

	uLong		l_lenl;
	uShort		l_lens;
	uChar		l_lenc;
	switch(__tca->__len_size)
	{
	case sizeof(uShort):
		{
			MemCpy((char*)&l_lens,GetPktAddr()+__tca->__len_offset,sizeof(uShort));
			m_len	=ntohs(l_lens);
			break;
		}
	case sizeof(uLong):
		{
			MemCpy((char*)&l_lenl,GetPktAddr()+__tca->__len_offset,sizeof(uLong));
			m_len	=ntohl(l_lenl);
			break;
		}
	case sizeof(uChar):
		{
			MemCpy((char*)&l_lenc,GetPktAddr()+__tca->__len_offset,sizeof(uChar));
			m_len	=l_lenc;
			break;
		}
	}
}
//--------------------------------------------------------------------
uLong RPacket::ReadSESS()const
{
	uLong	l_ses;
	if(!bool(*this))	return	0;
	if(!__tca->__rpc)	return	0;
	MemCpy((char*)&l_ses,GetDataAddr()-sizeof(uLong),sizeof(uLong));
	return ntohl(l_ses);
}
void RPacket::RestoreSESS(uLong ses)const
{
	if(!bool(*this))	return;
	if(!__tca->__rpc)	return;
	ses	=htonl(ses);
	MemCpy(const_cast<char*>(GetDataAddr())-sizeof(uLong),(char*)&ses,sizeof(uLong));
}
//--------------------------------------------------------------------
uShort	RPacket::ReadCmd()
{
	uShort	l_cmd;
	if(!bool(*this))	return	0;
	MemCpy((char*)&l_cmd,GetDataAddr(),sizeof(uShort));
	l_cmd	=ntohs(l_cmd);
	return	l_cmd;
}
//--------------------------------------------------------------------
uChar RPacket::ReadChar()
{
	uChar l_retval =0;
	if(RemainData() >=sizeof(uChar))
	{
		MemCpy((char*)&l_retval,GetDataAddr() +em_cmdsize +m_rpos,sizeof(uChar));
		m_rpos +=sizeof(uChar);
	}
	return l_retval;
}
//--------------------------------------------------------------------
uShort RPacket::ReadShort()
{
	uShort l_retval	=0;
	if(RemainData() >=sizeof(uShort))
	{
		MemCpy((char*)&l_retval,GetDataAddr() +em_cmdsize +m_rpos,sizeof(uShort));
		l_retval =ntohs(l_retval);
		m_rpos +=sizeof(uShort);
	}
	return l_retval;
}
//--------------------------------------------------------------------
uLong RPacket::ReadLong()
{
	uLong l_retval	=0;
	if(RemainData() >=sizeof(uLong))
	{
		MemCpy((char*)&l_retval,GetDataAddr() +em_cmdsize +m_rpos,sizeof(uLong));
		l_retval =ntohl(l_retval);
		m_rpos +=sizeof(uLong);
	}
	return l_retval;
}
//--------------------------------------------------------------------
LONG64 RPacket::ReadLongLong()
{
	struct _str_hl
	{
			uLong	high;
			uLong	low;
	};
	union  UnLong64
	{
		LONG64  llval;
		_str_hl	unit;		
		
	};
	LONG64 l_retval	=0;
	UnLong64 ll;

	ll.unit.high = ReadLong();
	ll.unit.low = ReadLong();

	l_retval = ll.llval;
	return l_retval;
}
//--------------------------------------------------------------------
cChar *RPacket::ReadSequence(uShort &retlen)
{
	cChar *l_retseq	=0;
	retlen			=ReadShort();
	if(RemainData() >=retlen)
	{
		l_retseq	=GetDataAddr() +em_cmdsize +m_rpos;
		m_rpos		+=retlen;
	}
	return	l_retseq;
}
cChar *RPacket::ReadString(uShort *len)
{
	uShort	l_retlen;
	cChar	*l_ret	=ReadSequence(l_retlen);
	if(l_ret && l_retlen >0)
	{
		const_cast<char*>(l_ret)[l_retlen-1]	=0;
		if(len)
		{
			*len	=l_retlen -1;
		}
		return l_ret;
	}else
	{
		return "";
	}
}
float RPacket::ReadFloat()
{
	float l_retval	=0;
	if(RemainData() >=sizeof(float))
	{
		MemCpy((char*)&l_retval,GetDataAddr() +em_cmdsize +m_rpos,sizeof(float));
		m_rpos +=sizeof(float);
	}
	return l_retval;
}
uChar RPacket::ReverseReadChar()
{
	uChar l_retval =0;
	if(GetDataLen() >=m_revrpos +sizeof(uChar))
	{
		m_revrpos +=sizeof(uChar);
		MemCpy((char*)&l_retval,GetDataAddr() +GetDataLen() -m_revrpos,sizeof(uChar));
	}
	return l_retval;
}
uShort RPacket::ReverseReadShort()
{
	uShort l_retval =0;
	if(GetDataLen() >=m_revrpos +sizeof(uShort))
	{
		m_revrpos +=sizeof(uShort);
		MemCpy((char*)&l_retval,GetDataAddr() +GetDataLen() -m_revrpos,sizeof(uShort));
		l_retval	=ntohs(l_retval);
	}
	return l_retval;
}
uLong RPacket::ReverseReadLong()
{
	uLong l_retval =0;
	if(GetDataLen() >=m_revrpos +sizeof(uLong))
	{
		m_revrpos +=sizeof(uLong);
		MemCpy((char*)&l_retval,GetDataAddr() +GetDataLen() -m_revrpos,sizeof(uLong));
		l_retval	=ntohl(l_retval);
	}
	return l_retval;
}
bool RPacket::DiscardLast(uLong len)
{
	if(m_len -len >=m_head)
	{
		m_len -=len;
		return true;
	}else
	{
		return false;
	}
}
