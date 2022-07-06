#include "DebugText.h"

void IF::DebugText::Initialize(unsigned int texNum)
{
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i].Initialize(texNum);
	}
}

void IF::DebugText::Print(float x, float y, float scale, const char* text, ...)
{
	posX = x;
	posY = y;
	size = scale;

	va_list args;
	va_start(args, text);
	int w = vsnprintf(buffer, bufferSize - 1, text, args);

	for (int i = 0; i < w; i++)
	{
		if (spriteIndex >= maxCharCount)break;

		const unsigned char& character = buffer[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)fontIndex = 0;

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;
		sprites[spriteIndex].SetPosition({ posX + fontWidth * size * i,posY });
		sprites[spriteIndex].SetTextureRect({ (float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight },
			{ (float)fontWidth,(float)fontHeight });
		sprites[spriteIndex].SetSize({ fontWidth * size,fontHeight * size });
		spriteIndex++;
	}
}

void IF::DebugText::Draw(std::vector<D3D12_VIEWPORT>viewport)
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		sprites[i].Update();
		// スプライト描画
		sprites[i].Draw();
	}

	spriteIndex = 0;
}
