/**
 * @file files.cpp
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines menus
 * @version 0.1
 * 
 */
#include "_menus.h"
#include "assets.h"
#include "helper.h"
#include "imgui_helper.h"
#include "program.h"

using namespace std;

namespace menus {
void files(bool* p_open) {
	ImGui::Begin("Files", p_open);
	// Filter code
	static ImGuiTextFilter filter;
	filter.Draw("Filter0", -1);

	for (auto& f : assets::entries) {
		if (!ENTRY_IS_DIR(f) &&
			filter.PassFilter(f.path().filename().c_str())) {
			ImGui::Text(f.path().filename().string().c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				menus::open_text_editor(f.path());
			}
		}
	}
	ImGui::End();
}

void assets(bool* p_open) {
	ImGui::Begin("Assets", p_open);
	if (ImGui::Button("Reload", ImVec2(-1, 0)))
		assets::reload_project();
	// Filter code

	// static std::shared_ptr<File> new_file;
	if (ImGui::Button("Add File", ImVec2(-1, 0)))
		ImGui::OpenPopup("add_file");

	if (auto asset = menus::select_asset("add_file", SELECT_FILTER_DEFAULT, Factory_Program, MenuSelectFlags_NewOnly)) {
		assets::add(std::dynamic_pointer_cast<File>(asset));
	}

	static ImGuiTextFilter filter;
	filter.Draw("Filter0", -1);

	for (auto& f : assets::files) {
		auto data_path = f->data_path();
		if (
			filter.PassFilter(data_path.c_str())) {
			auto filename = f->filename();
			ImGui::Text(filename.c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				if (auto idraw = std::dynamic_pointer_cast<IDraw>(f))
					menus::inspector_o.insert(idraw);
			}

			auto refs = f->my_ref();
			imgui_dnd_ref(refs, true, ImGuiDragDropFlags_SourceAllowNullID);
		}
	}
	ImGui::End();
}
}  // namespace menus
