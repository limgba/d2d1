#pragma once
#include <functional>
#include <time.h>

class CountDown
{
public:
	CountDown() = default;
	~CountDown() = default;
	void RegBegin(std::function<void()> func);
	void RegLoop(clock_t interval_time, int loop_times, std::function<void(clock_t)> func);
	void RegEnd(std::function<void()> func);

	void RunBeginFunc();
	void RunLoopFunc(clock_t time);
	void RunEndFunc();
	clock_t GetIntervalTime();
private:
	std::function<void()> m_begin_func;
	std::function<void(clock_t)> m_loop_func;
	std::function<void()> m_end_func;
	clock_t m_interval_time;
	int m_loop_times;
};
