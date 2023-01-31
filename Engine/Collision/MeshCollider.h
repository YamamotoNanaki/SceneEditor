#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "Model.h"

namespace IF
{
    class MeshCollider : public BaseCollider
    {
    public:
        MeshCollider()
        {
            shapeType = COLLISIONSHAPE_MESH;
        }

        void ConstructTriangle(Model* model);

        void Update()override;

        //bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr);
        bool CheckCollisionSphere(const Sphere& sphere, Vector3* inter = nullptr, Vector3* reject = nullptr);
        bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, Vector3* inter = nullptr);

    private:
        std::vector<Triangle>triangles;
        Matrix invMatWorld;
    };
}
