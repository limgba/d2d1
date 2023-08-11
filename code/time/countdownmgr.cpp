#include "countdownmgr.h"

CountDownMgr& CountDownMgr::Instance()
{
	static CountDownMgr mgr;
	return mgr;
}

void CountDownMgr::Update(clock_t time)
{
}
