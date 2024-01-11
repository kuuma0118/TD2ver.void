#pragma once
#include "Engine/2D/ImGuiManager.h"
#include "MathFunction.h"
#include <iostream>
#include <fstream>
#include <variant>
#include <string>
#include <map>
#include <Engine/Externals/nlohmann/json.hpp>

class GlobalVariables {
public:
	using json = nlohmann::json;
	using Item = std::variant<int32_t, float, Vector3>;
	using Group = std::map<std::string, Item>;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ディレクトリの全ファイルを読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName"></param>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;

	//項目の追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	//項目の追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	//項目の追加(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

public:
	const std::string kDirectoryPath = "Resources/";

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

private:
	// 全データ
	std::map<std::string, Group> datas_;
};