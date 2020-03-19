#include "menus/menus.h"
#include "assets.h"

using namespace std;


namespace menus{
	
    void files(string root,bool* p_open){
        ImGui::Begin("Files",p_open);
        
        // Filter code
        static ImGuiTextFilter filter;
        filter.Draw("Filter0",-1);
        
        static ImGuiTextFilter filter1;
        filter1.Draw("Filter1",-1);
		
        for(auto&f:assets::entries){
            if (!ENTRY_IS_DIR(f) && 
            filter.PassFilter(f.path().filename().c_str()) && 
            filter1.PassFilter(f.path().filename().c_str())
            ){
                ImGui::Text("%s", f.path().filename().string().c_str());
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                   menus::open_text_editor(f.path());
                }
            }
        }
        ImGui::End();
    }
}

