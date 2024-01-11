#pragma once
#include <Windows.h>
#include <string>
#include <format>

/// <summary>
/// メッセージを出力
/// </summary>
/// <param name="message"></param>
void Log(const std::string& message);

/// <summary>
/// stringをwstringに変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring ConvertString(const std::string& str);

/// <summary>
/// wstringをstringに変換
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string ConvertString(const std::wstring& str);