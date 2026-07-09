// FontManager.h
#pragma once
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

class FontManager {
public:
	static FontManager& GetInstance();

	// 初始化所有字体（在 ImGui 上下文和后端初始化后调用）
	bool InitFonts();

	// 注册需要加载的字体（在 InitFonts 调用前注册）
	void RegisterFont(ImFont*& fontPtr, const std::string& fontName, float fontSize);

	// 获取已加载的字体
	ImFont* GetFont(const std::string& fontName);

private:
	FontManager() = default;
	~FontManager() = default;
	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;

	struct FontInfo {
		ImFont** fontPtr;
		std::string fontName;
		float fontSize;
		bool loaded;
	};

	std::vector<FontInfo> m_fontsToLoad;
	std::unordered_map<std::string, ImFont*> m_loadedFonts;

	// 私有辅助函数
	std::string FindFontPath(const std::string& fontName);
	ImFont* LoadFont(const std::string& fontName, float fontSize);
	std::string FindSystemDefaultFont();
};
