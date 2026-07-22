// http.cpp
#include "http.h"
#include <fstream>
#include <iostream>

#ifndef WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_0
#define WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_0  0x00000040
#endif

#ifndef WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_1
#define WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_1  0x00000080
#endif

#ifndef WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2
#define WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2  0x00000100
#endif

// ============================================================
// DownloadTask 实现
// ============================================================


// ============================================================
// 【新增】清除已完成的任务
// ============================================================
void DownloadManager::ClearFinishedTasks() {
    // 从后往前遍历，安全删除
    for (int i = (int)m_tasks.size() - 1; i >= 0; --i) {
        auto& task = m_tasks[i];
        // 如果任务已完成（成功、失败或取消），则删除
        if (task.IsFinished() || task.HasError() || task.IsCanceled()) {
            task.Join();  // 确保线程结束
            m_tasks.erase(m_tasks.begin() + i);
        }
    }
}

// ============================================================
// 【新增】清除已完成的任务并触发全局回调
// ============================================================
void DownloadManager::ClearFinishedTasksAndCallback() {
    ClearFinishedTasks();

    // 如果所有任务都完成了，触发回调
    if (m_onAllFinished && m_tasks.empty()) {
        auto callback = m_onAllFinished;
        m_onAllFinished = nullptr;
        callback();
    }
}

DownloadTask::DownloadTask(const std::string& url, const std::string& savePath,
    const std::string& displayName)
    : m_url(url)
    , m_savePath(savePath)
    , m_displayName(displayName.empty() ? savePath : displayName)
    , m_progress(0.0f)
    , m_isDownloading(false)
    , m_isFinished(false)
    , m_isCanceled(false)
    , m_hasError(false)
    , m_errorMessage("") {
    m_status = "就绪";
}

// 移动构造函数
DownloadTask::DownloadTask(DownloadTask&& other) noexcept
    : m_url(std::move(other.m_url))
    , m_savePath(std::move(other.m_savePath))
    , m_displayName(std::move(other.m_displayName))
    , m_status(std::move(other.m_status))
    , m_errorMessage(std::move(other.m_errorMessage))
    , m_progress(other.m_progress.load())
    , m_isDownloading(other.m_isDownloading.load())
    , m_isFinished(other.m_isFinished.load())
    , m_isCanceled(other.m_isCanceled.load())
    , m_hasError(other.m_hasError.load()) {
    if (other.m_thread.joinable()) {
        other.Cancel();
        other.Join();
    }
}

// 移动赋值运算符
DownloadTask& DownloadTask::operator=(DownloadTask&& other) noexcept {
    if (this != &other) {
        Cancel();
        Join();

        m_url = std::move(other.m_url);
        m_savePath = std::move(other.m_savePath);
        m_displayName = std::move(other.m_displayName);
        m_status = std::move(other.m_status);
        m_errorMessage = std::move(other.m_errorMessage);
        m_progress = other.m_progress.load();
        m_isDownloading = other.m_isDownloading.load();
        m_isFinished = other.m_isFinished.load();
        m_isCanceled = other.m_isCanceled.load();
        m_hasError = other.m_hasError.load();

        if (other.m_thread.joinable()) {
            other.Cancel();
            other.Join();
        }
    }
    return *this;
}

void DownloadTask::StartAsync() {
    if (m_isDownloading) return;
    if (m_isFinished) return;

    m_isDownloading = true;
    m_isFinished = false;
    m_isCanceled = false;
    m_hasError = false;
    m_progress = 0.0f;
    m_status = "正在连接...";
    m_errorMessage = "";

    m_thread = std::thread(&DownloadTask::DownloadThreadFunc, this);
}

void DownloadTask::Cancel() {
    if (m_isDownloading) {
        m_isCanceled = true;
        m_status = "正在取消...";
    }
}

void DownloadTask::Reset() {
    if (m_isDownloading) {
        Cancel();
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }
    m_progress = 0.0f;
    m_isDownloading = false;
    m_isFinished = false;
    m_isCanceled = false;
    m_hasError = false;
    m_errorMessage = "";
    m_status = "就绪";
}

void DownloadTask::DrawProgress(float width) {
    ImGui::Text("%s", m_displayName.c_str());

    ImVec4 barColor = GetProgressColor();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, barColor);
    ImGui::ProgressBar(m_progress, ImVec2(width, 20.0f), m_status.c_str());
    ImGui::PopStyleColor();

    if (m_hasError) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "❌ %s", m_errorMessage.c_str());
    }
    else if (m_isFinished && !m_hasError) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "✅ 下载完成！");
    }
    else if (m_isCanceled) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "⏹ 已取消");
    }

    ImGui::SameLine(ImGui::GetWindowWidth() - 60);
    ImGui::Text("%.1f%%", m_progress * 100.0f);
}

void DownloadTask::Join() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

DownloadTask::~DownloadTask() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void DownloadTask::DownloadThreadFunc() {
    m_status = "下载中...";

    bool result = DownloadFileWithProgress(m_url, m_savePath,
        [&](size_t current, size_t total, void*) {
            if (m_isCanceled) {
                m_status = "已取消";
                return;
            }

            if (total > 0) {
                m_progress = (float)current / total;
                int kb = current / 1024;
                int totalKb = total / 1024;
                m_status = std::to_string(kb) + " KB / " + std::to_string(totalKb) + " KB";
            }
            else {
                m_status = "下载中... " + std::to_string(current / 1024) + " KB";
            }
        },
        &m_isCanceled
    );

    if (m_isCanceled) {
        m_isDownloading = false;
        return;
    }

    if (result) {
        m_progress = 1.0f;
        m_status = "✅ 完成";
        m_isFinished = true;
        m_hasError = false;
    }
    else {
        m_hasError = true;
        m_errorMessage = "下载失败，请检查网络";
        m_status = "❌ 失败";
    }

    m_isDownloading = false;

    // ============================================================
    // 【关键】触发完成回调
    // ============================================================
    if (m_onFinished) {
        m_onFinished(m_savePath, result, m_errorMessage);
    }
}

ImVec4 DownloadTask::GetProgressColor() const {
    if (m_hasError) return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    if (m_isCanceled) return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    if (m_isFinished) return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    return ImVec4(0.2f, 0.6f, 1.0f, 1.0f);
}

// ============================================================
// DownloadManager 实现
// ============================================================

DownloadManager& DownloadManager::GetInstance() {
    static DownloadManager instance;
    return instance;
}

void DownloadManager::AddTask(const std::string& url, const std::string& savePath,
    const std::string& displayName) {
    m_tasks.emplace_back(url, savePath, displayName);
}

void DownloadManager::AddTask(const std::string& url, const std::string& savePath,
    const std::string& displayName, OnTaskFinishedCallback callback) {
    m_tasks.emplace_back(url, savePath, displayName);
    m_tasks.back().SetOnFinished(callback);
}

void DownloadManager::StartAll() {
    for (auto& task : m_tasks) {
        task.StartAsync();
    }
}

void DownloadManager::CancelAll() {
    for (auto& task : m_tasks) {
        task.Cancel();
    }
}

void DownloadManager::CheckAllFinished() {
    if (!m_onAllFinished) return;

    for (auto& task : m_tasks) {
        if (!task.IsFinished() && !task.HasError() && !task.IsCanceled()) {
            return;  // 还有任务未完成
        }
    }

    // 所有任务都已完成
    auto callback = m_onAllFinished;
    m_onAllFinished = nullptr;  // 只触发一次
    callback();
}

void DownloadManager::DrawAllProgress(float width) {
    for (auto& task : m_tasks) {
        task.DrawProgress(width);
        ImGui::Separator();
    }

    // 检查是否所有任务都已完成
    CheckAllFinished();

    static int frameCount = 0;
    frameCount++;
    if (frameCount % 2 == 0) {  // 每2帧清理一次
        ClearFinishedTasks();
    }
}

void DownloadManager::Clear() {
    CancelAll();
    for (auto& task : m_tasks) {
        task.Join();
    }
    m_tasks.clear();
    m_onAllFinished = nullptr;
}

DownloadManager::~DownloadManager() {
    Clear();
}

// ============================================================
// 实际下载函数
// ============================================================
bool DownloadTask::DownloadFileWithProgress(const std::string& url,
    const std::string& savePath,
    ProgressCallback progressCb,
    std::atomic<bool>* cancelFlag) {
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

    std::wstring wHost(host.begin(), host.end());
    std::wstring wPath(path.begin(), path.end());

    // 初始化 WinHTTP
    HINTERNET hSession = WinHttpOpen(L"NeonVision/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        NULL, NULL, 0);
    if (!hSession) return false;

    // 如果是 HTTPS，启用安全协议
    if (isHttps) {
#if defined(WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2)
        DWORD protocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2 |
            WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_1 |
            WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_0;
        WinHttpSetOption(hSession, WINHTTP_OPTION_SECURE_PROTOCOLS, &protocols, sizeof(protocols));
#endif
    }

    HINTERNET hConnect = WinHttpConnect(hSession, wHost.c_str(),
        isHttps ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wPath.c_str(),
        NULL, NULL, NULL, isHttps ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 发送请求
    if (!WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 获取 Content-Length
    DWORD contentLength = 0;
    DWORD contentLengthSize = sizeof(contentLength);
    WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER,
        NULL, &contentLength, &contentLengthSize, NULL);

    // 读取并写入文件
    std::ofstream file(savePath, std::ios::binary);
    if (!file) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    DWORD bytesRead = 0;
    char buffer[8192];
    size_t totalRead = 0;

    while (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        if (cancelFlag && cancelFlag->load()) {
            file.close();
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return false;
        }

        file.write(buffer, bytesRead);
        totalRead += bytesRead;

        if (progressCb) {
            progressCb(totalRead, contentLength, nullptr);
        }
    }

    file.close();
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return true;
}