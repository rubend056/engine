#include "menus.h"
#include "assets.h"

using namespace std;


namespace menus{

    // static vector<fs::directory_entry> fl;
    // void list_dir(string d){
    //     for (const auto &entry : fs::directory_iterator(d)){
    //         fl.push_back(entry);
    //         if (entry.is_directory()){list_dir(entry.path().string());}
    //     }
    // }
	
    void files(string root){
        ImGui::Begin("Files");

        //Find files
        
        // if(fl.size() == 0){
        //     list_dir(root);
        // }
        
        // Filter code
        static ImGuiTextFilter filter;
        filter.Draw();
		
        for(auto&f:assets::entries){
            if (!f.is_directory() && filter.PassFilter(f.path().filename().c_str())){
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

