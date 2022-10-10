#include "Util.h"

float TypeConversionByte(int num)
{
    if (num > 255)num = 255;
    if (num < 0)num = 0;
    return (float)num / 255.0f;
}

short TypeConversion255(float num)
{
    if (num > 1)num = 1;
    if (num < 0)num = 0;
    return (short)num * 255;
}
