#pragma once

//256ŠK’²‚ğ0‚©‚ç1‚ÌŠÔ‚Å•Ô‚·ŠÖ”
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