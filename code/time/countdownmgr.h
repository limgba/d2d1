#pragma once
#include <time.h>
#include <vector>

class CountDown;

class CountDownMgr
{
public:
	static CountDownMgr& Instance();
	bool RegCountDown(CountDown* count_down);
	void Update(clock_t time);
private:
	std::vector<CountDown*> m_count_down_vec;
};
