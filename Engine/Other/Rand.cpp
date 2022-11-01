#include "Rand.h"
#include <stdlib.h>
#include <time.h>
#include "Ease.h"

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

float IF::Rand::GetRandF(float min, float max, int resolution)
{
	if (min == max)return 0;
	int r = rand() % resolution;
	float a = Ease::Lerp(min, max, resolution, r);
	return a;
}
