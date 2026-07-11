// Ensure proper includes and avoid breaking parser/highlighting
#pragma once
#include <Windows.h>
#include <imgui.h>
#include <iostream>
#include <string>
#include <vector>

struct KeyBlock {
    float x, y, RainingSpeed;
    std::string s_key;
    int vk;
    float max_height;
    float width;
    int i;
    ImU32 bg_col = IM_COL32(1,0,0,255);
    ImU32 bg_col_pressed = IM_COL32(255,0,0,255);
    ImU32 raining_col = IM_COL32(255,255,255,255);
    // spawn control for continuous raining when key held
    float spawnInterval = 0.1f; // seconds between spawned raining blocks
    float lastSpawnTime = 0.0f; // last spawn timestamp (ImGui::GetTime())
    bool lastPressed = false; // whether key was pressed in previous frame
};

struct RainingBlock {
    bool active = false;
    float topY = 0.0f;    // head/top y (smaller values as it moves up)
    float bottomY = 0.0f; // tail/bottom y (near spawn)
    float x = 0.0f;       // x position
    bool fixed = false;   // whether this block reached max length and is fixed
    bool shrinking = false; // whether this block is shrinking after release
};

class RainingKey {
public:
    // Add a key to the raining system. Many parameters have defaults.
    static void AddKey(float x, float y, int vk,
                       float RainingSpeed = 20.0f,
                       const std::string& key = std::string(),
                       float width = 50.0f,
                       float max_height = 300.0f,
                       ImU32 bg_col = IM_COL32(1,0,0,255),
                       ImU32 bg_col_pressed = IM_COL32(255,0,0,255),
                       ImU32 raining_col = IM_COL32(255,255,255,255),
                       float spawnInterval = 0.1f);

    static void UpdateRainingBlocks();

    // Accessors for UI/editor
    static int GetKeyCount();
    static KeyBlock GetKeyCopy(int index);
    static void UpdateKey(int index, const KeyBlock& kb);
    static void RemoveKey(int index);

private:
    // One vector of active raining blocks per key
    static std::vector<std::vector<RainingBlock>> RainingBlocks;
    static std::vector<KeyBlock> KeyBlocks;
};
