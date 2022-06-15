#pragma once
#include "Sprite.h"
#include <string>

namespace IF
{
	class DebugText
	{
	private:
		static const int maxCharCount = 256;
		static const int fontWidth = 9;
		static const int fontHeight = 18;
		static const int fontLineCount = 14;
		static const int bufferSize = 256;
		char buffer[bufferSize]{};

	private:
		Sprite sprites[maxCharCount];
		int spriteIndex = 0;

		float posX = 0.0f;
		float posY = 0.0f;
		float size = 1.0f;

	public:
		void Initialize(unsigned int texNum);
		void Print(float x, float y, float scale, const char* text, ...);
		void Draw(std::vector<D3D12_VIEWPORT>viewport);
	};
}