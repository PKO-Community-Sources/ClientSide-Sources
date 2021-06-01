
#include "Timer.h"

_DBC_USING

uLong Timekeeper::dwFrequency	=1;

Timer::Timer(uLong interval):m_interval(interval?interval:1),m_starttick(0),m_lasttick(0)
{
}
bool Timer::operator()()
{
	uLong	l_tick	=GetTickCount();
	if((l_tick -m_starttick)/m_interval > (m_lasttick-m_starttick)/m_interval)
	{
		m_lasttick	=l_tick;
		return true;
	}
	return false;
}
TimerMgr::TimerMgr():m_starttick(GetTickCount()),m_timercount(0)
{
}
TimerMgr::~TimerMgr()
{
}
void TimerMgr::Free()
{
	MutexArmor l_lockTime(m_mtxtime);
	for(uLong i=0;i<m_timercount;i++)
	{
		m_timer[i]->Free();
	}
	m_timercount	=0;
	l_lockTime.unlock();
}
bool TimerMgr::AddTimer(Timer *timer)
{
	if(!timer) return false;
	timer->m_starttick	=m_starttick;
	timer->m_lasttick	=m_starttick;

	MutexArmor l_lockTime(m_mtxtime);
	m_timer[m_timercount]	=timer;
	m_timercount++;
	l_lockTime.unlock();

	return true;
}
long TimerMgr::Process()
{
	while(!GetExitFlag())
	{
		MutexArmor l_lockTime(m_mtxtime);
		for(uLong i=0;i<m_timercount;i++)
		{
			if((*(m_timer[i]))())
			{
				m_timer[i]->Process();
			}
		}
		l_lockTime.unlock();
		Sleep(40);
	}
	return 0;
}
