#include "FontManager.h"
#include <imgui.h>
#include <iostream>
#include <algorithm>
#include <cctype>

// Singleton accessor implementation
FontManager& FontManager::GetInstance() {
	static FontManager instance;
	return instance;
}

bool FontManager::InitFonts() {
	ImGuiIO& io = ImGui::GetIO();

	// 添加默认字体
	io.Fonts->AddFontDefault();

	// 初始化所有注册的字体（不在这里调用 Build，外部会统一上传纹理）
	for (auto& fontInfo : m_fontsToLoad) {
		ImFont* font = LoadFont(fontInfo.fontName, fontInfo.fontSize);
		if (font) {
			if (fontInfo.fontPtr && *fontInfo.fontPtr != font) {
				*fontInfo.fontPtr = font;
			}
			fontInfo.loaded = true;
		}
	}

	// 不在此处调用 io.Fonts->Build()，由调用者在合适时机构建并上传纹理
	return true;
}

void FontManager::RegisterFont(ImFont*& fontPtr, const std::string& fontName, float fontSize) {
	FontInfo info;
	info.fontPtr = &fontPtr;
	info.fontName = fontName;
	info.fontSize = fontSize;
	info.loaded = false;
	m_fontsToLoad.push_back(info);
}

ImFont* FontManager::GetFont(const std::string& fontName) {
	auto it = m_loadedFonts.find(fontName);
	if (it != m_loadedFonts.end()) {
		return it->second;
	}
	return nullptr;
}

std::string FontManager::FindFontPath(const std::string& fontName) {
	std::vector<std::string> fontPaths = {
		"C:/Windows/Fonts/",
		"./fonts/",
		"../fonts/",
		"/usr/share/fonts/",
		"/System/Library/Fonts/"
	};

	std::vector<std::string> extensions = { ".ttf", ".ttc", ".otf", ".fon" };

	// 先尝试直接使用完整路径
	try {
		if (std::filesystem::exists(fontName)) {
			return fontName;
		}
	} catch (...) {
		// ignore filesystem errors
	}

	for (const auto& path : fontPaths) {
		for (const auto& ext : extensions) {
			std::string fullPath = path + fontName + ext;
			try {
				if (std::filesystem::exists(fullPath)) {
					return fullPath;
				}
			} catch (...) {}
		}
	}

	return "";
}

ImFont* FontManager::LoadFont(const std::string& fontName, float fontSize) {
	std::string fontPath = FindFontPath(fontName);

	if (fontPath.empty()) {
		std::cerr << "FontManager: cannot find font '" << fontName << "' in standard paths\n";
		fontPath = FindSystemDefaultFont();
		if (fontPath.empty()) {
			std::cerr << "FontManager: no system default font found either\n";
			return nullptr;
		}
	}

	ImGuiIO& io = ImGui::GetIO();

	// 选择合适的字形范围
	const ImWchar* glyphRanges = io.Fonts->GetGlyphRangesDefault();
	auto toLower = [](std::string s) {
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)std::tolower(c); });
		return s;
	};
	std::string lname = toLower(fontName);
	std::string lpath = toLower(fontPath);
	if (lname.find("msy") != std::string::npos || lpath.find("msyh") != std::string::npos || lpath.find("simsun") != std::string::npos) {
		glyphRanges = io.Fonts->GetGlyphRangesChineseSimplifiedCommon();
	}

	std::cerr << "FontManager: loading font '" << fontName << "' from '" << fontPath << "' size=" << fontSize << "\n";
	ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize, nullptr, glyphRanges);
	if (!font) {
		std::cerr << "FontManager: failed to load font from '" << fontPath << "'\n";
		return nullptr;
	}

	m_loadedFonts[fontName] = font;
	m_loadedFonts[fontPath] = font;
	std::cerr << "FontManager: loaded font OK\n";
	return font;
}

std::string FontManager::FindSystemDefaultFont() {
	std::vector<std::string> defaultFonts = {
		"C:/Windows/Fonts/msyh.ttc",
		"C:/Windows/Fonts/simsun.ttc",
		"C:/Windows/Fonts/arial.ttf",
		"C:/Windows/Fonts/consola.ttf"
	};

	for (const auto& font : defaultFonts) {
		try {
			if (std::filesystem::exists(font)) {
				return font;
			}
		} catch (...) {}
	}
	return "";
}

