#pragma once

namespace IF
{
	struct Rand
	{
		static void Initialize();
		static int GetRand();
		static int GetRand(int min, int max);
		/// <summary>
		/// �t���[�g�^�̃����_���֐�
		/// </summary>
		/// <param name="min">�ŏ��l</param>
		/// <param name="max">�ő�l</param>
		/// <returns>�����_1����float�^����</returns>
		static float GetRandF(float min, float max);
	};
}