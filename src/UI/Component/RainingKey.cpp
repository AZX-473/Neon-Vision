#include "RainingKey.h"
#include <Windows.h>
#include <imgui.h>
#include <iostream>
#include <string>
#include <vector>
#include "../../Utils/Input.h"
#include <algorithm>
#include "../../PublicVariable/Variable.h"

// Define static members declared in RainingKey.h
std::vector<KeyBlock> RainingKey::KeyBlocks;
std::vector<std::vector<RainingBlock>> RainingKey::RainingBlocks;

void RainingKey::AddKey(float x, float y, int vk, float RainingSpeed, const std::string& key, float width, float max_height, ImU32 bg_col, ImU32 bg_col_pressed, ImU32 raining_col, float spawnInterval) {
	int tmpi = (int)KeyBlocks.size();
	KeyBlocks.push_back({ x, y, RainingSpeed, key, vk, max_height, width, tmpi, bg_col, bg_col_pressed, raining_col, spawnInterval });
	RainingBlocks.push_back(std::vector<RainingBlock>());
}

int RainingKey::GetKeyCount() {
	return (int)KeyBlocks.size();
}

KeyBlock RainingKey::GetKeyCopy(int index) {
	if (index < 0 || index >= (int)KeyBlocks.size()) return KeyBlock();
	return KeyBlocks[index];
}

void RainingKey::UpdateKey(int index, const KeyBlock& kb) {
	if (index < 0 || index >= (int)KeyBlocks.size()) return;
	KeyBlocks[index] = kb;
}

void RainingKey::RemoveKey(int index) {
	if (index < 0 || index >= (int)KeyBlocks.size()) return;
	KeyBlocks.erase(KeyBlocks.begin() + index);
	RainingBlocks.erase(RainingBlocks.begin() + index);
	// update indices
	for (int i = 0; i < (int)KeyBlocks.size(); ++i) KeyBlocks[i].i = i;
}

void RainingKey::UpdateRainingBlocks() {
	if (KeyBlocks.empty()) return;
	// dragging state for interactive repositioning of KeyBlocks
	static int draggingIndex = -1;
	static ImVec2 dragOffset(0, 0);

	// Decide whether the overlay should accept inputs this frame.
	// If the mouse is over any key rect or a drag is active, accept inputs; otherwise allow clicks to pass through.
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	bool hoverAny = false;
	for (const auto& kb : KeyBlocks) {
		float bw = kb.width > 0.0f ? kb.width : 100.0f;
		float bh = 30.0f;
		ImVec2 kb0(kb.x - bw / 2.0f, kb.y - bh / 2.0f);
		ImVec2 kb1(kb.x + bw / 2.0f, kb.y + bh / 2.0f);
		if (mousePos.x >= kb0.x && mousePos.x <= kb1.x && mousePos.y >= kb0.y && mousePos.y <= kb1.y) {
			hoverAny = true;
			break;
		}
	}

	bool wantsInput = hoverAny || (draggingIndex != -1);

	// Prepare an invisible overlay window for drawing
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	// Allow inputs only if needed so underlying main window can receive clicks when not interacting with keys
	ImGuiWindowFlags OverlayWindowflags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoNavFocus;
	if (!wantsInput) OverlayWindowflags |= ImGuiWindowFlags_NoInputs;

	ImGui::Begin("RainingKeyOverlay", nullptr, OverlayWindowflags);

	ImDrawList* draw_list = ImGui::GetForegroundDrawList();

	double now = ImGui::GetTime();
	float dt = ImGui::GetIO().DeltaTime; // seconds since last frame

	for (size_t i = 0; i < KeyBlocks.size(); ++i) {
		ImGui::PushFont(g_MainFont);
		KeyBlock& kb = KeyBlocks[i];
		auto& blocks = RainingBlocks[i];

		// Draw the static key background at kb.x,kb.y
		float bw = kb.width > 0.0f ? kb.width : 100.0f;
		float bh = 30.0f;
		ImVec2 kb0(kb.x - bw / 2.0f, kb.y - bh / 2.0f);
		ImVec2 kb1(kb.x + bw / 2.0f, kb.y + bh / 2.0f);
		ImU32 bg = KEY_DOWN(kb.vk) ? kb.bg_col_pressed : kb.bg_col;
		draw_list->AddRectFilled(kb0, kb1, bg, 6.0f);
		draw_list->AddRect(kb0, kb1, IM_COL32(0, 0, 0, 200), 6.0f, 0, 1.0f);
		ImVec2 text_size = ImGui::CalcTextSize(kb.s_key.c_str());
		draw_list->AddText(ImVec2(kb.x - text_size.x / 2.0f, kb.y - text_size.y / 2.0f), IM_COL32(255, 255, 255, 255), kb.s_key.c_str());

		// Mouse interaction: start dragging when left mouse pressed inside key rect
		ImVec2 mousePos = ImGui::GetIO().MousePos;
		bool hover = ImGui::IsMouseHoveringRect(kb0, kb1, true);
		if (draggingIndex == -1) {
			if (hover && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				draggingIndex = (int)i;
				dragOffset = ImVec2(mousePos.x - kb.x, mousePos.y - kb.y);
			}
		}
		// while dragging this key, update its position to follow the mouse
		if (draggingIndex == (int)i) {
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				kb.x = mousePos.x - dragOffset.x;
				kb.y = mousePos.y - dragOffset.y;
				// apply immediate update so editor shows updated values
				RainingKey::UpdateKey((int)i, kb);
			}
			else {
				// mouse released
				draggingIndex = -1;
			}
		}

		bool isPressed = KEY_DOWN(kb.vk);
		// remember whether key was pressed in previous frame (kb.lastPressed stores that)
		bool prevPressed = kb.lastPressed;

		// On press: spawn new block if this is a new press; if continuing press, extend last block to connect
		if (isPressed) {
			if (!prevPressed || blocks.empty()) {
				// spawn a new block segment
				RainingBlock nb;
				nb.active = true;
				nb.x = kb.x;
				nb.topY = kb.y - 6.0f;    // small head
				nb.bottomY = kb.y;        // tail at base
				blocks.push_back(nb);
				kb.lastSpawnTime = (float)now;
			}
			else {
				// continuing press: extend last block's bottom to current base y to avoid gap
				if (!blocks.empty()) {
					RainingBlock& last = blocks.back();
					// ensure bottomY is at least kb.y (connect to base)
					if (kb.y > last.bottomY) last.bottomY = kb.y;
				}
			}
			kb.lastPressed = true;
		}
		else {
			kb.lastPressed = false;
			kb.lastSpawnTime = 0.0f;
		}

		// Update blocks: move upward (top and bottom decrease by speed * dt).
		float move = kb.RainingSpeed * dt;
		const float shrinkMultiplier = 1.5f; // bottom moves faster when shrinking
		for (auto& b : blocks) {
			if (!b.active) continue;

			// compute clip top (upper limit)
			float clipTop = kb.y - kb.max_height;
			// compute key top Y (button top edge)
			float bh = 30.0f;
			float keyTopY = kb.y - bh / 2.0f;
			// move top by speed and clamp to clipTop (do not pass the upper limit)
			float newTop = b.topY - move;
			if (newTop < clipTop) newTop = clipTop;
			b.topY = newTop;

			// determine if bottom was recently anchored to key top (within eps) or at base
			const float eps = 1.5f;
			bool bottomAnchoredPrev = (fabs(b.bottomY - keyTopY) <= eps) || (fabs(b.bottomY - kb.y) <= eps);

			if (b.shrinking) {
				// shrinking: top moves up, bottom moves up faster to reduce length
				b.topY -= move; // continue removing length
				b.bottomY -= move * (1.0f + shrinkMultiplier);
			}
			else if (b.fixed) {
				// fixed while key held: keep anchored to base (no movement)
				if (kb.lastPressed) {
					// if bottom was previously anchored or equal to base, keep it at key top to avoid overlap
					if (prevPressed && bottomAnchoredPrev) b.bottomY = keyTopY;
					// otherwise keep as-is while fixed
				}
				else {
					// key released: start shrinking
					b.fixed = false;
					b.shrinking = true;
				}
			}
			else {
				// normal moving block: move bottom up unless we should anchor it
				if (prevPressed && bottomAnchoredPrev) {
					// anchor bottom to key top
					b.bottomY = keyTopY;
					if (b.topY > b.bottomY) b.topY = b.bottomY;
				}
				else {
					b.bottomY -= move;
				}
			}
		}

		// Clamp blocks that reached max_height based on block length (bottom - top)
		for (auto& b : blocks) {
			if (!b.active || b.fixed || b.shrinking) continue;
			float length = b.bottomY - b.topY;
			if (length >= kb.max_height) {
				// ensure block length equals max_height, anchored at its bottom
				b.topY = b.bottomY - kb.max_height;
				b.fixed = true;
			}
		}

		// Draw active raining blocks
		for (const auto& b : blocks) {
			if (!b.active) continue;
			float w = kb.width > 0.0f ? kb.width : 100.0f;
			// clip the top so that nothing above kb.y - max_height is drawn
			float clipTop = kb.y - kb.max_height;
			float drawTop = b.topY;
			if (drawTop < clipTop) drawTop = clipTop;
			// if after clipping there's no height left, skip drawing
			if (drawTop >= b.bottomY) continue;
			ImVec2 p0(b.x - w / 2.0f, drawTop);
			ImVec2 p1(b.x + w / 2.0f, b.bottomY);
			draw_list->AddRectFilled(p0, p1, kb.raining_col, 4.0f);
		}

		// Remove blocks that moved completely above the visible clip (no longer visible)
		float clipTop = kb.y - kb.max_height;
		blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&](const RainingBlock& b) {
			if (!b.active) return true;
			// if bottom is above the clip top, the block is completely out of range
			if (b.bottomY <= clipTop + 0.5f) return true;
			// also remove tiny shrunk blocks
			if (b.shrinking && (b.bottomY - b.topY) <= 1.0f) return true;
			return false;
			}), blocks.end());

		// Remove inactive or fully shrunk blocks
		blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [&](const RainingBlock& b) {
			if (!b.active) return true;
			// if shrinking and very small, remove
			if (b.shrinking && (b.bottomY - b.topY) <= 1.0f) return true;
			return false;
			}), blocks.end());
		ImGui::PopFont();
	}

	ImGui::End();
}
