#pragma once
#include <array>
#include <fstream>
#include <wrl.h>
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

class Audio {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//音声データの最大数
	static const int kMaxSoundData = 256;

	/// <summary>
	/// チャンクヘッダ
	/// </summary>
	struct ChunkHeader {
		char id[4];//チャンク毎のID
		int32_t size;//チャンクサイズ
	};

	/// <summary>
	/// RIFFヘッダチャンク
	/// </summary>
	struct RiffHeader {
		ChunkHeader chunk;//"RIFF"
		char type[4];//"WAVE"
	};

	/// <summary>
	/// FMTチャンク
	/// </summary>
	struct FormatChunk {
		ChunkHeader chunk;//"fmt"
		WAVEFORMATEX fmt;//波形フォーマット
	};

	/// <summary>
	/// 音声データ
	/// </summary>
	struct SoundData {
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファのサイズ
		unsigned int bufferSize;
	};

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Audio* GetInstance();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	///// <summary>
	///// 音声データの読み込み
	///// </summary>
	///// <param name="filename"></param>
	///// <returns></returns>
	//SoundData SoundLoadWave(const char* filename);

	/// <summary>
	/// 音声データの読み込み
	/// </summary>
	/// <param name="filename"></param>
	/// <returns></returns>
	uint32_t SoundLoadWave(const char* filename);

	/// <summary>
	/// 音声データ開放
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnload(SoundData* soundData);

	///// <summary>
	///// 音声再生
	///// </summary>
	///// <param name="xAudio2"></param>
	///// <param name="soundData"></param>
	//void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="audioHandle"></param>
	/// <param name="roopFlag"></param>
	void SoundPlayWave(uint32_t audioHandle, bool roopFlag);

	/// <summary>
	/// 音声停止
	/// </summary>
	/// <param name="audioHandle"></param>
	void StopAudio(uint32_t audioHandle);

private:
	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = delete;
	const Audio& operator = (const Audio&) = delete;

private:
	ComPtr<IXAudio2> xAudio2_ = nullptr;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;
	std::array<SoundData, kMaxSoundData> soundDatas_{};
	std::array<IXAudio2SourceVoice*, kMaxSoundData> sourceVoices_{};
	uint32_t audioHandle_ = -1;

};