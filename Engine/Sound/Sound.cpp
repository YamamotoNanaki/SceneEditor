#include "Sound.h"
#include <cassert>

using namespace IF;

void IF::Sound::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio, 0, XAUDIO2_ANY_PROCESSOR);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
	result = xAudio->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result) && "サウンドの初期化に失敗しました");
}

unsigned short IF::Sound::LoadWave(const char* filename)
{
	for (int i = 0; i < Sound::maxSound; i++)
	{
		if (soundDatas[i].free == false)continue;
		if (soundDatas[i].name == filename)return i;
	}

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

	file.open(filename, std::ios_base::binary);
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

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	soundDatas[num] = soundData;

	delete pBuffer;

	return num;
}

void IF::Sound::SoundUnLoad(unsigned short soundNum)
{
	delete[]soundDatas[soundNum].pBuffer;

	soundDatas[soundNum].pBuffer = 0;
	soundDatas[soundNum].bufferSize = 0;
	soundDatas[soundNum].wfex = {};
}

void IF::Sound::SoundPlay(unsigned short soundNum, bool roop)
{
	HRESULT result;
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio->CreateSourceVoice(&pSourceVoice, &soundDatas[soundNum].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas[soundNum].pBuffer;
	buf.AudioBytes = soundDatas[soundNum].bufferSize;
	if (roop)buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	else buf.LoopCount = 0;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

Sound* IF::Sound::Instance()
{
	static Sound* inst = new Sound;
	return inst;
}

void IF::Sound::DeleteInstance()
{
	delete Sound::Instance();
}

void IF::Sound::Reset()
{
	xAudio.Reset();
}
