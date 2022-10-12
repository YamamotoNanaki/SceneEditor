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
