#pragma once
#include <functional>
#include <time.h>

class CountDown
{
public:
	CountDown() = default;
	~CountDown() = default;
	void Update(clock_t time);
	void RegBegin(std::function<void()> func);
	void RegLoop(clock_t interval_time, int loop_times, std::function<void(clock_t)> func);
	void RegEnd(std::function<void()> func);

	void RunBeginFunc();
	void RunLoopFunc(clock_t time);
	void RunEndFunc();
	clock_t GetNextRunTime() const;
	bool IsEnd() const;
private:
	std::function<void()> m_begin_func;
	std::function<void(clock_t)> m_loop_func;
	std::function<void()> m_end_func;
	clock_t m_next_run_time = 0;
	clock_t m_interval_time = 0;
	int m_loop_times = 0;
};
