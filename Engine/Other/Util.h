#pragma once

//256�K����0����1�̊ԂŕԂ��֐�
float TypeConversionByte(int num);
short TypeConversion255(float num);
template<class T>
T sign(T x)
{
	return (x > 0) - (x < 0);
};
