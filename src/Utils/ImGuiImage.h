#include <iostream>
#include <imgui.h>
#include <imgui_internal.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#include <GL/gl.h>
#include <GL/glu.h>
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#include <string>
#include <unordered_map>

// ============================================================
// 纹理信息结构体
// ============================================================
struct TextureInfo {
    GLuint id;
    int width;
    int height;

    TextureInfo() : id(0), width(0), height(0) {}
    TextureInfo(GLuint _id, int _w, int _h) : id(_id), width(_w), height(_h) {}
};

// ============================================================
// 加载图片为 OpenGL 纹理
// ============================================================
TextureInfo LoadTextureFromFile(const char* filename) {
    int width, height, channels;
    unsigned char* image_data = stbi_load(filename, &width, &height, &channels, 4);
    if (image_data == NULL) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return TextureInfo();
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    stbi_image_free(image_data);

    return TextureInfo(texture_id, width, height);
}

// ============================================================
// 显示图片（带缓存）
// ============================================================
std::unordered_map<std::string, TextureInfo> g_TextureCache;

void ShowImage(const std::string& file, bool keepOriginalSize = true, float m=1,ImVec2 size = ImVec2(0.0f, 0.0f)) {
    // 从缓存查找
    auto it = g_TextureCache.find(file);
    if (it == g_TextureCache.end()) {
        TextureInfo tex = LoadTextureFromFile((std::string("resources/") + file).c_str());
        if (tex.id == 0) return;
        g_TextureCache[file] = tex;
        it = g_TextureCache.find(file);
    }

    const TextureInfo& tex = it->second;

    ImVec2 displaySize;
    if (keepOriginalSize) {
        displaySize = ImVec2((float)tex.width*m, (float)tex.height*m);
    }
    else {
        displaySize = size;
    }

    ImGui::Image((ImTextureID)(intptr_t)tex.id, displaySize);
}

// ============================================================
// 清空纹理缓存（程序退出时调用）
// ============================================================
void ClearTextureCache() {
    for (auto& pair : g_TextureCache) {
        if (pair.second.id != 0) {
            glDeleteTextures(1, &pair.second.id);
        }
    }
    g_TextureCache.clear();
}