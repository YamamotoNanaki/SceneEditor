#pragma once
#include "IFMath.h"
#include "Timer.h"
namespace IF
{
	namespace Ease
	{
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float Lerp(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float InQuad(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float OutQuad(float startPos, float endPos, float maxTime, float time);

		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		Float3 OutQuadFloat3(Float3 startPos, Float3 endPos, Timer timer);
		IF::Float3 LerpFloat3(IF::Float3 startPos, Float3 endPos, Timer timer);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float InOutQuad(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float EaseInBack(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float EaseInBack2(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float EaseOutBounce(float startPos, float endPos, float maxTime, float time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="relayPos">���p�n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="relayTime">���p�n�_��ʉ߂��鎞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float OutInRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

		float InOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);

		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="relayPos">���p�n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="relayTime">���p�n�_��ʉ߂��鎞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		float OutOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time);
	}
}