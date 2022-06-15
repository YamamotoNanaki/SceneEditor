#include "Rand.h"
#include <stdlib.h>
#include <time.h>

using namespace IF;

void Rand::Initialize()
{
	srand(time(nullptr));
}

int Rand::GetRand()
{
	return rand();
}

int Rand::GetRand(int min, int max)
{
	return min + rand() % (max - min);
}

float IF::Rand::GetRandF(float min, float max)
{
	float a = (min * 10 + rand() % (int)((max - min) * 10)) / 10.0f;
	return a;
}
