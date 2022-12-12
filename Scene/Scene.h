#pragma once
#include "ConstBuff.h"
#include "Texture.h"
#include "Graphic.h"
#include "Fire.h"
#include "Light.h"
#include "Sprite.h"
#include "Sound.h"
#include "GUI.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "SpriteManager.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "ModelLoader.h"
#include "FBXModel.h"

namespace IF
{
	class Scene
	{
#ifdef _DEBUG
		GUI gui;
#endif
	private:
		//変数宣言
		//-----------------------
		Texture* tex = Texture::Instance();
		Graphic* graph = Graphic::Instance();


		ObjectManager* objM = ObjectManager::Instance();
		ModelManager* modelM = ModelManager::Instance();
		CameraManager* cameraM = CameraManager::Instance();
		LightManager* lightM = LightManager::Instance();
		SpriteManager* spriteM = SpriteManager::Instance();
		ParticleManager* particleM = ParticleManager::Instance();

		Object obj;
		FBXModel* model;
		ModelLoader loader;

	public:
		void InputJson(std::string failename);
		void Initialize();
		static void StaticInitialize();
		void Update();
		void Draw();
		void Delete();

		//デバッグ用
#ifdef _DEBUG
		void OutputJson(std::string failename);
		void DebugUpdate();
#endif // _DEBUG
	};
}