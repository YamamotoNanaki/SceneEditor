#include "Ease.h"

float IF::Ease::Lerp(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return change * time + startPos;
}
float IF::Ease::InQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return change * time * time + startPos;
}
float IF::Ease::OutQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	return -change * time * (time - 2) + startPos;
}
IF::Float3 IF::Ease::OutQuadFloat3(Float3 startPos, Float3 endPos,Timer timer)
{
	return { Ease::OutQuad(startPos.x, endPos.x, timer.GetEndTime(), timer.NowTime()),
			 Ease::OutQuad(startPos.y, endPos.y, timer.GetEndTime(), timer.NowTime()),
			 Ease::OutQuad(startPos.z, endPos.z, timer.GetEndTime(), timer.NowTime()) };
};

IF::Float3 IF::Ease::LerpFloat3(Float3 startPos, Float3 endPos, Timer timer)
{
	return { Ease::Lerp(startPos.x, endPos.x, timer.GetEndTime(), timer.NowTime()),
			 Ease::Lerp(startPos.y, endPos.y, timer.GetEndTime(), timer.NowTime()),
			 Ease::Lerp(startPos.z, endPos.z, timer.GetEndTime(), timer.NowTime()) };
};


float IF::Ease::InOutQuad(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime / 2;
	float change = endPos - startPos;
	if (time < 1) return change / 2 * time * time + startPos;
	return -change / 2 * ((--time) * (time - 2) - 1) + startPos;
}
float IF::Ease::OutInRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IF::Ease::OutQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IF::Ease::InQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}

float IF::Ease::InOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IF::Ease::InQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IF::Ease::OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
float IF::Ease::OutOutRelayQuad(float startPos, float endPos, float relayPos, float maxTime, float relayTime, float time) {
	if (time < relayTime) {
		return IF::Ease::OutQuad(relayPos, startPos, relayTime, time);
	}
	else {
		return IF::Ease::OutQuad(endPos, relayPos, maxTime - relayTime, time - relayTime);
	}
}
float IF::Ease::EaseInBack(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	float c1 = 1.70158;
	float c3 = c1 + 1;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
float IF::Ease::EaseInBack2(float startPos, float endPos, float maxTime, float time) {
	time /= maxTime;
	float change = endPos - startPos;
	float c1 = 1.2;
	float c3 = c1 + 0.8;
	return change * (c3 * time * time * time - c1 * time * time) + startPos;
}
float IF::Ease::EaseOutBounce(float startPos, float endPos, float time, float maxTime) {
	time /= maxTime;
	float change = endPos - startPos;
	float n1 = 7.5625;
	float d1 = 2.75;

	if (time < 1 / d1) {
		return n1 * time * time + startPos + change;
	}
	else if (time < 2 / d1) {
		return n1 * (time -= 1.5 / d1) * time + 0.75 + startPos + change;
	}
	else if (time < 2.5 / d1) {
		return n1 * (time -= 2.25 / d1) * time + 0.9375 + startPos + change;
	}
	else {
		return n1 * (time -= 2.625 / d1) * time + 0.984375 + startPos + change;
	}
}
