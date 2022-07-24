#include "Collision.h"

using namespace IF;

bool IF::Collision::CheckSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
    float distV = Vector3Dot(sphere.center, plane.normal);
    float dist = distV - plane.distance;
    if (fabsf(dist) > sphere.radius)return false;

    if (inter)*inter = -dist * plane.normal + sphere.center;

    return true;
}
