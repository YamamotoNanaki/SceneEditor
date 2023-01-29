#pragma once
#include <list>
#include <memory>
#include "Emitter.h"

namespace IF
{
	class ParticleManager
	{
		std::list<std::unique_ptr<Emitter>>emitterList;
	private:
		ParticleManager() {}
		ParticleManager(const ParticleManager&);
		ParticleManager& operator=(const ParticleManager&) {}
		~ParticleManager()
		{
			emitterList.clear();
		}

		int GetTagNum(std::string tag);
		inline bool GetNum(std::string tag)
		{
			for (std::unique_ptr<Emitter>& com : emitterList)
			{
				if (com->tag == tag)
				{
					return true;
				}
			}
			return false;
		}

	public:
		inline Emitter* GetAddress(std::string tag)
		{
			for (std::unique_ptr<Emitter>& com : emitterList)
			{
				if (com->tag == tag)
				{
					return com.get();
				}
			}
			return nullptr;
		}
		void Add();
		void Update();
		void Draw();
		void DrawPostEffect(ID3D12RootSignature* root);
		void Reset();
		void Start(std::string tag);
		void End(std::string tag);
		void SetObject(std::string ptag, std::string& otag);
		static ParticleManager* Instance();
		void InputJson(nlohmann::json& j);
		inline void SetDrawFlag(bool flag, std::string tag)
		{
			for (std::unique_ptr<Emitter>& com : emitterList)
			{
				if (com->tag == tag)
				{
					com->DrawF = flag;
					return;
				}
			}
		};

#ifdef _DEBUG
		bool debug = false;
		void GUI();
		void DebugUpdate();
		void OutputJson(nlohmann::json& j);
#endif
				};
			}
