// http.h
#pragma once
#include <string>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <imgui.h>
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

// ============================================================
// 进度回调函数类型
// ============================================================
using ProgressCallback = std::function<void(size_t, size_t, void*)>;

// ============================================================
// 下载完成回调函数类型
// ============================================================
using OnTaskFinishedCallback = std::function<void(const std::string& savePath, bool success, const std::string& errorMsg)>;

// ============================================================
// 下载任务类
// ============================================================
class DownloadTask {
public:
    DownloadTask() = default;
    DownloadTask(const std::string& url, const std::string& savePath,
        const std::string& displayName = "");

    // 禁止拷贝
    DownloadTask(const DownloadTask&) = delete;
    DownloadTask& operator=(const DownloadTask&) = delete;

    // 允许移动
    DownloadTask(DownloadTask&& other) noexcept;
    DownloadTask& operator=(DownloadTask&& other) noexcept;

    // ============================================================
    // 开始异步下载
    // ============================================================
    void StartAsync();

    // ============================================================
    // 取消下载
    // ============================================================
    void Cancel();

    // ============================================================
    // 重置任务（允许重新下载）
    // ============================================================
    void Reset();

    // ============================================================
    // 设置完成回调
    // ============================================================
    void SetOnFinished(OnTaskFinishedCallback callback) {
        m_onFinished = callback;
    }

    // ============================================================
    // 绘制进度条（ImGui）
    // ============================================================
    void DrawProgress(float width = 300.0f);

    // ============================================================
    // 获取状态
    // ============================================================
    bool IsDownloading() const { return m_isDownloading; }
    bool IsFinished() const { return m_isFinished; }
    bool HasError() const { return m_hasError; }
    bool IsCanceled() const { return m_isCanceled; }
    float GetProgress() const { return m_progress; }
    const std::string& GetStatus() const { return m_status; }
    const std::string& GetDisplayName() const { return m_displayName; }
    const std::string& GetSavePath() const { return m_savePath; }

    // ============================================================
    // 等待线程结束
    // ============================================================
    void Join();

    ~DownloadTask();

private:
    // ============================================================
    // 后台下载线程
    // ============================================================
    void DownloadThreadFunc();

    // ============================================================
    // 获取进度条颜色
    // ============================================================
    ImVec4 GetProgressColor() const;

    // ============================================================
    // 实际下载函数
    // ============================================================
    static bool DownloadFileWithProgress(const std::string& url,
        const std::string& savePath,
        ProgressCallback progressCb,
        std::atomic<bool>* cancelFlag);

private:
    std::string m_url;
    std::string m_savePath;
    std::string m_displayName;
    std::string m_status;
    std::string m_errorMessage;

    std::atomic<float> m_progress{ 0.0f };
    std::atomic<bool> m_isDownloading{ false };
    std::atomic<bool> m_isFinished{ false };
    std::atomic<bool> m_isCanceled{ false };
    std::atomic<bool> m_hasError{ false };

    std::thread m_thread;

    // 完成回调
    OnTaskFinishedCallback m_onFinished;
};

// ============================================================
// 下载管理器（管理多个任务）
// ============================================================
class DownloadManager {
public:
    static DownloadManager& GetInstance();

    // ============================================================
    // 添加下载任务
    // ============================================================
    void AddTask(const std::string& url, const std::string& savePath,
        const std::string& displayName = "");

    // ============================================================
    // 添加下载任务（带完成回调）
    // ============================================================
    void AddTask(const std::string& url, const std::string& savePath,
        const std::string& displayName, OnTaskFinishedCallback callback);

    // ============================================================
    // 开始所有任务
    // ============================================================
    void StartAll();

    // ============================================================
    // 停止所有任务
    // ============================================================
    void CancelAll();

    // ============================================================
    // 获取任务
    // ============================================================
    std::vector<DownloadTask>& GetTasks() { return m_tasks; }
    size_t GetTaskCount() const { return m_tasks.size(); }

    // ============================================================
    // 绘制所有任务的进度
    // ============================================================
    void DrawAllProgress(float width = 300.0f);

    // ============================================================
    // 清空所有任务
    // ============================================================
    void Clear();

    void ClearFinishedTasks();

    void ClearFinishedTasksAndCallback();

    // ============================================================
    // 设置全局完成回调（所有任务完成后触发）
    // ============================================================
    void SetOnAllFinished(std::function<void()> callback) {
        m_onAllFinished = callback;
    }

    ~DownloadManager();

private:
    DownloadManager() = default;
    std::vector<DownloadTask> m_tasks;
    std::function<void()> m_onAllFinished;

    // 检查所有任务是否完成
    void CheckAllFinished();
};

// ============================================================
// HTTP GET 请求（同步）
// ============================================================
inline std::string httpget(const std::string& url) {
    // 解析 URL
    std::string host, path;
    bool isHttps = false;

    std::string temp = url;
    if (temp.find("https://") == 0) {
        isHttps = true;
        temp = temp.substr(8);
    }
    else if (temp.find("http://") == 0) {
        temp = temp.substr(7);
    }

    size_t slashPos = temp.find('/');
    if (slashPos != std::string::npos) {
        host = temp.substr(0, slashPos);
        path = temp.substr(slashPos);
    }
    else {
        host = temp;
        path = "/";
    }

    // 转换为宽字符
    std::wstring wHost(host.begin(), host.end());
    std::wstring wPath(path.begin(), path.end());

    // 初始化 WinHTTP
    HINTERNET hSession = WinHttpOpen(L"NeonVision/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        NULL, NULL, 0);
    if (!hSession) return "fail";

    // 连接服务器
    HINTERNET hConnect = WinHttpConnect(hSession,
        wHost.c_str(),
        isHttps ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT,
        0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return "fail";
    }

    // 创建请求
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET",
        wPath.c_str(),
        NULL, NULL, NULL,
        isHttps ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "fail";
    }

    // 发送请求
    if (!WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "fail";
    }

    // 接收响应
    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "fail";
    }

    // 读取响应数据
    std::string response;
    DWORD bytesRead = 0;
    char buffer[4096];

    while (WinHttpReadData(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        response += buffer;
    }

    // 清理
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return response.empty() ? "fail" : response;
}