#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <array>
#include <string>

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
			WAVEFORMATEX wfex;
			float volume = 50;
			IXAudio2SourceVoice* pSourceVoice = nullptr;
			BYTE* pBuffer;
			unsigned int bufferSize;
			std::string name;
			bool free = false;
		};
	private:
		Microsoft::WRL::ComPtr<IXAudio2> xAudio;
		IXAudio2MasteringVoice* masterVoice;
		static const short maxSound = 128;
		std::array<SoundData, maxSound> soundDatas;

	public:
		void Initialize();
		unsigned short LoadWave(std::string);
		void SoundUnLoad(unsigned short soundNum);
		void SetVolume(unsigned short soundNum, int volume);
		void SoundPlay(unsigned short soundNum, bool roop = true);
		void StopSound(unsigned short soundNum);
		static Sound* Instance();
		void Reset();

	private:
		Sound() {};
		Sound(const Sound&) {};
		Sound& operator=(const Sound&) {}
		~Sound() {};
	};
}
