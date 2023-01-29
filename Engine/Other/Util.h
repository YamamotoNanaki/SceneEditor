#pragma once
#include <chrono>

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

// Œ»İ‚Ì‚ğƒ~ƒŠ•b’PˆÊ‚Åæ“¾
inline int GetNowCount(void) {
	return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
}

template<class T>
inline T clamp(T num, T min = 0, T max = 1)
{
	return (num < min) ? min : ((num > max) ? max : num);
}