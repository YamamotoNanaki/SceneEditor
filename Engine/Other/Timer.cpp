#include "Timer.h"

void Timer::Set(int end)
{
	this->end = end;
	timer = 0;
}

void Timer::Update()
{
	timer++;
}

int Timer::NowTime() const
{
	return timer;
}

bool Timer::IsEnd() const
{
	return timer == end;
}
