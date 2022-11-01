#pragma once

//256階調を0から1の間で返す関数
float TypeConversionByte(int num);
short TypeConversion255(float num);
template<class T>
T sign(T x)
{
	return (x > 0) - (x < 0);
};
