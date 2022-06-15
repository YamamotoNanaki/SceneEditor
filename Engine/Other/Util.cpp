#include "Util.h"

float TypeConversionByte(int num)
{
    if (num > 255)num = 255;
    if (num < 0)num = 0;
    return (float)num / 255.0f;
}
