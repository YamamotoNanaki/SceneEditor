#pragma once
#include "IFMath.h"
#include "Timer.h"
namespace IF
{
	namespace Ease
	{
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float Lerp(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float InQuad(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float OutQuad(float startPos, float endPos, float maxTime, float time);

		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		Float3 OutQuadFloat3(Float3 startPos, Float3 endPos, Timer timer);
		IF::Float3 LerpFloat3(IF::Float3 startPos, Float3 endPos, Timer timer);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float InOutQuad(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float EaseInBack(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float EaseInBack2(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float EaseOutBounce(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="relayPos">中継地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="relayTime">中継地点を通過する時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float OutInRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

		float InOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="relayPos">中継地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="relayTime">中継地点を通過する時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		float OutOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);
	}
}