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
		double Lerp(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		double InQuad(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		double OutQuad(double startPos, double endPos, double maxTime, double time);

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
		double InOutQuad(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		double EaseInBack(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		double EaseInBack2(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// 直線移動
		/// </summary>
		/// <param name="startPos">初期地点</param>
		/// <param name="endPos">終了地点</param>
		/// <param name="maxTime">最大時間</param>
		/// <param name="time">現在の時間</param>
		/// <returns></returns>
		double EaseOutBounce(double startPos, double endPos, double maxTime, double time);
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
		double OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);

		double InOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);

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
		double OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);
	}
}