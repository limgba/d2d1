#include "countdown.h"

void CountDown::Update(clock_t time)
{
	this->RunBeginFunc();
	this->RunLoopFunc(time);
	this->RunEndFunc();
}

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
	m_begin_func = nullptr;
}

void CountDown::RunLoopFunc(clock_t time)
{
	if (nullptr == m_loop_func)
	{
		return;
	}
	if (time < m_next_run_time)
	{
		return;
	}
	m_loop_func(time);
	--m_loop_times;
	if (m_loop_times > 0)
	{
		m_next_run_time = time + m_interval_time;
	}
	else
	{
		m_loop_func = nullptr;
	}
}

void CountDown::RunEndFunc()
{
	if (m_loop_times > 0)
	{
		return;
	}
	if (nullptr == m_end_func)
	{
		return;
	}
	m_end_func();
	m_end_func = nullptr;
}

clock_t CountDown::GetNextRunTime() const
{
	return m_next_run_time;
}

bool CountDown::IsEnd() const
{
	return nullptr == m_begin_func
		&& nullptr == m_loop_func
		&& nullptr == m_end_func
	;
}
