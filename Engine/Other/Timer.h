#pragma once


class Timer
{
private:
	int timer = 0;
	int end = 0;

public:
	void Set(int end);
	void Update();
	int NowTime()const;
	bool IsEnd()const;
};