#include <windows.h>
#include <string>
#include <iostream>

class WallpaperManager {
public:
    // ============================================================
    // 设置壁纸
    // ============================================================
    static bool SetWallpaper(const std::string& imagePath);

    // ============================================================
    // 恢复默认壁纸
    // ============================================================
    static bool RestoreDefaultWallpaper();

    // ============================================================
    // 获取当前壁纸路径
    // ============================================================
    static std::string GetCurrentWallpaper();

private:
    static bool FileExists(const std::string& path);
};