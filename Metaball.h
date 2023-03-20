#pragma once
#include "Particle.h"

namespace IF
{
	class Metaball :public Particle
	{
		struct TRIANGLE
		{
			Vector3 p[3];
		};
		struct GRIDCELL
		{
			Vector3 p[8];
			float val[8];
		};

	public:
		void Initialize()override;
		static void StaticInitialize();

	private:
		static int Polygonise(GRIDCELL grid, float isolevel, TRIANGLE* triangles);
		static Vector3 VertexInterp(float isolevel, Vector3 p1, Vector3 p2, float valp1, float valp2);
		static void CreateTexture();
	};
}
