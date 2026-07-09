#pragma once
#include <windows.h>
#include <string>
#include <vector>  // 添加这行！

/**
 * 窗口操作工具类
 * 提供各种窗口管理功能，如透明、无边框、置顶、穿透等
 */
class WindowUtils {
public:
    // ============================================
    // 窗口样式操作
    // ============================================

    /**
     * 移除窗口标题栏和边框
     */
    static void RemoveTitleBar(HWND hwnd);

    /**
     * 恢复窗口标题栏和边框
     */
    static void RestoreTitleBar(HWND hwnd);

    /**
     * 设置窗口为分层窗口（支持透明）
     */
    static void SetLayeredWindow(HWND hwnd);

    /**
     * 设置窗口透明色键（指定颜色变为透明）
     */
    static void SetTransparentColor(HWND hwnd, COLORREF color);

    /**
     * 设置窗口整体透明度
     */
    static void SetWindowAlpha(HWND hwnd, BYTE alpha);

    /**
     * 使窗口鼠标事件穿透
     */
    static void SetMouseTransparent(HWND hwnd, bool enable);

    /**
     * 设置窗口置顶
     */
    static void SetTopmost(HWND hwnd, bool topmost);

    /**
     * 设置窗口为全屏
     */
    static void SetFullscreen(HWND hwnd);

    /**
     * 恢复窗口为窗口模式
     */
    static void RestoreFromFullscreen(HWND hwnd);

    /**
     * 设置窗口大小
     */
    static void SetWindowSize(HWND hwnd, int width, int height);

    /**
     * 设置窗口位置
     */
    static void SetWindowPosition(HWND hwnd, int x, int y);

    /**
     * 居中窗口
     */
    static void CenterWindow(HWND hwnd);

    /**
     * 设置窗口圆角
     */
    static void SetWindowRoundCorners(HWND hwnd, int radius);

    /**
     * 设置窗口阴影效果
     */
    static void SetWindowShadow(HWND hwnd, bool enable);

    /**
     * 使窗口忽略所有鼠标事件（完全穿透）
     */
    static void MakeWindowClickThrough(HWND hwnd);

    /**
     * 设置窗口为工具窗口（不显示在任务栏）
     */
    static void SetToolWindow(HWND hwnd);

    // ============================================
    // 窗口生命周期操作
    // ============================================

    /**
     * 关闭窗口（发送关闭消息）
     */
    static void CloseWindow(HWND hwnd);

    /**
     * 强制关闭窗口
     */
    static void ForceCloseWindow(HWND hwnd);

    /**
     * 最小化窗口
     */
    static void MinimizeWindow(HWND hwnd);

    /**
     * 最大化窗口
     */
    static void MaximizeWindow(HWND hwnd);

    /**
     * 还原窗口
     */
    static void RestoreWindow(HWND hwnd);

    /**
     * 隐藏窗口
     */
    static void HideWindow(HWND hwnd);

    /**
     * 显示窗口
     */
    static void ShowWindow(HWND hwnd);

    // ============================================
    // 获取窗口信息
    // ============================================

    /**
     * 获取窗口位置和大小（一次性获取）
     */
    static RECT GetWindowRect(HWND hwnd);

    /**
     * 获取窗口位置（左上角坐标）
     */
    static void GetWindowPosition(HWND hwnd, int& x, int& y);

    /**
     * 获取窗口大小
     */
    static void GetWindowSize(HWND hwnd, int& width, int& height);

    /**
     * 获取窗口客户区大小（不包含标题栏和边框）
     */
    static void GetClientSize(HWND hwnd, int& width, int& height);

    /**
     * 获取窗口标题
     */
    static std::string GetWindowTitle(HWND hwnd);

    /**
     * 获取窗口标题（宽字符版本）
     */
    static std::wstring GetWindowTitleW(HWND hwnd);

    /**
     * 设置窗口标题
     */
    static void SetWindowTitle(HWND hwnd, const std::string& title);

    /**
     * 设置窗口标题（宽字符版本）
     */
    static void SetWindowTitleW(HWND hwnd, const std::wstring& title);

    /**
     * 获取鼠标当前位置所在的窗口句柄
     */
    static HWND GetWindowUnderCursor();

    /**
     * 获取鼠标当前位置所在的窗口句柄（包含子窗口）
     */
    static HWND GetWindowUnderCursorEx();

    /**
     * 获取指定屏幕坐标位置的窗口句柄
     */
    static HWND GetWindowAtPoint(int x, int y);

    /**
     * 获取指定屏幕坐标位置的窗口句柄（包含子窗口）
     */
    static HWND GetWindowAtPointEx(int x, int y);

    /**
     * 获取当前活动窗口句柄
     */
    static HWND GetActiveWindow();

    /**
     * 获取前台窗口句柄
     */
    static HWND GetForegroundWindow();

    /**
     * 获取桌面窗口句柄
     */
    static HWND GetDesktopWindow();

    /**
     * 获取窗口的父窗口句柄
     */
    static HWND GetParentWindow(HWND hwnd);

    /**
     * 获取窗口的所有子窗口
     */
    static std::vector<HWND> GetChildWindows(HWND hwnd, bool recursive = false);

    /**
     * 获取窗口的进程ID
     */
    static DWORD GetWindowProcessId(HWND hwnd);

    /**
     * 获取窗口的线程ID
     */
    static DWORD GetWindowThreadId(HWND hwnd);

    /**
     * 获取窗口的类名
     */
    static std::string GetWindowClassName(HWND hwnd);

    /**
     * 获取窗口的类名（宽字符版本）
     */
    static std::wstring GetWindowClassNameW(HWND hwnd);

    /**
     * 检查窗口是否可见
     */
    static bool IsWindowVisible(HWND hwnd);

    /**
     * 检查窗口是否最小化
     */
    static bool IsWindowMinimized(HWND hwnd);

    /**
     * 检查窗口是否最大化
     */
    static bool IsWindowMaximized(HWND hwnd);

    /**
     * 检查窗口是否置顶
     */
    static bool IsWindowTopmost(HWND hwnd);

    /**
     * 获取窗口的扩展样式
     */
    static LONG_PTR GetWindowExStyle(HWND hwnd);

    /**
     * 获取窗口样式
     */
    static LONG_PTR GetWindowStyle(HWND hwnd);

private:
    // 窗口样式备份
    struct WindowStyleBackup {
        LONG_PTR exStyle;
        LONG_PTR style;
        bool hasBackup = false;
    };
    static WindowStyleBackup s_backup;

    // 枚举子窗口回调
    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
};