#include "Wallpaper.h"

bool WallpaperManager::SetWallpaper(const std::string& imagePath) {
    // 检查文件是否存在
    if (!FileExists(imagePath)) {
        std::cerr << "❌ 图片文件不存在: " << imagePath << std::endl;
        return false;
    }

    // 转换为宽字符路径
    std::wstring wPath(imagePath.begin(), imagePath.end());

    // 设置壁纸
    BOOL result = SystemParametersInfoW(
        SPI_SETDESKWALLPAPER,
        0,
        (PVOID)wPath.c_str(),
        SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE
    );

    if (result) {
        std::cout << "✅ 壁纸设置成功: " << imagePath << std::endl;
    }
    else {
        std::cerr << "❌ 壁纸设置失败，错误码: " << GetLastError() << std::endl;
    }

    return result != FALSE;
}

bool WallpaperManager::RestoreDefaultWallpaper() {
    // 设置为空字符串，系统会恢复默认壁纸
    return SystemParametersInfoW(
        SPI_SETDESKWALLPAPER,
        0,
        L"",
        SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE
    ) != FALSE;
}

std::string WallpaperManager::GetCurrentWallpaper() {
    wchar_t buffer[MAX_PATH] = { 0 };
    if (SystemParametersInfoW(SPI_GETDESKWALLPAPER, MAX_PATH, buffer, 0)) {
        // 转换为 UTF-8
        int len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
        if (len > 0) {
            std::string result(len, '\0');
            WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &result[0], len, nullptr, nullptr);
            if (!result.empty() && result.back() == '\0') {
                result.pop_back();
            }
            return result;
        }
    }
    return "";
}

bool WallpaperManager::FileExists(const std::string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}