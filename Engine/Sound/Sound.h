#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <array>

#pragma comment(lib,"xaudio2.lib")

namespace IF
{
	class Sound
	{
	private:
		struct ChunkHeader
		{
			char id[4];
			int32_t size;
		};

		struct RiffHeader
		{
			ChunkHeader chunk;
			char type[4];
		};

		struct FormatChunk
		{
			ChunkHeader chank;
			WAVEFORMATEX fmt;
		};

		struct SoundData
		{
			WAVEFORMATEX wfex{};
			BYTE* pBuffer = nullptr;
			unsigned int bufferSize{};
			const char* name = nullptr;
			bool free = false;
		};
	private:
		Microsoft::WRL::ComPtr<IXAudio2> xAudio;
		IXAudio2MasteringVoice* masterVoice = nullptr;
		static const short maxSound = 128;
		std::array<SoundData, maxSound> soundDatas;

	public:
		void Initialize();
		unsigned short LoadWave(const char* filename);
		void SoundUnLoad(unsigned short soundNum);
		void SoundPlay(unsigned short soundNum, bool roop = true);
		static Sound* Instance();
		static void DeleteInstance();
		void Reset();

	private:
		Sound() {};
		Sound(const Sound&) {};
		Sound& operator=(const Sound&) {}
		~Sound() {};
	};
}
