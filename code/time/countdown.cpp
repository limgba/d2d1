#include "countdown.h"

void CountDown::RegBegin(std::function<void()> func)
{
	m_begin_func = func;
}

void CountDown::RegLoop(clock_t interval_time, int loop_times, std::function<void(clock_t)> func)
{
	m_interval_time = interval_time;
	m_loop_times = loop_times;
	m_loop_func = func;
}

void CountDown::RegEnd(std::function<void()> func)
{
	m_end_func = func;
}

void CountDown::RunBeginFunc()
{
	if (nullptr == m_begin_func)
	{
		return;
	}
	m_begin_func();
}

void CountDown::RunLoopFunc(clock_t time)
{
	if (nullptr == m_loop_func)
	{
		return;
	}
	m_loop_func(time);
}

void CountDown::RunEndFunc()
{
	if (nullptr == m_end_func)
	{
		return;
	}
	m_end_func();
}

clock_t CountDown::GetIntervalTime()
{
	return m_interval_time;
}
