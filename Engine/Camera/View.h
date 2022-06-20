#pragma once
#include "IFMath.h"

namespace IF
{
	class View
	{
	private:
		Matrix matView;

	public:
		static Matrix matBillBoard;
		static Matrix matBillBoardY;
		Float3 eye;
		Float3 target;
		Float3 up;
	public:
		View(Float3 eye = { 0,0,-100 }, Float3 target = { 0,0,1 }, Float3 up = { 0,1,0 });
		void Initialze(Float3 eye, Float3 target, Float3 up);
		void Update();
		Matrix Get()const;
		Matrix* GetAddressOf();
	};
}