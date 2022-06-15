#pragma once

namespace IF
{
	struct Rand
	{
		void Initialize();
		int GetRand();
		int GetRand(int min, int max);
		/// <summary>
		/// フロート型のランダム関数
		/// </summary>
		/// <param name="min">最小値</param>
		/// <param name="max">最大値</param>
		/// <returns>小数点1桁のfloat型乱数</returns>
		float GetRandF(float min, float max);
	};
}