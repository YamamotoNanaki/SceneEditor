#include "Sound.h"
#include "Debug.h"
#include <cassert>

using namespace IF;

void IF::Sound::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
	result = xAudio->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
}

unsigned short IF::Sound::LoadWave(std::string filename)
{
	for (int i = 0; i < Sound::maxSound; i++)
	{
		if (soundDatas[i].free == false)continue;
		if (soundDatas[i].name == filename)return i;
	}
	std::string name = "Data/Sound/";
	name += filename + ".wav";

	unsigned short num = -1;
	for (int i = 0; i < Sound::maxSound; i++)
	{
		if (soundDatas[i].free == false)
		{
			num = i;
			break;
		}
	}

	if (num == -1)assert(0 && "SoundDataの空きがありません");

	std::ifstream file;

	file.open(name, std::ios_base::binary);
	assert(file.is_open() && "ファイルが開けません");

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)assert(0 && "ファイルがRIFFではありません");
	if (strncmp(riff.type, "WAVE", 4) != 0)assert(0 && "フォーマットがWAVEではありません");

	FormatChunk format;

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chank.id, "fmt ", 4) != 0)assert(0 && "チャンクの確認に失敗しました");
	assert(format.chank.size <= sizeof(format.fmt) && "40byteは対応してません");
	file.read((char*)&format.fmt, format.chank.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data ", 4) != 0)assert(0 && "データの確認に失敗しました");

	char* pBuffer = DEBUG_NEW char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	soundDatas[num].wfex = format.fmt;
	soundDatas[num].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundDatas[num].bufferSize = data.size;
	soundDatas[num].free = true;

	return num;
}

void IF::Sound::SoundUnLoad(unsigned short soundNum)
{
	delete[]soundDatas[soundNum].pBuffer;

	soundDatas[soundNum].pBuffer = 0;
	soundDatas[soundNum].bufferSize = 0;
	soundDatas[soundNum].wfex = {};
	soundDatas[soundNum].free = false;
	soundDatas[soundNum].pSourceVoice = nullptr;
}

void IF::Sound::SoundPlay(unsigned short soundNum, bool roop)
{
	HRESULT result;
	result = xAudio.Get()->CreateSourceVoice(&soundDatas[soundNum].pSourceVoice, &soundDatas[soundNum].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas[soundNum].pBuffer;
	buf.AudioBytes = soundDatas[soundNum].bufferSize;
	if (roop)buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	else buf.LoopCount = 0;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundDatas[soundNum].pSourceVoice->SetVolume(soundDatas[soundNum].volume);
	result = soundDatas[soundNum].pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundDatas[soundNum].pSourceVoice->Start();
}

Sound* IF::Sound::Instance()
{
	static Sound* inst = DEBUG_NEW Sound;
	return inst;
}

void IF::Sound::SetVolume(unsigned short soundNum, int volume)
{
	soundDatas[soundNum].volume = (float)volume / 255.0f;
	if (soundDatas[soundNum].pSourceVoice != nullptr)soundDatas[soundNum].pSourceVoice->SetVolume(soundDatas[soundNum].volume);
}

void IF::Sound::DeleteInstance()
{
	delete Sound::Instance();
}

void IF::Sound::StopSound(unsigned short soundNum)
{
	if (soundDatas[soundNum].pSourceVoice == nullptr)return;
	HRESULT result = soundDatas[soundNum].pSourceVoice->Stop();
	soundDatas[soundNum].pSourceVoice = nullptr;
}

void IF::Sound::Reset()
{
	xAudio.Reset();
}
