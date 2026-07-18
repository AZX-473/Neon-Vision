#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <digitalv.h>
#pragma comment(lib, "winmm.lib")
#include <string>
#include <iostream>

// ============================================================
// 音频播放状态
// ============================================================
enum class AudioState {
    Stopped,    // 未播放
    Playing,    // 正在播放
    Paused,     // 已暂停
    Error       // 播放错误
};

class AudioPlayer {
private:
    static AudioState m_state;
    static std::string m_currentFile;
    static bool m_isLooping;

public:
    // ============================================================
    // 播放 MP3（使用 MCI）
    // ============================================================
    static void PlayMP3(const std::string& filename, bool loop = false) {
        std::string fullPath = "resources/" + filename;

        // 先停止当前播放
        StopMP3();

        // 打开文件
        std::string command = "open \"" + fullPath + "\" type mpegvideo alias music";
        if (mciSendStringA(command.c_str(), NULL, 0, NULL) != 0) {
            m_state = AudioState::Error;
            std::cerr << "Failed to open: " << fullPath << std::endl;
            return;
        }

        // 播放
        std::string playCmd = "play music";
        if (loop) {
            playCmd += " repeat";
            m_isLooping = true;
        }
        else {
            m_isLooping = false;
        }

        if (mciSendStringA(playCmd.c_str(), NULL, 0, NULL) != 0) {
            m_state = AudioState::Error;
            std::cerr << "Failed to play: " << fullPath << std::endl;
            return;
        }

        m_state = AudioState::Playing;
        m_currentFile = filename;
        std::cout << "▶ Playing: " << filename << std::endl;
    }

    // ============================================================
    // 播放 WAV（使用 PlaySound API）
    // ============================================================
    static void PlaySoundFile(const std::string& filename, bool loop = false) {
        std::string fullPath = "resources/" + filename;

        DWORD flags = SND_FILENAME | SND_ASYNC;
        if (loop) {
            flags |= SND_LOOP;
            m_isLooping = true;
        }
        else {
            m_isLooping = false;
        }

        if (!PlaySoundA(fullPath.c_str(), NULL, flags)) {
            m_state = AudioState::Error;
            std::cerr << "Failed to play sound: " << fullPath << std::endl;
            return;
        }

        m_state = AudioState::Playing;
        m_currentFile = filename;
        std::cout << "▶ Playing: " << filename << std::endl;
    }

    // ============================================================
    // 暂停播放
    // ============================================================
    static void PauseMP3() {
        if (m_state != AudioState::Playing) return;

        if (mciSendStringA("pause music", NULL, 0, NULL) == 0) {
            m_state = AudioState::Paused;
            std::cout << "⏸ Paused" << std::endl;
        }
    }

    // ============================================================
    // 恢复播放
    // ============================================================
    static void ResumeMP3() {
        if (m_state != AudioState::Paused) return;

        if (mciSendStringA("resume music", NULL, 0, NULL) == 0) {
            m_state = AudioState::Playing;
            std::cout << "▶ Resumed" << std::endl;
        }
    }

    // ============================================================
    // 停止播放
    // ============================================================
    static void StopMP3() {
        if (m_state == AudioState::Stopped) return;

        mciSendStringA("stop music", NULL, 0, NULL);
        mciSendStringA("close music", NULL, 0, NULL);

        m_state = AudioState::Stopped;
        m_currentFile.clear();
        std::cout << "⏹ Stopped" << std::endl;
    }

    // ============================================================
    // 切换播放/暂停
    // ============================================================
    static void TogglePlayPause() {
        if (m_state == AudioState::Playing) {
            PauseMP3();
        }
        else if (m_state == AudioState::Paused) {
            ResumeMP3();
        }
    }

    // ============================================================
    // 获取当前状态
    // ============================================================
    static AudioState GetState() {
        return m_state;
    }

    // ============================================================
    // 获取当前播放的文件名
    // ============================================================
    static const std::string& GetCurrentFile() {
        return m_currentFile;
    }

    // ============================================================
    // 是否正在播放
    // ============================================================
    static bool IsPlaying() {
        return m_state == AudioState::Playing;
    }

    // ============================================================
    // 是否已暂停
    // ============================================================
    static bool IsPaused() {
        return m_state == AudioState::Paused;
    }

    // ============================================================
    // 是否已停止
    // ============================================================
    static bool IsStopped() {
        return m_state == AudioState::Stopped;
    }

    // ============================================================
    // 获取状态文字
    // ============================================================
    static const char* GetStateText() {
        switch (m_state) {
        case AudioState::Stopped: return "⏹ 已停止";
        case AudioState::Playing: return "▶ 播放中";
        case AudioState::Paused:  return "⏸ 已暂停";
        case AudioState::Error:   return "❌ 错误";
        default: return "未知";
        }
    }
};

// ============================================================
// 静态成员初始化
// ============================================================
AudioState AudioPlayer::m_state = AudioState::Stopped;
std::string AudioPlayer::m_currentFile = "";
bool AudioPlayer::m_isLooping = false;

// ============================================================
// 便捷函数（兼容旧代码）
// ============================================================
void PlayMP3(const std::string& filename, bool loop = false) {
    AudioPlayer::PlayMP3(filename, loop);
}

void StopMP3() {
    AudioPlayer::StopMP3();
}

void PauseMP3() {
    AudioPlayer::PauseMP3();
}

void ResumeMP3() {
    AudioPlayer::ResumeMP3();
}

void TogglePlayPause() {
    AudioPlayer::TogglePlayPause();
}

AudioState GetAudioState() {
    return AudioPlayer::GetState();
}

bool IsAudioPlaying() {
    return AudioPlayer::IsPlaying();
}

bool IsAudioPaused() {
    return AudioPlayer::IsPaused();
}

bool IsAudioStopped() {
    return AudioPlayer::IsStopped();
}

const char* GetAudioStateText() {
    return AudioPlayer::GetStateText();
}