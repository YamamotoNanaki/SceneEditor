#pragma once

namespace IF
{
	struct Rand
	{
		void Initialize();
		int GetRand();
		int GetRand(int min, int max);
		/// <summary>
		/// �t���[�g�^�̃����_���֐�
		/// </summary>
		/// <param name="min">�ŏ��l</param>
		/// <param name="max">�ő�l</param>
		/// <returns>�����_1����float�^����</returns>
		float GetRandF(float min, float max);
	};
}