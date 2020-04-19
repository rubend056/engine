#include "assets.h"
#include "_menus.h"

#include "helper.h"
#include "program.h"

using namespace std;

// #include "program.h"

namespace menus {
void files(bool* p_open) {
    ImGui::Begin("Files", p_open);
    // Filter code
    static ImGuiTextFilter filter;
    filter.Draw("Filter0", -1);

    // static ImGuiTextFilter filter1;
    // filter1.Draw("Filter1",-1);

    for (auto& f : assets::entries) {
        if (!ENTRY_IS_DIR(f) &&
            filter.PassFilter(f.path().filename().c_str())
            // && filter1.PassFilter(f.path().filename().c_str())
        ) {
            ImGui::Text(f.path().filename().string().c_str());
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                menus::open_text_editor(f.path());
            }
			// else if (ImGui::IsItemClicked()){
			// 	auto o = assets::get_file<IDraw>( f.path());
			// 	if ()inspector_o = 
			// }
				
        }
    }
    ImGui::End();
}


void assets(bool* p_open) {
    ImGui::Begin("Assets", p_open);
	if(ImGui::Button("Reload"))assets::reload_project();
    // Filter code
	
	// static std::shared_ptr<File> new_file;
	if (ImGui::Button("Add File", ImVec2(-1,0)))
		ImGui::OpenPopup("add_file_popup");
	if (ImGui::BeginPopup("add_file_popup")) {
		
		static std::shared_ptr<File> new_file;
		std::shared_ptr<File> c;
		auto push = [&](const char* type_id_name) -> void {
			assets::add(c, type_id_name); new_file=c; ImGui::OpenPopup("modal_name");
		};
		if (c = helper::file_add_button<Program>()){push(typeid(Program).name());}
		// else if 
		
		if (ImGui::BeginPopupModal("modal_name")){
			static std::string name;
			ImGui::InputText("Filename", &name);
			if(ImGui::Button("Ok")){
				new_file->filename_set(name);
				new_file->next_id();
				new_file->create_supposed_ext();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		
		ImGui::EndPopup();
	}
	
	
	
    static ImGuiTextFilter filter;
    filter.Draw("Filter0", -1);
	
    for (auto& f : assets::files) {
		auto data_path = f->data_path();
        if ( 
			filter.PassFilter(data_path.c_str())
        ) {
			
			auto filename = f->filename();
            ImGui::Text(filename.c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				if(auto idraw = std::dynamic_pointer_cast<IDraw>(f))menus::inspector_o.insert(idraw);
            }
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceAllowNullID))
			{
				// auto file_p = &f;
				ImGui::SetDragDropPayload("DND_FILE", &f->file_id, sizeof(f->file_id));    // Set payload to carry the index of our item (could be anything)
				
				auto hash_path = f->hash_path();
				ImGui::Text(hash_path.c_str());
				ImGui::EndDragDropSource();
			}	
        }
    }
    ImGui::End();
}
}  // namespace menus
