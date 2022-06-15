#include "Sound.h"
#include <cassert>

using namespace IF;

void IF::Sound::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio, 0, XAUDIO2_ANY_PROCESSOR);
	assert(SUCCEEDED(result) && "�T�E���h�̏������Ɏ��s���܂���");
	result = xAudio->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result) && "�T�E���h�̏������Ɏ��s���܂���");
}

unsigned short IF::Sound::LoadWave(const char* filename)
{
	for (int i = 0; i < Sound::maxSound; i++)
	{
		if (soundDatas[i].free == false)continue;
		if (soundDatas[i].name == filename)return i;
	}

	unsigned short num = 0;
	for (int i = 0; i < Sound::maxSound; i++)
	{
		if (soundDatas[i].free == false)
		{
			num = i;
			break;
		}
	}
	std::ifstream file;

	file.open(filename, std::ios_base::binary);
	assert(file.is_open() && "�t�@�C�����J���܂���");

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)assert(0 && "�t�@�C����RIFF�ł͂���܂���");
	if (strncmp(riff.type, "WAVE", 4) != 0)assert(0 && "�t�H�[�}�b�g��WAVE�ł͂���܂���");

	FormatChunk format;

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chank.id, "fmt ", 4) != 0)assert(0 && "�`�����N�̊m�F�Ɏ��s���܂���");
	assert(format.chank.size <= sizeof(format.fmt) && "40byte�͑Ή����Ă܂���");
	file.read((char*)&format.fmt, format.chank.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data ", 4) != 0)assert(0 && "�f�[�^�̊m�F�Ɏ��s���܂���");

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	soundDatas[num] = soundData;

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
