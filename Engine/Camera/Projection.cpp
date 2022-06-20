#include "Projection.h"

using namespace IF;

Projection::Projection(float radian, float winWidth, float winHeight, float nearZ, float farZ)
	:fovAngle(radian), winWidth(winWidth), winHeight(winHeight), nearZ(nearZ), farZ(farZ)
{
	Inisialize(radian, winWidth, winHeight, nearZ, farZ);
}

void Projection::Inisialize(float radian, float winWidth, float winHeight, float nearZ, float farZ)
{
	matProjection = MatrixPerspectiveFovLH(
		ConvertToRadians(radian), (float)winWidth / winHeight, nearZ, farZ);
}

void IF::Projection::Update()
{
	matProjection = MatrixPerspectiveFovLH(
		ConvertToRadians(fovAngle), (float)winWidth / winHeight, nearZ, farZ);
}

Matrix Projection::Get() const
{
	return matProjection;
}

Matrix* IF::Projection::GetAddressOf()
{
	return &matProjection;
}
