#include "countdownmgr.h"
#include "countdown.h"
#include <algorithm>

CountDownMgr& CountDownMgr::Instance()
{
	static CountDownMgr mgr;
	return mgr;
}

bool CountDownMgr::RegCountDown(CountDown* count_down)
{
	if (nullptr == count_down)
	{
		return false;
	}
	m_count_down_vec.push_back(count_down);
	std::sort(m_count_down_vec.begin(), m_count_down_vec.end(), SortCountDown);
	return true;
}

void CountDownMgr::Update(clock_t time)
{
	if (m_count_down_vec.empty())
	{
		return;
	}
	CountDown* count_down = m_count_down_vec.front();
	if (nullptr == count_down)
	{
		return;
	}
	if (time <= count_down->GetNextRunTime())
	{
		return;
	}
	count_down->RunLoopFunc(time);
	std::sort(m_count_down_vec.begin(), m_count_down_vec.end(), SortCountDown);
}

bool SortCountDown(CountDown* l, CountDown* r)
{
	return l->GetNextRunTime() < r->GetNextRunTime();
}
