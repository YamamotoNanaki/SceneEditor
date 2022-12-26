#pragma once

//256�K����0����1�̊ԂŕԂ��֐�
float TypeConversionByte(int num);
short TypeConversion255(float num);
template<class T>
inline T sign(T x)
{
	return (x > 0) - (x < 0);
};
template<class T>
inline int GetSize(T* a)
{
	return (int)(sizeof(a) / sizeof(a[0]));
}
template<class T>
inline T clamp(T num, T min = 0, T max = 1)
{
	return (num < min) ? min : ((num > max) ? max : num);
}