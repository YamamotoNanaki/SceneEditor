#pragma once

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
		double Lerp(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		double InQuad(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		double OutQuad(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		double InOutQuad(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		double EaseInBack(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		double EaseInBack2(double startPos, double endPos, double maxTime, double time);
		/// <summary>
		/// �����ړ�
		/// </summary>
		/// <param name="startPos">�����n�_</param>
		/// <param name="endPos">�I���n�_</param>
		/// <param name="maxTime">�ő厞��</param>
		/// <param name="time">���݂̎���</param>
		/// <returns></returns>
		double EaseOutBounce(double startPos, double endPos, double maxTime, double time);
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
		double OutInRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);

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
		double OutOutRelayQuad(double startPos, double endPos, double relayPos, double maxTime, double relayTime, double time);
	}
}