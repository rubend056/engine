
#include "_menus.h"
#include "engine_globals.h"
#include "engine_scene.h"

void menus::stats(bool* p_open) {
	ImGui::Begin("Stats", p_open);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Number of GameObjects: %zu", engine::scene ? engine::scene->children.size() : (size_t)0);
	ImGui::Text("id_rpath");
	for (auto& idr : assets::id_rpath_ht) ImGui::TextDisabled("  %d:%s", idr.first, idr.second.c_str());
	ImGui::Text("rpath_asset");
	for (auto& idr : assets::rpath_asset_ht) ImGui::TextDisabled("  %s:%s", idr.first.c_str(), idr.second->filename().c_str());
	ImGui::Text("type_asset");
	for (auto& idr : assets::type_asset_ht) ImGui::TextDisabled("  %s:%s", idr.first.c_str(), idr.second->filename().c_str());

	ImGui::End();
}