#include "assets.h"
#include "menus/menus.h"

using namespace std;

// #include "rendering/program.h"

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
}  // namespace menus
