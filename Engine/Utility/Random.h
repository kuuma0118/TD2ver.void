#pragma once
#include <random>

class Random{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// int型のランダムな値を取得
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	static int GetRandomInt(int min, int max);

	/// <summary>
	/// float型のランダムな値を取得
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	static float GetRandomFloat(float min, float max);

private:
	//ランダムエンジン
	static std::mt19937 randomEngine_;

};

