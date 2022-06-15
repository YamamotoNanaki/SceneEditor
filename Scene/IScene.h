#pragma once

namespace IF
{
	class IScene
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}