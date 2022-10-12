#pragma once


class Timer
{
private:
	int timer = 0;
	int end = 0;

public:
	void Set(int end);
	void Update();
	void SafeDownUpdate();
	void SafeUpdate();//タイマーがendの数値を超えてカウントされないようになっています。
	int NowTime()const;
	bool IsEnd()const;
	int GetEndTime()const;
};