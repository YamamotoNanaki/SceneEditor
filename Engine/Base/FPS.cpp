#include "FPS.h"

using namespace IF;

void IF::FPS::Initialize(unsigned int FPS)
{
	MIN_FREAM_TIME = 1.0f / FPS;
	if (QueryPerformanceFrequency(&timeFreq))
	QueryPerformanceCounter(&timeStart);
}

void FPS::FPSFixed() {
	QueryPerformanceCounter(&timeEnd);
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) {
		// �~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		QueryPerformanceCounter(&timeStart);
		return;
	}

	timeStart = timeEnd; // ����ւ�
}

