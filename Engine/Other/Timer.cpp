#include "Timer.h"

void Timer::Set(int end)
{
	this->end = end;
	timer = 0;
}

void Timer::TimeSet(int timer, int end)
{
	this->timer = timer;
	this->end = end;
}

void Timer::SetIf(int end, bool flag)
{
	if (flag)Set(end);
}

void Timer::Update()
{
	timer++;
}

void Timer::SafeDownUpdate()
{
	if (timer > 0)
	{
		timer--;
	}
}

void Timer::SafeUpdate()
{
	if (IsEnd())
	{
		return;
	}
	timer++;
}

int Timer::NowTime() const
{
	return timer;
}

bool Timer::IsEnd() const
{
	return timer >= end;
}

int Timer::GetEndTime() const
{
	return end;
}
